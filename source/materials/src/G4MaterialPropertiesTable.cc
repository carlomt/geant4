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
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: G4MaterialPropertiesTable.cc,v 1.12 2001/10/17 07:59:54 gcosmo Exp $
// GEANT4 tag $Name: geant4-04-01 $
//
// 
////////////////////////////////////////////////////////////////////////
// G4MaterialPropertiesTable Implementation
////////////////////////////////////////////////////////////////////////
//
// File: G4MaterialPropertiesTable.cc 
// Version:     1.0
// Created:     1996-02-08
// Author:      Juliet Armstrong
// Updated:     1999-11-05 Migration from G4RWTPtrHashDictionary to STL
//                         by John Allison
//              1997-03-26 by Peter Gumplinger
//              > cosmetics (only)
// mail:        gum@triumf.ca
//
////////////////////////////////////////////////////////////////////////

#include "globals.hh"
#include "G4MaterialPropertiesTable.hh"

        //////////////
        // Operators
        //////////////

/**************
G4MaterialPropertiesTable&
G4MaterialPropertiesTable::operator =(const G4MaterialPropertiesTable& right)
{
        if (this == &right) return *this;
	
	// clear any current contents of MPT

        MPT.clearAndDestroy();

        // want to make an actual copy -- not a shallow copy which is
	// the default for RWTPrtHashDictionary's assignment operator
 
        G4RWTPtrHashDictionary<G4String, G4MaterialPropertyVector> 
						rightMPT(right.MPT);
        G4RWTPtrHashDictionaryIterator<G4String, G4MaterialPropertyVector> 
						rightIterator(rightMPT); 
        rightIterator.reset();
        while (++rightIterator) {
		G4MaterialPropertyVector *newProp =
                        new G4MaterialPropertyVector(*(rightIterator.value()));
                G4String *newKey =
                        new G4String(*(rightIterator.key()));
                MPT.insertKeyAndValue(newKey, newProp);
        }
        return *this;
}
**********/

        /////////////////
        // Constructors
        /////////////////

G4MaterialPropertiesTable::G4MaterialPropertiesTable() {}

/*********
G4MaterialPropertiesTable::G4MaterialPropertiesTable
			   (const G4MaterialPropertiesTable &right) : 
			   MPT(hashString), MPTiterator(MPT)
{
        // want to make an actual copy -- not a shallow copy which is
	// the default for RWTPrtHashDictionary's assignment operator

        G4RWTPtrHashDictionary<G4String, G4MaterialPropertyVector> 
						rightMPT(right.MPT);
        G4RWTPtrHashDictionaryIterator<G4String, G4MaterialPropertyVector> 
						rightIterator(rightMPT); 

        rightIterator.reset();

        while (++rightIterator) {
		G4MaterialPropertyVector *newProp =
                        new G4MaterialPropertyVector(*(rightIterator.value()));
                G4String *newKey =
                        new G4String(*(rightIterator.key()));
                MPT.insertKeyAndValue(newKey, newProp);
        }
}
*******/

        ////////////////
        // Destructors
        ////////////////

G4MaterialPropertiesTable::~G4MaterialPropertiesTable()
{
  //	MPT.clearAndDestroy();
  MPTiterator i;
  for (i = MPT.begin(); i != MPT.end(); ++i) {
    delete (*i).second;
  }
  MPT.clear();
}

        ////////////
        // Methods
        ////////////

void G4MaterialPropertiesTable::AddProperty(const char     *key,
					    G4double *PhotonMomenta,
					    G4double *PropertyValues,
					    G4int     NumEntries)
{
	G4MaterialPropertyVector *mpv = 
			new G4MaterialPropertyVector(PhotonMomenta, 
					  	     PropertyValues, 
						     NumEntries);
	MPT [G4String(key)] = mpv;
}

void G4MaterialPropertiesTable::AddProperty(const char *key,
					    G4MaterialPropertyVector *mpv)
{
//	Provides a way of adding a property to the Material Properties
//	Table given an G4MaterialPropertyVector Reference and a key 

	MPT [G4String(key)] = mpv;
} 

void G4MaterialPropertiesTable::RemoveProperty(const char *key)
{
	MPT.erase(G4String(key));
}

G4MaterialPropertyVector* G4MaterialPropertiesTable::GetProperty(const char *key)
{
	return MPT [G4String(key)];
}

void G4MaterialPropertiesTable::AddEntry(const char     *key,
					 G4double  aPhotonMomentum,
					 G4double  aPropertyValue)
{
	G4MaterialPropertyVector *targetVector=MPT [G4String(key)];
	if (targetVector != 0) {
		targetVector->AddElement(aPhotonMomentum, aPropertyValue);
	}
	else {
		G4Exception("G4MaterialPropertiesTable::AddEntry ==> "
			    "Material Property Vector not found.");
	}
}

void G4MaterialPropertiesTable::RemoveEntry(const char *key,  
					    G4double  aPhotonMomentum)
{
        G4MaterialPropertyVector *targetVector=MPT [G4String(key)];
	if (targetVector) {
		targetVector->RemoveElement(aPhotonMomentum);
 	}
        else {
                G4Exception("G4MaterialPropertiesTable::RemoveEntry ==> "
			    "Material Property Vector not found.");
        }
}
void G4MaterialPropertiesTable::DumpTable()
{
  MPTiterator i;
  for (i = MPT.begin(); i != MPT.end(); ++i) {
		G4cout << (*i).first << G4endl;
                if ( (*i).second != 0 ) {
		  (*i).second->DumpVector();
                }
                else {
                  G4cout << "NULL Material Property Vector Pointer." << G4endl;
                }
  }
}