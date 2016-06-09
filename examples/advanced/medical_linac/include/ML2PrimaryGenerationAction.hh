//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// The code was written by :
//	^Claudio Andenna claudio.andenna@iss.infn.it, claudio.andenna@ispesl.it
//      *Barbara Caccia barbara.caccia@iss.it
//      with the support of Pablo Cirrone (LNS, INFN Catania Italy)
//
// ^ISPESL and INFN Roma, gruppo collegato Sanità, Italy
// *Istituto Superiore di Sanità and INFN Roma, gruppo collegato Sanità, Italy
//  Viale Regina Elena 299, 00161 Roma (Italy)
//  tel (39) 06 49902246
//  fax (39) 06 49387075
//
// more information:
// http://g4advancedexamples.lngs.infn.it/Examples/medical-linac
//
//*******************************************************//


#ifndef CML2PrimaryGenerationActionH
#define CML2PrimaryGenerationActionH


#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include "G4Timer.hh"
#include "Randomize.hh" 

#include "G4ParticleDefinition.hh"
#include "ML2SinputData.hh"
#include "ML2SDWithParticle.hh"
#include "ML2SDWithVoxels.hh"

class G4ParticleGun;
class G4ParticleDefinition;
class CML2PrimaryGenerationActionMessenger;

class CML2PrimaryGenerationAction : public G4VUserPrimaryGeneratorAction
{
public:
	CML2PrimaryGenerationAction(SPrimaryParticle *primaryParticleData);
	~CML2PrimaryGenerationAction(void);
	void design();
	void GeneratePrimaries(G4Event *anEvent);
	inline void setNIdenticalParticles(G4int val){this->nIdenticalParticles=val;};
	inline void setNLoopsPhSpParticles(G4int val){this->nLoopsPhSpParticles=val;};
	inline void setNMaxParticlesInRamPhaseSpace(G4int val){this->nMaxParticlesInRamPhaseSpace=val;};

	inline void setGunMeanEnergy(G4double val){this->GunMeanEnegy=val;};
	inline void setGunStdEnergy(G4double val){this->GunStdEnegy=val;};
	inline void setGunRadious(G4double val){this->GunRadious=val;};
	inline void setCalculatedPhaseSpaceFileIN(G4String val){this->calculatedPhaseSpaceFileIN=val;};
	inline void setSourceTypeName(G4String val)
	{
		this->sourceTypeName=val;
		if (this->sourceTypeName=="randomTarget")
		{
			this->idParticleSource=id_randomTarget;
		}
		else if (this->sourceTypeName=="phaseSpace")
		{
			this->idParticleSource=id_phaseSpace;
		}
	};
	
private:
	void setGunRandom();
	void setGunCalculatedPhaseSpace();
	void GenerateFromRandom();
	void GenerateFromCalculatedPhaseSpace();
	void fillParticlesContainer();
	bool itIsTheSameParticle(Sparticle *p1, Sparticle *p2);

	G4int nBeam, nIdenticalParticles, nLoopsPhSpParticles, idGunType, nMaxParticlesInRamPhaseSpace, idParticleSource;
	G4double GunMeanEnegy, GunStdEnegy, GunRadious;
	G4String calculatedPhaseSpaceFileIN;

	CML2PrimaryGenerationActionMessenger *PrimaryGenerationActionMessenger;


	G4ThreeVector dir, pos;
	G4double ek;

	G4Timer myTime;
	G4double sinTheta, cosTheta, phi;
	G4double ro, alfa;
	G4ParticleGun *particleGun;
	G4ParticleDefinition *gamma;
	G4ParticleDefinition *electron;
	G4ParticleDefinition *positron;
	G4int nEventsPerRun;
	SPrimaryParticle *primaryParticleData;
	Sparticle *particles, *particle;
	int nParticle, nPhSpParticles, nRandomParticles, idCurrentParticleSource;
	Sparticle *firstFileParticle, *lastLoadedParticle;
	G4String sourceTypeName;
};

#endif