// ~/c/MDISim/MultiView.C   based on   tutorials/eve/MultiView.C

#include <TEveManager.h>

#include <TEveViewer.h>
#include <TGLViewer.h>

#include <TEveScene.h>

#include <TEveProjectionManager.h>
#include <TEveProjectionAxes.h>

#include <TEveBrowser.h>
#include <TEveWindow.h>

#include "MultiView.h"

// MultiView
//
// Structure encapsulating standard views: 3D, r-phi and rho-z.
// Includes scenes and projection managers.


MultiView::MultiView() // constructor
{
  // Constructor --- creates required scenes, projection managers and GL viewers.

  if(gEve) gEve->Redraw3D(kFALSE, kTRUE); //hbu to get reasonable scales in projections

  // Scenes
  //========

  fRPhiGeomScene  = gEve->SpawnNewScene("RPhi Geometry", "Scene holding projected geometry for the RPhi view.");
  fRhoZGeomScene  = gEve->SpawnNewScene("RhoZ Geometry", "Scene holding projected geometry for the RhoZ view.");
  fRPhiEventScene = gEve->SpawnNewScene("RPhi Event Data", "Scene holding projected event-data for the RPhi view.");
  fRhoZEventScene = gEve->SpawnNewScene("RhoZ Event Data",  "Scene holding projected event-data for the RhoZ view.");

  // Projection managers
  //=====================

  fRPhiMgr = new TEveProjectionManager(TEveProjection::kPT_RPhi);
  gEve->AddToListTree(fRPhiMgr, kFALSE);
  {
    TEveProjectionAxes* a = new TEveProjectionAxes(fRPhiMgr);
    a->SetMainColor(kWhite);
    a->SetTitle("R-Phi");
    a->SetTitleSize(0.05);
    a->SetTitleFont(102);
    a->SetLabelSize(0.025);
    a->SetLabelFont(102);
    fRPhiGeomScene->AddElement(a);
  }

  fRhoZMgr = new TEveProjectionManager(TEveProjection::kPT_RhoZ);
  gEve->AddToListTree(fRhoZMgr, kFALSE);
  {
    TEveProjectionAxes* a = new TEveProjectionAxes(fRhoZMgr);
    a->SetMainColor(kWhite);
    a->SetTitle("Rho-Z");
    a->SetTitleSize(0.05);
    a->SetTitleFont(102);
    a->SetLabelSize(0.025);
    a->SetLabelFont(102);
    fRhoZGeomScene->AddElement(a);
  }


  // Viewers
  //=========

  TEveWindowSlot *slot = 0;
  TEveWindowPack *pack = 0;

  slot = TEveWindow::CreateWindowInTab(gEve->GetBrowser()->GetTabRight());
  pack = slot->MakePack();
  pack->SetElementName("Multi View");
  pack->SetHorizontal();
  pack->SetShowTitleBar(kFALSE);
  pack->NewSlot()->MakeCurrent();
  f3DView = gEve->SpawnNewViewer("3D View", "");
  f3DView->AddScene(gEve->GetGlobalScene());
  f3DView->AddScene(gEve->GetEventScene());

  pack = pack->NewSlot()->MakePack();
  pack->SetShowTitleBar(kFALSE);
  pack->NewSlot()->MakeCurrent();
  fRPhiView = gEve->SpawnNewViewer("RPhi View", "");
  fRPhiView->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraOrthoXOY);
  fRPhiView->AddScene(fRPhiGeomScene);
  fRPhiView->AddScene(fRPhiEventScene);

  pack->NewSlot()->MakeCurrent();
  fRhoZView = gEve->SpawnNewViewer("RhoZ View", "");
  fRhoZView->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraOrthoXOY);
  fRhoZView->AddScene(fRhoZGeomScene);
  fRhoZView->AddScene(fRhoZEventScene);
}

//---------------------------------------------------------------------------

void MultiView::SetDepth(Float_t d)
{
  // Set current depth on all projection managers.

  fRPhiMgr->SetCurrentDepth(d);
  fRhoZMgr->SetCurrentDepth(d);
}

//---------------------------------------------------------------------------

void MultiView::ImportGeomRPhi(TEveElement* el)
{
  fRPhiMgr->ImportElements(el, fRPhiGeomScene);
}

void MultiView::ImportGeomRhoZ(TEveElement* el)
{
  fRhoZMgr->ImportElements(el, fRhoZGeomScene);
}

void MultiView::ImportEventRPhi(TEveElement* el)
{
  fRPhiMgr->ImportElements(el, fRPhiEventScene);
}

void MultiView::ImportEventRhoZ(TEveElement* el)
{
  fRhoZMgr->ImportElements(el, fRhoZEventScene);
}

//---------------------------------------------------------------------------

void MultiView::DestroyEventRPhi()
{
  fRPhiEventScene->DestroyElements();
}

void MultiView::DestroyEventRhoZ()
{
  fRhoZEventScene->DestroyElements();
}
