
Instructions
--------
This is a systemd service to run pulseaudio in system-wide mode.

You can use it on Ubuntu. enjoy it


Step
-------

1. add follow cmd on the preinst
```
getent group pulse &>/dev/null || groupadd -r pulse >/dev/null
getent group pulse-access &>/dev/null || groupadd -r pulse-access >/dev/null
getent user pulse &>/dev/null || useradd -r pulse -d /var/run/pulse -g pulse -G audio -c "PulseAudio" -s /bin/false >/dev/null
adduser <user> pulse-access
adduser root pulse-access
```

2. pulse-systemd script
```
cp pulsePost /usr/share/alsa/init
cp pulseaudio.service /lib/systemd/system/
systemctl enable pulseaudio
```

3. disable the pulse default daemon
```
autospawn = no
daemon-binary = /usr/bin/pulse
; extra-arguments = --log-target=syslog
```
