/* chown-core.c -- core functions for changing ownership */

#include "chown-core.h"
#include "xalloc.h"
#include "intprops.h"
#include "inttostr.h"
#include "xfts.h"
#include "root-dev-ino.h"
#include "ignore-value.h"
#include "quote.h"
#include "openat.h"


#include <grp.h>
#include <error.h>
#include <string.h>
#include <pwd.h>

#ifndef STREQ
#define STREQ(a, b) (strcmp(a, b) == 0)
#endif

#define FTSENT_IS_DIRECTORY(E)      \
    ((E)->fts_info == FTS_D         \
     || (E)->fts_info == FTS_DC     \
     || (E)->fts_info == FTS_DP     \
     || (E)->fts_info == FTS_DNR)

enum RCH_status
{
    /* we called fchown and close, and both succeeded */
    RC_ok = 2,

    /* required_uid and/or required_gid are speicified, but don't match */
    RC_excluded,

    /* SAME_INODE check failed */
    RC_inode_changed,

    /* open/fchown isn't needed, isn't safe, or doesn't work due to
       permissions problems; fall back on chown */
    RC_do_ordinary_chown,

    /* open, fstat, fchown, or close failed */
    RC_error
};

void chopt_init(struct Chown_option* chopt)
{
    chopt->verbosity = V_off;
    chopt->root_dev_ino = NULL;
    chopt->affect_symlink_referent = true;
    chopt->recurse = false;
    chopt->force_silent = false;
    chopt->user_name = NULL;
    chopt->group_name = NULL;
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it. If there's no corresponding group name, use the decimal
   representation of the ID. */
char* gid_to_name(gid_t gid)
{
    char buf[INT_BUFSIZE_BOUND(intmax_t)];
    struct group *grp = getgrgid(gid);
    return xstrdup(grp ? grp->gr_name
                    : TYPE_SIGNED(gid_t) ? imaxtostr(gid, buf)
                    : umaxtostr(gid, buf));
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it. If there's no corresponding user name, use the decimal
   representation of the ID. */
char* uid_to_name(uid_t uid)
{
    char buf[INT_BUFSIZE_BOUND(intmax_t)];
    struct passwd* pwd = getpwuid(uid);
    return xstrdup(pwd ? pwd->pw_name
                   : TYPE_SIGNED(uid_t) ? imaxtostr(uid, buf)
                   : umaxtostr(uid, buf));
}


/* Tell the user how/if the user and group of FILE have been changed.
   If USER is NULL, give the group-oriented messages.
   CHANGED describes what (if anything) has happened */

static void describe_change(char* file, enum Changed_status changed,
                            char* user, char* group)
{
    const char* fmt;
    char* spec;
    char* spec_allocated = NULL;

    if(changed == CH_NOT_APPLIED)
    {
        printf(_("neither symbolic link %s nor referent has been changed\n"),
                file);
        return;
    }

    if(user)
    {
        if(group)
        {
            spec_allocated = xmalloc(strlen(user) + 1 + strlen(group) + 1);
            stpcpy(stpcpy(stpcpy(spec_allocated, user), ":"), group);
            spec = spec_allocated;
        }
        else
            spec = user;
    }
    else
        spec = group;

    switch(changed)
    {
        case CH_SUCCEEDED:
            fmt = (user ? _("changed ownership of %s to %s\n")
                    : group ? _("changed group of %s to %s\n")
                    : _("no change to ownership of %s\n"));
            break;
        case CH_FAILED:
            fmt = (user ? _("failed to change ownership of %s to %s\n")
                    : group ? _("failed to change group of %s to %s\n")
                    : _("failed to change ownership of %s\n"));
            break;
        case CH_NO_CHANGE_REQUESTED:
            fmt = (user ? _("ownership of %s retained as %s\n")
                    : group ? _("group of %s retained as %s\n")
                    : _("ownership of %s retained\n"));
            break;
        default:
            abort();
    }

    printf(fmt, file, spec);

    free(spec_allocated);
}


/* Change the owner and/or group of the FILE to UID and/or GID (safely)
   only if REQUIRED_UID and REQUIRED_GID match the owner and group IDs
   of FILE. ORIG_ST must be the result of `stat'ing FILE.

   The `safely' part above means that we can't simply use chown(2),
   since FILE might be replaced with some other file between the time
   of the preceding stat/lstat and this chown call. So here we open 
   FILE and do everything else via the resulting file descriptor.
   We first call fstat and verify that the dev/inode match those from
   the preceding stat call, and only then, if appropriate (given the
   required_uid and required_gid constraints) do we call fchown.

   Return RC_do_ordinary_chown if we can't open FILE, or if FILE is a
   special file that might have undesirable side effects when opening.
   In this case the caller can use the less-safe ordinary chown. 

   Return one of the RCH_status values. */
static enum RCH_status
restricted_chown(int cwd_fd, char* file,
                 struct stat* orig_st,
                 uid_t uid, gid_t gid,
                 uid_t required_uid, gid_t required_gid)
{
    enum RCH_status status = RC_ok;
    struct stat st;
    int open_flags = O_NONBLOCK | O_NOCTTY;
    int fd;

    if(required_uid == (uid_t) -1 && required_gid == (gid_t) -1)
        return RC_do_ordinary_chown;

    if(! S_ISREG(orig_st->st_mode))
    {
        if(S_ISDIR(orig_st->st_mode))
            open_flags |= O_DIRECTORY;
        else
            return RC_do_ordinary_chown;
    }

    fd = openat(cwd_fd, file, O_RDONLY | open_flags);
    if(! (fd >= 0
          || (errno == EACCES && S_ISREG(orig_st->st_mode)
                && 0 <= (fd = openat(cwd_fd, file, O_WRONLY | open_flags)))))
        return (errno == EACCES ? RC_do_ordinary_chown : RC_error);

    if(fstat(fd, &st) != 0)
        status = RC_error;
    else if(! SAME_INODE(*orig_st, st))
        status = RC_inode_changed;
    else if((required_uid == (uid_t) -1 || required_uid == st.st_uid)
            && (required_gid == (gid_t) -1 || required_gid == st.st_gid))
    {
        if(fchown(fd, uid, gid) == 0)
        {
            status = (close(fd) == 0
                      ? RC_ok : RC_error);
            return status;
        }
        else
            status = RC_error;
    }

    { /* FIXME: remove these curly braces when we assume C99 */
        int saved_errno = errno;
        close(fd);
        errno = saved_errno;
        return status;
    }
}


/* Change the owner and/or group of the file specified by FTS and ENT
   to UID and/or GID as appropriate.
   If REQUIRED_UID is not -1, then skip files with any other user ID.
   If REQUIRED_GID is not -1, then skip files with any other group ID.
   CHOPT specifies additional options.
   Return true if successful. */
static bool change_file_owner(FTS* fts, FTSENT* ent,
                              uid_t uid, gid_t gid,
                              uid_t required_uid, gid_t required_gid,
                              struct Chown_option* chopt)
{
    char* file_full_name = ent->fts_path;
    char* file = ent->fts_accpath;
    struct stat* file_stats;
    struct stat  stat_buf;
    bool ok = true;
    bool do_chown;
    bool symlink_changed = true;

    switch(ent->fts_info)
    {
        case FTS_D:
            if(chopt->recurse)
            {
                if(ROOT_DEV_INO_CHECK(chopt->root_dev_ino, ent->fts_statp))
                {
                    /* This happens e.g., with "chown -R --preserve-root 0 /"
                       and with "chown -RH --preserve-root 0 symlink-to-root". */
                    ROOT_DEV_INO_WARN(file_full_name);
                    /* Tell fts not to traverse into this hierarchy */
                    fts_set(fts, ent, FTS_SKIP);
                    /* Ensure that we do not process "/" on the second visit. */
                    ignore_ptr(fts_read(fts));
                    return false;
                }
                return true;
            }
            break;
        case FTS_DP:
            if(!chopt->recurse)
                return true;
            break;
        case FTS_NS:
            /* For a top-level file or directory, this FTS_NS (stat failed)
               indicator is determined at the time of the initial fts_open call.
               With programs like chmod, chown, and chgrp, that modify
               permissions, it is possible that the file in question is
               accessible when control reaches this point. So, if this is 
               the first time we've seen the FTS_NS for this file, tell
               fts_read to stat it "again". */
            if(ent->fts_level == 0 && ent->fts_number == 0)
            {
                ent->fts_number = 1;
                fts_set(fts, ent, FTS_AGAIN);
                return true;
            }
            if(!chopt->force_silent)
                error(0, ent->fts_errno, _("cannot access %s"),
                        quote(file_full_name));
            ok = false;
            break;
        case FTS_ERR:
            if(!chopt->force_silent)
                error(0, ent->fts_errno, _("%s"), quote(file_full_name));
            ok = false;
            break;
        case FTS_DNR:
            if(!chopt->force_silent)
                error(0, ent->fts_errno, _("cannot read directory %s"),
                        quote(file_full_name));
            ok = false;
            break;
        case FTS_DC:        /* directory that causes cycles */
            if(cycle_warning_required(fts, ent))
            {
                emit_cycle_warning(file_full_name);
                return false;
            }
            break;
        default:
            break;
    }
    if(!ok)
    {
        do_chown = false;
        file_stats = NULL;
    }
    else if(required_uid == (uid_t) -1 && required_gid == (gid_t) -1
            && chopt->verbosity == V_off
            && ! chopt->root_dev_ino
            && ! chopt->affect_symlink_referent)
    {
        do_chown = true;
        file_stats = ent->fts_statp;
    }
    else
    {
        file_stats = ent->fts_statp;

        /* If this is a symlink and we're dereferencing them,
           stat it to get info on the referent */
        if(chopt->affect_symlink_referent && S_ISLNK(file_stats->st_mode))
        {
            if(fstatat(fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
                if(! chopt->force_silent)
                    error(0, errno, _("cannot dereference %s"),
                            quote(file_full_name));
                ok = false;
            }
            file_stats = &stat_buf;
        }

        do_chown = (ok
                    && (required_uid == (uid_t) -1
                        || required_uid == file_stats->st_uid)
                    && (required_gid == (gid_t) -1
                        || required_gid == file_stats->st_gid));
    }

    /* This happens when chown -LR --preserve-root encounters a symlink-to-/ */
    if(ok
       && FTSENT_IS_DIRECTORY(ent)
       && ROOT_DEV_INO_CHECK(chopt->root_dev_ino, file_stats))
    {
        ROOT_DEV_INO_WARN(file_full_name);
        return false;
    }

    if(do_chown)
    {
        if(! chopt->affect_symlink_referent)
        {
            ok = (lchownat(fts->fts_cwd_fd, file, uid, gid) == 0);

            /* Ignore any error due to lack of support; POSIX requires
               this behavior for top-level symbolic links with -h, and
               implies that it's required for all symbolic links */
            if(!ok && errno == EOPNOTSUPP)
            {
                ok = true;
                symlink_changed = false;
            }
        }
        else
        {
            /* If possible, avoid a race condition with --from=0:G and without the
               (-h) --no-dereference option. If fts's stat call determined
               that the uid/gid of FILE matches the --from=0:G-selected
               owner and group IDs, blindly using chown(2) here could lead
               chown(1) or chgrp(1) mistakenly to dereference a *symlink*
               to an arbitrary file that an attacker had moved into the 
               place of FILE during the window between the stat and
               chown(2) calls. If FILE is a regular file or a directory
               that can be opened, this race condition can be avoided safely */
            enum RCH_status err
                = restricted_chown(fts->fts_cwd_fd, file, file_stats, uid, gid,
                                    required_uid, required_gid);
            switch(err)
            {
                case RC_ok:
                    break;
                case RC_do_ordinary_chown:
                    ok = (chownat(fts->fts_cwd_fd, file, uid, gid) == 0);
                    break;
                case RC_error:
                    ok = false;
                    break;
                case RC_inode_changed:
                case RC_excluded:
                    do_chown = false;
                    ok = false;
                    break;
                default:
                    abort();
            }
        }

        /* On some systems (e.g., GNU/Linux 2.4.x),
           the chown function resets the `special' permission bits.
           Do *not* restore those bits; doing so would open a window in
           which a malicious user, M, could subvert a chown command run
           by some other user and operating on files in a directory
           where M has write access. */
        if(do_chown && !ok && ! chopt->force_silent)
            error(0, errno, (uid != (uid_t) -1
                              ? _("changing ownership of %s")
                              : _("changing group of %s")),
                          quote(file_full_name));
    }

    if(chopt->verbosity != V_off)
    {
        bool changed = 
                ((do_chown && ok && symlink_changed)
                  && ! ((uid == (uid_t) -1 || uid == file_stats->st_uid)
                        && (gid == (gid_t) -1 || gid == file_stats->st_gid)));
        if(changed || chopt->verbosity == V_high)
        {
            enum Changed_status ch_status =
                (!ok ? CH_FAILED
                 : !symlink_changed ? CH_NOT_APPLIED
                 : !changed ? CH_NO_CHANGE_REQUESTED
                 : CH_SUCCEEDED);
            describe_change(file_full_name, ch_status,
                                chopt->user_name, chopt->group_name);
        }
    }

    if(!chopt->recurse)
        fts_set(fts, ent, FTS_SKIP);

    return ok;
}



/* Change the owner and/or group of the specified FILES.
   BIT_FLAGS specifies how to treat each symlink-to-directory
   that is encountered during a recursive traversal.
   CHOPT specifies additional options.
   If UID is not -1, then change the owner id of each file to UID.
   If GID is not -1, then change the group id of each file to GID.
   If REQUIRED_UID and/or REQUIRED_GID is not -1, then change only
   files with user ID and group ID that match the non-(-1) value(s).
   Return true if successful. */
bool chown_files(char** files, int bit_flags,
                 uid_t uid, gid_t gid,
                 uid_t required_uid, gid_t required_gid,
                 struct Chown_option* chopt)
{
    bool ok = true;

    /* Use lstat and stat only if they're needed */
    int stat_flags = ((required_uid != (uid_t) -1 || required_gid != (gid_t) -1
                        || chopt->affect_symlink_referent
                        || chopt->verbosity != V_off)
                      ? 0
                      : FTS_NOSTAT);
    FTS* fts = xfts_open(files, bit_flags | stat_flags, NULL);

    while(1)
    {
        FTSENT* ent;

        ent = fts_read(fts);
        if(ent == NULL)
        {
            if(errno != 0)
            {
                if(!chopt->force_silent)
                    error(0, errno, _("fts_read failed"));
                ok = false;
            }
            break;
        }
        ok &= change_file_owner(fts, ent, uid, gid,
                                required_uid, required_gid, chopt);
    }

    if(fts_close(fts) != 0)
    {
        error(0, errno, _("fts_close failed"));
        ok = false;
    }

    return ok;
}

void chopt_free(struct Chown_option* chopt ATTRIBUTE_UNUSED)
{
    /* Deliberately do not free chopt->user_name or ->group_name.
       They're not always allocated. */
}
