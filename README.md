# dwm - dynamic window manager

dwm is an extremely fast, small, and dynamic window manager for X.

## Requirements

In order to build dwm you need the Xlib header files.

## Installation

If you use nix than just use it as any other flake.
Otherwise

```
./patch.sh
cd patched
make clean install
```

## Running dwm

Add the following line to your .xinitrc to start dwm using startx:

```
exec dwm
```

In order to connect dwm to a specific display, make sure that
the DISPLAY environment variable is set correctly, e.g.:

```
DISPLAY=foo.bar:1 exec dwm
```

(This will start dwm on display :1 of the host foo.bar.)

In order to display status info in the bar, you can do something
like this in your .xinitrc:

```
while xsetroot -name "`date` `uptime | sed 's/.*,//'`"
do
  sleep 1
done &
exec dwm
```

## Applied patches

### Community

Some of those was modified to fit new version or others:

- smartborders
- deck-double
- functionalgaps+pertag
- activetagindicatorbar
- attachdirection
- awesomebar
- xresources
- focusmaster
- swapfocus
- centeredwindowname

Those were modified more significantly:
- taglayouts - by default introduces uninitialized second layout by 
  moving initialization to the end of function; this was simply removed 
  and it works now
- unfloatvisible - moved code from separate file to `dwm.c`
- push_no_master - to work in both master and client area but not cross 
  boundary between them

### Mine

- fixes:
  * make resize calls correct
  * fix segfaults on:
    + empty bar clicked
- additions:
  * loading Xresources at runtime
  * just in case line for mediakeys patch for slock
  * toggle hidden state of window
