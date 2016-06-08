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
// $Id: G4Conic.cc,v 1.6 2001/07/11 09:59:43 gunter Exp $
// GEANT4 tag $Name: geant4-04-01 $
//
// ----------------------------------------------------------------------
// GEANT 4 class source file
//
// G4conic.cc
//
// ----------------------------------------------------------------------

#include "G4Conic.hh"

G4Conic::G4Conic () : pShift(0)
{
}

G4Conic::~G4Conic()
{
}

G4Conic::G4Conic(const G4Conic& right)
  : position(right.position), pShift(right.pShift)
{
  bBox      = right.bBox;
  start     = right.start;
  end       = right.end;
  pStart    = right.pStart;
  pEnd      = right.pEnd;
  pRange    = right.pRange;
  bounded   = right.bounded;
  sameSense = right.sameSense;
}

G4Conic& G4Conic::operator=(const G4Conic& right)
{
  if (&right == this) return *this;

  pShift   = right.pShift;
  position = right.position;
  bBox      = right.bBox;
  start     = right.start;
  end       = right.end;
  pStart    = right.pStart;
  pEnd      = right.pEnd;
  pRange    = right.pRange;
  bounded   = right.bounded;
  sameSense = right.sameSense;

  return *this;
}

/*
void
G4ConicalCurve::ProjectCurve(const G4Plane& Pl1, const G4Plane& Pl2)
{  
  // Curve start
  Project(ProjStart, Start, Pl1, Pl2);
  // Curve end
  Project(ProjEnd, End, Pl1, Pl2);
  // Placement
  Position.ProjectPlacement(Pl1,Pl2);
}

G4int
G4ConicalCurve::HitPartOfCurve(G4double Angle, G4double Solution,
                               const G4Point2d& ProjHit)
{
    	// Check if Solution1 is part of the curve i.e. in the "pie"
	G4double TmpSol1 = Solution - ProjHit.X();
	G4Point2d ArcHit1(TmpSol1, ProjHit.Y());
	G4double Cross1 = CrossProduct( ProjStart, ArcHit1);
	G4double Cross2 = CrossProduct( ArcHit1  , ProjEnd);
	if( (Angle<=0 && Cross1<=0 && Cross2 <=0) ||
	    (Angle> 0 && !(Cross1>=0 && Cross2 >=0)) )
	    // Solution1 is on the curve
	  return 1;
	return 0;
}
*/