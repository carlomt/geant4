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
// $Id: G4VRML2File.hh,v 1.10 2002/06/23 03:31:42 stanaka Exp $
// GEANT4 tag $Name: geant4-04-01 $
//
// G4VRML2File.hh
// Satoshi Tanaka & Yasuhide Sawada

#ifndef G4VRML2FILE_HH
#define G4VRML2FILE_HH

#include "G4VGraphicsSystem.hh"

class G4VSceneHandler;

class G4VRML2File: public G4VGraphicsSystem {
public:
	G4VRML2File(); 
	virtual ~G4VRML2File();
	G4VSceneHandler* CreateSceneHandler(const G4String& name = "");
	G4VViewer*  CreateViewer(G4VSceneHandler&, const G4String& name = "");

};

#endif //G4VRML2FILE_HH