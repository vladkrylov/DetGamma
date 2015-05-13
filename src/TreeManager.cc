#include "Cintex/Cintex.h"
#include <TTree.h>
#include <TFile.h>
#include <TSystem.h>
#include <TROOT.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include <stdexcept>

#include "Event.hh"
#include "TreeManager.hh"
#include "DetGammaMessenger.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

TreeManager::TreeManager(DetGammaMessenger *messenger)
:_rootFile(0), 
 _tree(0),
 _event(0),
 _messenger(messenger) 
{}


TreeManager::~TreeManager()
{
  closePosFile();
  if ( _rootFile ) delete _rootFile;
}


void TreeManager::initialize(G4String fileName)
{ 
  openPosFile();
  // initialize ROOT
  TSystem ts ;
  
  if (_messenger) gSystem->Load(_messenger->GetLibPath()) ;
  else gSystem->Load("libDetGammaClassesDict");

  ROOT::Cintex::Cintex::SetDebug(0);
  ROOT::Cintex::Cintex::Enable();
  //gDebug = 1;

 // Creating a tree container to handle  ntuples.
 // This tree is associated to an output file.
 //
  _event = new Event() ;

  if (_messenger && _messenger->isPhotonisQE()) _event->usePhotonisQE();

    
  if (fileName.isNull()) {
    if ( _messenger) fileName = _messenger->GetRootFileName() ;
    else fileName = "DetGamma3.root" ;
    _rootFile = new TFile(fileName,"RECREATE");
    if(!_rootFile) {
      G4cout << " TreeManager::initialize :" 
	     << " problem creating the ROOT TFile "
	     << G4endl;
      return;
    }
   
    
    _tree = new TTree("DetGamma", "Gamma detector");
    _tree->Branch("Event", "Event", _event) ;

    G4cout << "\n----> Root file is opened in " << fileName << G4endl;
  } else {
    _rootFile = new TFile(fileName,"READ");
    _tree = (TTree*) gROOT->FindObject("DetGamma");
    if(!_tree) throw std::runtime_error("Could not find tree ") ;
    _tree->SetBranchAddress("Event", &_event);
  }
}

void TreeManager::write()
{ 
  if (_rootFile) {
    G4cout << "TreeManager::write: " << _tree->GetEntries() << G4endl ;
    _rootFile->Write();       // Writing tree to the file
    _rootFile->Close();        // and closing the tree (and the file)
    G4cout << "\n----> DetGamma Tree is saved \n" << G4endl;
  }
}

void TreeManager::beginEvent(G4int eventNb) {
  _event->clean() ;
  _event->setEventNum(eventNb);
}

void TreeManager::endEvent() {
  _tree->Fill();
}

void TreeManager::fill(G4int id,  const G4String& process_name, G4double energy, G4double theta, 
		       G4double x, G4double y, G4double z, G4double time, G4int PM_number, G4double pre_theta) 
{
  _event->fill(id,process_name,energy,theta,x,y,z,time, PM_number, pre_theta) ;
}

void TreeManager::openPosFile() {
	myfile.open("Coordinates.txt");
}

void TreeManager::WriteCoords(G4ThreeVector& pos) {
	if (myfile.is_open()) {
		myfile << pos.x() <<"\t"<< pos.y() <<"\t"<< pos.z() <<"\t\n";
	}
}

void TreeManager::closePosFile() {
	if (myfile.is_open()) {
		myfile.close();
	}
}

