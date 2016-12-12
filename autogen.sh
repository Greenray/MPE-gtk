#!/bin/bash

rm -rf autom4te.cache aclocal.m4 configure config.log config.status stamp-h1 Makefile Makefile.in src/.deps src/Makefile src/Makefile.in po/Makefile
touch NEWS README AUTHORS ChangeLog
gettextize -c -f
autoheader -f --warnings=all
aclocal -I m4
autoconf -f --warnings=all
autoupdate
automake --add-missing --copy -a
./configure $@
