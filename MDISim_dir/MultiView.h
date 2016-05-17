// ~/c/MDISim/MultiView.h
#ifndef MultiView_h
#define MultiView_h 1

class MultiView
{
public:
  TEveProjectionManager *fRPhiMgr;
  TEveProjectionManager *fRhoZMgr;
  
  TEveViewer            *f3DView;
  TEveViewer            *fRPhiView;
  TEveViewer            *fRhoZView;
  
  TEveScene             *fRPhiGeomScene;
  TEveScene             *fRhoZGeomScene;
  TEveScene             *fRPhiEventScene;
  TEveScene             *fRhoZEventScene;
  
  MultiView(); // constructor
  
  void SetDepth(Float_t d);
  void ImportGeomRPhi(TEveElement* el);
  void ImportGeomRhoZ(TEveElement* el);
  void ImportEventRPhi(TEveElement* el);
  void ImportEventRhoZ(TEveElement* el);
  
  void DestroyEventRPhi();
  void DestroyEventRhoZ();
};

#endif
