#!/bin/bash
g_port=("10122" "22")
g_rcdpasswd=("Rjrcd123@" "Rjrcd123@rcos_2019" "fjrjroot_123" "1" )
g_rootpasswd=("rootRj369@"  "rootRj369@rcos_2019" "fjrjroot_123" "1" )


g_rcdname=rcd
g_rootname=root
g_expect_timeout=-1
g_expect_pre=/tmp/.expect.pre.$$.`date +%N`
g_expect_tmp_tarfile=${g_expect_pre}_tmpfile.tar.bz2
g_expect_tmp_untar_script=${g_expect_pre}_untar.script
g_expect_log=${g_expect_pre}_log.log
g_expect_list=${g_expect_pre}_list.list
g_expect_cmd=${g_expect_pre}_cmd.cmd
g_expect_run=${g_expect_pre}_run.run
g_expect_runfile=${g_expect_pre}_runfile.runfile
g_expect_run_finish_msg="===expect autorun finish==="

function kecho()
{
  echo =-=-=-=-=-=-=$*
  #echo =-=-=-=-=-=-=$* >/dev/null
}

function aexit_f()
{
  rm -rf ${g_expect_pre}*
  exit 1
}

function aexpect_f()
{
  local l_remote_ip=$1
  local l_runcmd=$2
  shift 2
  local l_rcd_passwd_cachefile=/tmp/.expect.rcd.passwd.$l_remote_ip
  local l_root_passwd_cachefile=/tmp/.expect.root.passwd.$l_remote_ip
  local l_port_cachefile=/tmp/.expect.port.$l_remote_ip
  local l_rcdpasswd=( ` [ -f $l_rcd_passwd_cachefile ] && cat $l_rcd_passwd_cachefile` `echo ${g_rcdpasswd[@]}` )
  local l_rootpasswd=( `[ -f $l_root_passwd_cachefile ] && cat $l_root_passwd_cachefile` `echo ${g_rootpasswd[@]}` )
  local l_port=( `[ -f $l_port_cachefile ] && cat $l_port_cachefile` `echo ${g_port[@]}` )
  local l_cur_rcdpasswd=$l_rcdpasswd
  local l_cur_rootpasswd=$l_rootpasswd
  local l_cur_port=$l_port
  local l_rcd_passwd_retry_cnt=0
  local l_root_passwd_retry_cnt=0
  local l_port_idx=0
  local l_passwd_idx=0

  kecho ===l_rcdpasswd:${l_rcdpasswd[@]}=l_rootpasswd:${l_rootpasswd[@]}=l_port:${l_port[@]}===

  while :
  do
    kecho ===l_passwd_idx:$l_passwd_idx==l_rcd_passwd_retry_cnt:$l_rcd_passwd_retry_cnt===l_root_passwd_retry_cnt:$l_root_passwd_retry_cnt=l_port_idx:$l_port_idx===
    kecho ===create g_expect_list:$g_expect_list=l_cur_rcdpasswd:$l_cur_rcdpasswd=l_cur_rootpasswd:$l_cur_rootpasswd=l_cur_port:$l_cur_port===
    if [ "$l_runcmd" = "ssh" ]; then
      echo $l_runcmd -p $l_cur_port $*  > $g_expect_cmd
    elif [ "$l_runcmd" = "scp" ]; then
      echo $l_runcmd -P $l_cur_port $*  > $g_expect_cmd
    else
      kecho unknow cmd..
      aexit_f
    fi
    cat >$g_expect_list <<EOF
#!/usr/bin/expect -f
set timeout $g_expect_timeout

spawn bash $g_expect_cmd
expect {
  "yes\no" {send "yes\n"; exp_continue }
  "yes/no" {send "yes\n"; exp_continue }
  "$g_rcdname@*password:" {send "$l_cur_rcdpasswd\n"; exp_continue }
  "$g_rcdname@*:~" {send "su\n"}
  "100%" { expect eof; exit }
  "lost connection" { expect eof; exit }
}

expect {
  "$g_rcdname@*:~" {send "su\n"; exp_continue}
  "Password:" { send "$l_cur_rootpasswd\n"; exp_continue }
  "su: Authentication failure" { send "exit\n"; expect eof; exit }
  "$g_rootname@*:*#" {send "\n"}
}

if { [file exist $g_expect_run] == 1 } {
  set timeout -1
  send "bash $g_expect_run\n"
  expect "$g_expect_run_finish_msg"
  expect "$g_rootname@*:"
  send "\[ -f $g_expect_runfile \] && rm -f $g_expect_runfile\n"
  send "rm -f $g_expect_run\n"
  expect "$g_rootname@*:"
  set timeout $g_expect_timeout
  send "exit\n"
  expect "$g_rcdname@*:"
  send "exit\n"
  expect eof
} else {
  interact
}
EOF
    if [ -f $g_expect_list ]; then
      /usr/bin/expect -f $g_expect_list | tee $g_expect_log 2>&1
      if [ -f $g_expect_log ]; then
        #if tail -n 10 $g_expect_log | grep -A 1 "rcd@$l_remote_ip" | grep "Permission denied, please try again">/dev/null; then
        if tail -n 10 $g_expect_log | grep "Permission denied, please try again">/dev/null; then
          l_rcd_passwd_retry_cnt=$(($l_rcd_passwd_retry_cnt+1))
          if [ $l_rcd_passwd_retry_cnt -ge ${#l_rcdpasswd[@]} ]; then
            kecho rcd passwd is wrong
            aexit_f
          fi
          l_passwd_idx=$(( ($l_passwd_idx + 1) % ${#l_rcdpasswd[@]} ))
          l_cur_rcdpasswd=${l_rcdpasswd[$l_passwd_idx]}
          l_cur_rootpasswd=${l_rootpasswd[${l_passwd_idx}]}
          echo $l_cur_rcdpasswd > $l_rcd_passwd_cachefile
          echo $l_cur_rootpasswd > $l_root_passwd_cachefile
        elif tail -n 10 $g_expect_log | grep "su: Authentication failure">/dev/null; then
          l_root_passwd_retry_cnt=$(($l_root_passwd_retry_cnt+1))
          if [ $l_root_passwd_retry_cnt -ge ${#l_rootpasswd[@]} ]; then
            [ -f $l_port_cachefile ] && rm -f $l_port_cachefile
            kecho root passwd is wrong
            aexit_f
          fi
          l_passwd_idx=$(( ($l_passwd_idx + 1) % ${#l_rootpasswd[@]} ))
          l_cur_rootpasswd=${l_rootpasswd[${l_passwd_idx}]}
          echo $l_cur_rootpasswd > $l_root_passwd_cachefile
        elif tail -n 10 $g_expect_log | grep "^ssh: connect to host " | grep "Connection refused">/dev/null; then
          l_port_idx=$(($l_port_idx + 1))
          if [ $l_port_idx -ge ${#l_port[@]} ]; then
            kecho Can not connect, port error.
            aexit_f
          fi
          l_cur_port=${l_port[$l_port_idx]}
          echo $l_cur_port > $l_port_cachefile
        elif tail -n 10 $g_expect_log | grep -B 5 "^Host key verification failed" | grep "ssh-keygen -f">/dev/null; then
          kecho Host key verification failed, remove and retry.
          ssh-keygen -R [$l_remote_ip]:$l_cur_port
        elif tail -n 10 $g_expect_log | grep "^ssh: connect to host " | grep "No route to host">/dev/null; then
          kecho Can not connect, check ip.
          aexit_f
        else
          kecho run /usr/bin/expect -f $g_expect_list finish.
          break
        fi
      else
        kecho unknow error
        aexit_f
      fi
    else
      kecho unknow error
      aexit_f
    fi
  done
}

function ascp_f()
{
  local l_param1=$1
  local l_param2=$2
  local l_local_file
  local l_username
  local l_remote_ip
  local l_remote_path
  if [ -z "$l_param1" ] || [ -z "$l_param2" ]; then
    kecho input param invalue.
    aexit_f
  fi
  if echo $l_param1 | egrep -o "(\b([1-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-4])\.\b){3}\b([1-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-4])\b">/dev/null; then
    kecho cp from remote.
    l_local_file=${l_param2}
    l_username=${l_param1%%@*}
    l_remote_ip=${l_param1#*@}
    l_remote_ip=${l_remote_ip%%:*}
    l_remote_path=${l_param1#*:}

    if [ "${l_username}" = "${g_rcdname}" ]; then
      aexpect_f ${l_remote_ip} scp -r ${g_rcdname}@${l_remote_ip}:${l_remote_path} ${l_local_file}
    else
      echo tar cjf ${g_expect_tmp_tarfile} -C "$(dirname ${l_remote_path})" "$(basename ${l_remote_path})" > ${g_expect_tmp_untar_script}
      echo chmod 777 ${g_expect_tmp_tarfile} >> ${g_expect_tmp_untar_script}
      arun_f ${l_remote_ip} ${g_expect_tmp_untar_script}
      aexpect_f ${l_remote_ip} scp -r ${g_rcdname}@${l_remote_ip}:${g_expect_tmp_tarfile} ${g_expect_tmp_tarfile}
      tar xjf ${g_expect_tmp_tarfile} -C ${l_local_file}
      echo rm -f ${g_expect_tmp_tarfile} > ${g_expect_tmp_untar_script}
      arun_f ${l_remote_ip} ${g_expect_tmp_untar_script}
      rm -f ${g_expect_tmp_tarfile}
      rm -f ${g_expect_tmp_untar_script}
    fi

  elif echo $l_param2 | egrep -o "(\b([1-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-4])\.\b){3}\b([1-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-4])\b">/dev/null; then
    kecho cp to remote.
    l_local_file=${l_param1}
    l_username=${l_param2%%@*}
    l_remote_ip=${l_param2#*@}
    l_remote_ip=${l_remote_ip%%:*}
    l_remote_path=${l_param2#*:}

    if [ "${l_username}" = "${g_rcdname}" ]; then
      aexpect_f ${l_remote_ip} scp -r ${l_local_file} ${g_rcdname}@${l_remote_ip}:${l_remote_path}
    else
      tar cjf ${g_expect_tmp_tarfile} -C "$(dirname ${l_local_file})" "$(basename ${l_local_file})"
      aexpect_f ${l_remote_ip} scp -r ${g_expect_tmp_tarfile} ${g_rcdname}@${l_remote_ip}:${g_expect_tmp_tarfile}
      rm -f ${g_expect_tmp_tarfile}
      echo tar xjf ${g_expect_tmp_tarfile} -C ${l_remote_path} > ${g_expect_tmp_untar_script}
      echo rm -f ${g_expect_tmp_tarfile} >> ${g_expect_tmp_untar_script}
      arun_f ${l_remote_ip} ${g_expect_tmp_untar_script}
    fi

    kecho cp to remote finish.
  else
    kecho param invalue.
    return
  fi
}

function assh_f()
{
  local l_remote_ip=$1
#  local l_username=${l_remote_ip%%@*}
  l_remote_ip=${l_remote_ip#*@}

#  if [ "$l_username" = "${g_rcdname}" ]; then
    aexpect_f ${l_remote_ip} ssh ${g_rcdname}@${l_remote_ip}
#  else
#    aexpect_f ${l_remote_ip} ssh ${g_rootname}@${l_remote_ip}
#  fi
}

function arun_f()
{
  local l_remote_ip=$1
  shift 1
  local l_cmd_file="$*"
  kecho remote run script
  if [ -n "$l_cmd_file" ]; then
    if [ -f "$l_cmd_file" ]; then
      cp $l_cmd_file $g_expect_runfile
      ascp_f $g_expect_runfile ${g_rcdname}@$l_remote_ip:$g_expect_runfile
      echo "bash $g_expect_runfile" > $g_expect_run
    elif [ -n "$l_cmd_file" ]; then
      echo "$l_cmd_file" > $g_expect_run
   fi
    echo "echo $g_expect_run_finish_msg" >> $g_expect_run
    ascp_f $g_expect_run ${g_rcdname}@$l_remote_ip:$g_expect_run
    assh_f $l_remote_ip $g_expect_run
    rm -f $g_expect_runfile
    rm -f $g_expect_run
  else
    assh_f $l_remote_ip
  fi
  kecho remote run script finish.
}

case `basename $0` in
  ascp)
  ascp_f $*
  ;;
  assh)
  assh_f $*
  ;;
  arun)
  arun_f $*
  ;;
  *)
  rm -f /bin/ascp /bin/assh /bin/arun
  ln -s `realpath $0` /bin/ascp
  ln -s `realpath $0` /bin/assh
  ln -s `realpath $0` /bin/arun
  ;;
esac
rm -rf ${g_expect_pre}*
exit 0
