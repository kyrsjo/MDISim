# BASH SCRIPT TO RUN MDISIM - by Francesco Collamati - 05-17-2016
:'
    Prerequisites: Add to your .bashrc/.bash_profile the lines:
    export MDISim_dir=[pathToMDISimdir]
    (e.g.: export MDISim_dir=/Users/$LOGNAME/GIT/MDISim/MDISim_dir/)
    source $MDISim_dir/setup.sh         #Runs MDISim initialization
    export MADX_dir=/Applications/      #Point to MADX installation dir

Files organization (starting from $MDISim_dir):
    - Optics files in Optics/FILENAME.madx
    - Geometry files in Geom/FILENAME_LR.root
    - Output files are creted in Output
'

#!/bin/bash

echo "I was in: "$PWD
cd $MDISim_dir
echo "I am in: "$PWD

##############################
FILENAME=fcc_ring_v4_baseline     #Common part of filename
##############################


mkdir /tmp/$LOGNAME/;             #Create TEMP dir, enter and clean it
cd /tmp/$LOGNAME/; rm  -f /tmp/$LOGNAME/*;
echo 'title,"'$FILENAME' ' > tit.txt ;
time $MADX_dir/./madx $MDISim_dir/../others/Optics/$FILENAME.madx 2>&1 | tee $FILENAME.out ;

mkdir $MDISim_dir/../others/Output/$FILENAME

cp -pf $FILENAME*.tfs  $MDISim_dir/../others/Output/$FILENAME

echo "I am in: "$PWD
SURVEYSUFFIX1="_b1_survey.tfs"
SURVEYTOT1=$FILENAME$SURVEYSUFFIX1
SURVEYSUFFIX2="_b2_survey.tfs"
SURVEYTOT2=$FILENAME$SURVEYSUFFIX2

TWISSSUFFIX1="_b1_twiss.tfs"
TWISSTOT1=$FILENAME$TWISSSUFFIX1
TWISSSUFFIX2="_b2_twiss.tfs"
TWISSTOT2=$FILENAME$TWISSSUFFIX2

GEOMSUFFIX="_LR.root"
GEOMFILE=$FILENAME$GEOMSUFFIX

echo "NAME OF SURVEY1" $SURVEYTOT1
echo "NAME OF TWISS1" $TWISSTOT1
echo "NAME OF GEOM" $GEOMFILE


rm -f /tmp/$LOGNAME*.pdf /tmp/$LOGNAME*.eps ; #cleans again temp folder

#now I open root to plot the survey
time root -l $MDISim_dir/MDISim_init.C<<EOF
unsigned int verbose=1;
bool square=true; //setting verbose level

const char* Fname_b1_s="$SURVEYTOT1";
Ntuple nt_b1(Fname_b1_s);  //creates ntuple object
if(verbose) nt_b1.PrintSummary();
Plot_x_y(nt_b1,"Z","X",verbose,square); //Plots xy plane to check it is a circle..
TH2F* hpx = new TH2F("","",10,-1000,1000, 10,-1,1); My_Labels_TH2F(hpx,"Z [m]","X [m]"); hpx->Draw(); hpx->SetStats(kFALSE);
Plot_x_y(nt_b1,"Z","X",verbose,square=false,"_zoomed",true); //Plots beam1

const char* Fname_b2_s="$SURVEYTOT2";
Ntuple nt_b2(Fname_b2_s);
if(verbose) nt_b2.PrintSummary();
Plot_x_y(nt_b2,"Z","X",verbose,square=false,"_zoomed",true);//Plots beam1


.q;
EOF


find . -name "*.eps" -exec pstopdf {} \;
#open *.pdf
cp -pf *.pdf  $MDISim_dir/../others/output/$FILENAME




##3##


cd $MDISim_dir/../others/output;
#cd $MDISim_dir/../

rm VisMacro.C


echo "

void VisMacro(string Fname_b1_o=\"$FILENAME/$TWISSTOT1\", string Fname_b1_s=\"$FILENAME/$SURVEYTOT1\", string GeomPath=\"$MDISim_dir/../others/Geom/$GEOMFILE\")
{

unsigned int verbose=1; TEveElement* el; bool AllowForProjections; //some useful variables

Ntuple nt_b1(ReadAndMerge(Fname_b1_o.c_str(),Fname_b1_s.c_str(),verbose)); if(verbose) nt_b1.PrintSummary();  //creates and prints the ntuple object merging twiss and survey


Beam b1(Fname_b1_o.c_str(),verbose);  //creates the beam reading parameters fro twiss (cfr Twiss_util.C)
b1.RFHV_from_twiss(nt_b1);  // reads RF from twiss and looks for harmonic (!?)
if(b1.RFHV==0) b1.SetRF( 1., 201000 ); // if RF not already set from MADX (e.g. for protons) here it is set
b1.Get_Part().PrintLorentz(cout);
b1.Print();
double nsig=1; // numbero of sigma at which calculate synrad for quads (0 for sawtooth only)only
double emitxN=2.2e-6,emityN=2.2e-6; // normalized emittances set by hand for protons
Lorentz Part=b1.Get_Part();
double emitx=Part.Emit(emitxN);
double emity=Part.Emit(emityN);
cout << setprecision(6) << \"emitxN=\" << 1.e6*emitxN << \" mum  emitx=\" << 1.e9*emitx << \" nm\" << '\n';
cout << setprecision(6) << \"emityN=\" << 1.e6*emityN << \" mum  emity=\" << 1.e9*emity << \" nm\" << '\n';


CalcBeamSizeDivergence(nt_b1,emitx,emity,0,verbose) ;//calculate size and divergence of beam and adds them to ntuple
CalcSynradFr(nt_b1,b1,verbose=3,nsig); //calculates syndrad (Ecrit and NGamma) for dipoles and quadrupoles (calling PrintBends and PrintQuads)

el=StartEveWithGeomDisplay(ResolveHome(GeomPath.c_str()),AllowForProjections=false,verbose); // setup EventDisplay b1,b2, IR and start of arcs

double zmin=-100,zmax=500,Scale_xy=100; //some graphin settings
EveSingleTrackDisplay(nt_b1,zmin,zmax,kGreen,Scale_xy,verbose); // plot design path
bool goto_CM_units=true; // normally true

Plot_Bend_SR_Cones(nt_b1,b1,zmin,zmax,Scale_xy,verbose,goto_CM_units,-1); // myGraph3D.C


}


" >> VisMacro.C


open VisMacro.C

root -l $MDISim_dir/MDISim_init.C;

#NOW MANUALLY RUN THE MACRO
:'
.L VisMacro.C
VisMacro()
'
mv $FILENAME*.root  $MDISim_dir/../others/output/$FILENAME
