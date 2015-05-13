#ifndef TreeManager_h
#define TreeManager_h 1

#include "globals.hh"
#include <vector>
#include <fstream>
#include "G4ThreeVector.hh"

class TFile;
class TTree ;
class Event;
class DetGammaMessenger ;

class TreeManager {

  
  TFile*   _rootFile;
  TTree*   _tree;    
  
  Event*  _event ;
  DetGammaMessenger* _messenger ;

public:
  
  TreeManager(DetGammaMessenger *messenger=0);
  ~TreeManager();

  //create or read tree
  void initialize(G4String fileName = "");

  void write();
  
  void beginEvent(G4int eventNb) ;
  void endEvent() ;
  void fill(G4int id,  const G4String& process_name, G4double energy, 
	    G4double theta, G4double x, G4double y, G4double z, G4double time, G4int PM_number,
	    G4double pre_theta) ;
  
  TTree* GetTree() {return _tree;}

  void WriteCoords(G4ThreeVector& pos);

private:
  std::ofstream myfile;
  void openPosFile();
  void closePosFile();
};

#endif

