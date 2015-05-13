#ifndef DetGammaMessenger_h
#define DetGammaMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;

class DetGammaMessenger: public G4UImessenger
{
  
  G4UIdirectory *_directory, *_detectorDirectory, *_outputDirectory ;

  G4UIcmdWithABool *_secondPM_cmd, *_withGrease_cmd, *_withWindow_cmd, *_usePhotonisQE_cmd ;  
  G4UIcmdWithAString *_coating_cmd, *_shape_cmd, *_rootFileName_cmd, *_libPath_cmd, *_greaseName_cmd ;  
  G4UIcmdWithADouble *_crystalLength_cmd ;

  G4double _crystalLength ;

  G4bool _secondPM, _withGrease, _withWindow, _photonisQE ;
  G4String _coating, _shape, _greaseName ;

  G4String _rootFileName, _libPath ;

public:
  DetGammaMessenger();
  virtual ~DetGammaMessenger();
  
  virtual void SetNewValue(G4UIcommand*, G4String);

  G4bool isSecondPM() const {return _secondPM;}
  G4bool isWithGrease() const {return _withGrease;}
  G4bool isWithWindow() const {return _withWindow;}
  G4bool isPhotonisQE() const {return _photonisQE;}

  G4double GetCrystalLength() const {return _crystalLength;}
  G4String GetCoating() const {return _coating;}
  G4String GetGreaseName() const {return _greaseName;}
  G4String GetShape() const {return _shape;}
  G4String GetRootFileName() const {return _rootFileName;}
  G4String GetLibPath() const {return _libPath;}
    
};

#endif
