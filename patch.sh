#!/usr/bin/env sh

set -e pipefail

SCRIPT_DIR="$(readlink -f "$0")"
SCRIPT_DIR="${SCRIPT_DIR%/*}"
PATCH_DIR="$SCRIPT_DIR/patches"
SRC="$SCRIPT_DIR/src"
PATCHED="$SCRIPT_DIR/patched"

rm -fr "$PATCHED"
cp -r "$SRC" "$PATCHED"
cd "$PATCHED"

# dwm-activemonitor-20230825-e81f17d.diff
# dwm-attachdirection-6.2.diff
# dwm-awesomebar-20230431-6.4.diff
# dwm-deck-double-smartborders-6.2.diff
# dwm-fancybar-20220527-d3f93c7.diff
# dwm-pertag_with_sel-20231003-9f88553.diff
# dwm-xresources-20210827-138b405.diff
# dwm-xtile-6.2.diff
# togglebartag-20230421-e81f17d.diff

for patch in \
    dwm-functionalgaps+pertag-6.2.diff \
    dwm-activetagindicatorbar-6.2.diff \

do
    patch -p1 <"$PATCH_DIR/$patch"
done

cp "$SCRIPT_DIR/config.h" "$PATCHED"
if [ "$(uname)" = "FreeBSD" ]; then
    cp "$SCRIPT_DIR/config.bsd.mk" "$PATCHED/config.mk"
fi
