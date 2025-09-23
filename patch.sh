#!/usr/bin/env sh

set -e pipefail

SCRIPT_DIR="$(readlink -f "$0")"
SCRIPT_DIR="${SCRIPT_DIR%/*}"
PATCH_DIR="$SCRIPT_DIR/patches"
SRC="$SCRIPT_DIR/src"
PATCHED="$SCRIPT_DIR/patched"

mkdir -p "$PATCHED"
rm -fr "${PATCHED:?}"/*
cp -r "$SRC"/* "$PATCHED"
cd "$PATCHED"

# dwm-pertag_with_sel-20231003-9f88553.diff \

for patch in \
    dwm-smartborders-6.2.diff \
    dwm-deck-double-smartborders-6.2.diff \
    dwm-functionalgaps+pertag-6.2.diff \
    dwm-taglayouts-6.4.diff \
    dwm-activetagindicatorbar-6.2.diff \
    dwm-attachdirection-6.2.diff \
    dwm-awesomebar-20230431-6.4.diff \
    dwm-xresources-20210827-138b405.diff \
    dwm-focusmaster-20210804-138b405.diff \
    dwm-swapfocus-20160731-56a31dc.diff \
    dwm-centeredwindowname.diff \
    dwm-swapmonitors-20250509-4cd2832.diff \
    dwm-fixes-6.6.diff \
    dwm-additions-6.6.diff \

    # prints.diff \

do
    echo "Applying $patch"
    patch -p1 <"$PATCH_DIR/$patch"
done

cp "$SCRIPT_DIR/config.h" "$PATCHED"
if [ "$(uname)" = "FreeBSD" ]; then
    cp "$SCRIPT_DIR/config.freebsd.mk" "$PATCHED/config.mk"
fi
