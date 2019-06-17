#!/bin/sh

test -d build
if [ "$?" = "0" ]; then
	rm -rf build
fi

if [ "$1" = "test" ]; then
	cmake -H. -Bbuild -DBUILD_TESTS=ON && \
	cmake --build build --target all \
	&& cmake --build build --target test || die Error!
else
	cmake -H. -Bbuild -DCMAKE_INSTALL_PREFIX=testing && \
	cmake --build build --target all
fi
