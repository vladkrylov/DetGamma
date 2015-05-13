#ifndef DetGammaRunAction_h
#define DetGammaRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;
class G4LogicalVolume;
class TreeManager;

/// Run action class
///

class DetGammaRunAction : public G4UserRunAction
{

  TreeManager* _tree_manager ;

  public:
    DetGammaRunAction(TreeManager* tree_manager=0);
    virtual ~DetGammaRunAction();

    virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);
};


#endif

