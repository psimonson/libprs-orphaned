#!/bin/sh
find ./ -iname "*.bak" -exec rm {} \;
rm -rf *~ include/*~ src/*~ test/*~ test/sockhelp/*~ test/test.log build/ \
doc/ testing/ test/test7.txt
