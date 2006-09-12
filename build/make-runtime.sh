#!/bin/bash

RUNTIMEDIR="./"
ALL_LINGUAS="nl de eo pl da es zh_TW ru fr pt_BR"

if [ "x$1" != "x" ]; then
    if [ -e $1 ]; then
        ALL_LINGUAS=`grep ALL_LING $1/configure.ac | grep -v "for a in" | sed -e 's/^ALL_LINGUAS="\([a-zA-Z_ ]*\)"/\1/g'`
        echo $ALL_LINGUAS
    fi
fi

. $HOME/bin/mingw32

mkdir -p $RUNTIMEDIR/runtime-base
mkdir -p $RUNTIMEDIR/runtime-gtk

## Helper

function copy_dir()
{
    sourcedir=$1;
    source=$2
    dest=$3;

    prefix=`dirname $source`
    mkdir -p $TARGETDIR/$dest/$prefix
    cp -a $CROSSROOT/$sourcedir/$source $TARGETDIR/$dest/$prefix
}

## Base runtime

TARGETDIR=$RUNTIMEDIR/runtime-base 

copy_dir bin     gnet-2.0.dll                                   lib

find $TARGETDIR -name "*.dll" -print | xargs $STRIP

## Gtk runtime

if [ -d $CROSSROOT/lib/gtk-2.0/2.4.0 ]; then
    GTKVER=2.4.0
elif [ -d $CROSSROOT/lib/gtk-2.0/2.10.0 ]; then
    GTKVER=2.10.0
fi

if [ -d $CROSSROOT/lib/pango/1.4.0 ]; then
    PANVER=1.4.0
elif [ -d $CROSSROOT/lib/pango/1.5.0 ]; then
    PANVER=1.5.0
fi

TARGETDIR=$RUNTIMEDIR/runtime-gtk

copy_dir  etc    gtk-2.0                                        etc
copy_dir  etc    pango						etc
copy_dir  bin    zlib1.dll					lib
copy_dir  bin    iconv.dll					lib
copy_dir  bin    intl.dll                           		lib
copy_dir  bin    libpng13.dll                         		lib
#copy_dir  bin    libpng12.dll                         		lib
copy_dir  bin    libatk-1.0-0.dll                   		lib
copy_dir  bin    libgdk-win32-2.0-0.dll             		lib
copy_dir  bin    libgdk_pixbuf-2.0-0.dll            		lib
copy_dir  bin    libglib-2.0-0.dll                  		lib
copy_dir  bin    libgmodule-2.0-0.dll               		lib
copy_dir  bin    libgobject-2.0-0.dll               		lib
copy_dir  bin    libgthread-2.0-0.dll               		lib
copy_dir  bin    libgtk-win32-2.0-0.dll             		lib
copy_dir  bin    libpango-1.0-0.dll                 		lib
copy_dir  bin    libpangoft2-1.0-0.dll              		lib
copy_dir  bin    libpangowin32-1.0-0.dll            		lib
copy_dir  bin    libpangocairo-1.0-0.dll                        lib
copy_dir  bin    libcairo-2.dll                                 lib

copy_dir  lib    gtk-2.0/$GTKVER/immodules/*.dll                   lib
copy_dir  lib    gtk-2.0/$GTKVER/loaders/libpixbufloader-ico.dll   lib 
copy_dir  lib    gtk-2.0/$GTKVER/loaders/libpixbufloader-png.dll   lib
copy_dir  lib    gtk-2.0/$GTKVER/loaders/libpixbufloader-pnm.dll   lib

if [ -f $CROSSROOT/lib/gtk-2.0/$GTKVER/engines/libwimp.dll ]; then
    copy_dir  lib    gtk-2.0/$GTKVER/engines/libwimp.dll        lib
    copy_dir  share  themes/*   share
fi

copy_dir  lib    pango/$PANVER/modules      			lib

for lang in $ALL_LINGUAS; do
    copy_dir lib locale/$lang lib  
done

find $TARGETDIR -name "*.dll" -not -name "iconv.dll" -not -name "intl.dll" -print | xargs $STRIP
