#ifndef DetGammaEventAction_h
#define DetGammaEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class TreeManager ;

/// Event action class
///
/// It instantiates DetGammaEventInformation object at the beginning of
/// each event.
class DetGammaEventAction : public G4UserEventAction
{
  public:
    DetGammaEventAction(TreeManager* tree_manager=0);
    virtual ~DetGammaEventAction();
    
    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

  private:
    G4int  fPrintModulo;
  
    TreeManager* _tree_manager ;
  
};

#endif

    
