#include "DetGammaRun.hh"
#include "DetGammaEventInformation.hh"

#include "G4Event.hh"
#include "G4SystemOfUnits.hh"

DetGammaRun::DetGammaRun()
: G4Run(),
  fEdepRun(0.), fEdep2Run(0.)
{ ; } 


DetGammaRun::~DetGammaRun()
{ ; } 

void DetGammaRun::RecordEvent(const G4Event* event)
{  
  DetGammaEventInformation* evInfo
    = static_cast<DetGammaEventInformation*>(event->GetUserInformation());
  G4double EdepEvent = evInfo->GetEdepEvent();
  fEdepRun  += EdepEvent;
  fEdep2Run += EdepEvent*EdepEvent;

  G4Run::RecordEvent(event);
}
 

void DetGammaRun::Merge(const G4Run* aRun)
{
  const DetGammaRun* localRun = static_cast<const DetGammaRun*>(aRun);
  fEdepRun  += localRun->fEdepRun;
  fEdep2Run += localRun->fEdep2Run;

  G4Run::Merge(aRun); 
} 

