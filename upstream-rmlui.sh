#!/bin/bash

set -e

trap "{ if [ -d '_rmlui' ]; then rm -rf _rmlui; fi; exit 255; }" SIGINT SIGTERM ERR EXIT

RMLUI='5.1'

if [ -d RmlUi/Source/Godot ]; then
	cp -r RmlUi/Source/Godot _Godot
fi

rm -rf _rmlui

git clone -b $RMLUI --depth=1 --recursive --single-branch https://github.com/mikke89/RmlUi _rmlui

rm -rf RmlUi
mkdir RmlUi

mv _rmlui/README.md RmlUi/
mv _rmlui/changelog.md RmlUi/changelog.md
mv _rmlui/Source RmlUi/
mv _rmlui/Include RmlUi/
mv _Godot RmlUi/Source/Godot

rm -rf _rmlui

if [ -f patch.txt ]; then
	echo "**"
	echo "** PATCHING .."
	echo "**"

	patch -p 1 < patch.txt
fi

echo '#include "thirdparty/misc/c++/flat_map.h"' > RmlUi/Include/RmlUi/Core/Containers/itlib/flat_map.hpp
echo '#include "thirdparty/misc/c++/flat_set.h"' > RmlUi/Include/RmlUi/Core/Containers/itlib/flat_set.hpp
