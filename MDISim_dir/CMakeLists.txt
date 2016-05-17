# ~/c/MDISim/CMakeLists.txt
#
# open ~/info/CMake.txt

cmake_minimum_required (VERSION 2.8)

project(MDISim)
message("PROJECT_NAME = ${PROJECT_NAME} CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR} CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR} CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

set(${PROJECT_NAME}_VERSION "1.0.0")
set(${PROJECT_NAME}_VERSION_MAJOR "2")
set(${PROJECT_NAME}_VERSION_MINOR "3")

# skip rpath
# set(CMAKE_SKIP_BUILD_RPATH  TRUE)

# library names with .so rather then .dylib  which would be the OSX default
set (CMAKE_SHARED_LIBRARY_SUFFIX ".so")

include_directories ("$ENV{HOME}/c/MyClib")
include_directories ("$ENV{HOME}/c/MyRoot")
include_directories ("$ENV{HOME}/myroot")
set(ShowMyExtraHeaders "$ENV{HOME}/myroot/SeeIncludes.h" )

message("ShowMyExtraHeaders =${ShowMyExtraHeaders}")

# include_directories ("$ENV{ROOTSYS}/include")  # or via CMAKE_CXX_FLAGS see next line :
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} $ENV{ROOT_CFLAGS} ")

# http://www.cmake.org/cmake/help/v3.0/policy/CMP0008.html     Libraries linked by full-path must have a valid library file name.
cmake_policy(SET CMP0008 NEW)
# cmake_policy(SET CMP0042 NEW) # suppress the MACOSX_RPATH is not specified warning, see cmake --help-policy CMP0042  for information
# better explicitly enable or disable       otherwise  getting  libMDISim.so   which is neither rpath nor absolute
set(CMAKE_MACOSX_RPATH TRUE)  # with TRUE      @rpath/libMDISim.so   with FALSE  shows abolute link   /tmp/hbu/MDISim/libMDISim.so   installed just libMDISim.so

set(CMAKE_INSTALL_RPATH           "/Users/hbu/lib/clang/" )
# set(CMAKE_INSTALL_NAME_DIR        "${CMAKE_INSTALL_PREFIX}/lib")
# set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)



#trial
set(MACOSX_RPATH /Users/hbu/lib/clang)

# use my env  MyClib_LIB   MyRoot_LIB
set(MyClib $ENV{MyClib_LIB})
set(MyRoot $ENV{MyRoot_LIB})

set(MyLibDir "$ENV{HOME}/lib/$ENV{Use_clang}") # install directory, like  /Users/hbu/lib/clang/



# for My_VGM
include_directories ("$ENV{HOME}/c2/vgm/include")


file (GLOB main_files     RELATIVE ${CMAKE_SOURCE_DIR} "main_*.C" )  # all .C
file (GLOB my_src_files   RELATIVE ${CMAKE_SOURCE_DIR} "*.C" )  # all .C
file (GLOB my_headers     RELATIVE ${CMAKE_SOURCE_DIR} "*.h" )  # all .h
# remove the main_ files
foreach (my_removeitem ${main_files})
  list(REMOVE_ITEM my_src_files "${my_removeitem}")
endforeach (my_removeitem)


# show/edit also the relevant MAD-X jobs
set(ShowExtra_general       "$ENV{HOME}/mad/tools/twiss_select.madx" "$ENV{HOME}/mad/tools/survey_select.madx" )
set(ShowExtra_CLIC          "$ENV{HOME}/mad/clic/v_10_10_11/job_clic.madx" )
set(ShowExtra_FCC           "$ENV{HOME}/mad/fcc-ee/tlep_v12a_cern/run.madx" "$ENV{HOME}/mad/fcc-ee/TLEP_V14_IR_6-13-2/run_fccq_13_2.madx" "$ENV{HOME}/mad/fcc-ee/TLEP_V14_IR_6-14-3/run_fccq_14_3.madx" "$ENV{HOME}/mad/fcc-ee/FCC_arc_17_IR_8/fcc_ee_arc_17_ir_8.madx"
                            "$ENV{HOME}/mad/fcc-ee/Oide/fcc_ee_t_26_cw_nosol.madx" "$ENV{HOME}/mad/fcc-ee/Oide/fcc_ee_t_42_6a_cw.madx" "$ENV{HOME}/mad/fcc-ee/Oide/fcc_ee_t_42_7c_cw.madx"
                            "$ENV{HOME}/mad/fcc-ee/Oide/fcc_ee_t_45_16_cw_nosol.madx" "$ENV{HOME}/mad/fcc-ee/Oide/fcc_ee_t_74_11_nosol.madx"
                            "$ENV{HOME}/mad/fcc_hh/fcc_hh_race_v3.madx" "$ENV{HOME}/mad/fcc_hh/fcc_hh_ring_v3.madx"  "$ENV{HOME}/mad/fcc_hh/fcc_ring_v4_baseline.madx" "$ENV{HOME}/mad/fcc_hh/fcc-hh_IR_V0.3.madx"
                            "$ENV{HOME}/mad/fcc-ee/Anton/fcc_ee_9_3.madx"
                            "$ENV{HOME}/mad/fcc-ee/HeadOn/headon.madx"
                            "$ENV{HOME}/mad/fcc-ee/ReadMe_fcc_ee.txt" "$ENV{HOME}/mad/fcc_hh/ReadMe_fcc_hh.txt" )
set(ShowExtra_LEP           "$ENV{HOME}/mad/LEP/lep.madx" )
set(ShowExtra_LHeC          "$ENV{HOME}/mad/LHeC/LHeC_sample.madx" "$ENV{HOME}/mad/LHeC/job.SOLUTION.e-.30m0.1m.madx" "$ENV{HOME}/mad/LHeC/LHeC_3beam_sample.madx"  "$ENV{HOME}/mad/LHeC/ReadMe.txt" )
set(ShowExtra_LHC           "$ENV{HOME}/mad/LHC/lhc_V6_5_survey.madx" )
set(ShowExtra_LEIR          "$ENV{HOME}/mad/makethin/leir/LEIR_job.madx" )
set(ShowExtra_SingleBend    "$ENV{HOME}/mad/synrad/SingleBend.madx" )

# main and sources  cd ~/c/MDISim/ ; ls -x *.C *.h         give also header files such that they show up in Xcode
set(AddTracksToGeom_SOURCES   myGraph3D.C myGraph3D.h main_AddTracksToGeom.C myEve.C myEve.h Twiss_util.C Twiss_util.h )
set(CLIC_SOURCES              Apertures.C Apertures.h  IPpos.C IPpos.h  main_CLIC.C        myGraph.C myGraph.h myGraph3D.C myGraph3D.h  MyNtuple2Geom.C MyNtuple2Geom.h Plot.C Plot.h  Twiss_util.C Twiss_util.h
                              ${ShowExtra_general} ${ShowExtra_CLIC} )
set(FCC_SOURCES               Apertures.C Apertures.h  IPpos.C IPpos.h  main_FCC.C         myGraph.C myGraph.h myGraph3D.C myGraph3D.h  MyNtuple2Geom.C MyNtuple2Geom.h Plot.C Plot.h  Twiss_util.C Twiss_util.h myEve.C myEve.h PlotAddTrack.C PlotAddTrack.h MultiView.C MultiView.h ${ShowExtra_general} ${ShowExtra_FCC} )
set(Fields_from_tfs_SOURCES   main_Fields_from_tfs.C ${ShowExtra_LEIR} ${ShowExtra_SingleBend} )
set(LEP_SOURCES               Apertures.C Apertures.h  IPpos.C IPpos.h  main_LEP.C         myGraph.C myGraph.h myGraph3D.C myGraph3D.h  MyNtuple2Geom.C MyNtuple2Geom.h Plot.C Plot.h  Twiss_util.C Twiss_util.h
                              ${ShowExtra_general} ${ShowExtra_LEP} )
set(LHC_SOURCES               Apertures.C Apertures.h                               IPpos.C IPpos.h  main_LHC.C myGraph.C myGraph.h myGraph3D.C myGraph3D.h  MyNtuple2Geom.C MyNtuple2Geom.h myEve.C myEve.h ${ShowExtra_LHC} )
set(LHeC_SOURCES              Apertures.C Apertures.h  IPpos.C IPpos.h  main_FCC.C         myGraph.C myGraph.h myGraph3D.C myGraph3D.h  MyNtuple2Geom.C MyNtuple2Geom.h Plot.C Plot.h  Twiss_util.C Twiss_util.h myEve.C myEve.h
${ShowExtra_general} ${ShowExtra_LHeC} )
set(My_VGM_SOURCES            main_My_VGM.C )
set(MyGeoManager_SOURCES                                                                             main_MyGeoManager.C                        MyGeoManager.C MyGeoManager.h myEve.C myEve.h MultiView.C MultiView.h )
set(MyNtuple2Geom_SOURCES                                                                            main_MyNtuple2Geom.C                       MyNtuple2Geom.C MyNtuple2Geom.h ${ShowExtra_LEIR} ${ShowExtra_SingleBend} ReadMe.txt )
set(TwissTrack_tfs_SOURCES    Apertures.C Apertures.h  main_TwissTrack_tfs.C myGraph.C myGraph.h myGraph3D.C myGraph3D.h  TwissTrack_Param.C TwissTrack_Param.h  Twiss_util.C Twiss_util.h Plot.C )

# make root available via  ~/c/cmake/Modules/FindROOT.cmake
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{HOME}/c/cmake/Modules/ )
find_package(ROOT REQUIRED)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # options for loader on Mac
  set(CMAKE_CXX_LINK_FLAGS "-v -undefined dynamic_lookup" )         # make verbose, and allow for undefined like   RunFlags::ShowUsage() 
else() # options for loader on lxplus
  set(CMAKE_CXX_LINK_FLAGS "-v -Xlinker --allow-shlib-undefined" )  # make verbose, and allow for undefined like   RunFlags::ShowUsage() 
endif()

add_executable (AddTracksToGeom  ${AddTracksToGeom_SOURCES}  ${ShowMyExtraHeaders} )
add_executable (CLIC             ${CLIC_SOURCES}             ${ShowMyExtraHeaders} )
add_executable (FCC              ${FCC_SOURCES}              ${ShowMyExtraHeaders} )
add_executable (Fields_from_tfs  ${Fields_from_tfs_SOURCES}  ${ShowMyExtraHeaders} )
add_executable (LEP              ${LEP_SOURCES}              ${ShowMyExtraHeaders} )
add_executable (LHC              ${LHC_SOURCES}              ${ShowMyExtraHeaders} )
add_executable (LHeC             ${LHeC_SOURCES}             ${ShowMyExtraHeaders} )
add_executable (My_VGM           ${My_VGM_SOURCES}                                 )
add_executable (MyGeoManager     ${MyGeoManager_SOURCES}     ${ShowMyExtraHeaders} )
add_executable (MyNtuple2Geom    ${MyNtuple2Geom_SOURCES}    ${ShowMyExtraHeaders} )
add_executable (TwissTrack_tfs   ${TwissTrack_tfs_SOURCES}   ${ShowMyExtraHeaders} )

add_library ( MDISim SHARED ${my_src_files} ${my_headers} ReadMe.txt makefile )  # libMIDSimlib.so

# in link_directories $ENV{HOME}/lib/4.8 look for libMyClib_clang.so, libMyRoot_clang.so
# -lGeom only needed for 3D Geo Manager
# -lEve  for event manager
# -lGui  for TGMainFrame
target_link_libraries (AddTracksToGeom  ${MyClib} ${MyRoot} ${ROOT_LIBRARIES} Geom Eve Gui RGL)
target_link_libraries (CLIC             ${MyClib} ${MyRoot} ${ROOT_LIBRARIES} Geom Eve Gui RGL)
target_link_libraries (FCC              ${MyClib} ${MyRoot} ${ROOT_LIBRARIES} Geom Eve Gui RGL)
target_link_libraries (Fields_from_tfs  ${MyClib} ${MyRoot} ${ROOT_LIBRARIES} Geom Eve Gui RGL)
target_link_libraries (LEP              ${MyClib} ${MyRoot} ${ROOT_LIBRARIES} Geom Eve Gui RGL)
target_link_libraries (LHC              ${MyClib} ${MyRoot} ${ROOT_LIBRARIES} Geom Eve Gui RGL)
target_link_libraries (LHeC             ${MyClib} ${MyRoot} ${ROOT_LIBRARIES} Geom Eve Gui RGL)
target_link_libraries (My_VGM           ${MyRoot}           ${ROOT_LIBRARIES} Geom Eve Gui RGL)
target_link_libraries (MyGeoManager     ${MyClib} ${MyRoot} ${ROOT_LIBRARIES} Geom Eve Gui RGL)
target_link_libraries (MyNtuple2Geom    ${MyClib} ${MyRoot} ${ROOT_LIBRARIES} Geom Eve Gui RGL Gdml) # Gdml added 3/12/2014
target_link_libraries (TwissTrack_tfs   ${MyClib} ${MyRoot} ${ROOT_LIBRARIES} Geom Eve Gui RGL)

#seems needed again
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # options for loader on Mac
  set(CMAKE_SHARED_LINKER_FLAGS "-v -undefined dynamic_lookup" )         # link with root libs, and allow for undefined like   RunFlags::ShowUsage() 
else() # options for loader on lxplus
  set(CMAKE_SHARED_LINKER_FLAGS "-v -Xlinker --allow-shlib-undefined" )
endif()
# or specify other libs used :
# target_link_libraries (MDISim ${MyClib} ${MyRoot} ${ROOT_LIBRARIES} Geom Eve Gui RGL) # libMIDSimlib.so    specify other libs used, alternative could be 

install(TARGETS AddTracksToGeom CLIC FCC Fields_from_tfs LEP LHC LHeC My_VGM MyGeoManager MyNtuple2Geom TwissTrack_tfs
RUNTIME DESTINATION ${PROJECT_NAME}/${CMAKE_BUILD_TYPE} )  # install in CMAKE_BUILD_TYPE directory,    here typically  /tmp/$LOGNAME/Release

install(TARGETS MDISim LIBRARY DESTINATION ${MyLibDir} ) # libMIDSimlib.so