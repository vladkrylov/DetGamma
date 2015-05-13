#ifndef DetGammaEventInformation_h
#define DetGammaEventInformation_h 1

#include "G4VUserEventInformation.hh"
#include "globals.hh"

class DetGammaEventInformation : public G4VUserEventInformation
{
  public:
    DetGammaEventInformation();
    virtual ~DetGammaEventInformation();
    
    virtual void Print() const;
    
    G4double GetEdepEvent() const { return fEdepEvent; }
    void AddEdep(G4double de) { fEdepEvent += de; }
     
  private:
    G4double  fEdepEvent;
};


#endif

    
