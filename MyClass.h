//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed May 13 10:39:45 2015 by ROOT version 5.34/26
// from TTree DetGamma/Gamma detector
// found on file: DetGamma.root
//////////////////////////////////////////////////////////

#ifndef MyClass_h
#define MyClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.
   const Int_t kMaxparticles = 196;
   const Int_t kMaxphotons = 9;

class MyClass {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
 //Event           *Event;
   Bool_t          photonisQE;
   Int_t           evtnum;
   Int_t           ngamma_cathode;
   Int_t           ngamma_cathode_qe;
   Int_t           ngamma_cathode1;
   Int_t           ngamma_cathode_qe1;
   Int_t           ngamma_window;
   Int_t           ngamma_window1;
   Int_t           ngamma_grease;
   Int_t           ngamma_cherenkov;
   Int_t           nelectrons;
   Double_t        time;
   Int_t           particles_;
   Int_t           particles_id[kMaxparticles];   //[particles_]
   Int_t           particles_processID[kMaxparticles];   //[particles_]
   Double_t        particles_energy[kMaxparticles];   //[particles_]
   Double_t        particles_theta[kMaxparticles];   //[particles_]
   Double_t        particles_pre_theta[kMaxparticles];   //[particles_]
   Double_t        particles_x[kMaxparticles];   //[particles_]
   Double_t        particles_y[kMaxparticles];   //[particles_]
   Double_t        particles_z[kMaxparticles];   //[particles_]
   Double_t        particles_t[kMaxparticles];   //[particles_]
   Int_t           photons_;
   Int_t           photons_id[kMaxphotons];   //[photons_]
   Int_t           photons_processID[kMaxphotons];   //[photons_]
   Double_t        photons_energy[kMaxphotons];   //[photons_]
   Double_t        photons_theta[kMaxphotons];   //[photons_]
   Double_t        photons_pre_theta[kMaxphotons];   //[photons_]
   Double_t        photons_x[kMaxphotons];   //[photons_]
   Double_t        photons_y[kMaxphotons];   //[photons_]
   Double_t        photons_z[kMaxphotons];   //[photons_]
   Double_t        photons_t[kMaxphotons];   //[photons_]

   // List of branches
   TBranch        *b_Event_photonisQE;   //!
   TBranch        *b_Event_evtnum;   //!
   TBranch        *b_Event_ngamma_cathode;   //!
   TBranch        *b_Event_ngamma_cathode_qe;   //!
   TBranch        *b_Event_ngamma_cathode1;   //!
   TBranch        *b_Event_ngamma_cathode_qe1;   //!
   TBranch        *b_Event_ngamma_window;   //!
   TBranch        *b_Event_ngamma_window1;   //!
   TBranch        *b_Event_ngamma_grease;   //!
   TBranch        *b_Event_ngamma_cherenkov;   //!
   TBranch        *b_Event_nelectrons;   //!
   TBranch        *b_Event_time;   //!
   TBranch        *b_Event_particles_;   //!
   TBranch        *b_particles_id;   //!
   TBranch        *b_particles_processID;   //!
   TBranch        *b_particles_energy;   //!
   TBranch        *b_particles_theta;   //!
   TBranch        *b_particles_pre_theta;   //!
   TBranch        *b_particles_x;   //!
   TBranch        *b_particles_y;   //!
   TBranch        *b_particles_z;   //!
   TBranch        *b_particles_t;   //!
   TBranch        *b_Event_photons_;   //!
   TBranch        *b_photons_id;   //!
   TBranch        *b_photons_processID;   //!
   TBranch        *b_photons_energy;   //!
   TBranch        *b_photons_theta;   //!
   TBranch        *b_photons_pre_theta;   //!
   TBranch        *b_photons_x;   //!
   TBranch        *b_photons_y;   //!
   TBranch        *b_photons_z;   //!
   TBranch        *b_photons_t;   //!

   MyClass(TTree *tree=0);
   virtual ~MyClass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef MyClass_cxx
MyClass::MyClass(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("DetGamma.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("DetGamma.root");
      }
      f->GetObject("DetGamma",tree);

   }
   Init(tree);
}

MyClass::~MyClass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t MyClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t MyClass::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void MyClass::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("photonisQE", &photonisQE, &b_Event_photonisQE);
   fChain->SetBranchAddress("evtnum", &evtnum, &b_Event_evtnum);
   fChain->SetBranchAddress("ngamma_cathode", &ngamma_cathode, &b_Event_ngamma_cathode);
   fChain->SetBranchAddress("ngamma_cathode_qe", &ngamma_cathode_qe, &b_Event_ngamma_cathode_qe);
   fChain->SetBranchAddress("ngamma_cathode1", &ngamma_cathode1, &b_Event_ngamma_cathode1);
   fChain->SetBranchAddress("ngamma_cathode_qe1", &ngamma_cathode_qe1, &b_Event_ngamma_cathode_qe1);
   fChain->SetBranchAddress("ngamma_window", &ngamma_window, &b_Event_ngamma_window);
   fChain->SetBranchAddress("ngamma_window1", &ngamma_window1, &b_Event_ngamma_window1);
   fChain->SetBranchAddress("ngamma_grease", &ngamma_grease, &b_Event_ngamma_grease);
   fChain->SetBranchAddress("ngamma_cherenkov", &ngamma_cherenkov, &b_Event_ngamma_cherenkov);
   fChain->SetBranchAddress("nelectrons", &nelectrons, &b_Event_nelectrons);
   fChain->SetBranchAddress("time", &time, &b_Event_time);
   fChain->SetBranchAddress("particles", &particles_, &b_Event_particles_);
   fChain->SetBranchAddress("particles.id", particles_id, &b_particles_id);
   fChain->SetBranchAddress("particles.processID", particles_processID, &b_particles_processID);
   fChain->SetBranchAddress("particles.energy", particles_energy, &b_particles_energy);
   fChain->SetBranchAddress("particles.theta", particles_theta, &b_particles_theta);
   fChain->SetBranchAddress("particles.pre_theta", particles_pre_theta, &b_particles_pre_theta);
   fChain->SetBranchAddress("particles.x", particles_x, &b_particles_x);
   fChain->SetBranchAddress("particles.y", particles_y, &b_particles_y);
   fChain->SetBranchAddress("particles.z", particles_z, &b_particles_z);
   fChain->SetBranchAddress("particles.t", particles_t, &b_particles_t);
   fChain->SetBranchAddress("photons", &photons_, &b_Event_photons_);
   fChain->SetBranchAddress("photons.id", photons_id, &b_photons_id);
   fChain->SetBranchAddress("photons.processID", photons_processID, &b_photons_processID);
   fChain->SetBranchAddress("photons.energy", photons_energy, &b_photons_energy);
   fChain->SetBranchAddress("photons.theta", photons_theta, &b_photons_theta);
   fChain->SetBranchAddress("photons.pre_theta", photons_pre_theta, &b_photons_pre_theta);
   fChain->SetBranchAddress("photons.x", photons_x, &b_photons_x);
   fChain->SetBranchAddress("photons.y", photons_y, &b_photons_y);
   fChain->SetBranchAddress("photons.z", photons_z, &b_photons_z);
   fChain->SetBranchAddress("photons.t", photons_t, &b_photons_t);
   Notify();
}

Bool_t MyClass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void MyClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t MyClass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef MyClass_cxx
