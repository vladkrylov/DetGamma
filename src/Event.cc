#include <stdexcept>

#include "Event.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"


// Photoctahode quantum efficiency function

G4double Event::QuantumEfficiency_standard(G4double energy) {
  
  G4double wavelength = 1239.841930/(energy/eV) ; // in nm
  
  if (wavelength >= 750.0) return 0.0 ;
  if (wavelength < 330.0) return 0.22 ;

  G4double x1=0, x2=0, y1=0, y2=0 ;
  
  /*
  if (wavelength < 300.0) { 
    x1 = 240.0 ;
    y1 = 0     ;
    x2 = 300.0 ;
    y2 = 0.22  ;
  } else if (wavelength < 330.0) {
    x1 = 300.0 ;
    y1 = 0.22  ;
    x2 = 330.0 ;
    y2 = 0.20  ;    
    } else if (wavelength < 400.0) {
    x1 = 330.0 ;
    y1 = 0.20  ;
    x2 = 400.0 ;
    y2 = 0.25  ;
  */
  if (wavelength < 400.0) {                                                                                                                                                     
    x1 = 330.0 ;                                                                                                                                                                            y1 = 0.22  ;
    x2 = 400.0 ;
    y2 = 0.25  ;
  } else if (wavelength < 430.0) {
    x1 = 400.0 ;
    y1 = 0.25  ;
    x2 = 430.0 ;
    y2 = 0.23  ;
  } else if (wavelength < 530.0) {
    x1 = 430.0 ;
    y1 = 0.23  ;
    x2 = 530.0 ;
    y2 = 0.12  ;
  } else {
    x1 = 530.0 ;
    y1 = 0.12  ;
    x2 = 750.0 ;
    y2 = 0.0   ;
  }

  return (y2-y1)/(x2-x1)*wavelength + (x2*y1-x1*y2)/(x2-x1) ;
}

// Use XP85112 curve (10 micro-metres holes)
G4double Event::QuantumEfficiency_Photonis(G4double energy) {
  
  G4double wavelength = 1239.841930/(energy/eV) ; // in nm
  
  if (wavelength < 200.0 || wavelength >= 650.0) return 0.0 ;
  
  G4double x1=0, x2=0, y1=0, y2=0 ;
  /*  
  if (wavelength < 240.0) return 0.16 ;
  else if (wavelength < 340.0) {
    x1 = 240.0 ;
    y1 = 0.16  ;
    x2 = 340.0 ;
    y2 = 0.21  ;
  */
  if (wavelength < 340.0) {
    return 0.21 ;
  } else if (wavelength < 380.0) {
    x1 = 340.0 ;
    y1 = 0.21  ;
    x2 = 380.0 ;
    y2 = 0.22  ;
  } else if (wavelength < 500.0) {
    x1 = 380.0 ;
    y1 = 0.21  ;
    x2 = 500.0 ;
    y2 = 0.11  ;
  } else if (wavelength < 540.0) {
    x1 = 500.0 ;
    y1 = 0.11  ;
    x2 = 540.0 ;
    y2 = 0.04  ;
  } else {
    x1 = 540.0 ;
    y1 = 0.04  ;
    x2 = 650.0 ;
    y2 = 0.0   ;
  }

  return (y2-y1)/(x2-x1)*wavelength + (x2*y1-x1*y2)/(x2-x1) ;
}

Particle::Particle ():
  id(0), processID(0),  energy(0), theta(0), pre_theta(0),
  x(0), y(0), z(0), t(0) {}

Particle::Particle (G4int id_in,  G4double processID_in, G4double energy_in, 
		    G4double theta_in, G4double pre_theta_in, 
		    G4double x_in, G4double y_in, G4double z_in, G4double time_in) :
  id(id_in), processID(processID_in),  energy(energy_in), theta(theta_in), pre_theta(pre_theta_in), 
  x(x_in), y(y_in), z(z_in), t(time_in) {} 


Particle::~Particle() {}

Event::Event() :  photonisQE(false), evtnum(0), 
		  ngamma_cathode(0), ngamma_cathode_qe(0), 
		  ngamma_cathode1(0), ngamma_cathode_qe1(0), 
		  ngamma_window(0), ngamma_window1(0), ngamma_grease(0), 
		  ngamma_cherenkov(0), nelectrons(0), 
		  time(-1000.0) {}
Event::~Event() {}

void Event::fill(G4int id,  const G4String& process_name, G4double energy, G4double theta, 
		 G4double x, G4double y, G4double z, 
		 G4double time_in, G4int PM_number, G4double pre_theta) 
{
 
  bool inrange = false  ;
  if (id == 22 && 1239.841930/(energy/eV) < 800.0 && 1239.841930/(energy/eV) > 200) inrange = true ;

  G4int processID = 0 ;
  if (process_name == "compt") processID = 1 ;
  else if (process_name == "phot") processID = 2 ;
  else if (process_name == "Cerenkov") { processID = 10 ; if (inrange) ngamma_cherenkov++;}
  else if (process_name == "SurfCrystalGrease") { processID = 20 ; if (inrange) ngamma_grease++ ;}
  else if (process_name == "SurfCrystalWindow") { processID = 21 ; 
    if (inrange) ngamma_window++ ;
    if (inrange && PM_number ==1) ngamma_window1++ ;
  } else if (process_name == "SurfGreaseWindow")  { processID = 21 ; 
    if (inrange) ngamma_window++ ;
    if (inrange && PM_number ==1) ngamma_window1++ ;
  } else if (process_name == "SurfWindowCathode") { processID = 22 ; 
    if (inrange) ngamma_cathode++; 
    if (inrange && PM_number ==1) ngamma_cathode1++ ;
  } else if (process_name == "SurfCrystalCathode") { processID = 22 ; 
    if (inrange) ngamma_cathode++; 
    if (inrange && PM_number ==1) ngamma_cathode1++ ;
  } else if (process_name == "SurfWindowCrystal") { processID = 19 ; 
  } else if (process_name == "SurfGreaseCrystal") { processID = 19 ; 
  } else {
    G4cout << "Process name [" << process_name<< "]" << G4endl ;
    throw std::runtime_error("Unknown process name") ;
  }
  
  if (id==11) nelectrons++ ;

  particles.push_back(Particle(id, processID, energy/eV, theta, pre_theta, x, y, z, time_in)) ;

  // convolute with quantum effitiency of the photocathode
  if (process_name != "SurfWindowCathode" && process_name != "SurfCrystalCathode")  return ;
  
  // randomly choose events according to the photocathode quantum efficiency
  // and photoelectron efficiency 60%
  //  if (photonisQE && G4UniformRand()  > QuantumEfficiency_Photonis(energy)*0.6) return ;
  if (photonisQE && G4UniformRand()  > QuantumEfficiency_Photonis(energy)) return ;
  else if ( !photonisQE && G4UniformRand()  > QuantumEfficiency_standard(energy)*0.6) return ;
 
  ngamma_cathode_qe++ ;
  if (PM_number == 1) ngamma_cathode_qe1++ ;
  if (time < 0 || time_in < time) time = time_in ;
  photons.push_back(Particle(id, processID, energy/eV, theta, pre_theta, x, y, z, time_in)) ;

}

void Event::clean() {
  evtnum = 0 ;
  ngamma_cathode = 0 ;
  ngamma_cathode_qe = 0 ;
  ngamma_cathode1 = 0 ;
  ngamma_cathode_qe1 = 0 ;
  ngamma_window = 0 ;
  ngamma_window1 = 0 ;
  ngamma_grease = 0 ;
  ngamma_cherenkov = 0  ;
  nelectrons = 0 ;
  time = -1000.0 ;
  particles.clear() ;
  photons.clear() ;
}
