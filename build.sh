#!/bin/sh
test -d build
if [ "$?" = "0" ]; then
	rm -rf build
fi

if [ "$1" = "test" ]; then
	cmake -H. -Bbuild -DBUILD_TESTS=ON && \
	cmake --build build --target all \
	&& cmake --build build --target test || die Error!
elif [ "$1" = "win32" ]; then
	cmake -H. -Bbuild \
	-DCMAKE_TOOLCHAIN_FILE=Ubuntu-mingw_w64-toolchain.cmake && \
	cmake --build build --target all || exit
else
	cmake -H. -Bbuild -DCMAKE_INSTALL_PREFIX=testing && \
	cmake --build build --target all
fi
