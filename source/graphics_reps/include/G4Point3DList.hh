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
// $Id: G4Point3DList.hh,v 1.10 2001/08/20 23:28:42 johna Exp $
// GEANT4 tag $Name: geant4-04-01 $
//
// 
// John Allison  July 1995

// Class Description:
// A set of 3D points.
// Class Description - End:

#ifndef G4POINT3DLIST_HH
#define G4POINT3DLIST_HH

#include "g4std/vector"
#include "G4Point3D.hh"
#include "g4std/iostream"

class G4Point3DList: public G4std::vector<G4Point3D> {
  
  friend G4std::ostream& operator << (G4std::ostream& os, const G4Point3DList& points);
  
public:

  typedef G4std::vector<G4Point3D>::iterator iterator;

  virtual ~G4Point3DList();

};

#endif