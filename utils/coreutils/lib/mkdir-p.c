/* mkdir-p.c -- Ensure that a directory and its parents exist. */

#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stddef.h>
#include <errno.h>
#include <error.h>
#include <unistd.h>
#include <linux/stat.h>

#include "savewd.h"
#include "dirname.h"
#include "mkancesdirs.h"
#include "quote.h"
#include "system.h"
#include "dirchownmod.h"

#ifndef HAVE_FCHMOD
# define HAVE_FCHMOD false
#endif

/* Ensure that the directory DIR exist.

   WD is the working directory, as in savewd.c.

   If MAKE_ANCESTOR is not null, create any ancestor directories that
   don't already exist, by invoking MAKE_ANCESTOR (DIR, ANCESTOR, OPTIONS).
   This function should return zero if successful, -1 (setting errno)
   otherwise. In this case, DIR may be modified by storing '\0' bytes
   into it, to access the ancestor directories, and this modification
   is retained on return if the ancestor directories could not be
   created.

   Create DIR as a new directory with using mkdir with permissions
   MODE. It is also OK if MAKE_ANCESTOR is not null and a
   directory DIR already exists.

   Call ANNOUNCE (DIR, OPTIONS) just after successfully making DIR,
   even if some of the following actions fail.

   Set DIR's owner to OWNER and group to GROUP, but leave the owner
   alone if OWNER is (uid_t) -1, and similarly for GROUP.

   Set DIR's mode bits to MODE, except preserve any of the bits that
   correspond to zero bits in MODE_BITS. In other words, MODE_BITS is
   a mask that specifies which of DIR's mode bits should be set or 
   cleared. MODE should be a subset of MODE_BITS, which in turn
   should be a subset of CHMOD_MODE_BITS. Changing the mode in this
   way is necessary if DIR already existed or if MODE and MODE_BITS
   specify non-permissions bits like S_ISUID.

   However, if PRESERVE_EXISTING is true and DIR already exists,
   do not attempt to set DIR's ownership and file mode bits.

   This implementation assumes the current umask to zero.

   Return true if DIR exists and a directory with the proper ownership
   and file mode bits when done, or if a child process has been
   dispatched to do the read work (though the child process may not
   have finished yet -- it is the caller's responsibility to handle
   this). Report a diagnostic and return false on failure, storing
   '\0' into *DIR if an ancestor directory had problems. */
bool make_dir_parents(char* dir,
                      struct savewd* wd,
                      int (*make_ancestor)(char*, char*, void*),
                      void* options,
                      mode_t mode,
                      void (*announce)(char*, void*),
                      mode_t mode_bits,
                      uid_t owner,
                      gid_t group,
                      bool preserve_existing)
{
    int mkdir_errno = (IS_ABSOLUTE_FILE_NAME(dir) ? 0 : savewd_errno(wd));

    if(mkdir_errno == 0)
    {
        ptrdiff_t prefix_len = 0;
        int savewd_chdir_options = (HAVE_FCHMOD ? SAVEWD_CHDIR_SKIP_READABLE : 0);

        if(make_ancestor)
        {
            prefix_len = mkancesdirs(dir, wd, make_ancestor, options);
            if(prefix_len < 0)
            {
                if(prefix_len < -1)
                    return true;
                mkdir_errno = errno;
            }
        }

        if(prefix_len >= 0)
        {
            /* If the ownership might change, or if the directory will be
               writable to other users and its special mode bits may
               change after the directory is created, create it with
               more restrictive permissions at first, so unauthorized
               users cannot nip in before the directory is ready */
            bool keep_owner = owner == (uid_t) -1 && group == (gid_t) -1;
            bool keep_special_mode_bits = 
                    ((mode_bits & (S_ISUID | S_ISGID)) | (mode & S_ISVTX)) == 0;
            mode_t mkdir_mode = mode;
            if(!keep_owner)
                mkdir_mode &= ~(S_IRWXG | S_IRWXO);
            else if(! keep_special_mode_bits)
                mkdir_mode &= ~(S_IWGRP | S_IWOTH);

            if(mkdir(dir + prefix_len, mkdir_mode) == 0)
            {
                announce(dir, options);
                preserve_existing = keep_owner & keep_special_mode_bits;
                savewd_chdir_options |=
                        (SAVEWD_CHDIR_NOFOLLOW
                            | (mode & S_IRUSR ? SAVEWD_CHDIR_READABLE : 0));
            }
            else
            {
                mkdir_errno = errno;
                mkdir_mode = -1;
            }

            if(preserve_existing)
            {
                struct stat st;
                if(mkdir_errno == 0
                    || (mkdir_errno != ENOENT && make_ancestor
                        && stat(dir + prefix_len, &st) == 0
                        && S_ISDIR(st.st_mode)))
                    return true;
            }
            else
            {
                int open_result[2];
                int chdir_result = 
                    savewd_chdir(wd, dir + prefix_len,
                                 savewd_chdir_options, open_result);
                if(chdir_result < -1)
                    return true;
                else
                {
                    bool chdir_ok = (chdir_result == 0);
                    int chdir_errno = errno;
                    int fd = open_result[0];
                    bool chdir_failed_unexpectedly =
                        (mkdir_errno == 0
                         && ((!chdir_ok && (mode & S_IXUSR))
                             || (fd < 0 && (mode & S_IRUSR))));

                    if(chdir_failed_unexpectedly)
                    {
                        /* No need to save errno here; it's irrelevant */
                        if(fd >= 0)
                            close(fd);
                    }
                    else
                    {
                        char* subdir = (chdir_ok ? "." : dir + prefix_len);
                        if(dirchownmod(fd, subdir, mkdir_mode, owner, group,
                                        mode, mode_bits) == 0)
                            return true;
                    }
                    if(mkdir_errno == 0
                        || (mkdir_errno != ENOENT && make_ancestor
                            && errno != ENOTDIR))
                    {
                        error(0,
                              (! chdir_failed_unexpectedly ? errno
                               : ! chdir_ok && (mode & S_IXUSR) ? chdir_errno
                               : open_result[1]),
                               _(keep_owner
                                 ? "cannot change permissions of %s"
                                 : "cannot change owner and permissions of %s"),
                                 quote(dir));
                        return false;
                    }
                }
            }
        }
    }
    error(0, mkdir_errno, _("cannot create directory %s"), quote(dir));
    return false;
}
