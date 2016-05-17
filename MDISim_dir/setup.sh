#!/bin/bash
#
# setup.sh for  MDISim distribution version

#export MDISim_dir=/Users/francesco/MDISim_dir
#export MDISim_dir=$PWD/MDISim_dir
#echo export MDISim_dir=$MDISim_dir

if [ -e "$ROOTSYS/bin/root" ] ; then
  echo "ok, root is installed"
else
  echo "root must be set up first, see https://root.cern.ch"
exit
fi

echo "MDISim is setup up"
echo "to use MDISim execute as first command in root:"
echo .x $MDISim_dir/MDISim_init.C
# echo StartEveWithGeomDisplay("http://hbu.web.cern.ch/hbu/Geom/LHC_IR_5.root",false,2);
# echo StartEveWithGeomDisplay("http://hbu.web.cern.ch/hbu/Geom/LHC_IR_5_LR_eve.root",false,2);
