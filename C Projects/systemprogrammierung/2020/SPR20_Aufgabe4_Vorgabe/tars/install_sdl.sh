#!/bin/bash

## install sdl
INSTALL_DIR=${1:-$HOME/SDL}

tar xzvf tars/SDL2-2.0.12.tar.gz
(mkdir SDL2-2.0.12/build; cd SDL2-2.0.12/build;../configure --prefix=$INSTALL_DIR;make -j8;make -j8 install)

tar xzvf tars/SDL2_image-2.0.5.tar.gz
(mkdir SDL2_image-2.0.5/build; cd SDL2_image-2.0.5/build; ./autogen.sh; ../configure --prefix=$INSTALL_DIR; make -j8; make -j8 install)
echo 'export PATH=$PATH:~/SDL/bin/' >> ~/.bashrc 
