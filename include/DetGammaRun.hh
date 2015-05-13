#ifndef DetGammaRun_h
#define DetGammaRun_h 1

#include "G4Run.hh"
#include "globals.hh"

class G4Event;

/// Run class
///

class DetGammaRun : public G4Run
{
  public:
    DetGammaRun();
    virtual ~DetGammaRun();

    virtual void RecordEvent(const G4Event*);
    virtual void Merge(const G4Run*);

  public:
    // get methods
    G4double GetEdepRun()  const { return fEdepRun; }
    G4double GetEdep2Run() const { return fEdep2Run; }

  private:
    G4double  fEdepRun;
    G4double  fEdep2Run;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

