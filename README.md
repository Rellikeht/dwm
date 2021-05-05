# my-dwm
My build of dwm
It has many patches applied

# Requirements
In order to build dwm you need the Xlib header files.

# Installation
Edit config.mk to match your local setup (dwm is installed into
the /usr/local namespace by default).

Afterwards enter the following command to build and install dwm (if
necessary as root):

    make clean install

# Running dwm
Add the following line to your .xinitrc to start dwm using startx:

    exec dwm

In order to connect dwm to a specific display, make sure that
the DISPLAY environment variable is set correctly, e.g.:

    DISPLAY=foo.bar:1 exec dwm

(This will start dwm on display :1 of the host foo.bar.)

In order to display status info in the bar, you can do something
like this in your .xinitrc:

    while xsetroot -name "`date` `uptime | sed 's/.*,//'`"
    do
    	sleep 1
    done &
    exec dwm

# Configuration
The configuration of dwm is done by creating a custom config.h
and (re)compiling the source code.

# Patches:
- activetagindicator
- attachdirection
- awesomebar
- deck-double
- fullgaps
- pertag
I don't know if these are all xd

# Programs used (bind to keys):
- dmenu
- st
- tabbed
- urxvt
- alacritty
- xrandr
- xclip
- xmessage
- xlock
- xmodmap
- xclip
- doas (much friendlier than sudo,
used for workaround to suspend computer)
- i3lock-fancy
- elogind (loginctl command, I don't use systemd)
- dzen2
- rofi
- qutebrowser
- pass (passmenu)
- amixer (it is hard to make it work)
- pactl (it is slightly easier than alsa)
and shell scripts in ~/.scrs, because it is sometimes easier
to bind script than single command

# Some info
All xmodmap files are separate keyboard layouts

.xinitrc (.xinitrc_dwm), and scripts used are in my dotfiles
repository I wanted to have modes like in i3, i wrote one
for moc with dzen2.

I have problems with NVIDIA drivers, when I suspend computer
I get black screen and it doesn't react to keyboard, hence
long command to suspend.
