
#include "DetGammaEventInformation.hh"

#include "G4SystemOfUnits.hh"



DetGammaEventInformation::DetGammaEventInformation()
: G4VUserEventInformation(),
  fEdepEvent(0.)
{ ; } 



DetGammaEventInformation::~DetGammaEventInformation()
{ ; } 



void DetGammaEventInformation::Print() const
{  
  G4cout << "Stored energy deposited per event : " << fEdepEvent/MeV << " MeV" 
         << G4endl;
}
 

