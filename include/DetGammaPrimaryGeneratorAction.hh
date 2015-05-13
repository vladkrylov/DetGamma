#ifndef DetGammaPrimaryGeneratorAction_h
#define DetGammaPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class G4Box;

/// The primary generator action class with particle gun.
///
/// The default kinematic is a 511 keV gamma, randomly distribued 
/// across 20% of the (X,Y) detector size  (e.g. beam is collimated)

class DetGammaPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    DetGammaPrimaryGeneratorAction();    
    virtual ~DetGammaPrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);         
  
    // method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
  
  private:
    G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
    G4Box* fEnvelopeBox;
};


#endif


