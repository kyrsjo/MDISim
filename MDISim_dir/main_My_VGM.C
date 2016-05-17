/*
 ~/c/MDISim/main_My_VGM.C      H. Burkhardt  01/07/2014   to some extend inspired by  ~/c2/vgm/examples/E04/rootgeom.cxx



      https://root.cern.ch/drupal/content/geant4-vmc
      https://root.cern.ch/drupal/content/magnetic-field   magnetic field in VMC


 vmc comes with root

 ./montecarlo/vmc/inc/TVirtualMC.h:   //                  - ifield = 2 if tracking performed with g3helix;
 ./montecarlo/vmc/inc/TVirtualMCGeometry.h:   //  performed with g3helix; ifield = 3 if tracking performed with g3helx3.
 ./montecarlo/vmc/src/TGeoMCGeometry.cxx:  //  performed with g3helix; ifield = 3 if tracking performed with g3helx3.
 ./montecarlo/vmc/src/TGeoMCGeometry.cxx:  //  performed with g3helix; ifield = 3 if tracking performed with g3helx3.

 cd ~/root_git/src   ; find . -type f -exec grep -H "THelix" {} \;




 rm -rf "/tmp/$LOGNAME/Release"* ; mkdir /tmp/$LOGNAME/Release ; cd /tmp/$LOGNAME/Release ; cmake ~/c/MDISim ; make My_VGM ; /tmp/$LOGNAME/Release/My_VGM


 cd ~/c/MDISim/ ; make clean ; make My_VGM -j5 ; ./My_VGM


 OTHER_LDFLAGS = $(ROOT_LIBS) $(HOME)/ROOT/5.34.18/lib/libCint.so -L$(HOME)/c2/vgm/lib/ -lRootGM -lXmlVGM
 OTHER_LDFLAGS = $(ROOT_LIBS) -L$(HOME)/c2/vgm/lib/ -lRootGM -lXmlVGM
 OTHER_CPLUSPLUSFLAGS = $(inherited) -I$(HOME)/c2/vgm/include

 unset DYLD_LIBRARY_PATH

 source ~/bin/my_Xcode_env.sh clang 1 root5 ; export DYLD_LIBRARY_PATH=$ROOTSYS/lib:$HOME/c2/vgm/lib
 cd /tmp/$LOGNAME ; xcodebuild -project ~/c/MDISim/MDISim.xcodeproj -target My_VGM -configuration Release ; Release/My_VGM



 ok when linkd without XmlVGM
 clang++ -std=c++11 -stdlib=libc++ -lstdc++ -v -o My_VGM main_My_VGM.o `root-config --libs` -lGeom -L/Users/hbu/c2/vgm/lib/ -lRootGM




 clang++ -std=c++11 -stdlib=libc++ -lstdc++ -v -o My_VGM main_My_VGM.o -L/Users/hbu/ROOT/5.34.18/lib -lEve -lEG -lTreePlayer -lGeom -lGed -lRGL -lGui -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -stdlib=libc++ -lm -ldl -L/Users/hbu/c2/vgm/lib/ -lRootGM -lXmlVGM

 clang++ -std=c++11 -stdlib=libc++ -lstdc++ -v -o My_VGM main_My_VGM.o -L/Users/hbu/c2/vgm/lib/ -lRootGM -lXmlVGM -L/Users/hbu/ROOT/5.34.18/lib -lEve -lEG -lTreePlayer -lGeom -lGed -lRGL -lGui -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -stdlib=libc++ -lm -ldl

 next is enough for link, still same problem
 clang++ -std=c++11 -stdlib=libc++ -lstdc++ -v -o My_VGM main_My_VGM.o -L/Users/hbu/c2/vgm/lib/ -lRootGM -lXmlVGM `root-config --libs` -lGeom


 with minimum root libs -   still same problem
 clang++ -std=c++11 -stdlib=libc++ -lstdc++ -v -o My_VGM main_My_VGM.o -L/Users/hbu/c2/vgm/lib/ -lRootGM -lXmlVGM -L/Users/hbu/ROOT/5.34.18/lib -lCore -lGeom






 problems libpng .. come with
 XmlVGM      ~/c2/vgm/lib/libXmlVGM.dylib

 libpng warning: Application built with libpng-1.2.50 but running with 1.6.12

 try with, still same
 export DYLD_LIBRARY_PATH=$HOME/c2/vgm/lib:$ROOTSYS/lib


 the lib is made by       goes first to     ~/c2/vgm_build/packages/XmlVGM/libXmlVGM.dylib

 cd /Users/hbu/c2/vgm_build/packages/XmlVGM && /opt/local/bin/cmake -E cmake_link_script CMakeFiles/XmlVGM.dir/link.txt --verbose=1
 /opt/local/bin/cmake -E cmake_progress_report /Users/hbu/c2/vgm_build/CMakeFiles 61


 less libs, ok with -undefined dynamic_lookup   but also with   libpng-1.2.50 but running with 1.6.12

 cd ~/c2/vgm_build/packages/XmlVGM ; /usr/bin/c++  -undefined dynamic_lookup  -O3 -Wall -std=c++11 -stdlib=libc++ -ftls-model=initial-exec -dynamiclib -Wl,-headerpad_max_install_names   -o libXmlVGM.dylib -install_name /Users/hbu/c2/vgm_build/packages/XmlVGM/libXmlVGM.dylib CMakeFiles/XmlVGM.dir/source/AGDDExporter.cxx.o CMakeFiles/XmlVGM.dir/source/AGDDWriter.cxx.o CMakeFiles/XmlVGM.dir/source/GDMLExporter.cxx.o CMakeFiles/XmlVGM.dir/source/GDMLWriter.cxx.o CMakeFiles/XmlVGM.dir/source/Maps.cxx.o CMakeFiles/XmlVGM.dir/source/utilities.cxx.o CMakeFiles/XmlVGM.dir/source/VExporter.cxx.o ../BaseVGM/libBaseVGM.dylib ../ClhepVGM/libClhepVGM.dylib

 cd ~/c2/vgm_build ; make install
 cd ~/c/MDISim/ ; make clean ; rm -f My_VGM ;  make My_VGM -j5 ; ./My_VGM



 I typically link like
 clang++ -std=c++11 -stdlib=libc++ -lstdc++ -dynamiclib -undefined dynamic_lookup -compatibility_version 0.0 -current_version 1.0 Ntuple.o MyTGCocoa.o -o /Users/hbu/lib/4.8/libMyRoot_clang.so


 libpng libraries are in
 /opt/local
 /opt/X11/lib   ------- here with some really old stuff -   try to get a cleaner /opt/X11

 find /opt/X11 -name "libpng*"
 /opt/X11/bin/libpng-config
 /opt/X11/bin/libpng15-config
 /opt/X11/include/libpng15
 /opt/X11/lib/libpng.3.dylib
 /opt/X11/lib/libpng.dylib
 /opt/X11/lib/libpng12.0.dylib
 /opt/X11/lib/libpng14.14.dylib
 /opt/X11/lib/libpng15.15.dylib
 /opt/X11/lib/libpng15.dylib
 /opt/X11/lib/pkgconfig/libpng.pc
 /opt/X11/lib/pkgconfig/libpng15.pc
 /opt/X11/share/man/man3/libpng.3
 /opt/X11/share/man/man3/libpngpf.3


 sudo rm -rf /opt/X11
 still same problem,  where is   libpng-1.2.50

 so the problem is really    the libpng  which comes with root
 ls -l ~/ROOT/5.34.18/src/root/graf2d/asimage/src/libAfterImage/libpng
 ls -l ~/root_git/graf2d/asimage/src/libAfterImage/libpng


 problem probably in how XmlVGM  is made, maybe using png from root
 appears with Draw("ogl");   otherwise ok !!!

 trials to get working in CINT failed, in line mode ok

 */


#if defined( __CINT__)  //   || defined(__CLING__)
.include /Users/hbu/c2/vgm/packages
#else
#include "RootGM/volumes/Factory.h"
#include "XmlVGM/AGDDExporter.h"
#include "XmlVGM/GDMLExporter.h"

#include "TApplication.h"
#include "TROOT.h"
#include "TInterpreter.h"
#include "TGeoManager.h"

#include "TSystem.h" // gSystem
#endif


#ifdef __CINT__
//void main_my_vgm() {
#else
int main(int argc,char** argv)
#endif
{
  #ifndef __CINT__
  TApplication theApp("otherwise it does not work", &argc, argv);
  #endif

  // gSystem->Load("libGeom");
  // TGeoManager::Import("http://root.cern.ch/files/alice2.root"); // works ok
  // TGeoManager::Import("http://hbu.web.cern.ch/hbu/Geom/TLEP_geom.root"); // segmentation violation
  // TGeoManager::Import("/Users/hbu/www/Geom/TLEP_geom.root");  // segmentation violation  -- maybe rembers ogl ??   -- or with threads ??
  // TGeoManager::Import("/Users/hbu/www/Geom/TLEP_geom_ROOT5.root"); // works ok
  TGeoManager::Import("/Users/hbu/www/Geom/LHC_IR_1.root"); // segmentation violation
  //TGeoManager::Import("/Users/hbu/www/Geom/CLIC_BDS.root"); // from 4/3/2014, ok


  // TGeoManager::Import("/Users/hbu/www/Geom/TLEP_geom.C"); // not a root file, cannot open

  // Load geometry in .C format  -- worked
  //gROOT->LoadMacro("/Users/hbu/www/Geom/TLEP_geom.C");
  //gInterpreter->ProcessLine("TLEP_geom()");

  // gGeoManager->DefaultColors();

  if(gGeoManager->GetTopVolume()==NULL) { std::cout << " *** Error *** top volume is NULL" << '\n'; exit(1); }

  std::cout << "gGeoManager->GetTopVolume()=" << gGeoManager->GetTopVolume() << std::endl;

  // Draw ROOT geometry
  // gGeoManager->SetVisLevel(4);
  // gGeoManager->GetTopVolume()->Draw(); //not needed

  // gGeoManager->GetTopVolume()->Draw("ogl");  // causes problems when used together with XmlVGM

  // import root geometry to VGM
  RootGM::Factory factory;
  factory.Import(gGeoManager->GetTopNode());

  std::cout << " root import to VGM done" << std::endl;

  // Export geometry to AGDD
  //
  XmlVGM::AGDDExporter xmlExporter1(&factory);
  xmlExporter1.GenerateXMLGeometry();
  std::cout << " export to AGDD done" << std::endl;

  // Export geometry to GDML
  //
  XmlVGM::GDMLExporter xmlExporter2(&factory);
  xmlExporter2.GenerateXMLGeometry();
  std::cout << " export to GDML done" << std::endl;

  gGeoManager->GetTopVolume()->Draw(); // canvas draw ok
  // gGeoManager->GetTopVolume()->Draw("ogl"); //try to draw after export - segmentation violation

  gGeoManager->Export("TLEP_geom_ROOT5.root"); // export in root format from ROOT5

  theApp.Run();  // could be used to look at display - not essential

  return 0;
}
