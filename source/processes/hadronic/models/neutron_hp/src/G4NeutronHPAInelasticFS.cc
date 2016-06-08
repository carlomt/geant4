//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * authors in the GEANT4 collaboration.                             *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
// neutron_hp -- source file
// J.P. Wellisch, Nov-1996
// A prototype of the low energy neutron transport model.
//
#include "G4NeutronHPAInelasticFS.hh"
#include "G4Nucleus.hh"
#include "G4Alpha.hh"

void G4NeutronHPAInelasticFS::Init (G4double A, G4double Z, G4String & dirName, G4String & aFSType)
{
   G4NeutronHPInelasticCompFS::Init(A, Z, dirName, aFSType);
   G4double ResidualA = A-3;
   G4double ResidualZ = Z-2;
   G4NeutronHPInelasticCompFS::InitGammas(ResidualA, ResidualZ);
}

G4ParticleChange * G4NeutronHPAInelasticFS::ApplyYourself(const G4Track & theTrack)
{

// do the final state
   G4NeutronHPInelasticCompFS::CompositeApply(theTrack, G4Alpha::Alpha());
             
// return the result
   return &theResult;
}