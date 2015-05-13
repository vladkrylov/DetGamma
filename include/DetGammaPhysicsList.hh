#ifndef DetGammaPhysicsList_h
#define DetGammaPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class DetGammaPhysicsList: public G4VModularPhysicsList
{
  public:

    DetGammaPhysicsList(G4int verbose=0);
    virtual ~DetGammaPhysicsList();

  public:

    // SetCuts()
    virtual void SetCuts();

};

#endif
