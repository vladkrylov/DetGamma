#ifndef DetGammaDetectorConstruction_h
#define DetGammaDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class DetGammaMessenger;

/// Detector construction class to define materials and geometry.

class DetGammaDetectorConstruction : public G4VUserDetectorConstruction
{

  DetGammaMessenger* _messenger ;
  
  public:

  DetGammaDetectorConstruction(DetGammaMessenger* messenger);
  virtual ~DetGammaDetectorConstruction();
  
  virtual G4VPhysicalVolume* Construct();

  static G4double PbF2RefractionIndex(G4double energy) ;
  static G4double SapphireRefractionIndex(G4double energy) ;
  static G4double QuartzRefractionIndex(G4double energy) ;
  static G4double BK7RefractionIndex(G4double energy) ;
  static G4double PbF2AbsorptionLength(G4double energy) ;
  static G4double SapphireAbsorptionLength(G4double energy) ;
  static G4double BK7AbsorptionLength(G4double energy) ;
  static G4double LambertianCoatingReflection(G4double energy) ;
};

#endif

