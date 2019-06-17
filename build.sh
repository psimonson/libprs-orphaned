#!/bin/sh

test -d build
if [ "$?" = "0" ]; then
	rm -rf build
fi

#cmake -H. -Bbuild -DBUILD_TESTS=ON && cmake --build build --target all \
#&& cmake --build build --target test || die Error!

cmake -H. -Bbuild && cmake --build build --target all
