#!/bin/sh

LIB_DIR=libs
INSTALL_DIR=include

test -d $INSTALL_DIR || mkdir -p $INSTALL_DIR

cp -r $LIB_DIR/cpputils/include/cpputils $INSTALL_DIR
cp -r $LIB_DIR/gsl/include/ $INSTALL_DIR/gsl/
cp -r $LIB_DIR/range-v3/include/meta $INSTALL_DIR
cp -r $LIB_DIR/range-v3/include/range $INSTALL_DIR
