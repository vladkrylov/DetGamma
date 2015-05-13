#include "DetGammaDetectorConstruction.hh"
#include "DetGammaMessenger.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4UIcommand.hh"

#include <stdexcept>
#include <string>

using namespace std;

enum SHAPES {CYLINDER, BOX} ;
enum COATINGS {WHITE, BLACK, DIFFUSE_WHITE, BLACK_AND_WHITE, BLACK_AND_DIFFUSE_WHITE} ;

G4double  DetGammaDetectorConstruction::PbF2RefractionIndex(G4double energy) {

  //  from http://refractiveindex.info

  G4double x = 1.239841930/energy ; // wavelenth in micrometers

  // refraction index not measured for values less than ~300 nm (large absorption)
  if (x < 0.285) return 2.1 ;

  return std::sqrt( 1 + 0.66959342*pow(x,2) / (pow(x,2)-pow(0.00034911,2)) + 
		    1.3086319*pow(x,2)/(pow(x,2)-pow(0.17144455,2)) + 
		    0.01670641*pow(x,2)/(pow(x,2)-pow(0.28125513,2)) + 
		    2007.8865*pow(x,2)/(pow(x,2)-pow(796.67469,2)) ) ;
}


G4double  DetGammaDetectorConstruction::SapphireRefractionIndex(G4double energy) {
  //  from http://refractiveindex.info                                                                                                                              

  G4double x = 1.239841930/energy ; // wavelenth in micrometers  

  return sqrt( 1 + 1.4313493*pow(x,2)/(pow(x,2)-pow(0.0726631,2)) + 
	       0.65054713*pow(x,2)/(pow(x,2)-pow(0.1193242,2)) + 
	       5.3414021*pow(x,2)/(pow(x,2)-pow(18.028251,2))) ;

}

G4double  DetGammaDetectorConstruction::QuartzRefractionIndex(G4double energy) {

  //  from http://refractiveindex.info

  G4double x = 1.239841930/energy ; // wavelenth in micrometers

  return std::sqrt( 1.28604141 + 1.07044083*pow(x,2)/(pow(x,2)-1.00585997e-2) + 
		    1.10202242*pow(x,2)/(pow(x,2)-100) );
}

G4double  DetGammaDetectorConstruction::BK7RefractionIndex(G4double energy) {

  //  from http://refractiveindex.info

  G4double x = 1.239841930/energy ; // wavelenth in micrometers

  return std::sqrt( 1 + 1.03961212*pow(x,2)/(pow(x,2)-0.00600069867) + 
		    0.231792344*pow(x,2)/(pow(x,2)-0.0200179144) + 
		    1.01046945*pow(x,2)/(pow(x,2)-103.560653) ) ;
}

G4double  DetGammaDetectorConstruction::PbF2AbsorptionLength(G4double energy) {

  // approximation to the data in 
  // ACHENBACH: SCATTERING AND ABSORPTION CENTERS IN LEAD FLUORIDE CRYSTALS
  // EEE TRANSACTIONS ON NUCLEAR SCIENCE, VOL. 48, NO. 1, FEBRUARY 2001
  // crystals of the length 150 mm -- 185 mm

  // A = 1 - T - R
  // R is a Fresnle correction (refraction) is about R ~18%
  // absorption threshold is  245 nm
  // lenght is 150 mm
  // lambda > 800 nm  T ~ 80.4% 
  // lambda > 400 nm  T ~ 74.1%
  // lambda > 330 nm  T ~ 64.8%
  // lambda > 270 nm  T ~ 15.1% 

  G4double x = 1239.841930/energy ; // wavelenth in nanometers  

  const int N=5 ;
  G4double y[N]={245.0, 270.0, 330.0, 400.0, 800.0} ;
  if (x <= y[0]) return 0 ;

  G4double T[N]={0.0, 0.151, 0.648, 0.741, 0.804} ;  

  // absoption length
  G4double L[N] ;
  L[0] = 0.0 ;  

  int i = 1 ;
  for (; i<N; i++) {
    L[i] = -150*mm /log(0.18 + T[i]) ; //  L = L/ln(1-A) ;
    if (x < y[i])  
      return  L[i-1] + (x - y[i-1])*(L[i]-L[i-1])/(y[i]-y[i-1]);
  }

  return L[N-1] ;
}


G4double  DetGammaDetectorConstruction::SapphireAbsorptionLength(G4double energy) {

  // see  http://www.crystran.co.uk/optical-materials/sapphire-al2o3
  // absorption threshold is  230 nm

  G4double x = 1239.841930/energy ; // wavelenth in nanometers                                                                                                                           
  if (x < 230.0)  return 0 ;
  return 100*cm;
}

G4double  DetGammaDetectorConstruction::BK7AbsorptionLength(G4double energy) {

  // see  http://rmico.com/technical-notes/bk7-quartz-ge-si
  // absorption threshold is  300 nm

  G4double x = 1239.841930/energy ; // wavelenth in nanometers                                                                                                                           
  if (x < 300.0)  return 0 ;
  return 100*cm;
}


// 6080 White Reflectance Coating
G4double  DetGammaDetectorConstruction::LambertianCoatingReflection(G4double energy) {

  G4double x = 1239.841930/energy ; // wavelenth in nanometers 
  if (x <  275  ) return 0.930;
  else if (x<350) return 0.950;
  else if (x<450) return 0.970;
  else if (x<550) return 0.975;
  else if (x<650) return 0.975;
  else if (x<750) return 0.975;
  else if (x<850) return 0.975;
  else if (x<950) return 0.970; 
  else if (x<1050) return 0.965;
  else if (x<1150) return 0.960;
  else if (x<1250) return 0.950;
  else if (x<1350) return 0.945;
  else if (x<1450) return 0.930;
  else if (x<1550) return 0.915;
  else if (x<1650) return 0.920;
  else if (x<1750) return 0.918;
  else if (x<1900) return 0.910;
  else if (x<2100) return 0.850;
  else if (x<2300) return 0.865;
  return 0.86 ;
}

DetGammaDetectorConstruction::DetGammaDetectorConstruction(DetGammaMessenger* messenger)
: G4VUserDetectorConstruction()
{ 
  if (!messenger) throw std::runtime_error("DetGammaMessenger should be created before initializing DetGammaDetectorConstruction") ;
  _messenger = messenger ;
}


DetGammaDetectorConstruction::~DetGammaDetectorConstruction()
{ }


G4VPhysicalVolume* DetGammaDetectorConstruction::Construct()
{ 


  COATINGS coating = WHITE ;
  if (_messenger->GetCoating() == "black") coating = BLACK ;
  else if (_messenger->GetCoating() == "diffuse_white") coating = DIFFUSE_WHITE;
  // front surface is black everything else is white
  else if (_messenger->GetCoating() == "black_and_white") coating = BLACK_AND_WHITE;
  else if (_messenger->GetCoating() == "black_and_diffuse_white") coating = BLACK_AND_DIFFUSE_WHITE;

  bool noGrease= !_messenger->isWithGrease();

  if (!noGrease &&  !_messenger->isWithWindow())
    throw std::runtime_error("ERROR: options noGrease and not withWindow are not compatible!") ;
  

  SHAPES shape = CYLINDER ;
  if (_messenger->GetShape() == "box") shape = BOX ;

  // add PM in front of detector
  bool secondPM = _messenger->isSecondPM() ;
  
  if (secondPM && coating == BLACK_AND_WHITE) 
    throw std::runtime_error("ERROR: options secondPM and black_and_white are not compatible!") ;
  if (secondPM && coating == BLACK_AND_DIFFUSE_WHITE) 
    throw std::runtime_error("ERROR: options secondPM and black_and_diffuse_white are not compatible!") ;

  //==================== GEOMETRY ====================

  // Crystal 
  G4double crystal_dx = 23*mm, crystal_dy = 23*mm ;
  G4double crystal_d = 24*mm ;

  G4double crystal_dz = _messenger->GetCrystalLength()*mm;

  // PM Window
  G4double window_dx = 26*mm; 
  G4double window_dz = shape == CYLINDER ? 3.0*mm : 1.5*mm ;
  G4double window_d = 26*mm ;
  if (!_messenger->isWithWindow()) window_dz = 0 ;

  // Optical grease
  G4double grease_dx = crystal_dx;
  G4double grease_d = crystal_d;
  G4double grease_dz = noGrease ? 0 : 0.3*mm ;

  // PM Photocathode
  G4double cathode_dz = 0.5*mm;

  //PM body 
  G4double PM_dz = 24*mm ;
  G4double PM_d = window_d ;
  

  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // World parameters
  //
  G4double world_sizeXY = 50*mm, world_sizeZ = 70*mm;
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");  
  
  G4MaterialPropertiesTable *AirPT = new G4MaterialPropertiesTable();
  {
    const G4int num = 2;
    G4double ephoton[num] = {1*eV, 7*eV};  
    G4double rindex[num] = {1.0, 1.0};    
    
    AirPT->AddProperty("RINDEX",ephoton,rindex,num);
  }
  world_mat->SetMaterialPropertiesTable(AirPT);

  /*  
  {
    G4cout << world_mat << std::endl ;
    
    G4cout << "==================== Properties world_mat ====================" << G4endl ;  
    G4MaterialPropertiesTable* table = world_mat->GetMaterialPropertiesTable() ;
    if (table) table->DumpTable() ;
    else G4cout << "NO TABLE!" ;
    G4cout << G4endl ;
    G4cout << "=========================================================" << G4endl ;  
  }

  */


  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
  //     
  // Crystal
  //


  G4Element* Pb = nist->FindOrBuildElement("Pb");
  //  G4cout << Pb << G4endl ;

  G4Element* Fluorine = nist->FindOrBuildElement("F");
  //  G4cout << Fluorine << G4endl ;

  G4double density = 7.77*g/cm3;
  G4Material* PbF2 = new G4Material("Lead_Fluoride",density,2);

  PbF2->AddElement(Pb, 1);
  PbF2->AddElement(Fluorine, 2);

  // Add optical properties of the material

  G4MaterialPropertiesTable *MPT = new G4MaterialPropertiesTable();
  
  const G4int NUMENTRIES = 120;
  
  G4double ppckov[NUMENTRIES]  ;
  G4double rindex[NUMENTRIES]  ;
  G4double absorption[NUMENTRIES] ;
  
  
  G4double threshold = 240.0 ; // threshold in nm
  G4double step =  5.0 / NUMENTRIES ; // in eV
  for (G4int i=0; i < NUMENTRIES; i++) {
    G4double energy = 1239.841930/threshold  - i*step ; // in eV
    ppckov[i] = energy * eV ;
    rindex[i] = PbF2RefractionIndex(energy) ;
    absorption[i] =  PbF2AbsorptionLength(energy) ;
    //G4cout << "energy="<< energy << " eV lambda="<<1239.841930/energy << " nm R=" << rindex[i] << " abs=" << absorption[i]/mm << " mm" << G4endl ;
  }
  
  //  MPT -> AddConstProperty("SCINTILLATIONYIELD",100./MeV);
  // MPT -> AddProperty("RINDEX",ppckov,rindex,NUMENTRIES)->SetSpline(true);
  MPT -> AddProperty("RINDEX",ppckov,rindex,NUMENTRIES);
  //  MPT -> AddProperty("ABSLENGTH",ppckov,absorption,NUMENTRIES)->SetSpline(true);
  MPT -> AddProperty("ABSLENGTH",ppckov,absorption,NUMENTRIES);
  
  PbF2 -> SetMaterialPropertiesTable(MPT);
     
  G4ThreeVector pos_crystal = G4ThreeVector(0*mm, 0*mm, 0*mm);

  G4LogicalVolume*  crystal=0 ;
  G4LogicalVolume*  window=0 ;

  if (shape == CYLINDER) {
    G4Tubs* solidCrystal = new G4Tubs("Crystal", 0*mm, crystal_d/2, crystal_dz/2, 0.*deg, 360.*deg) ;
    crystal = new G4LogicalVolume(solidCrystal, PbF2, "Crystal"); 
  } else {
    G4Box* solidCrystal =    
      new G4Box("Crystal", 0.5*crystal_dx, 0.5*crystal_dy, 0.5*crystal_dz);        
    crystal = new G4LogicalVolume(solidCrystal, PbF2, "Crystal"); 
  }
  
  G4VPhysicalVolume* physCrystal = new G4PVPlacement(0,                       //no rotation
						     pos_crystal,             //at position
						     crystal,                //its logical volume
						     "Crystal",               //its name
						     logicWorld,                //its mother  volume
						     false,                   //no boolean operation
						     0,                       //copy number
						     checkOverlaps);          //overlaps checking


  //     
  // Window
  //

  //  G4Material* window_mat = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

  // use Sapphire (Al2O3) for the cylinder  geometry
  G4Material* window_mat =  shape == CYLINDER ? nist->FindOrBuildMaterial("G4_ALUMINUM_OXIDE")
    : nist->FindOrBuildMaterial("G4_Pyrex_Glass");

  for (G4int i=0; i < NUMENTRIES; i++) {
    G4double energy = 1239.841930/threshold  - i*step ; // in eV
    ppckov[i] = energy * eV ;
    rindex[i] = shape == CYLINDER ? SapphireRefractionIndex(energy) : BK7RefractionIndex(energy) ;
    absorption[i] =  shape == CYLINDER ? SapphireAbsorptionLength(energy) : BK7AbsorptionLength(energy) ;    
    //    G4cout << "energy="<< energy << " eV lambda="<<1239.841930/energy << " nm R=" << rindex[i] << " abs=" << absorption[i]/mm << " mm" << G4endl ;
  }

  G4MaterialPropertiesTable *MPT1 = new G4MaterialPropertiesTable();
  //MPT1 -> AddProperty("RINDEX",ppckov,rindex,NUMENTRIES)->SetSpline(true);
  MPT1 -> AddProperty("RINDEX",ppckov,rindex,NUMENTRIES);
  //  MPT1 -> AddProperty("ABSLENGTH",ppckov,absorption,NUMENTRIES)->SetSpline(true);
  MPT1 -> AddProperty("ABSLENGTH",ppckov,absorption,NUMENTRIES);
  window_mat -> SetMaterialPropertiesTable(MPT1);

 
  G4Material* grease_mat = nist->FindOrBuildMaterial("G4_Si");

  for (G4int i=0; i < NUMENTRIES; i++) {
    G4double energy = 1239.841930/threshold  - i*step ; // in eV
    ppckov[i] = energy * eV ;

    if (_messenger->GetGreaseName().find("OCF452") != string::npos) {
      rindex[i] = 1 + 0.4882 + 10485.0 / pow(1239.841930/energy,2);
      if (1239.841930/energy <= 300 || 1239.841930/energy >= 1000.0) absorption[i] = 0 ;
      else absorption[i] =  50*mm ;
    } else if (_messenger->GetGreaseName().find("OCF463") != string::npos) {
      rindex[i] = 1 + 0.5985 + 10629.0 / pow(1239.841930/energy,2);
      if (1239.841930/energy <= 400 || 1239.841930/energy >= 1000.0) absorption[i] = 0 ;
      else absorption[i] =  50*mm ;      
    } else {      
      rindex[i] = 1.465 ;
      if (1239.841930/energy <= 270 || 1239.841930/energy >= 700.0) absorption[i] = 0 ;
      else absorption[i] =  50*mm ;
    }
  }

  G4MaterialPropertiesTable *MPT2 = new G4MaterialPropertiesTable();
  MPT2 -> AddProperty("RINDEX",ppckov,rindex,NUMENTRIES);
  MPT2 -> AddProperty("ABSLENGTH",ppckov,absorption,NUMENTRIES);

  grease_mat -> SetMaterialPropertiesTable(MPT2);
  

  G4Material* PM_mat = 0 ;
  if (secondPM) {

    // need to define independently Al2O3 from sapphire, in order to specify different absorption length
    G4Element* Al = nist->FindOrBuildElement("Al");
    G4Element* Oxygen = nist->FindOrBuildElement("O");
    
    PM_mat = new G4Material("Al2O3",3.97*g/cm3, 2);
    PM_mat->AddElement(Al, 2);
    PM_mat->AddElement(Oxygen, 3);

    //    PM_mat = nist->FindOrBuildMaterial("G4_ALUMINUM_OXIDE") ;  
    G4MaterialPropertiesTable *PMPT = new G4MaterialPropertiesTable();
    
    const G4int num = 2;
    G4double ephoton[num] = {1*eV, 7*eV};  
    G4double rindex1[num] = {1.0, 1.0} ;
    G4double absorption1[num] = {0.01*mm, 0.01*mm} ;
    PMPT->AddProperty("RINDEX",ephoton,rindex1,num);
    PMPT->AddProperty("ABSLENGTH",ephoton,absorption1,num);
    PM_mat -> SetMaterialPropertiesTable(PMPT);
  }


  // print properties of the material
  /*
  G4cout << PbF2 << G4endl ;
  
  G4cout << "==================== Properties PbF2 ====================" << G4endl ;
  G4MaterialPropertiesTable* table = PbF2->GetMaterialPropertiesTable() ;
  if (table) table->DumpTable() ;
  else G4cout << "NO TABLE!" ;
  G4cout << G4endl ;
  G4cout << "=========================================================" << G4endl ;  


  G4cout << window_mat << G4endl ;

  G4cout << "==================== Window properties ====================" << G4endl ;
  G4MaterialPropertiesTable*  table = window_mat->GetMaterialPropertiesTable() ;
  if (table) table->DumpTable() ;
  else G4cout << "NO TABLE!" ;
  G4cout << G4endl ;
  G4cout << "=========================================================" << G4endl ;  


  {
    G4cout << grease_mat << std::endl ;
    
    G4cout << "==================== Properties grease_mat ====================" << G4endl ;  
    G4MaterialPropertiesTable* table = grease_mat->GetMaterialPropertiesTable() ;
    if (table) table->DumpTable() ;
    else G4cout << "NO TABLE!" ;
    G4cout << G4endl ;
    G4cout << "=========================================================" << G4endl ;  
  }
  */


  // optical grease
  


  G4ThreeVector pos_grease = G4ThreeVector(0, 0, 0.5*crystal_dz+grease_dz/2);

  G4LogicalVolume* logGrease = 0 ;
  if (!noGrease && shape == CYLINDER) {
    G4Tubs* solidGrease  = noGrease ? 0 : new G4Tubs("Grease", 0, grease_d/2, 0.5*grease_dz, 0.*deg, 360.*deg);
    logGrease = new G4LogicalVolume(solidGrease, grease_mat, "Grease");
  } else if (!noGrease) {
    G4Box* solidGrease  = noGrease ? 0 : new G4Box("Grease", 0.5*grease_dx, 0.5*grease_dx, 0.5*grease_dz);
    logGrease = new G4LogicalVolume(solidGrease, grease_mat, "Grease");
  }

  G4VPhysicalVolume* physGrease = noGrease ? 0 : new G4PVPlacement(0,                       //no rotation
								   pos_grease,                    //at position
								   logGrease,             //its logical volume
								   "Grease",                //its name
								   logicWorld,                //its mother  volume
								   false,                   //no boolean operation
								   0,                       //copy number
								   checkOverlaps);          //overlaps checking


  
  G4VPhysicalVolume* physWindow = 0 ;
  if (_messenger->isWithWindow()) {
    
    G4ThreeVector pos_window = G4ThreeVector(0, 0, 0.5*crystal_dz+grease_dz+window_dz/2);
    
    
    if (shape == CYLINDER) {
      G4Tubs* solidWindow = new G4Tubs("Window", 0*mm, window_d/2, window_dz/2, 0.*deg, 360.*deg) ;  
      window = new G4LogicalVolume(solidWindow, window_mat, "Window");    
    } else {
      G4Box* solidWindow  = new G4Box("Window", 0.5*window_dx, 0.5*window_dx, 0.5*window_dz);     //its size
      window = new G4LogicalVolume(solidWindow, window_mat, "Window");    
    }
    
    physWindow = new G4PVPlacement(0,                       //no rotation
						      pos_window,                    //at position
						      window,             //its logical volume
						      "Window",                //its name
						      logicWorld,                //its mother  volume
						      false,                   //no boolean operation
						      0,                       //copy number
						      checkOverlaps);          //overlaps checking
  }

  G4Material* bialkali = nist->FindOrBuildMaterial("G4_Sb");    
  /*
  G4MaterialPropertiesTable *BialkaliPT = new G4MaterialPropertiesTable();
  {
    const G4int num = 2;
    G4double ephoton[num] = {1*eV, 7*eV};  
    G4double rindex1[num] = {1.0, 1.0}
    G4double absorption[num] = {0.1*mm, 0.1*mm}
    BialkaliPT->AddProperty("RINDEX",ephoton,rindex1,num);
    BialkaliPT->AddProperty("ABSLENGTH",ephoton,absorption,num);
  }
  bialkali->SetMaterialPropertiesTable(BialkaliPT);
  */


  G4ThreeVector pos_cathode = G4ThreeVector(0, 0, 0.5*crystal_dz+grease_dz+window_dz+cathode_dz/2);

  G4LogicalVolume* logCathode = 0 ;

  if (shape == CYLINDER) {
    G4Tubs* solidCathode = new G4Tubs("Cathode", 0*mm, window_d/2, cathode_dz/2, 0.*deg, 360.*deg) ;
    logCathode = new G4LogicalVolume(solidCathode, bialkali,"Cathode");   
  } else {
    G4Box* solidCathode = new G4Box("Cathode", 0.5*window_dx, 0.5*window_dx, cathode_dz/2) ;
    logCathode = new G4LogicalVolume(solidCathode, bialkali,"Cathode");   
  }
  
  G4VPhysicalVolume* physCathode = new G4PVPlacement(0,                       //no rotation
						    pos_cathode,             //at position
						    logCathode,                //its logical volume
						    "Cathode",          //its name
						    logicWorld,              //its mother  volume
						    false,                   //no boolean operation
						    0,                       //copy number
						    checkOverlaps);          //overlaps checking


  //================================================================================

  G4VPhysicalVolume* physGrease1 = 0 ;
  G4VPhysicalVolume* physWindow1 = 0 ;
  G4VPhysicalVolume* physCathode1 = 0 ;
  G4VPhysicalVolume* physEmptySpace = 0; 

  if (secondPM) {    
    
    G4ThreeVector pos_grease1 = G4ThreeVector(0, 0, -0.5*crystal_dz-grease_dz/2);
    physGrease1 = noGrease ? 0 : 
      new G4PVPlacement(0,                       //no rotation
			pos_grease1,              //at position
			logGrease,              //its logical volume
			"Grease",                //its name
			logicWorld,              //its mother  volume
			false,                   //no boolean operation
			1,                       //copy number
			checkOverlaps);          //overlaps checking


  
  if (_messenger->isWithWindow()) {
    G4ThreeVector pos_window1 = G4ThreeVector(0, 0, -0.5*crystal_dz-grease_dz-window_dz/2);
    physWindow1 = new G4PVPlacement(0,                       //no rotation
				    pos_window1,                    //at position
				    window,             //its logical volume
				    "Window",                //its name
				    logicWorld,                //its mother  volume
				    false,                   //no boolean operation
				    1,                       //copy number
				    checkOverlaps);          //overlaps checking
  }
    
    G4ThreeVector pos_cathode1 = G4ThreeVector(0, 0, -0.5*crystal_dz-grease_dz-window_dz-cathode_dz/2);  
    physCathode1 = new G4PVPlacement(0,                       //no rotation
				     pos_cathode1,             //at position
				     logCathode,                //its logical volume
				     "Cathode",          //its name
				     logicWorld,              //its mother  volume
				     false,                   //no boolean operation
				     1,                       //copy number
				     checkOverlaps);          //overlaps checking
    
    
    G4ThreeVector pos_PM = G4ThreeVector(0, 0, -0.5*crystal_dz-grease_dz-window_dz-cathode_dz-PM_dz/2);

    G4LogicalVolume* logPM = 0 ;
    
    if (shape == CYLINDER) {
      G4Tubs* solidPM = new G4Tubs("PM", 0*mm, PM_d/2, PM_dz/2, 0.*deg, 360.*deg) ;
      logPM = new G4LogicalVolume(solidPM, PM_mat,"PM");   
    } else {
      G4Box* solidPM = new G4Box("PM", 0.5*window_dx, 0.5*window_dx, PM_dz/2) ;
      logPM = new G4LogicalVolume(solidPM, PM_mat,"PM");   
    }
  
    new G4PVPlacement(0,             //no rotation
		      pos_PM,        //at position
		      logPM,         //its logical volume
		      "PM",          //its name
		      logicWorld,    //its mother  volume
		      false,         //no boolean operation
		      0,             //copy number
		      checkOverlaps);//overlaps checking
  }  else if (coating == BLACK_AND_WHITE || coating == BLACK_AND_DIFFUSE_WHITE) {

    // define an effective volum in order to have one more surface
    
    G4double EmptySpace_dz = 1*mm ;
    G4LogicalVolume* logEmptySpace = 0 ;
    if (shape == CYLINDER) {
      G4Tubs* solidEmptySpace = new G4Tubs("EmptySpace", 0*mm, crystal_d/2, EmptySpace_dz/2, 0.*deg, 360.*deg) ;
      logEmptySpace = new G4LogicalVolume(solidEmptySpace, world_mat,"EmptySpace");   
    } else {
      G4Box* solidEmptySpace = new G4Box("EmptySpace", 0.5*crystal_dx, 0.5*crystal_dx, EmptySpace_dz/2) ;
      logEmptySpace = new G4LogicalVolume(solidEmptySpace, world_mat,"EmptySpace");   
    }
    
    G4ThreeVector pos_EmptySpace = G4ThreeVector(0, 0, -0.5*crystal_dz-EmptySpace_dz/2);
    physEmptySpace = new G4PVPlacement(0,             //no rotation
		      pos_EmptySpace,        //at position
		      logEmptySpace,         //its logical volume
		      "EmptySpace",          //its name
		      logicWorld,    //its mother  volume
		      false,         //no boolean operation
		      0,             //copy number
		      checkOverlaps);//overlaps checking

                     
  }
// if (secondPM)
  
  /*
    //---pmt sensitive detector
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    DetGammaPMTSD* pmt_SD = new DetGammaPMTSD("/DetGamma/pmtSD");
    SDman->AddNewDetector(pmt_SD);
    pmt_SD->InitPMTs(1) ;
    pmt_SD->SetPMTPos(1, 0, 0, 0.5*crystal_dz+window_dz+cathode_dz/2) ;
    //-------

    //sensitive detector is not actually on the photocathode.
    //processHits gets done manually by the stepping action.
    //It is used to detect when photons hit and get absorbed&detected at the
    //boundary to the photocathode (which doesnt get done by attaching it to a
    //logical volume.
    //It does however need to be attached to something or else it doesnt get
    //reset at the begining of events
    cathode->SetSensitiveDetector(pmt_SD);
  */

    // surface grease/crystal
    
  G4OpticalSurface* optSurfGrease = 0 ;
  if (!noGrease) {
    optSurfGrease = new G4OpticalSurface("optSurfGrease");
    optSurfGrease->SetType(dielectric_dielectric);
    optSurfGrease->SetModel(unified);
    optSurfGrease->SetFinish(polished);

    G4MaterialPropertiesTable *BorderGPT = new G4MaterialPropertiesTable();
    {
      const G4int num = 2;
      G4double ephoton[num] = {1.00*eV, 7*eV};
      G4double reflectivity[num] = {1.0, 1.0};
      BorderGPT->AddProperty("REFLECTIVITY", ephoton, reflectivity, num);
    }
    optSurfGrease->SetMaterialPropertiesTable(BorderGPT) ;

    new G4LogicalBorderSurface("logSurfGrease",physCrystal,physGrease,optSurfGrease);
    if (secondPM) new G4LogicalBorderSurface("logSurfGrease",physCrystal,physGrease1,optSurfGrease);
  }
  
  // surface window/grease or window crystal
  
  if (_messenger->isWithWindow()) {
    G4OpticalSurface* optSurfWindow = new G4OpticalSurface("optSurfWindow");
    optSurfWindow->SetType(dielectric_dielectric);
    optSurfWindow->SetModel(unified);
    optSurfWindow->SetFinish(polished);
    G4MaterialPropertiesTable *BorderWPT = new G4MaterialPropertiesTable();
    {
      const G4int num = 2;
      G4double ephoton[num] = {1*eV, 7*eV};
      // here (for the "polished" model) reflectivity is (1-absorption)
      G4double reflectivity[num] = {1.0, 1.0};
      BorderWPT->AddProperty("REFLECTIVITY", ephoton, reflectivity, num);
    }
    
    optSurfWindow->SetMaterialPropertiesTable(BorderWPT) ;
  }



  // surface crystal/World  
 
  G4MaterialPropertiesTable *BorderPT = new G4MaterialPropertiesTable();
  {
    G4double reflectivity[NUMENTRIES] ;
    for (G4int i=0; i < NUMENTRIES; i++) {
      G4double energy = 1239.841930/threshold  - i*step ; // in eV
      ppckov[i] = energy * eV ;
      reflectivity[i] = 0.0 ;
      if (coating == WHITE || coating == BLACK_AND_WHITE) reflectivity[i] = 1.0 ;
      else if (coating == DIFFUSE_WHITE || coating == BLACK_AND_DIFFUSE_WHITE) reflectivity[i] = LambertianCoatingReflection(energy) ;
    }
    BorderPT->AddProperty("REFLECTIVITY", ppckov, reflectivity, NUMENTRIES);      
  }
  
  G4OpticalSurface* optSurfWorldCryst = new G4OpticalSurface("optSurfWorldCryst");
  new G4LogicalBorderSurface("logSurfWorldCryst",physCrystal,physWorld,optSurfWorldCryst);
  optSurfWorldCryst->SetType(dielectric_dielectric);
  optSurfWorldCryst->SetModel(unified);
  if (coating == DIFFUSE_WHITE) optSurfWorldCryst->SetFinish(groundfrontpainted) ;
  else optSurfWorldCryst->SetFinish(polishedfrontpainted) ;
  optSurfWorldCryst->SetMaterialPropertiesTable(BorderPT) ;
  
  
  // Surface window-photocathode or crystal photocathode
  G4OpticalSurface* optSurfCathode = new G4OpticalSurface("optSurfCathode",unified,polished,
							  dielectric_metal);
  G4MaterialPropertiesTable* cathode_mt = new G4MaterialPropertiesTable();
  {
    const G4int num = 2;
    G4double ephoton[num] = {1*eV, 7*eV};
    G4double efficiency[num]={1.,1.}; //Enables 'detection' of photons
    G4double reflectivity[num] = {0.0, 0.0} ;

    cathode_mt->AddProperty("REFLECTIVITY", ephoton, reflectivity, num);      
    cathode_mt->AddProperty("EFFICIENCY",ephoton,efficiency,num);
  }
   
  optSurfCathode->SetMaterialPropertiesTable(cathode_mt);

  if (_messenger->isWithWindow()) {
    new G4LogicalBorderSurface("logSurfCathode", physWindow, physCathode, optSurfCathode);
    if (secondPM) new G4LogicalBorderSurface("logSurfCathode", physWindow1, physCathode1, optSurfCathode);
  } else {
    new G4LogicalBorderSurface("logSurfCathode", physCrystal, physCathode, optSurfCathode);
    if (secondPM) new G4LogicalBorderSurface("logSurfCathode", physCrystal, physCathode1, optSurfCathode);
  }

  if (physEmptySpace) {

  // front surface
    
    G4MaterialPropertiesTable *frontPT = new G4MaterialPropertiesTable();
    {
      G4double reflectivity[NUMENTRIES] ;
      for (G4int i=0; i < NUMENTRIES; i++) {
	G4double energy = 1239.841930/threshold  - i*step ; // in eV
	ppckov[i] = energy * eV ;
	reflectivity[i] = 0.0 ;
      }
      frontPT->AddProperty("REFLECTIVITY", ppckov, reflectivity, NUMENTRIES);      
    }

    G4OpticalSurface* optSurfFrontCryst = new G4OpticalSurface("optSurfFrontCryst");
    new G4LogicalBorderSurface("logSurfFrontCryst",physCrystal,physEmptySpace,optSurfFrontCryst);
    optSurfFrontCryst->SetType(dielectric_dielectric);
    optSurfFrontCryst->SetModel(unified);
    optSurfFrontCryst->SetMaterialPropertiesTable(frontPT) ;
  }

  //
  //always return the physical World
  //
  return physWorld;
}

