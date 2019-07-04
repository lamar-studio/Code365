#!/bin/bash

nb_display=$(xrandr -q |grep -w connected| wc -l)

if [ "$(hostname -s)" == "rennllxrdl3765" ]; then
    if [ $nb_display -eq 3 ]; then
	xrandr --output HDMI1 --off --output VGA1 --off --output eDP1 --off
        xrandr --output DP1   --auto --primary \
               --output HDMI3 --auto --right-of DP1
    elif [ $nb_display -eq 2 ]; then
        xrandr --output DP1 --off --output DP2 --off --output HDMI3 --off --output VGA1 --off
        xrandr --output eDP1  --auto --primary \
               --output DP2 --auto --right-of eDP1
    else
        xrandr --output DP1 --off --output DP2 --off --output HDMI3 --off --output VGA2 --off
        xrandr --output eDP1  --auto --primary
    fi
elif [ "$(hostname -s)" == "RENNLLXRDL4480" ]; then
    if [ $nb_display -eq 3 ]; then
	xrandr -q |grep -w connected |grep -q DP-1-1
	if [ $? -eq 0 ]; then
	    xrandr --output eDP-1 --off --output HDMI-1 --off
	    xrandr --output DP-1-1 --auto --primary --preferred --output DP-1-2 --auto --right-of DP-1-1
	else
	    xrandr --output eDP-1 --off --output DP-1-1 --off --output HDMI-2 --off
	    xrandr --output HDMI-1 --auto --primary --preferred --output DP-1 --auto --right-of HDMI-1
	fi
    elif [ $nb_display -eq 2 ]; then
	xrandr -q |grep -w connected |grep -q DP-1-1
	if [ $? -eq 0 ]; then
	    xrandr --output HDMI-1 --off --output DP-1-2 --off
	    xrandr --output eDP-1 --auto --primary --preferred --output DP-1-1 --auto --right-of eDP-1
	else
	    xrandr --output DP-2 --off --output HDMI-2 --off
	    xrandr --output eDP-1 --auto --primary --preferred --output HDMI-1 --auto --right-of eDP-1
	fi
    else
	xrandr --output DP-1 --off --output HDMI-1 --off --output DP-2 --off --output HDMI-2 --off
	xrandr --output eDP-1 --auto --primary --preferred
    fi
elif [ "$(hostname -s)" == "renndlxrdp0201" ]; then
    if [ $nb_display -eq 2 ]; then
	xrandr --output HDMI1 --mode 1920x1080 --primary --preferred --output VGA1 --mode 1680x1050 --right-of HDMI1
    else
	xrandr --output HDMI1 --mode 1920x1080 --primary --preferred --output VGA1 --off
    fi
else
    # home
    xrandr --output HDMI-0 --auto --primary --preferred --output DVI-D-0 --auto --right-of HDMI-0
fi
