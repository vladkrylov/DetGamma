#include "DetGammaSteppingAction.hh"
#include "DetGammaEventInformation.hh"
#include "DetGammaDetectorConstruction.hh"
#include "TreeManager.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4ProcessManager.hh"

#include <math.h> 

DetGammaSteppingAction::DetGammaSteppingAction(TreeManager* tree_manager)
: G4UserSteppingAction(),
  _tree_manager(tree_manager)
{ ; }

DetGammaSteppingAction::~DetGammaSteppingAction()
{ ; }


void DetGammaSteppingAction::UserSteppingAction(const G4Step* step)
{

  const G4Track* track  = step->GetTrack();

  /*
    const G4StepPoint* point = step->GetPostStepPoint();
  //  const G4StepPoint* point = step->GetPreStepPoint();
  if (step->GetTrack()->GetTrackStatus() == fAlive) {

    G4int id = step->GetTrack()->GetParticleDefinition()->GetPDGEncoding() ;
    G4double energy = point->GetKineticEnergy();      
    G4ThreeVector direction = point->GetMomentumDirection();
    G4double costheta = direction.x();

    G4cout << ">>>>>>>>>> Primary:\t " << id << "\t " << energy << "\t " << costheta << G4endl ;

    _tree_manager->fill(id,energy,costheta);
    }
  */

  G4int StepNo = track->GetCurrentStepNumber();
  const G4VProcess* current_process = track->GetCreatorProcess();
  if (current_process) {
	  G4String processName = current_process->GetProcessName();
	  if ((StepNo == 1) && (processName == "phot")) {
		  G4ThreeVector pos = track->GetVertexPosition();
		  _tree_manager->WriteCoords(pos);
	  }
  }


  //secondaries from the intial photon 
  if (step->GetTrack()->GetParentID() == 0 ){
    const std::vector<const G4Track*>* secondary 
      = step->GetSecondaryInCurrentStep();    
    for (std::vector<const G4Track*>::const_iterator lp = secondary->begin(); lp != secondary->end(); lp++ ) {
      G4int id = (*lp)->GetParticleDefinition()->GetPDGEncoding() ;
      G4double energy = (*lp)->GetKineticEnergy();
      G4ThreeVector direction = (*lp)->GetMomentumDirection();      
      G4ThreeVector position  = (*lp)->GetPosition() ;
      G4double costheta = direction.x();
      G4double time = (*lp)->GetGlobalTime()*ns*1000 ;

      const G4String& process = (*lp)->GetCreatorProcess()->GetProcessName(); 
   
      if (G4RunManager::GetRunManager()->GetVerboseLevel()>0) 
	G4cout << ">>>>>>>>>> Secondary:\t " << process << "\t " << id << "\t " << energy/keV << " keV \t " << costheta << G4endl ;
      _tree_manager->fill(id,process,energy,acos(costheta), position.x(), position.y(), position.z(), time, 0, 0.0);      
    }
    
  } else if (step->GetTrack()->GetParticleDefinition()->GetPDGEncoding() == 11){

    const std::vector<const G4Track*>* secondary
      = step->GetSecondaryInCurrentStep();
    for (std::vector<const G4Track*>::const_iterator lp = secondary->begin(); lp != secondary->end(); lp++ ) {
      const G4String& process = (*lp)->GetCreatorProcess()->GetProcessName();
      if ( process != "Cerenkov") continue ;

      G4int id = (*lp)->GetTrackID() ;
      G4double energy = (*lp)->GetKineticEnergy();
      G4ThreeVector direction = (*lp)->GetMomentumDirection();
      G4ThreeVector position  = (*lp)->GetPosition() ;
      G4double costheta = direction.x();
      G4double time = (*lp)->GetGlobalTime()*ns*1000 ;

      if (G4RunManager::GetRunManager()->GetVerboseLevel()>0) 
	G4cout << ">>>>>>>>>> Cherenkov photon :\t " << process << "\t "<< id << "\t " << energy/eV << " eV \t " << costheta << G4endl ;

      // optical photons have PDGEncoding 0, so add manually PDG id 22
      _tree_manager->fill(22,process,energy,acos(costheta),position.x(), position.y(), position.z(), time, 0, 0.0);

    }
  } 

  //======================================== optical photon part ========================================

  G4ParticleDefinition* particleType = track->GetDefinition();
  // G4cout << " >>>>>>>>>> SteppingAction particle type: " << particleType->GetParticleName() << G4endl ; return ; 

  
  //Optical photon only
  if(particleType!=G4OpticalPhoton::OpticalPhotonDefinition()) return ;
  
  static G4OpBoundaryProcess* boundary=NULL;
  
  //find the boundary process only once
  if(!boundary){
    G4ProcessManager* pm = track->GetDefinition()->GetProcessManager();
    G4int nprocesses = pm->GetProcessListLength();
    G4ProcessVector* pv = pm->GetProcessList();
    G4int i;
    for( i=0;i<nprocesses;i++){
      if((*pv)[i]->GetProcessName()=="OpBoundary"){
        boundary = (G4OpBoundaryProcess*)(*pv)[i];
        break;
      }
    }
  }

  if(!boundary) {G4cout << " >>>>>>>>>> SteppingAction no boundary" << G4endl ; return ;  }

  G4StepPoint* thePrePoint = step->GetPreStepPoint();
  G4VPhysicalVolume* thePrePV = thePrePoint->GetPhysicalVolume();

  G4StepPoint* thePostPoint = step->GetPostStepPoint();
  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();

  /*
  G4OpBoundaryProcessStatus boundaryStatus = boundary->GetStatus();

  G4cout << " >>>>>>>>>> SteppingAction status " ;

    switch(boundaryStatus){
    case Undefined : G4cout << "Undefined" ; break ;
    case FresnelRefraction : G4cout << "FresnelRefraction" ; break ; 
    case FresnelReflection : G4cout << "FresnelReflection" ; break ;
    case TotalInternalReflection : G4cout << "TotalInternalReflection" ; break ;
    case LambertianReflection : G4cout << "LambertianReflection" ; break ; 
    case LobeReflection : G4cout << "LobeReflection" ; break ;
    case SpikeReflection : G4cout << "SpikeReflection" ; break ; 
    case BackScattering : G4cout << "BackScattering" ; break ;
    case Absorption : G4cout << "Absorption" ; break ; 
    case Detection : G4cout << "Detection" ; break ; 
    case NotAtBoundary : G4cout << "NotAtBoundary" ; break ;
    case SameMaterial : G4cout << "SameMaterial" ; break ; 
    case StepTooSmall : G4cout << "StepTooSmall" ; break ; 
    case NoRINDEX : G4cout << "NoRINDEX" ; break ;
    default : G4cout << boundaryStatus; break ;
    }
    G4cout <<  "\t pre volume " << thePrePV->GetName() <<   "\t post volume " << thePostPV->GetName() ;
  G4cout << G4endl ;
  */

  /*
  switch(boundaryStatus){

  case Absorption:
    //    G4cout << " >>>>>>>>>> SteppingAction absorption" << G4endl ;
    break;
  case Detection: //Note, this assumes that the volume causing detection
    //is the photocathode because it is the only one with
    //non-zero efficiency
      {
	//	G4cout << " >>>>>>>>>> SteppingAction detection" << G4endl ;
	break;
      }
  default : {
    //    G4cout << " >>>>>>>>>> SteppingAction unknown status " << boundaryStatus << G4endl ; 
    break ;
  }
  }
  */

  G4String process = "NotUsed" ;
  if (thePrePV->GetName() == "Window" && thePostPV->GetName() == "Cathode") process="SurfWindowCathode";
  if (thePrePV->GetName() == "Crystal" && thePostPV->GetName() == "Cathode") process="SurfCrystalCathode";
  else if (thePrePV->GetName() == "Crystal" && thePostPV->GetName() == "Window") process="SurfCrystalWindow" ;
  else if (thePrePV->GetName() == "Crystal" && thePostPV->GetName() == "Grease") process="SurfCrystalGrease" ;
  else if (thePrePV->GetName() == "Grease" && thePostPV->GetName() == "Window") process="SurfGreaseWindow" ;
  else if (thePrePV->GetName() == "Window" && thePostPV->GetName() == "Crystal") process="SurfWindowCrystal" ;
  else if (thePrePV->GetName() == "Grease" && thePostPV->GetName() == "Crystal") process="SurfGreaseCrystal" ;

  if (process != "NotUsed") {
  
    G4int id = 22 ; // for optical photon force ID
    G4double energy = track->GetKineticEnergy();
    G4ThreeVector direction = track->GetMomentumDirection();      
    G4ThreeVector pre_direction = thePrePoint->GetMomentumDirection();      
    G4ThreeVector position = track->GetPosition() ;
    G4double costheta = direction.z();
    G4double pre_costheta = pre_direction.z();
    G4double time = track->GetGlobalTime()*ns*1000 ;
    G4int PM_number =  thePostPV->GetCopyNo() ;

    if (G4RunManager::GetRunManager()->GetVerboseLevel()>0) {
      G4cout.precision(6) ;
      G4cout << ">>>>>>>>>> in  (" << thePrePV->GetName() << " " << thePrePV->GetCopyNo() 
	     << "/" << thePostPV->GetName()  << " " << thePostPV->GetCopyNo()
	     << ") \t process " << process
	     << "\t " << id   
	     << "\t " << 1239.841930/(energy/eV) << " nm \t (" ;
      G4cout.precision(3) ;
      G4cout << position.x()<< "," <<  position.y()<< "," <<  position.z() << ") cm \t "
	     << G4endl
	     << "theta=" << acos(costheta)*180/pi << " \t phi_x=" <<  acos(direction.x())*180/pi 
	     << " \t phi_y=" <<  acos(direction.y())*180/pi
	     << G4endl ;
    }
    
    bool cherenkov_photon = false;
    const G4String& process_aux = track->GetCreatorProcess()->GetProcessName();
    if ( process_aux == "Cerenkov") cherenkov_photon = true ;
    _tree_manager->fill(id,process,energy,acos(costheta)*180/pi,position.x(), position.y(), position.z(), time, PM_number,
			acos(pre_costheta)*180/pi, cherenkov_photon);
  
  }
    
    
}



