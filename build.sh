#!/bin/sh
test -d build
if [ "$?" = "0" ]; then
	rm -rf build
fi
install_dir=-DCMAKE_INSTALL_PREFIX=testing
if [ ! "$1" = "" ]; then
	install_dir=-DCMAKE_INSTALL_PREFIX=$1
fi
if [ "$2" = "test" ]; then
	cmake -H. -Bbuild -DBUILD_TESTS=ON $install_dir && \
	cmake --build build --target all \
	&& cmake --build build --target test || die Error!
elif [ "$2" = "win32-test" ]; then
	cmake -H. -Bbuild -DBUILD_TESTS=ON \
	-DCMAKE_TOOLCHAIN_FILE=Ubuntu-mingw_w64-toolchain.cmake && \
	cmake --build build --target all && \
	cmake --build build --target test || exit
elif [ "$2" = "win32" ]; then
	cmake -H. -Bbuild \
	-DCMAKE_TOOLCHAIN_FILE=Ubuntu-mingw_w64-toolchain.cmake && \
	cmake --build build --target all || exit
else
	cmake -H. -Bbuild $install_dir && \
	cmake --build build --target all
fi
