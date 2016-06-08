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
// $Id: G4UIcommandTree.hh,v 1.13 2002/04/26 22:03:35 asaim Exp $
// GEANT4 tag $Name: geant4-04-01 $
//

#ifndef G4UIcommandTree_h
#define G4UIcommandTree_h 1


#include "G4UIcommand.hh"
#include "globals.hh"
#include "g4std/vector"

// class description:
//
//  This class is exclusively used by G4UImanager for handling the
// tree structure of the commands. The user MUST NOT construct/use
// this class object.

class G4UIcommandTree 
{
  public:
      G4UIcommandTree();
      G4UIcommandTree(const char * thePathName);
      ~G4UIcommandTree();
      G4int operator==(const G4UIcommandTree &right) const;
      G4int operator!=(const G4UIcommandTree &right) const;

  public:
      void AddNewCommand(G4UIcommand * newCommand);
      void RemoveCommand(G4UIcommand * aCommand);
      G4UIcommand * FindPath(const char* commandPath);
      void List();
      void ListCurrent();
      void ListCurrentWithNum();
      void CreateHTML();

  private:
      G4String CreateFileName(const char* pName);
      G4String ModStr(const char* strS);

      G4std::vector<G4UIcommand*> command;
      G4std::vector<G4UIcommandTree*> tree;
      G4UIcommand *guidance;
      G4String pathName;

  public:
      inline const G4UIcommand * GetGuidance() const
      { return guidance; };
      inline const G4String GetPathName() const
      { return pathName; };
      inline G4int GetTreeEntry() const
      { return tree.size(); };
      inline G4int GetCommandEntry() const
      { return command.size(); };
      inline G4UIcommandTree * GetTree(G4int i)
      { return tree[i-1]; };
      inline G4UIcommandTree * GetTree(const char* comNameC)
      { 
        G4String comName = comNameC;
        for( size_t i=0; i < tree.size(); i++)
        {
          if( comName == tree[i]->GetPathName() )
          { return tree[i]; }
        }
        return NULL;
      };
      inline G4UIcommand * GetCommand(G4int i)
      { return command[i-1]; };
      inline const G4String GetTitle() const
      { 
	    if(guidance==NULL)
	    { return G4String("...Title not available..."); }
    	else
	    { return guidance->GetTitle(); }
      };

};

#endif
