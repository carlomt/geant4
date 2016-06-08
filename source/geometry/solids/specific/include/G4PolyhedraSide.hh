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
// $Id: G4PolyhedraSide.hh,v 1.4 2001/07/11 10:00:15 gunter Exp $
// GEANT4 tag $Name: geant4-04-01 $
//
// 
// --------------------------------------------------------------------
// GEANT 4 class header file
//
//
// G4PolyhedraSide
//
// Class description:
//
//   Class implementing a face that represents one segmented side
//   of a polyhedra:
//
//   G4PolyhedraSide( const G4PolyhedraSideRZ *prevRZ,
//                    const G4PolyhedraSideRZ *tail,
//                    const G4PolyhedraSideRZ *head,
//                    const G4PolyhedraSideRZ *nextRZ,
//                          G4int    numSide,
//                          G4double phiStart, G4double phiTotal, 
//                          G4bool phiIsOpen,  G4bool isAllBehind=false )
//
//   Values for r1,z1 and r2,z2 should be specified in clockwise
//   order in (r,z).

// Author: 
//   David C. Williams (davidw@scipp.ucsc.edu)
// --------------------------------------------------------------------

#ifndef G4PolyhedraSide_hh
#define G4PolyhedraSide_hh

#include "G4VCSGface.hh"

class G4IntersectingCone;

typedef struct {
	G4double r, z;	// start of vector
} G4PolyhedraSideRZ;

class G4PolyhedraSide : public G4VCSGface
{

  public:

	G4PolyhedraSide( const G4PolyhedraSideRZ *prevRZ,
			 const G4PolyhedraSideRZ *tail,
			 const G4PolyhedraSideRZ *head,
			 const G4PolyhedraSideRZ *nextRZ,
			       G4int    numSide,
			       G4double phiStart, G4double phiTotal, 
			       G4bool phiIsOpen,  G4bool isAllBehind=false );
	virtual ~G4PolyhedraSide();
	
	G4PolyhedraSide( const G4PolyhedraSide &source );
	G4PolyhedraSide& operator=( const G4PolyhedraSide &source );
	
	G4bool Intersect( const G4ThreeVector &p, const G4ThreeVector &v,	
			  G4bool outgoing, G4double surfTolerance,
			  G4double &distance, G4double &distFromSurface,
			  G4ThreeVector &normal, G4bool &allBehind );

        G4double Distance( const G4ThreeVector &p, G4bool outgoing );
	
	EInside Inside( const G4ThreeVector &p, G4double tolerance, 
			G4double *bestDistance );
	
	G4ThreeVector Normal( const G4ThreeVector &p,  G4double *bestDistance );

	G4double Extent( const G4ThreeVector axis );
	
	void CalculateExtent( const EAxis axis, 
			      const G4VoxelLimits &voxelLimit,
			      const G4AffineTransform &tranform,
			      G4SolidExtentList &extentList        );

	G4VCSGface *Clone() { return new G4PolyhedraSide( *this ); }
	
  protected:

	//
	// A couple internal data structures
	//
	struct sG4PolyhedraSideVec;		// Secret recipe for allowing
	friend struct sG4PolyhedraSideVec;	// protected nested structures

	typedef struct sG4PolyhedraSideEdge {
		G4ThreeVector	normal;		// Unit normal to this edge
		G4ThreeVector	corner[2];	// The two corners of this phi edge
		G4ThreeVector	cornNorm[2];	// The normals of these corners
	} G4PolyhedraSideEdge;
	
	typedef struct sG4PolyhedraSideVec {
		G4ThreeVector	normal, 	// Normal (point out of the shape)
			  	center,		// Point in center of side
				surfPhi,	// Unit vector on surface pointing along phi
				surfRZ;		// Unit vector on surface pointing along R/Z
		G4PolyhedraSideEdge *edges[2];	// The phi boundary edges to this side 
						//     [0]=low phi [1]=high phi
		G4ThreeVector	edgeNorm[2];	// RZ edge normals [i] at {r[i],z[i]}
	} G4PolyhedraSideVec;

	G4int	 numSide;	// Number sides
	G4double r[2], z[2];	// r, z parameters, in specified order
	G4double startPhi,	// Start phi (0 to 2pi), if phiIsOpen
		 deltaPhi,	// Delta phi (0 to 2pi), if phiIsOpen
		 endPhi;	// End phi (>startPhi), if phiIsOpen
	G4bool	 phiIsOpen;	// True if there is a phi slice
	G4bool	 allBehind;	// True if the entire solid is "behind" this face
	
	G4IntersectingCone	*cone;	// Our intersecting cone
	
	G4PolyhedraSideVec	*vecs;		// Vector set for each facet of our face
	G4PolyhedraSideEdge	*edges;		// The edges belong to vecs
	G4double 		lenRZ,		// RZ length of each side
				lenPhi[2];	// Phi dimensions of each side
	G4double		edgeNorm;	// Normal in RZ/Phi space to each side
	
	G4bool IntersectSidePlane( const G4ThreeVector &p, const G4ThreeVector &v,
				   const G4PolyhedraSideVec vec,
				         G4double normSign, 
				         G4double surfTolerance,
				   G4double &distance, G4double &distFromSurface );

	G4int LineHitsSegments( const G4ThreeVector &p, const G4ThreeVector &v,
				G4int *i1, G4int *i2 );

	G4int ClosestPhiSegment( G4double phi );
	
	G4int PhiSegment( G4double phi );
	
	G4double DistanceToOneSide( const G4ThreeVector &p,
				    const G4PolyhedraSideVec &vec,
				    G4double *normDist );

	G4double DistanceAway( const G4ThreeVector &p,
			       const G4PolyhedraSideVec &vec,
			       G4double *normDist );
			       
	void CopyStuff( const G4PolyhedraSide &source );
};


#endif