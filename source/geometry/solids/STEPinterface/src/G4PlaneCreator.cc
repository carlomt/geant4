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
// $Id: G4PlaneCreator.cc,v 1.4 2001/07/11 10:00:11 gunter Exp $
// GEANT4 tag $Name: geant4-04-01 $
//
// 
// ----------------------------------------------------------------------
// Class G4PlaneCreator
//
// Authors: J.Sulkimo, P.Urban.
// Revisions by: L.Broglia, G.Cosmo.
//
// History:
//   18-Nov-1999: First step of re-engineering - G.Cosmo
// ----------------------------------------------------------------------

#include "G4PlaneCreator.hh"
#include "G4GeometryTable.hh"
#include "G4FPlane.hh"

G4PlaneCreator G4PlaneCreator::csc;

G4PlaneCreator::G4PlaneCreator()
{
  G4GeometryTable::RegisterObject(this);
}

G4PlaneCreator::~G4PlaneCreator() {}

void G4PlaneCreator::CreateG4Geometry(STEPentity& Ent)
{
  // L. Broglia
  // G4Placement* place;
  G4Axis2Placement3D* place=0;
  
  Ent.ResetAttributes();
  STEPattribute *Attr;
  Attr = Ent.NextAttribute();    
  while (Attr->NonRefType() == STRING_TYPE)
    Attr = Ent.NextAttribute();

  // Get placement
  STEPentity* TmpEnt = *Attr->ptr.c;
  void *tmp =G4GeometryTable::CreateObject(*TmpEnt);
  
  G4FPlane* fplane = 0;

  if (tmp)
  {
    place = (G4Axis2Placement3D*)tmp;
    fplane = new G4FPlane( place->GetRefDirection(),
			   place->GetAxis(),
			   place->GetLocation());
  }
  else
    G4cerr << "WARNING - G4PlaneCreator::CreateG4Geometry" << G4endl
           << "\tUnexpected NULL axis placement (G4Axis2Placement3D) !" << G4endl
	   << "\tPlane Surface NOT created." << G4endl;

  createdObject = fplane;
}

void G4PlaneCreator::CreateSTEPGeometry(void* G4obj)
{
}
