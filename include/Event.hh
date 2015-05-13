#ifndef Event_h
#define Event_h 1

#include "globals.hh"
#include <vector>


class TFile;

class Particle {

  
  G4int id, processID ;
  G4double energy, theta, pre_theta ;
  G4double x, y, z ;
  G4double t ;

public: 
  Particle() ;
    
  Particle (G4int id_in,  G4double processID_in, G4double energy_in, 
	    G4double theta_in,  G4double pre_theta_in, 
	    G4double x_in, G4double y_in, G4double z_in, G4double time_in) ;  

  ~Particle() ;
} ;


class Event {

  G4bool photonisQE ;

  G4int evtnum ; 

  // the total number of photons with lambda  200 -- 800 nm
  G4int ngamma_cathode ;
  // the total number of photons after quantum efficiency is taken into acount
  G4int ngamma_cathode_qe ; 
  // the same only for the second cathode (if present)
  G4int ngamma_cathode1 ;
  G4int ngamma_cathode_qe1 ; 

  G4int ngamma_window;
  G4int ngamma_window1 ;
  G4int ngamma_grease ;
  G4int ngamma_cherenkov ;

  G4int nelectrons ;

  G4double time ; // arrival time in ps  of the first photon detected by the photocathode (efficiency is taken into account)
  G4double time_cherenkov; // arrival time in ps  of the first cherenkov photon detected by the photocathode (efficiency is taken into account)

  std::vector<Particle> particles ;
  
  // photons detected by the photocathode
  std::vector<Particle> photons ;

public :


  Event() ; 
  ~Event() ;

  void setEventNum(G4int evt) {evtnum=evt;}
  void usePhotonisQE() {photonisQE=true ;} 

  void fill(G4int id,  const G4String& process_name, G4double energy, 
	    G4double theta, G4double x, G4double y, G4double z, 
	    G4double time_in, G4int PM_number, G4double pre_theta) ;

  static G4double QuantumEfficiency_standard(G4double energy) ;
  static G4double QuantumEfficiency_Photonis(G4double energy) ;

  void clean() ;

} ;


#endif

