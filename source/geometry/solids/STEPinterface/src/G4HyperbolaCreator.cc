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
// $Id: G4HyperbolaCreator.cc,v 1.3 2001/07/11 10:00:10 gunter Exp $
// GEANT4 tag $Name: geant4-04-01 $
//
// 
// ----------------------------------------------------------------------
// Class G4HyperbolaCreator
//
// Authors: J.Sulkimo, P.Urban.
// Revisions by: L.Broglia, G.Cosmo.
//
// History:
//   18-Nov-1999: First step of re-engineering - G.Cosmo
// ----------------------------------------------------------------------

#include "G4HyperbolaCreator.hh"
#include "G4GeometryTable.hh"

G4HyperbolaCreator G4HyperbolaCreator::csc;

G4HyperbolaCreator::G4HyperbolaCreator()
{
  G4GeometryTable::RegisterObject(this);
}

G4HyperbolaCreator::~G4HyperbolaCreator() {}

void G4HyperbolaCreator::CreateG4Geometry(STEPentity& Ent)
{
  G4double semi1,semi_imag;

  // L. Broglia
  // G4Placement* place;
  G4Axis2Placement3D* place;
  
  Ent.ResetAttributes();
  STEPattribute* Attr = Ent.NextAttribute();
  while(Attr->NonRefType() == STRING_TYPE ||
	Attr->NonRefType() == sdaiSTRING )
    Attr = Ent.NextAttribute();	

  // Get the placement
    STEPentity* TmpEnt= *Attr->ptr.c;
  void *tmp =G4GeometryTable::CreateObject(*TmpEnt);

  // L. Broglia
  // place = (G4Placement*)tmp;
  place = (G4Axis2Placement3D*)tmp;
  
  // get semi axises
  Attr = Ent.NextAttribute();
  semi1 = *Attr->ptr.r;

  Attr = Ent.NextAttribute();	
  semi_imag = *Attr->ptr.r;  
}

void G4HyperbolaCreator::CreateSTEPGeometry(void* G4obj)
{
}