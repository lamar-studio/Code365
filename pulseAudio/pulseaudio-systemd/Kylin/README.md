
Instructions
--------
This is a systemd service to run pulseaudio in system-wide mode.

You can use it on Ubuntu. enjoy it


Step
-------

## 添加如下脚本到preinst
```
getent group pulse &>/dev/null || groupadd -r pulse >/dev/null
getent group pulse-access &>/dev/null || groupadd -r pulse-access >/dev/null
getent user pulse &>/dev/null || useradd -r pulse -d /var/run/pulse -g pulse -G audio -c "PulseAudio" -s /bin/false >/dev/null
adduser <user> pulse-access
adduser root pulse-access
```

## pulse守护脚本
```
cp pulsePost /usr/share/alsa/init
cp pulseaudio.service /lib/systemd/system/
systemctl enable pulseaudio
```

## 按如下修改/etc/pulse/client.conf 文件
```
autospawn = no
daemon-binary = /usr/bin/pulse
; extra-arguments = --log-target=syslog
```
