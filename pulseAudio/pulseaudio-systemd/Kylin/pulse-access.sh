#!/bin/bash
getent group pulse &>/dev/null || groupadd -r pulse >/dev/null
getent group pulse-access &>/dev/null || groupadd -r pulse-access >/dev/null
getent user pulse &>/dev/null || useradd -r pulse -d /var/run/pulse -g pulse -G audio -c "PulseAudio" -s /bin/false >/dev/null
#adduser <user> pulse-access
adduser rcd pulse-access
adduser root pulse-access

