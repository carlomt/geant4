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
// $Id: G4VUserPhysicsList.cc,v 1.26 2001/10/23 02:36:38 kurasige Exp $
// GEANT4 tag $Name: geant4-04-01 $
//
// 
// ------------------------------------------------------------
//	GEANT 4 class header file 
//
// ------------------------------------------------------------
//	History
//        first version                   09 Jan. 1998 by H.Kurashige 
//        modified                        24 Jan. 1998 by H.Kurashige 
//        modified                        06 June 1998  by H.Kurashige 
//        add G4ParticleWithCuts::SetEnergyRange
//                                        18 June 1998  by H.Kurashige 
//       modifeid for short lived particles 27  June 1998  by H.Kurashige
//       G4BestUnit on output             12 nov. 1998  mma  
//       Added RemoveProcessManager        9 Feb. 1999 by H.Kurashige
//       Fixed RemoveProcessManager       15 Apr. 1999 by H.Kurashige
//       Removed ConstructAllParticles()  15 Apr. 1999 by H.Kurashige
//       modified                         08, Mar 2001 by H.Kurashige
// ------------------------------------------------------------

#include "globals.hh"
#include "G4VUserPhysicsList.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleWithCuts.hh"
#include "G4Material.hh"
#include "G4UserPhysicsListMessenger.hh"
#include "G4UImanager.hh"
#include "G4UnitsTable.hh"

#include "G4ios.hh"
#include "g4std/iomanip"                
#include "g4std/fstream"                

////////////////////////////////////////////////////////
G4VUserPhysicsList::G4VUserPhysicsList()
                   :verboseLevel(1), 
		    fRetrievePhysicsTable(false),
		    fStoredInAscii(true),
		    fIsCheckedForRetrievePhysicsTable(false),
		    fIsRestoredCutValues(false),
                    directoryPhysicsTable("."),
		    numberOfMaterial(0)

{
  // default cut value  (1.0mm) 
  defaultCutValue = 1.0*mm;

  // set energy range for SetCut calcuration
  G4ParticleWithCuts::SetEnergyRange(0.99*keV, 100*TeV);

  // pointer to the particle table
  theParticleTable = G4ParticleTable::GetParticleTable();
  theParticleIterator = theParticleTable->GetIterator();
 
  // UI Messenger 
  theMessenger = new G4UserPhysicsListMessenger(this);
}

////////////////////////////////////////////////////////
G4VUserPhysicsList::~G4VUserPhysicsList()
{
  if (theMessenger != 0) {
    delete theMessenger;
    theMessenger = 0;
  }
}

////////////////////////////////////////////////////////
void G4VUserPhysicsList::AddProcessManager(G4ParticleDefinition* newParticle,
					   G4ProcessManager*     newManager)
{
  if (newParticle == 0) return;
  if (newParticle->GetProcessManager() != 0) {
#ifdef G4VERBOSE    
    if (verboseLevel >1){
      G4cout << "G4VUserPhysicsList::AddProcessManager: ";
      G4cout  << newParticle->GetParticleName();
      G4cout << " already has ProcessManager " << G4endl;
    }
#endif
    return;
  }

  // create new process manager if newManager  == 0
  if (newManager  == 0){
    // Add ProcessManager
    if (newParticle->GetParticleType() == "nucleus") {
      // Create a copy of the process manager of "GenericIon" in case of "nucleus"
      G4ParticleDefinition* genericIon = 
	   (G4ParticleTable::GetParticleTable())->FindParticle("GenericIon");

      if (genericIon != 0) {
	G4ProcessManager* ionMan = genericIon->GetProcessManager();
	if (ionMan != 0) {
	  newManager = new G4ProcessManager(*ionMan);
	} else {
	  // no process manager has been registered yet 
	  newManager = new G4ProcessManager(newParticle);
	}
      } else {
	// "GenericIon" does not exist
	newManager = new G4ProcessManager(newParticle);
      }

    } else {
      // create process manager for particles other than "nucleus"
      newManager = new G4ProcessManager(newParticle);
    }
  }

  // set particle type   
  newManager->SetParticleType(newParticle);

  // add the process manager
  newParticle->SetProcessManager(newManager);

#ifdef G4VERBOSE    
  if (verboseLevel >2){
    G4cout << "G4VUserPhysicsList::AddProcessManager: ";
    G4cout  << "adds ProcessManager to ";
    G4cout  << newParticle->GetParticleName() << G4endl;
    newManager->DumpInfo();
  } 
#endif

}


////////////////////////////////////////////////////////
void G4VUserPhysicsList::InitializeProcessManager()
{
  // loop over all particles in G4ParticleTable 
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if  (pmanager==0) {
      // create process manager if the particle has no its one
      pmanager = new G4ProcessManager(particle);
      particle->SetProcessManager(pmanager);
    }
  }
}

////////////////////////////////////////////////////////
void G4VUserPhysicsList::RemoveProcessManager()
{
  // loop over all particles in G4ParticleTable 
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if  (pmanager!=0) delete pmanager;
    particle->SetProcessManager(0);
#ifdef G4VERBOSE    
    if (verboseLevel >2){
      G4cout << "G4VUserPhysicsList::RemoveProcessManager: ";
      G4cout  << "remove ProcessManager from ";
      G4cout  << particle->GetParticleName() << G4endl;
    }
#endif
  } 
}


////////////////////////////////////////////////////////
#include "G4Transportation.hh"
void G4VUserPhysicsList::AddTransportation()
{
  G4Transportation* theTransportationProcess= new G4Transportation();

  // loop over all particles in G4ParticleTable 
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (!particle->IsShortLived()) {
      // Add transportation process for all particles other than  "shortlived"
      if ( pmanager == 0) {
	// Error !! no process manager
	G4Exception("G4VUserPhysicsList::AddTransportation : no process manager!");
      } else {
	// add transportation with ordering = ( -1, "first", "first" )
	pmanager ->AddProcess(theTransportationProcess);
	pmanager ->SetProcessOrderingToFirst(theTransportationProcess, idxAlongStep);
	pmanager ->SetProcessOrderingToFirst(theTransportationProcess, idxPostStep);
      }
    } else {
      // shortlived particle case
    }
  }
}


////////////////////////////////////////////////////////
void G4VUserPhysicsList::SetDefaultCutValue(G4double value)
{
   if (value<=0.0) {
#ifdef G4VERBOSE    
     if (verboseLevel >0){
       G4cout << "G4VUserPhysicsList::SetDefaultCutValue: negative cut values";
       G4cout << "  :" << value/mm << "[mm]" << G4endl;
     }
#endif
   } else { 
#ifdef G4VERBOSE    
     if (verboseLevel >1){
       G4cout << "G4VUserPhysicsList::SetDefaultCutValue:";
       G4cout << "default cut value is changed to   :" ;
       G4cout << value/mm << "[mm]" << G4endl;
     }
#endif
     defaultCutValue = value;
     ResetCuts();
   }
}

////////////////////////////////////////////////////////
void G4VUserPhysicsList::ResetCuts()
{
#ifdef G4VERBOSE    
  if (verboseLevel >1) {
    G4cout << "G4VUserPhysicsList::ResetCuts()" << G4endl;
    G4cout << "  cut values in energy will be calculated later" << G4endl;
  }
#endif

  // Reset cut values for other particles
  // loop over all particles in G4ParticleTable 
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    if (!particle->IsShortLived()) particle->ResetCuts();
  }
    
  // inform that cut values are modified to the run manager
  // i.e state will be changed and SetCuts will be invoked 
  // just before event loop
  
  //   set /control/verbose 0
  G4int tempVerboseLevel = G4UImanager::GetUIpointer()->GetVerboseLevel();
  G4UImanager::GetUIpointer()->SetVerboseLevel(0);
  //   issue /run/cutoffModified
  G4UImanager::GetUIpointer()->ApplyCommand("/run/cutoffModified");
  //   retreive  /control/verbose 
  G4UImanager::GetUIpointer()->SetVerboseLevel(tempVerboseLevel);

}


////////////////////////////////////////////////////////
void G4VUserPhysicsList::SetCutValueForOtherThan(G4double cutValue,
                                 G4ParticleDefinition* first,
                                 G4ParticleDefinition* second,
				 G4ParticleDefinition* third,
                                 G4ParticleDefinition* fourth,
				 G4ParticleDefinition* fifth,
                                 G4ParticleDefinition* sixth,
				 G4ParticleDefinition* seventh,
                                 G4ParticleDefinition* eighth,
				 G4ParticleDefinition* nineth,
                                 G4ParticleDefinition* tenth  )
{
  // check cut value is positive
  if (cutValue <= 0.0) {
#ifdef G4VERBOSE    
    if (verboseLevel >0){
      G4cout << "G4VUserPhysicsList::SetCutValueForOtherThan: negative cut values";
      G4cout << "  :" << cutValue/mm << "[mm]" << G4endl;
    }
#endif
    return;
  } else {
#ifdef G4VERBOSE    
    if (verboseLevel >1) {
      G4cout << "G4VUserPhysicsList::SetCutValueForOtherThan ";
      G4cout << "  :" << cutValue/mm << "[mm]" << G4endl;
    }
#endif
  }

  // check specified particle types in arguments
  G4ParticleDefinition* specifiedParticles[10];
  G4int numberOfSpecifiedParticles = 0;
  if (first != 0) {
    specifiedParticles[numberOfSpecifiedParticles] = first;
    numberOfSpecifiedParticles +=1;
  }
  if (second != 0) {
    specifiedParticles[numberOfSpecifiedParticles] = second;
    numberOfSpecifiedParticles +=1;
  }
  if (third != 0) {
    specifiedParticles[numberOfSpecifiedParticles] = third;
    numberOfSpecifiedParticles +=1;
  }
  if (fourth != 0) {
    specifiedParticles[numberOfSpecifiedParticles] = fourth;
    numberOfSpecifiedParticles +=1;
  }
  if (fifth != 0) {
    specifiedParticles[numberOfSpecifiedParticles] = fifth;
    numberOfSpecifiedParticles +=1;
  }
  if (sixth != 0) {
    specifiedParticles[numberOfSpecifiedParticles] = sixth;
    numberOfSpecifiedParticles +=1;
  }
  if (seventh != 0) {
    specifiedParticles[numberOfSpecifiedParticles] = seventh;
    numberOfSpecifiedParticles +=1;
  }
  if (eighth != 0) {
    specifiedParticles[numberOfSpecifiedParticles] = eighth;
    numberOfSpecifiedParticles +=1;
  }
  if (nineth != 0) {
    specifiedParticles[numberOfSpecifiedParticles] = nineth;
    numberOfSpecifiedParticles +=1;
  }
  if (tenth != 0) {
    specifiedParticles[numberOfSpecifiedParticles] = tenth;
    numberOfSpecifiedParticles +=1;
  }

   // Set cut values for other particles
  G4bool isSpecified;
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    isSpecified = false;
    for (G4int index = 0; index <numberOfSpecifiedParticles; index++) {
      isSpecified = isSpecified || (particle == specifiedParticles[index]);
    }
    if ( (!isSpecified) && (!particle->IsShortLived()) ){
      // set cut value
      SetParticleCuts( cutValue ,particle );
      // build physics table
      BuildPhysicsTable(particle);

#ifdef G4VERBOSE    
      if (verboseLevel >1) G4cout << "Set cuts for " << particle->GetParticleName() << G4endl;
#endif
    }
  }
}


////////////////////////////////////////////////////////
void G4VUserPhysicsList::SetCutValue(G4double aCut, const G4String& name)
{
  G4ParticleDefinition* particle = theParticleTable->FindParticle(name);
#ifdef G4VERBOSE    
  if (particle != 0){
    if (verboseLevel >1) {
      G4cout << "G4VUserPhysicsList::SetCutValue       :";
      G4cout << "Set cuts for " << name << G4endl;
    }
  } else {
    if (verboseLevel >0) {
      G4cout << "G4VUserPhysicsList::SetCutValue       :";
      G4cout << name << " is not found in ParticleTable" << G4endl;
    }
  }
#endif

  if (particle != 0){
    if (!particle->IsShortLived()) {
      //set cut value
      SetParticleCuts( aCut ,particle );
      // build physics table
      BuildPhysicsTable(particle);
    }
  } 
}



////////////////////////////////////////////////////////
void G4VUserPhysicsList::SetCutsWithDefault()
{
  // default cut value
  G4double cut = defaultCutValue;

#ifdef G4VERBOSE    
  if (verboseLevel >1){
    G4cout << "G4VUserPhysicsList::SetCutsWithDefault:";
    G4cout << "CutLength : " << cut/mm << " (mm)" << G4endl;
  }  
#endif

  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma 
  SetCutValue(cut, "gamma");
  SetCutValue(cut, "e-");
  SetCutValue(cut, "e+");
 
  // set cut values for proton and anti_proton before all other hadrons
  // because some processes for hadrons need cut values for proton/anti_proton 
  SetCutValue(cut, "proton");
  SetCutValue(cut, "anti_proton");
  
  SetCutValueForOthers(cut);

  if (verboseLevel>1) {
    DumpCutValuesTable();
  }
}  


////////////////////////////////////////////////////////
void  G4VUserPhysicsList::SetApplyCuts(G4bool value, const G4String& name)
{
  // Sets a value to particle 
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();

    // check if the cut value has already been set
    if (name =="all" || name =="ALL" ){
      particle->SetApplyCutsFlag(value);
    } else if (name == particle->GetParticleName()){
      particle->SetApplyCutsFlag(value);
#ifdef G4VERBOSE    
      if (verboseLevel >1) {
	G4cout << "Set ApplyCutsFLag ";
	if (value) {
	  G4cout << "TRUE ";
	} else {
	  G4cout << "FALSE ";
	}
	G4cout << "for " << particle->GetParticleName() << G4endl;
      }
#endif
      break;
    }
  }
}
////////////////////////////////////////////////////////
G4bool G4VUserPhysicsList::GetApplyCuts(const G4String& name) const
{
  // Sets a value to particle 
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();

    // check if the cut value has already been set
    if (name == particle->GetParticleName()){
      return particle->GetApplyCutsFlag();
    }
  }
#ifdef G4VERBOSE    
  if (verboseLevel >0){
    G4cout << "G4VUserPhysicsList::GetApplyCuts: ";
    G4cout << " particle " <<  name  ;
    G4cout << " does not exist " << G4endl;
  }
#endif
  return false;
}

////////////////////////////////////////////////////////
void G4VUserPhysicsList::SetCutValueForOthers(G4double cutValue)
{
 // check cut value is positive
  if (cutValue <= 0.0) {
#ifdef G4VERBOSE    
    if (verboseLevel >0){
      G4cout << "G4VUserPhysicsList::SetCutValueForOthers: negative cut values";
      G4cout << "  :" << cutValue/mm << "[mm]" << G4endl;
    }
#endif
    return;
  }

#ifdef G4VERBOSE    
  if (verboseLevel >1) {
      G4cout << "G4VUserPhysicsList::SetCutValueForOthers ";
      G4cout << "  :" << cutValue/mm << "[mm]" << G4endl;
  }
#endif

  // Sets a cut value to particle types which have not be called SetCuts() 
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();

    if (!particle->IsShortLived()) {
      // check if the cut value has already been set
      if ((particle->GetLengthCuts()==0) ||(particle->GetEnergyCuts()==0)){
	// set cut value
	SetParticleCuts( cutValue ,particle );
	// build physics table 
	BuildPhysicsTable(particle);

#ifdef G4VERBOSE    
	if (verboseLevel >1) 
	  G4cout << "Set cuts for " << particle->GetParticleName() << G4endl;
#endif
      }
      // special treatment for particles for store cuts
      G4int idx = isParticleForStoreCuts(particle);
      if (idx >=0) {
	if (!isBuildPhysicsTable[idx])BuildPhysicsTable(particle);
      }
    }
  }
}


////////////////////////////////////////////////////////
void G4VUserPhysicsList::ReCalcCutValue(const G4String& name)
{
  G4ParticleDefinition* particle = theParticleTable->FindParticle(name);
  if (particle != 0 ){
    if (!particle->IsShortLived()) {
      particle->ReCalcCuts();
      BuildPhysicsTable(particle);

#ifdef G4VERBOSE    
      if (verboseLevel >1) G4cout << "Recalc cuts for " << name << G4endl;
#endif
    }
  } 

#ifdef G4VERBOSE    
  if (( particle ==0 ) && (verboseLevel >0)) {
    G4cout << name << " is not found in ParticleTable" << G4endl;
  }
#endif

}


////////////////////////////////////////////////////////
void G4VUserPhysicsList::ReCalcCutValueForOthers()
{
#ifdef G4VERBOSE    
  if (verboseLevel >1) {
    G4cout << "G4VUserPhysicsList::ReCalcCutValueForOthers ";
  }
#endif

  // Sets a cut value to particle types which have not be called SetCuts() 
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){

    G4ParticleDefinition* particle = theParticleIterator->value();

    if (!particle->IsShortLived()) {
      if (particle->GetEnergyCuts()==0) {
	particle->ReCalcCuts();
	BuildPhysicsTable(particle);

#ifdef G4VERBOSE    
	if (verboseLevel >1) 
	  G4cout << "ReCalc cuts for " << particle->GetParticleName() << G4endl;
#endif
      }
    }

  }
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void G4VUserPhysicsList::SetParticleCuts( G4double cut, G4ParticleDefinition* particle)
{
  if (fRetrievePhysicsTable && !fIsRestoredCutValues) {
#ifdef G4VERBOSE  
    if (verboseLevel>2){
      G4cout << "G4VUserPhysicsList::SetParticleCuts  ";
      G4cout << " Retrieve Cut Values for ";
      G4cout << particle->GetParticleName() << G4endl;
    }
#endif
    RetrieveCutValues(directoryPhysicsTable, fStoredInAscii);
    fIsRestoredCutValues = true;
  }

  if (particle->GetEnergyCuts() == 0) {
    particle->SetCuts(cut);
  } 
}    

///////////////////////////////////////////////////////////////
void G4VUserPhysicsList::BuildPhysicsTable(G4ParticleDefinition* particle)
{
  G4int idx = isParticleForStoreCuts(particle);
  if (idx >=0) isBuildPhysicsTable[idx] = true;
  if (fRetrievePhysicsTable) {
#ifdef G4VERBOSE  
    if (verboseLevel>2){
      G4cout << "G4VUserPhysicsList::BuildPhysicsTable  ";
      G4cout << " Retrieve Physics Table for ";
      G4cout << particle->GetParticleName() << G4endl;
    }
#endif
    if (CheckForRetrievePhysicsTable(directoryPhysicsTable, fStoredInAscii)) {
      //  Retrieve PhysicsTable from files for proccesses
      RetrievePhysicsTable(particle, directoryPhysicsTable, fStoredInAscii);

      return;
    } else {
#ifdef G4VERBOSE  
      if (verboseLevel>2){
	G4cout << "CheckForRetrievePhysicsTable failed " << G4endl;
      }
#endif
    }
  }

#ifdef G4VERBOSE  
    if (verboseLevel>2){
      G4cout << "G4VUserPhysicsList::BuildPhysicsTable  ";
      G4cout << " for " << particle->GetParticleName() << G4endl;
    }
#endif
  G4int j;
  // Rebuild the physics tables for every process for this particle type
  G4ProcessVector* pVector = (particle->GetProcessManager())->GetProcessList();
  for ( j=0; j < pVector->size(); ++j) {
    (*pVector)[j]->BuildPhysicsTable(*particle);
  }
  for ( j=0; j < pVector->size(); ++j) {
    // temporary addition to make the integral schema
    BuildIntegralPhysicsTable((*pVector)[j], particle); 
  }
}

///////////////////////////////////////////////////////////////
void  G4VUserPhysicsList::BuildIntegralPhysicsTable(G4VProcess* process,
						    G4ParticleDefinition* particle) 
{
  //*********************************************************************
  // temporary addition to make the integral schema of electromagnetic
  // processes work.
  //

  if ( (process->GetProcessName() == "Imsc") ||
       (process->GetProcessName() == "IeIoni") ||
       (process->GetProcessName() == "IeBrems") ||
       (process->GetProcessName() == "Iannihil") ||
       (process->GetProcessName() == "IhIoni") ||
       (process->GetProcessName() == "IMuIoni") ||
       (process->GetProcessName() == "IMuBrems") ||
       (process->GetProcessName() == "IMuPairProd")  ) {
#ifdef G4VERBOSE  
    if (verboseLevel>2){
      G4cout << "G4VUserPhysicsList::BuildIntegralPhysicsTable  ";
      G4cout << " BuildPhysicsTable is invoked for ";
      G4cout << process->GetProcessName();
      G4cout << "(" << particle->GetParticleName() << ")" << G4endl;
    }
#endif
    process->BuildPhysicsTable(*particle);
  }
}

///////////////////////////////////////////////////////////////
void G4VUserPhysicsList::DumpList() const
{
  theParticleIterator->reset();
  G4int idx = 0;
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4cout << particle->GetParticleName();
    if ((idx++ % 4) == 3) {
      G4cout << G4endl;
    } else {
      G4cout << ", ";
    }      
  }
  G4cout << G4endl;
}

///////////////////////////////////////////////////////////////
void G4VUserPhysicsList::DumpCutValues(const G4String &particle_name) const
{
  G4ParticleDefinition* particle;
  if ((particle_name == "ALL") || (particle_name == "all")) {
    theParticleIterator->reset();
    while( (*theParticleIterator)() ){
      particle = theParticleIterator->value();
      DumpCutValues(particle);
    }
  } else {
     particle = theParticleTable->FindParticle(particle_name);
     if (particle != 0) DumpCutValues(particle);
  }
}

///////////////////////////////////////////////////////////////
void G4VUserPhysicsList::DumpCutValues( G4ParticleDefinition* particle) const
{
  if (particle == 0) return;
  
  G4int prec = G4cout.precision(3);

  if (particle->IsShortLived()) {
    // name field
    G4cout << " --- " << particle->GetParticleName() << " is a short lived particle ------ " << G4endl;
  } else {
    // name field
    G4cout << " --- " << particle->GetParticleName() << " ------ " << G4endl;

    // cut value in range
    G4double* theRangeCuts = particle->GetLengthCuts();

    // material and energy cut value for the material 
    G4double*  theKineticEnergyCuts = particle->GetEnergyCuts();
    
    if (theRangeCuts != 0) {
      const G4MaterialTable* materialTable = G4Material::GetMaterialTable();
      size_t numberOfMaterials = G4Material::GetNumberOfMaterials();
      G4cout << "   - Material --- Cut in range ------------- Energy Cut ---" << G4endl;
      for (size_t idx=0; idx<numberOfMaterials; idx++){
	G4cout << "     " << G4std::setw(19) << (*materialTable)[idx]->GetName(); 
	G4cout <<" : ";
	G4cout << G4std::setw(12)<< G4BestUnit(theRangeCuts[idx],"Length");
	if (theKineticEnergyCuts ==0) {
	  G4cout << " : --------------------------------";
	} else {
	  G4cout << " : ";
	  G4cout << G4std::setw(10)<< G4BestUnit(theKineticEnergyCuts[idx],"Energy");
	}	
	G4cout << G4endl;
      }
      if (theKineticEnergyCuts ==0) {
	G4cout << "   - Cuts in energy are not calculated yet --" << G4endl;
	G4cout << " Enter /run/initialize command to calculate cuts " << G4endl;
      }
    } else {
      G4cout << "   - Cuts in range are not set yet --" << G4endl;
    }

  }
  G4cout.precision(prec);
}

///////////////////////////////////////////////////////////////
void G4VUserPhysicsList::DumpCutValuesTable() const
{
  // This methods Print out a table of cut value information
  // for "e-", "gamma", "mu-", "proton" and "neutron"
  G4int prec = G4cout.precision(3);
  const G4int size = 5;
  G4String name[size] = {"gamma", "e-", "mu-", "proton", "neutron"};
  G4ParticleDefinition* particle[size];
  G4bool IsOK = true;
  G4int size_display=2;
  G4int idx; 
  for (idx=0; idx <size_display; idx++) {
    particle[idx] = theParticleTable->FindParticle(name[idx]);
  }

  //line 1 //-- Commented out (M.Asai)
  //G4cout << "Default cut value in range :";
  //G4cout << defaultCutValue/mm << "[mm]" << G4endl;

  //line 2
  G4cout << "============= The cut Energy ==============================" <<G4endl;
  
  // line 3
  G4cout << "                     ";
  for (idx=0; idx <size_display; idx++) {
    G4cout << " " << G4std::setw(11) << name[idx] << "    ";
  }
  G4cout << G4endl;

  const G4MaterialTable* materialTable = G4Material::GetMaterialTable();
  size_t numberOfMaterials = G4Material::GetNumberOfMaterials();
  // line 4
  G4cout << "Cut in range       " << G4endl;
  for (size_t I=0; I<numberOfMaterials; I++) {
    G4cout << " " << G4std::setw(18) << ((*materialTable)[I])->GetName();
    for (idx=0; idx <size_display; idx++) {
      if (particle[idx] == 0) {
        G4cout << "            ";
      } else {
        if (particle[idx]->GetLengthCuts() == 0) {
          G4cout << " ---------- ";
        } else {
          G4cout << " " << G4std::setw(11) << G4BestUnit((particle[idx]->GetLengthCuts())[I],"Length");
        }
      }
    }
    G4cout << G4endl;
  }

  // line 5
  G4cout << "Cut in energy";
  G4cout << G4endl;

  // line 6 ..
  for (size_t J=0; J<numberOfMaterials; J++) {
    G4cout << " " << G4std::setw(18) << ((*materialTable)[J])->GetName();
    for (idx=0; idx <size_display; idx++) {
      if (particle[idx] == 0) {
	G4cout << "            ";
      } else {
        if (particle[idx]->GetEnergyCuts() == 0) {
	  G4cout << " ---------- ";
          IsOK = false;
	} else {
	  G4cout << " " << G4std::setw(11) << G4BestUnit((particle[idx]->GetEnergyCuts())[J],"Energy");
	}
      }
    }
    G4cout << G4endl;
  }
  
  if (!IsOK) {
    G4cout << " Cuts in energy have not calculated yet !!" << G4endl;
    G4cout << " Enter /run/initialize command to calculate cuts " << G4endl;
  }

  // last line 
  G4cout << "===================================================" << G4endl;
  G4cout.precision(prec);
}


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
G4bool G4VUserPhysicsList::StorePhysicsTable(const G4String& directory)
{
  numberOfMaterial = G4Material::GetNumberOfMaterials();

  G4bool   ascii = fStoredInAscii;
  G4String dir   = directory;
  if (dir.isNull()) dir = directoryPhysicsTable; 
  else directoryPhysicsTable = dir; 
  if (!StoreMaterialInfo(dir, ascii)) return false;
  if (!StoreCutValues(dir,ascii)) return false;
#ifdef G4VERBOSE  
  if (verboseLevel>2){
    G4cout << "G4VUserPhysicsList::StorePhysicsTable   ";
    G4cout << " Store material and cut values successfully" << G4endl;
  }
#endif

  G4bool success= true;  

  // loop over all particles in G4ParticleTable 
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    // Store physics tables for every process for this particle type
    G4ProcessVector* pVector = (particle->GetProcessManager())->GetProcessList();
    G4int  j;
    for ( j=0; j < pVector->size(); ++j) {
      if (!(*pVector)[j]->StorePhysicsTable(particle,dir,ascii)){   
#ifdef G4VERBOSE  
	if (verboseLevel>2){
	  G4cout << "G4VUserPhysicsList::StorePhysicsTable   ";
	  G4cout << " Fail to store for ";
	  G4cout << (*pVector)[j]->GetProcessName();
	  G4cout << "(" << particle->GetParticleName() <<")" << G4endl;
	}
#endif
	success = false;
      }
    }
    // end loop over processes
  }
  // end loop over particles
  return success;
}


///////////////////////////////////////////////////////////////
G4bool G4VUserPhysicsList::StoreMaterialInfo(const G4String& directory, 
					     G4bool          ascii)
{
  const G4String fileName = directory + "/" + "material.dat";
  const G4String key = "MATERIAL";
  G4std::ofstream fOut;  

  // open output file //
#ifdef G4USE_STD_NAMESPACE
  if (!ascii ) 
    fOut.open(fileName,G4std::ios::out|G4std::ios::binary);
  else 
#endif
    fOut.open(fileName,G4std::ios::out);

  
  // check if the file has been opened successfully 
  if (!fOut) {
#ifdef G4VERBOSE  
    G4cerr << "G4VUserPhysicsList::StoreMaterialInfo  ";
    G4cerr << " Can not open file " << fileName << G4endl;
#endif
    return false;
  }
  
  const G4MaterialTable* matTable = G4Material::GetMaterialTable(); 
  // number of materials in the table
  numberOfMaterial =   G4Material::GetNumberOfMaterials();

  if (ascii) {
    /////////////// ASCII mode  /////////////////
    // key word
    fOut << key << G4endl;
    
    // number of materials in the table
    fOut << numberOfMaterial << G4endl;
    
    fOut.setf(G4std::ios::scientific);
  
    // material name and density
    for (size_t idx=0; idx<size_t(numberOfMaterial); ++idx){
      fOut << G4std::setw(FixedStringLengthForStore) << ((*matTable)[idx])->GetName();
      fOut << G4std::setw(FixedStringLengthForStore) << ((*matTable)[idx])->GetDensity()/(g/cm3) << G4endl;
    }
    
    fOut.unsetf(G4std::ios::scientific);

  } else {
    /////////////// Binary mode  /////////////////
    char temp[FixedStringLengthForStore];
    size_t i;

    // key word
    for (i=0; i<FixedStringLengthForStore; ++i) temp[i] = '\0'; 
    for (i=0; i<key.length() && i<FixedStringLengthForStore-1; ++i) temp[i]=key[i];
    fOut.write(temp, FixedStringLengthForStore);

    // number of materials in the table
    fOut.write( (char*)(&numberOfMaterial), sizeof (G4int));
    
    // material name and density
    for (size_t imat=0; imat<size_t(numberOfMaterial); ++imat){
      G4String name =  ((*matTable)[imat])->GetName();
      G4double density = ((*matTable)[imat])->GetDensity()/(g/cm3);
      for (i=0; i<FixedStringLengthForStore; ++i) temp[i] = '\0'; 
      for (i=0; i<name.length() && i<FixedStringLengthForStore-1; ++i) temp[i]=name[i];
      fOut.write(temp, FixedStringLengthForStore);
      fOut.write( (char*)(&density), sizeof (G4double));
    }    
  }    

  fOut.close();
  return true;
}

///////////////////////////////////////////////////////////////
G4bool G4VUserPhysicsList::StoreCutValues(const G4String& directory, 
					  G4bool          ascii)
{
  const G4String fileName = directory + "/" + "cut_value.dat";
  const G4String key = "CUT_VALUE";
  G4std::ofstream fOut;  

  // open output file //
#ifdef G4USE_STD_NAMESPACE
  if (!ascii ) 
    fOut.open(fileName,G4std::ios::out|G4std::ios::binary);
  else 
#endif
    fOut.open(fileName,G4std::ios::out);

  // check if the file has been opened successfully 
  if (!fOut) {
#ifdef G4VERBOSE  
    G4cerr << "G4VUserPhysicsList::StoreCutValues  ";
    G4cerr << " Can not open file " << fileName << G4endl;
#endif
    return false;
  }

  char temp[FixedStringLengthForStore];
  size_t i;
  if (ascii) {
    /////////////// ASCII mode  /////////////////
    // key word
    fOut << key << G4endl;
    
    fOut.setf(G4std::ios::scientific);
    // default cut value
    fOut << "Default " <<  G4std::setw(20) << defaultCutValue/mm  << G4endl;
    fOut.unsetf(G4std::ios::scientific);
  } else {
    /////////////// Binary mode  /////////////////
    // key word
    for (i=0; i<FixedStringLengthForStore; ++i) temp[i] = '\0'; 
    for (i=0; i<key.length() && i<FixedStringLengthForStore-1; ++i) temp[i]=key[i];
    fOut.write(temp, FixedStringLengthForStore);
   
    const G4String defKey="Default";
    for (i=0; i<FixedStringLengthForStore; ++i) temp[i] = '\0';
    for (i=0; i<key.length() && i<FixedStringLengthForStore-1; ++i) temp[i]=defKey[i];
    fOut.write(temp, FixedStringLengthForStore);

    G4double defaultCutValueInmm = defaultCutValue/mm;
    // default cut value
    fOut.write( (char*)(&defaultCutValueInmm), sizeof (G4double));
  }

 // loop over all particles in G4ParticleTable 
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    // Only following particles are concerned
    G4int idx = isParticleForStoreCuts(particle);
    if (idx >=0 ){
      // particle name 
      if (ascii) {
	/////////////// ASCII mode  /////////////////
	fOut.setf(G4std::ios::scientific);
	fOut << G4std::setw(FixedStringLengthForStore) << particle->GetParticleName();
	fOut.unsetf(G4std::ios::scientific);
      } else {
	/////////////// Binary mode  /////////////////
	G4String name =  particle->GetParticleName();
	for (i=0; i<FixedStringLengthForStore; ++i) temp[i] = '\0'; 
	for (i=0; i<name.length() && i<FixedStringLengthForStore-1; ++i) temp[i]=name[i];
	fOut.write(temp, FixedStringLengthForStore);
      }

      // range cut  for each material
      G4double* rangeArray =  particle->GetLengthCuts();
      if (rangeArray ==0) {
#ifdef G4VERBOSE
        if (verboseLevel>0){
          G4cout << "G4VUserPhysicsList::StoreCutValues        " ;
          G4cout << ":  Range Cut Values have not be set for";
          G4cout << particle->GetParticleName() << G4endl;
        }
#endif
        return false;
      }
      if (ascii) {
        /////////////// ASCII mode  /////////////////
        fOut.setf(G4std::ios::scientific);
        G4int ii =0;
        for(G4int jdx=0; jdx<numberOfMaterial; ++jdx, ++ii) {
          if (ii==4) {
            fOut << G4endl;
            ii =0;
          }
          fOut << G4std::setw(20) << rangeArray[jdx];
        }
        fOut << G4endl;
        fOut.unsetf(G4std::ios::scientific);
      } else {
        /////////////// Binary mode  /////////////////
        fOut.write( (char*)(rangeArray), numberOfMaterial*(sizeof (G4double)) );
      }

      // cut energy for each material
      G4double*  cutArray =  particle->GetEnergyCuts(); 
      if (cutArray ==0) {
#ifdef G4VERBOSE  
	if (verboseLevel>0){
	  G4cout << "G4VUserPhysicsList::StoreCutValues        " ;
	  G4cout << ":  Energy Cut Values have not be calculated  for";
	  G4cout << particle->GetParticleName() << G4endl;
	}
#endif
	return false;
      }
      if (ascii) {
	/////////////// ASCII mode  /////////////////
	fOut.setf(G4std::ios::scientific);
	G4int jj =0;
	for(G4int idx=0; idx<numberOfMaterial; ++idx, ++jj) {
	  if (jj==4) {
	    fOut << G4endl;
	    jj =0;
	  }
	  fOut << G4std::setw(20) << cutArray[idx];
	}
	fOut << G4endl;
	fOut.unsetf(G4std::ios::scientific);
      } else { 
	/////////////// Binary mode  /////////////////
	fOut.write( (char*)(cutArray), numberOfMaterial*(sizeof (G4double)) );
      }
    }
  }
 
  fOut.close();
  return true;
}

///////////////////////////////////////////////////////////////
 void  G4VUserPhysicsList::SetPhysicsTableRetrieved(const G4String& directory)
{
  fRetrievePhysicsTable = true;
  if(!directory.isNull()) {
    directoryPhysicsTable = directory;
  }
  fIsCheckedForRetrievePhysicsTable=false;
  fIsRestoredCutValues = false;
  ResetCuts();
}

///////////////////////////////////////////////////////////////
void G4VUserPhysicsList::RetrievePhysicsTable(G4ParticleDefinition* particle, 
					      const G4String& directory,
					      G4bool          ascii)
{
  G4int  j;
  G4bool success[100];  
  // Retrieve physics tables for every process for this particle type
  G4ProcessVector* pVector = (particle->GetProcessManager())->GetProcessList();
  for ( j=0; j < pVector->size(); ++j) {
    success[j] = 
       (*pVector)[j]->RetrievePhysicsTable(particle,directory,ascii);

    if (!success[j]) {
#ifdef G4VERBOSE  
      if (verboseLevel>2){
	G4cout << "G4VUserPhysicsList::RetrievePhysicsTable   ";
	G4cout << " Fail to retrieve for ";
        G4cout << (*pVector)[j]->GetProcessName();
        G4cout << "(" << particle->GetParticleName() <<")" << G4endl;
      }
#endif
      (*pVector)[j]->BuildPhysicsTable(*particle);
    }
  }
  for ( j=0; j < pVector->size(); ++j) {
    // temporary addition to make the integral schema
    if (!success[j]) BuildIntegralPhysicsTable((*pVector)[j], particle); 
  }
}

///////////////////////////////////////////////////////////////
G4bool  G4VUserPhysicsList::CheckForRetrievePhysicsTable(const G4String& directory, 
							 G4bool          ascii)
{
  numberOfMaterial = G4Material::GetNumberOfMaterials();

  if (!fIsCheckedForRetrievePhysicsTable) {
    fIsCheckedForRetrievePhysicsTable = CheckMaterialInfo(directory, ascii);
  }
  return fIsCheckedForRetrievePhysicsTable;
}

///////////////////////////////////////////////////////////////
G4bool G4VUserPhysicsList::CheckMaterialInfo(const G4String& directory, 
					     G4bool          ascii)
{
  const G4String fileName = directory + "/" + "material.dat";
  const G4String key = "MATERIAL";
  G4std::ifstream fIn;  

  // open input file //
#ifdef G4USE_STD_NAMESPACE
  if (!ascii )
    fIn.open(fileName,G4std::ios::in|G4std::ios::binary);
  else
#endif
    fIn.open(fileName,G4std::ios::in);

  // check if the file has been opened successfully 
  if (!fIn) {
#ifdef G4VERBOSE  
    G4cerr << "G4VUserPhysicsList::CheckMaterialInfo  ";
    G4cerr << " Can not open file " << fileName << G4endl;
#endif
    return false;
  }
  
  char temp[FixedStringLengthForStore];

  // key word
  G4String keyword;    
  if (ascii) {
    fIn >> keyword;
  } else {
    fIn.read(temp, FixedStringLengthForStore);
    keyword = (const char*)(temp);
  }
  if (key!=keyword) {
#ifdef G4VERBOSE  
    if (verboseLevel>0){
      G4cout << "G4VUserPhysicsList::CheckMaterialInfo  ";
      G4cout << " Can not find key word " << keyword << G4endl;
    }
#endif
    return false;
  }

  const G4MaterialTable* matTable = G4Material::GetMaterialTable(); 
  numberOfMaterial = G4Material::GetNumberOfMaterials();
  // number of materials in the table
  G4int nmat;
  if (ascii) {
    fIn >> nmat;
  } else {
    fIn.read( (char*)(&nmat), sizeof (G4int));
  }
  if (nmat!=numberOfMaterial) {
#ifdef G4VERBOSE  
    if (verboseLevel>0){
      G4cout << "G4VUserPhysicsList::CheckMaterialInfo  ";
      G4cout << "Number of material is inconsistent "<< G4endl;
    }
#endif
    return false;
  }
  
  // list of material
  for (size_t idx=0; idx<size_t(numberOfMaterial); ++idx){
    // check eof
    if(fIn.eof()) {
#ifdef G4VERBOSE  
     if (verboseLevel>0){
       G4cout << "G4VUserPhysicsList::CheckMaterialInfo  ";
       G4cout << " encountered End of File" << G4endl;
     }
#endif   
      fIn.close();
      return false;
    }

    // check material name and density
    char name[FixedStringLengthForStore];
    double density;
    if (ascii) {
      fIn >> name >> density;
    } else {
      fIn.read(name, FixedStringLengthForStore);
      fIn.read((char*)(&density), sizeof (G4double));
    }
    if (fIn.fail()) {
#ifdef G4VERBOSE  
     if (verboseLevel>0){
       G4cout << "G4VUserPhysicsList::CheckMaterialInfo  ";
       G4cout << " Bad data format " << G4endl;
     }
#endif   
      fIn.close();
      return false;
    }
    G4double ratio = abs( (density*g/cm3)/((*matTable)[idx])->GetDensity() );
    if ( (name != ((*matTable)[idx])->GetName()) || (0.999>ratio) || (ratio>1.001) ){
#ifdef G4VERBOSE  
      if (verboseLevel>0){
	G4cout << "G4VUserPhysicsList::CheckMaterialInfo  ";
	G4cout << " Inconsistent material name or density" << G4endl;;
	G4cout << G4std::setw(40) << name;
	G4cout << G4std::setw(20) << G4std::setiosflags(G4std::ios::scientific) << density << G4endl;
 	G4cout << G4std::resetiosflags(G4std::ios::scientific);
     }
#endif   
      fIn.close();
      return false;
    }
  }
  fIn.close();
  return true;
}


///////////////////////////////////////////////////////////////
G4bool  G4VUserPhysicsList::RetrieveCutValues(const G4String&  directory,
					      G4bool           ascii     )
{
  if (!CheckMaterialInfo(directory, ascii)) {
#ifdef G4VERBOSE  
    G4cout << "G4VUserPhysicsList::RetrieveCutValues  ";
    G4cout << " Can not retrieve cut values " << G4endl;
#endif
  }

  //  file name
  const G4String fileName = directory + "/" + "cut_value.dat";
  
  G4std::ifstream fIn;  
  // open input file //
#ifdef G4USE_STD_NAMESPACE
  if (!ascii )
    fIn.open(fileName,G4std::ios::in|G4std::ios::binary);
  else 
#endif
    fIn.open(fileName,G4std::ios::in);

  // check if the file has been opened successfully 
  if (!fIn) {
#ifdef G4VERBOSE  
    G4cerr << "G4VUserPhysicsList::RetrieveCutValues  ";
    G4cerr << " Can not open file " << fileName << G4endl;
#endif
    return false;
  }

  char temp[FixedStringLengthForStore];

  // key word
  const G4String key = "CUT_VALUE";
  G4String keyword;    
  if (ascii) {
    fIn >> keyword;
  } else {
    fIn.read(temp, FixedStringLengthForStore);
    keyword = (const char*)(temp);
  }
  if (key!=keyword) {
#ifdef G4VERBOSE  
    if (verboseLevel>0){
      G4cout << "G4VUserPhysicsList::RetrieveCutValues ";
      G4cout << " Can not find key word " << keyword << G4endl;
    }
#endif
  }

  // default cut value
  G4double defaultCut;
  if (ascii) {
    fIn >> keyword >> defaultCut;
  } else {
    fIn.read(temp, FixedStringLengthForStore);
    keyword = (const char*)(temp);
    fIn.read( (char*)(&defaultCut), sizeof (G4double));
  }
  if (fIn.fail()) {
#ifdef G4VERBOSE  
    if (verboseLevel>0){
      G4cout << "G4VUserPhysicsList::RetrieveCutValues  ";
      G4cout << " Bad data format " << G4endl;
    }
#endif   
    fIn.close();
    return false;
  } 

   // set default value
  defaultCutValue = defaultCut*mm;
#ifdef G4VERBOSE  
  if (verboseLevel>2){
    G4cout.setf(G4std::ios::scientific);
    G4cout << "Default " <<  G4std::setw(20) << defaultCutValue/mm  << G4endl;
    G4cout.unsetf(G4std::ios::scientific);
  }
#endif

  // Reset cut values for other particles
  // loop over all particles in G4ParticleTable 
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    if (!particle->IsShortLived()) particle->ResetCuts();
  } 

  // number of materials in the table
  numberOfMaterial = G4Material::GetNumberOfMaterials();

  // loop over all particles 
  while(!fIn.eof()){
    // read in particle name
    char name[FixedStringLengthForStore];
    if (ascii) {
      fIn >> name ;
    } else {
     fIn.read(name, FixedStringLengthForStore);
    }
    if (fIn.eof()) break;
    if (fIn.fail()) {
#ifdef G4VERBOSE  
      if (verboseLevel>0){
	G4cout << "G4VUserPhysicsList::RetrieveCutValues  ";
	G4cout << " Bad data format " << G4endl;
      }
#endif
      fIn.close();
      return false;
    }

#ifdef G4VERBOSE  
    if (verboseLevel>2){
      G4cout.setf(G4std::ios::scientific);
      G4cout << name << G4endl;
      G4cout.unsetf(G4std::ios::scientific);
    }
#endif

    // Search particle
    G4ParticleDefinition* particle = theParticleTable->FindParticle(name);
    if (particle==0) {
#ifdef G4VERBOSE  
      if (verboseLevel>0){
	G4cout << "G4VUserPhysicsList::RetrieveCutValues  ";
	G4cout << " Particle " << name <<" is not found "<< G4endl;
      } 
      fIn.close();
      return false;
#endif     
    } 

    // read in range cut for all materials
    G4double* rangeArray = new G4double[numberOfMaterial];
    if (ascii) {
      /////////////// ASCII mode  /////////////////
      for(G4int jdx=0; jdx<numberOfMaterial; ++jdx) {
        G4double value;
        fIn >> value;
        rangeArray[jdx] = value;
      }
    } else {
      /////////////// Binary mode  /////////////////
      fIn.read( (char*)(rangeArray), numberOfMaterial*(sizeof (G4double)) );
    }

    // read in energy cut for all materials 
    G4double* cutArray = new G4double[numberOfMaterial];
    if (ascii) {
      /////////////// ASCII mode  /////////////////
      for(G4int idx=0; idx<numberOfMaterial; ++idx) {
	G4double value;
	fIn >> value;
	cutArray[idx] = value;
      }
    } else {
      /////////////// Binary mode  /////////////////
      fIn.read( (char*)(cutArray), numberOfMaterial*(sizeof (G4double)) );
    }
 
  
    // restore cut values
    particle->RestoreCuts( rangeArray , cutArray) ; 
#ifdef G4VERBOSE    
    if (verboseLevel >2)DumpCutValues(particle); 
#endif
    G4int idx = isParticleForStoreCuts(particle);
    if (idx >=0) isBuildPhysicsTable[idx] = false;
   }
  
#ifdef G4VERBOSE    
  if (verboseLevel >1){
    G4cout << "G4VUserPhysicsList::RetrieveCutValues: Cut Values are successfully restored ";
    G4cout << "CutLength : " << defaultCutValue/mm << " (mm)" << G4endl;
  }  
#endif

  fIsRestoredCutValues = true;
  fIn.close();
  return true;
}

G4int G4VUserPhysicsList::isParticleForStoreCuts(const G4ParticleDefinition* particle) const
{
  G4int idx;
  for ( idx =0; idx < NumberOfParticlesForStoreCuts; ++idx) {
    if (particle->GetParticleName()==particleForStoreCuts[idx]) return idx;
  }
  return -1;
}

const G4String 
  G4VUserPhysicsList::particleForStoreCuts[NumberOfParticlesForStoreCuts] =
{
  "gamma",
  "e-",
  "e+",
  "mu-",
  "mu+",
  "proton",
  "anti_proton",
  "neutron",
  "anti_neutron"
};



