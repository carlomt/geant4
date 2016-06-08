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
// $Id: G3MedTable.cc,v 1.13 2001/07/16 15:38:20 gcosmo Exp $
// GEANT4 tag $Name: geant4-04-01 $
//
// by I.Hrivnacova, 27 Sep 99

#include "G3MedTable.hh"

G3MedTable::G3MedTable()
{
  fMedVector = new G3MediumVector();
}

G3MedTable::~G3MedTable()
{
  Clear();
  delete fMedVector;
}

G3MedTableEntry* G3MedTable::get(G4int id) const
{
  for (size_t i=0; i< fMedVector->size(); i++) {
    G3MedTableEntry* mte = (*fMedVector)[i];
    if (id == mte->GetID()) return mte;
  }
  return 0;
}    

void G3MedTable::put(G4int id, G4Material* material, G4MagneticField* field,
       G4UserLimits* limits, G4int isvol)
{
  G3MedTableEntry* mte 
    = new G3MedTableEntry(id, material, field, limits, isvol);
  fMedVector->push_back(mte);
}

void G3MedTable::Clear()
{
  G3MedTableEntry* a;
  while (fMedVector->size()>0) {
    a = fMedVector->back();
    fMedVector->pop_back();
    for (G3MediumVector::iterator i=fMedVector->begin();
                                  i!=fMedVector->end(); i++){
      if (*i==a) {
	fMedVector->erase(i);
	i--;
      }
    } 
    if ( a )  delete a;    
  } 
}