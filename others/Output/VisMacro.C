

void VisMacro(string Fname_b1_o="fcc_ring_v4_baseline/fcc_ring_v4_baseline_b1_twiss.tfs", string Fname_b1_s="fcc_ring_v4_baseline/fcc_ring_v4_baseline_b1_survey.tfs", string GeomPath="/Users/francesco/GIT/MDISim/MDISim_dir/../others/Geom/fcc_ring_v4_baseline_LR.root")
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
cout << setprecision(6) << "emitxN=" << 1.e6*emitxN << " mum  emitx=" << 1.e9*emitx << " nm" << '\n';
cout << setprecision(6) << "emityN=" << 1.e6*emityN << " mum  emity=" << 1.e9*emity << " nm" << '\n';


CalcBeamSizeDivergence(nt_b1,emitx,emity,0,verbose) ;//calculate size and divergence of beam and adds them to ntuple
CalcSynradFr(nt_b1,b1,verbose=3,nsig); //calculates syndrad (Ecrit and NGamma) for dipoles and quadrupoles (calling PrintBends and PrintQuads)

el=StartEveWithGeomDisplay(ResolveHome(GeomPath.c_str()),AllowForProjections=false,verbose); // setup EventDisplay b1,b2, IR and start of arcs

double zmin=-100,zmax=500,Scale_xy=100; //some graphin settings
EveSingleTrackDisplay(nt_b1,zmin,zmax,kGreen,Scale_xy,verbose); // plot design path
bool goto_CM_units=true; // normally true

Plot_Bend_SR_Cones(nt_b1,b1,zmin,zmax,Scale_xy,verbose,goto_CM_units,-1); // myGraph3D.C


}



