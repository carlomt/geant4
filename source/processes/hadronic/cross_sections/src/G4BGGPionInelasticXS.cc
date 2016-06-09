//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: G4BGGPionInelasticXS.cc,v 1.12 2011-01-09 02:37:48 dennis Exp $
// GEANT4 tag $Name: geant4-09-04-patch-01 $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:     G4BGGPionInelasticXS
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 01.10.2003
// Modifications:
//
// -------------------------------------------------------------------
//

#include "G4BGGPionInelasticXS.hh"
#include "G4GlauberGribovCrossSection.hh"
#include "G4UPiNuclearCrossSection.hh"
#include "G4HadronNucleonXsc.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4NistManager.hh"


G4BGGPionInelasticXS::G4BGGPionInelasticXS(const G4ParticleDefinition* p) 
 : G4VCrossSectionDataSet("Barashenkov-Glauber-Gribov")
{
  verboseLevel = 0;
  fGlauberEnergy = 91.*GeV;
  fLowEnergy = 20.*MeV;
  SetMinKinEnergy(0.0);
  SetMaxKinEnergy(100*TeV);

  for (G4int i = 0; i < 93; i++) {
    theGlauberFac[i] = 0.0;
    theCoulombFac[i] = 0.0;
  }
  fPion = 0;
  fGlauber = 0;
  fHadron  = 0;
  particle = p;
  isPiplus = false;
  isInitialized = false;
}


G4BGGPionInelasticXS::~G4BGGPionInelasticXS()
{
  delete fGlauber;
  delete fPion;
  delete fHadron;
}


G4double
G4BGGPionInelasticXS::GetZandACrossSection(const G4DynamicParticle* dp, 
                                           G4int Z, G4int A, G4double)
{
  G4double cross = 0.0;
  G4double ekin = dp->GetKineticEnergy();
//  G4int iz = G4int(Z);
  if(Z > 92) Z = 92;

  if(ekin <= fLowEnergy) {
    cross = theCoulombFac[Z];
    if(isPiplus) { cross *= CoulombFactor(ekin, A); } 

  } else if(Z == 1) {

    if( A < 2) {
      fHadron->GetHadronNucleonXscNS(dp, G4Proton::Proton());
      cross = fHadron->GetInelasticHadronNucleonXsc();
    } else {
      fHadron->GetHadronNucleonXscNS(dp, G4Proton::Proton());
      cross = fHadron->GetInelasticHadronNucleonXsc();
      fHadron->GetHadronNucleonXscNS(dp, G4Neutron::Neutron());
      cross += fHadron->GetInelasticHadronNucleonXsc();
    }

  } else if(ekin > fGlauberEnergy) {
    cross = theGlauberFac[Z]*fGlauber->GetInelasticGlauberGribov(dp, Z, A);

  } else {
    cross = fPion->GetZandACrossSection(dp, Z, A);
  }

  if(verboseLevel > 1) 
    G4cout << "G4BGGPionInelasticXS::GetCrossSection  for "
	   << dp->GetDefinition()->GetParticleName()
	   << "  Ekin(GeV)= " << dp->GetKineticEnergy()
	   << " in nucleus Z= " << Z << "  A= " << A
	   << " XS(b)= " << cross/barn 
	   << G4endl;

  return cross;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4BGGPionInelasticXS::BuildPhysicsTable(const G4ParticleDefinition& p)
{
  if(&p == G4PionPlus::PionPlus() || &p == G4PionMinus::PionMinus()) {
    particle = &p;
    Initialise();
  } else {
    G4cout << "### G4BGGPionInelasticXS WARNING: is not applicable to " 
	   << p.GetParticleName()
	   << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4BGGPionInelasticXS::DumpPhysicsTable(const G4ParticleDefinition&) 
{
  G4cout << "G4BGGPionInelasticXS:"<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4BGGPionInelasticXS::Initialise() 
{
  if(isInitialized) return;
  isInitialized = true;

  fPion = new G4UPiNuclearCrossSection();
  fGlauber = new G4GlauberGribovCrossSection();
  fHadron  = new G4HadronNucleonXsc();
  fPion->BuildPhysicsTable(*particle);
  fGlauber->BuildPhysicsTable(*particle);
  if(particle == G4PionPlus::PionPlus()) isPiplus = true;

  G4ParticleDefinition* part = const_cast<G4ParticleDefinition*>(particle);
  G4ThreeVector mom(0.0,0.0,1.0);
  G4DynamicParticle dp(part, mom, fGlauberEnergy);

  G4NistManager* nist = G4NistManager::Instance();

  G4double csup, csdn;
  G4int A;

  if(verboseLevel > 0) G4cout << "### G4BGGPionInelasticXS::Initialise for "
			      << particle->GetParticleName()
			      << " isPiplus: " << isPiplus
			      << G4endl;

  for(G4int iz=2; iz<93; iz++) {

    G4double Z = G4double(iz);
    A = G4lrint(nist->GetAtomicMassAmu(iz));

    csup = fGlauber->GetInelasticGlauberGribov(&dp, iz, A);
    csdn = fPion->GetInelasticCrossSection(&dp, iz, A);

    theGlauberFac[iz] = csdn/csup;
    if(verboseLevel > 0) G4cout << "Z= " << Z <<  "  A= " << A 
				<< " factor= " << theGlauberFac[iz] << G4endl; 
  }
  dp.SetKineticEnergy(fLowEnergy);
  fHadron->GetHadronNucleonXscNS(&dp, G4Proton::Proton());
  theCoulombFac[1] = fHadron->GetInelasticHadronNucleonXsc();
  if(isPiplus) { theCoulombFac[1] /= CoulombFactor(fLowEnergy,1); }
     
  for(G4int iz=2; iz<93; iz++) {

    G4double Z = G4double(iz);
    A = G4lrint(nist->GetAtomicMassAmu(iz));

    theCoulombFac[iz] = fPion->GetZandACrossSection(&dp, iz, A);
    if(isPiplus) { theCoulombFac[iz] /= CoulombFactor(fLowEnergy,A); }

    if(verboseLevel > 0) G4cout << "Z= " << Z <<  "  A= " << A 
				<< " factor= " << theCoulombFac[iz] << G4endl; 
  }
}


G4double G4BGGPionInelasticXS::CoulombFactor(G4double kinEnergy, G4int A)
{
  G4double res= 0.0;
  if(kinEnergy <= DBL_MIN) return res;
  else if(A < 2) return kinEnergy*kinEnergy;
  
  G4double elog = std::log10(kinEnergy/GeV);
  G4double aa = A;

  // from G4ProtonInelasticCrossSection
  G4double f1 = 8.0  - 8.0/aa - 0.008*aa;
  G4double f2 = 2.34 - 5.4/aa - 0.0028*aa;

  res = 1.0/(1.0 + std::exp(-f1*(elog + f2)));
 
  f1 = 5.6 - 0.016*aa;
  f2 = 1.37 + 1.37/aa;
  res *= ( 1.0 + (0.8 + 18./aa - 0.002*aa)/(1.0 + std::exp(f1*(elog + f2))));
  return res;  
}


