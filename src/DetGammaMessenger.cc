#include "DetGammaMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"

DetGammaMessenger::DetGammaMessenger()
  : G4UImessenger(), _secondPM(false), _withGrease(false),_withWindow(false), _photonisQE(true),
    _coating("white"), _shape("cylinder"), _rootFileName("DetGamma1.root")

{
  _directory = new G4UIdirectory("/DetGamma/");
  _directory->SetGuidance("Commands specific for DetGamma simulation");

  _detectorDirectory = new G4UIdirectory("/DetGamma/detector/");
  _detectorDirectory->SetGuidance("Detector configuration");

  _outputDirectory = new G4UIdirectory("/DetGamma/output/");
  _outputDirectory->SetGuidance("Output configuration");

  _crystalLength_cmd = new G4UIcmdWithADouble("/DetGamma/detector/CrystalLength",this);
  _crystalLength_cmd->SetGuidance("Length of the crystal in mm");
  _crystalLength_cmd->SetParameterName("crystalLength",false);

  _secondPM_cmd = new G4UIcmdWithABool("/DetGamma/detector/AddSecondPM",this);
  _secondPM_cmd->SetGuidance("Add front photon-multiplier to the detector");
  _secondPM_cmd->SetParameterName("secondPM",true);
  _secondPM_cmd->SetDefaultValue("false");

  _withGrease_cmd = new G4UIcmdWithABool("/DetGamma/detector/AddGrease",this);
  _withGrease_cmd->SetGuidance("Add grease between windown and crystal");
  _withGrease_cmd->SetParameterName("withGrease",true);
  _withGrease_cmd->SetDefaultValue("false");

  _greaseName_cmd = new G4UIcmdWithAString("/DetGamma/detector/GreaseName",this);
  _greaseName_cmd->SetGuidance("Specify grease type");
  _greaseName_cmd->SetParameterName("GreaseName",true);
  _greaseName_cmd->SetDefaultValue("default");
  _greaseName_cmd->SetCandidates("default OCF452 OCF463") ;

  _withWindow_cmd = new G4UIcmdWithABool("/DetGamma/detector/WithWindow",this);
  _withWindow_cmd->SetGuidance("Use a window between crystal and photocathode");
  _withWindow_cmd->SetParameterName("withWindow",true);
  _withWindow_cmd->SetDefaultValue("true");

  _usePhotonisQE_cmd = new G4UIcmdWithABool("/DetGamma/detector/UsePhotonisQE",this);
  _usePhotonisQE_cmd->SetGuidance("Use Photonis quantum efficiency or starndard one");
  _usePhotonisQE_cmd->SetParameterName("usePhotonisQE",true);
  _usePhotonisQE_cmd->SetDefaultValue("true");

  _coating_cmd = new G4UIcmdWithAString("/DetGamma/detector/Coating",this);
  _coating_cmd->SetGuidance("Specify crystal coating which is applied to the border between the crystal and the world");
  _coating_cmd->SetParameterName("Coating",true);
  _coating_cmd->SetDefaultValue("white");
  _coating_cmd->SetCandidates("white black diffuse_white black_and_white black_and_diffuse_white");

  
  _shape_cmd = new G4UIcmdWithAString("/DetGamma/detector/CrystalShape",this);
  _shape_cmd->SetGuidance("Specify crystal shape") ;
  _shape_cmd->SetParameterName("Shape",true);
  _shape_cmd->SetDefaultValue("cylinder");
  _shape_cmd->SetCandidates("cylinder box") ;

  _rootFileName_cmd = new G4UIcmdWithAString("/DetGamma/output/RootFileName",this);
  _rootFileName_cmd->SetGuidance("Specify output root file name as path/file_name.root") ;
  _rootFileName_cmd->SetParameterName("RootFileName",true);
  _rootFileName_cmd->SetDefaultValue("DetGamma2.root");

  _libPath_cmd = new G4UIcmdWithAString("/DetGamma/output/Library",this);
  _libPath_cmd->SetGuidance("Specify library for the root class dictionaries") ;
  _libPath_cmd->SetParameterName("Library",true);
  _libPath_cmd->SetDefaultValue("/home/rita/geant4_workdir/tmp/Linux-g++/mainDetGamma/libDetGammaClassesDict.so");
}


DetGammaMessenger::~DetGammaMessenger()
{
  delete _crystalLength_cmd ;
  delete _secondPM_cmd;
  delete _withGrease_cmd ;
  delete _withWindow_cmd ;
  delete _usePhotonisQE_cmd ;
  delete _coating_cmd ;
  delete _shape_cmd ;
  delete _rootFileName_cmd ;
  delete _libPath_cmd ;

  delete _detectorDirectory;
  delete _outputDirectory;
  delete _directory ;
}

void DetGammaMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
  if( command == _crystalLength_cmd) _crystalLength = _crystalLength_cmd->GetNewDoubleValue(newValue) ;
  if( command == _secondPM_cmd )  _secondPM = _secondPM_cmd->GetNewBoolValue(newValue);
  if( command == _withGrease_cmd )  _withGrease = _withGrease_cmd->GetNewBoolValue(newValue);
  if( command == _withWindow_cmd )  _withWindow = _withWindow_cmd->GetNewBoolValue(newValue);
  if( command == _usePhotonisQE_cmd) _photonisQE = _usePhotonisQE_cmd->GetNewBoolValue(newValue);
  if( command == _coating_cmd) _coating = newValue ;
  if( command == _greaseName_cmd) _greaseName = newValue ;
  if( command == _shape_cmd) _shape = newValue ;
  if( command == _rootFileName_cmd) _rootFileName = newValue ;
  if( command == _libPath_cmd) _libPath = newValue ;
}
