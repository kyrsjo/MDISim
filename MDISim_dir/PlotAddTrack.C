// ~/c/MDISim/PlotAddTrack.C   reads  TEveLine  objects from .root files   runs in standard root

#ifndef __CLING__
#include <TFile.h>
#include <TKey.h>
#include <TEveLine.h>
#include <TEveManager.h>
#include "MyCppStdHeaders.H"
#endif

#include "PlotAddTrack.h"

void PlotAddTrack(const char* fname)
{
  TFile* f=new TFile(fname); TIter nextit(f->GetListOfKeys()); TKey *key= (TKey*)nextit();
  cout << "PlotAddTrack " << fname << " key->GetName()=" << key->GetName() << '\n';

  if(string(key->GetClassName())==string("TEveLine"))
  {
	TEveLine* line=(TEveLine*) f->Get(key->GetName());
	line->SetMainColor(kBlack);
	gEve->AddElement(line);
	gEve->Redraw3D(kTRUE);
  }

  if(string(key->GetClassName())==string("TPolyMarker3D"))
  {
	// TPolyMarker3D* line=(TPolyMarker3D*) f->Get(key->GetName());
    TEveLine* line=(TEveLine*) f->Get(key->GetName());
	gEve->AddElement(line);
	// line->Draw();
  }
}
