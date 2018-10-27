#!/bin/bash
# 
# commit_check.sh for download the git hooks scritps to
# to .git/hooks path which the git project exist
# you can exec the commit_check script in the git project
# root dir or set the git project manual with it -p parameter
#
#
#  commit_chechk.sh in git project root dir
#  commit_check.sh -p [git project path]
#  commit_check.sh -g [git hooks download addr] -w [git hooks download password]
#

GIT_HOOKS_ADDR=rain@172.21.112.191:/work/repo/git-hooks
GIT_HOOKS_DIR=${GIT_HOOKS_ADDR##*/}
PASSWORD=RAIN

CURRENT_DIR=`pwd`
GIT_HOOKS_TEMP=./temp_git
GIT_CLONE_EXP=./git_clone.exp
GIT_PROJECT_DIR=$CURRENT_DIR

function git_clone_hooks()
{
    if [ -f /usr/bin/expect ]; then
        cat >> $GIT_CLONE_EXP << EOF
#!/usr/bin/expect -f
set timeout 10
eval spawn git clone $GIT_HOOKS_ADDR $GIT_HOOKS_TEMP/$GIT_HOOKS_DIR
expect "password:"
send "$PASSWORD\n"
expect "(100%)"
EOF
        chmod +x $GIT_CLONE_EXP
        $GIT_CLONE_EXP
        rm -f $GIT_CLONE_EXP
    else
        git clone $GIT_HOOKS_ADDR $GIT_HOOKS_TEMP/$GIT_HOOKS_DIR
    fi
}

while getopts "p:w:g:" opt
do
    case $opt in
        p)GIT_PROJECT_DIR=$OPTARG;;
        g)GIT_HOOKS_ADDR=$OPTARG;;
        w)PASSWORD=$OPTARG;;
        ?)echo "invaild arg";;
    esac
done

if [ ! -d "$GIT_PROJECT_DIR/.git/" ]; then
    echo "exec dir $GIT_PROJECT_DIR not git project, please check"
    exit 1
fi

mkdir -p $GIT_HOOKS_TEMP
if [ ! -d "$GIT_HOOKS_TEMP" ]; then
    echo "mkdir temp_git failed. please check"
    exit 1
fi

git_clone_hooks
if [ ! -d "./$GIT_HOOKS_TEMP/$GIT_HOOKS_DIR" ]; then
    echo "git hooks clone failed, please check"
    rm -rf $GIT_HOOKS_TEMP
    exit 1
fi

cp ./$GIT_HOOKS_TEMP/$GIT_HOOKS_DIR/* $GIT_PROJECT_DIR/.git/hooks/
rm -rf $GIT_HOOKS_TEMP
sync

if [ ! -f "$GIT_PROJECT_DIR/.git/hooks/pre-commit" -o ! -f "$GIT_PROJECT_DIR/.git/hooks/commit-msg" ]; then
   echo "copy the git hooks scripts to hooks dir failed, try next time"
   exit 1
fi

echo "fetch the git hooks sctrpts to git project $GIT_PROJECT_DIR success"

exit 0

