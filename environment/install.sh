#!/bin/sh

##---------------------------------------------------------------------------##
## Install YAYP environment
##
## install.sh <dir> where dir is the directory to install the shell script
##---------------------------------------------------------------------------##

# current directory
current=$(dirname $0 | pwd)

# find pythonpath
PYTHON=`which python`

# install directory
prefix=$1

if test -z $prefix; then
  echo "usage: $0 install_dir"
  exit 1
fi

# if install directory exists remove it
remove='remove'
if test -d "$prefix"; then
  echo ">>> Found $prefix, remove or exit [remove]"
  read remove
  if test "$remove" == 'exit'; then
    echo ">>> Exiting, to finish install, manually remove $prefix, or"
    echo "    choose a different install location"
    exit 0
  else
    echo ">>> Removing $prefix"
    rm -rf $prefix
  fi
fi

# Make the prefix directory
echo ">>> Making $prefix"
mkdir $prefix

# Copy python directory recursively
echo ">>> Installing python scripts in $prefix/python"
cp -rp python $prefix/python

# Install binaries
echo ">>> Installing bin in $prefix/bin"
cp -rp bin $prefix/bin

echo
echo "========================================================================"
echo "Congratulations, you're almost finished the YAYP environment install!"
echo
echo "For complete operation you should set the following"
echo "environment variables (depending upon your shell):"
echo
echo " - Add $prefix/bin to PATH"
echo "   e.g. export PATH=\$PATH:$prefix/bin"
echo "========================================================================"

##---------------------------------------------------------------------------##
## end of install.sh
##---------------------------------------------------------------------------##
