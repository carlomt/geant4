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
//
// $Id: G4VPreCompoundNucleon.hh,v 1.8 2002/06/06 17:15:00 larazb Exp $
// GEANT4 tag $Name: geant4-04-01 $
//
// by V. Lara

#ifndef G4VPreCompoundNucleon_h
#define G4VPreCompoundNucleon_h 1

#include "G4VPreCompoundFragment.hh"
#include "G4VCoulombBarrier.hh"


class G4VPreCompoundNucleon : public G4VPreCompoundFragment
{
protected:
  // default constructor
  G4VPreCompoundNucleon() {}

public:

  // copy constructor
  G4VPreCompoundNucleon(const G4VPreCompoundNucleon &right): 
    G4VPreCompoundFragment(right) {}

  // constructor  
    G4VPreCompoundNucleon(const G4double anA, 
			  const G4double aZ, 
			  G4VCoulombBarrier* aCoulombBarrier,
			  const G4String & aName): 
	G4VPreCompoundFragment(anA,aZ,aCoulombBarrier,aName) {}

    virtual ~G4VPreCompoundNucleon() {}

    // operators  
    const G4VPreCompoundNucleon & 
    operator=(const G4VPreCompoundNucleon &right) {
	if (&right != this) this->G4VPreCompoundFragment::operator=(right);
	return *this;
    }

    G4bool operator==(const G4VPreCompoundNucleon &right) const 
	{ return G4VPreCompoundFragment::operator==(right);}
    
    G4bool operator!=(const G4VPreCompoundNucleon &right) const 
	{ return G4VPreCompoundFragment::operator!=(right);}
    
    virtual G4double ProbabilityDistributionFunction(const G4double eKin,
						     const G4Fragment& aFragment);
    
protected:
  virtual G4double GetAlpha() = 0;
  virtual G4double GetBeta() = 0;
  virtual G4bool IsItPossible(const G4Fragment&) = 0; 
    
    
};

#endif