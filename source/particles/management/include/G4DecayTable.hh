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
// $Id: G4DecayTable.hh,v 1.7 2001/07/11 10:01:55 gunter Exp $
// GEANT4 tag $Name: geant4-04-01 $
//
//
// ------------------------------------------------------------
//      GEANT 4 class header file
//
//      History: first implementation, based on object model of
//      7 July 1996 H.Kurashige
//
// ----------------------------------------
//      implementation for STL          14 Feb. 2000 H.Kurashige
//      
// ------------------------------------------------------------

#ifndef G4DecayTable_h
#define G4DecayTable_h 1

#include "G4ios.hh"
#include "g4std/vector"
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4VDecayChannel.hh"

class G4DecayTable 
{
 // Class Description
 //   G4DecayTable is the table of pointer to G4VDecayChannel.
 //   Decay channels inside is sorted by using decay branching ratio
 //

 public:
  typedef G4std::vector<G4VDecayChannel*> G4VDecayChannelVector;

  //constructors
 public:
    G4DecayTable();
    ~G4DecayTable();

 private:
  // hide copy constructor and assignment operator by declaring "private"
  //  (Implementation does not make sense )
    G4DecayTable(const G4DecayTable &){};
    G4DecayTable & operator=(const G4DecayTable &){return *this;};

 public:
    // equality operators
    G4int operator==(const G4DecayTable &right) const {return (this == &right);};
    G4int operator!=(const G4DecayTable &right) const {return (this != &right);};

 public: // With Description
    void  Insert( G4VDecayChannel* aChannel);
    // Insert a decay channel at proper position 
    // (i.e. sorted by using branching ratio ) 

    G4int entries() const;
    // Returns number of decay channels inside

 public: // With Description
    G4VDecayChannel* SelectADecayChannel();
    // A decay channel is selected at random according to the branching ratio 
  
    G4VDecayChannel* GetDecayChannel(G4int index) const;
    G4VDecayChannel* operator[](G4int index);
    // Get index-th Decay channel
 
    void DumpInfo() const;

 private:
    G4ParticleDefinition       *parent;
    G4VDecayChannelVector       *channels;
};

inline     
 G4int G4DecayTable::entries() const
{
  return channels->size();
}

inline     
 G4VDecayChannel* G4DecayTable::operator[](G4int index)
{
  return (*channels)[index];
}

 
inline     
 G4VDecayChannel* G4DecayTable::GetDecayChannel(G4int index) const
{
  G4VDecayChannel* selectedChannel = 0;
  if ( (index>=0) && (index<G4int(channels->size())) ){
    selectedChannel = (*channels)[index];
  }
  return selectedChannel;
}
 
 
#endif