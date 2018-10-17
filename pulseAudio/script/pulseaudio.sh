#!/bin/sh

start()
{
  if /usr/bin/pulseaudio --check; then
    echo "pulseaudio is running."
  else
    echo "Starting pulseaudio..."
    /usr/bin/pulseaudio --start --log-target=syslog 
  fi
}

stop()
{
  if /usr/bin/pulseaudio --check; then
    printf "Stopping pulseaudio..."
    /usr/bin/pulseaudio --kill
    while /usr/bin/pulseaudio --check; do
      printf "."
      sleep 1
    done
    echo "Done"
  else
    echo "pulseaudio is not running."
  fi
}

status()
{
  if /usr/bin/pulseaudio --check; then
    echo "pulseaudio is running."
  else
    echo "pulseaudio is not running."
  fi
}

case "$1" in
'start')start;;
'stop')stop;;
'restart')stop;start;;
'status')status;;
*)echo "$0 start|stop|restart|status"
esac
