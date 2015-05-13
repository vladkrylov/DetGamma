#ifndef DetGammaActionInitialization_h
#define DetGammaActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "globals.hh"

class TreeManager ;

/// Action initialization class.
///

class DetGammaActionInitialization : public G4VUserActionInitialization
{
  public:
  DetGammaActionInitialization(TreeManager* tree_manager=0);
    virtual ~DetGammaActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

private :  
  TreeManager* _tree_manager ;
};

#endif

    
