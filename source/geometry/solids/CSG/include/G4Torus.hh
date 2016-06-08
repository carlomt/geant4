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
// $Id: G4Torus.hh,v 1.13 2001/07/11 09:59:55 gunter Exp $
// GEANT4 tag $Name: geant4-04-01 $
//
// 
// --------------------------------------------------------------------
// GEANT 4 class header file
//
// G4Torus
//
// Class description:
//
//   A torus or torus segment with curved sides parallel to the z-axis.
//   The torus has a specified swept radius about which it is centered,
//   and a given minimum and maximum radius. A minimum radius of 0
//   signifies a filled torus.
//   The torus segment is specified by starting and delta angles for phi,
//   with 0 being the +x axis, PI/2 the +y axis. A delta angle of 2PI
//   signifies a complete, unsegmented torus/cylindr.
//
//   Member functions:
//
//   As inherited from G4CSGSolid+
//
//     G4Torus(const G4String      &pName
//             G4double      pRmin
//             G4double      pRmax
//             G4double      pRtor
//             G4double      pSPhi
//             G4double      pDPhi )
//
//     - Construct a torus with the given name and dimensions.
//       The angles are provided is radians. pRtor >= pRmax
//
//
//   Protected:
//
//     G4ThreeVectorList*
//     CreateRotatedVertices(const G4AffineTransform& pTransform) const
//
//     - Create the List of transformed vertices in the format required
//       for G4VSolid:: ClipCrossSection and ClipBetweenSections.
//   
//   Member Data:
//
//	fRmin	Inside radius
//	fRmax	Outside radius
//	fRtor	swept radius of torus
//
//	fSPhi	The starting phi angle in radians,
//              adjusted such the fSPhi+fDPhi<=2PI,
//              fSPhi>-2PI
//
//	fDPhi	Delta angle of the segment in radians
//
//   You could find very often in G4Torus:: functions the values like pt or
//   it . These are the distances from p or i G4ThreeVector points in the
//   plane (Z axis points p or i) to fRtor point in XY plane. This value is
//   similar to rho for G4Tubs and is used for definiton of the point
//   relative to fRmin and fRmax, i.e. for solution of inside/outside
//   problems
 
// History:
// 30.10.96 V.Grichine     First version of G4Torus
// 21.04.98 J.Apostolakis  Added SetAllParameters function
// 26.05.00 V.Grichine, new SolveBiQuadratic/Cubic developed by O.Cremonesi were
//          added     
// 31.08.00 E.Medernach    Added SolveNumeric Functions 
// --------------------------------------------------------------------

#ifndef G4Torus_HH
#define G4Torus_HH

#include "G4CSGSolid.hh"

class G4Torus : public G4CSGSolid
{

public:

  G4Torus(const G4String &pName,
		  G4double pRmin,
		  G4double pRmax,
		  G4double pRtor,
		  G4double pSPhi,
		  G4double pDPhi);

  virtual ~G4Torus();
    
  void SetAllParameters(G4double pRmin, G4double pRmax, G4double pRtor,
			G4double pSPhi, G4double pDPhi);
 
  void ComputeDimensions(G4VPVParameterisation* p,
			 const G4int n,
			 const G4VPhysicalVolume* pRep);
			   
  G4int TorusRoots(G4double Ri,
		   const G4ThreeVector& p,
		   const G4ThreeVector& v) const ;

  G4bool CalculateExtent(const EAxis pAxis,
			 const G4VoxelLimits& pVoxelLimit,
			 const G4AffineTransform& pTransform,
			 G4double& pmin, G4double& pmax) const;

  G4double    GetRmin() const { return fRmin ; }
  G4double    GetRmax() const { return fRmax ; } 
  G4double    GetRtor() const { return fRtor ; }
  G4double    GetSPhi() const { return fSPhi ; }
  G4double    GetDPhi() const { return fDPhi ; }

  EInside Inside(const G4ThreeVector& p) const;

  G4ThreeVector SurfaceNormal( const G4ThreeVector& p) const;

  G4double DistanceToIn(const G4ThreeVector& p,const G4ThreeVector& v) const;
  G4double DistanceToIn(const G4ThreeVector& p) const;
  G4double DistanceToOut(const G4ThreeVector& p,const G4ThreeVector& v,
			 const G4bool calcNorm=G4bool(false),
			 G4bool *validNorm=0,G4ThreeVector *n=0) const;
  G4double DistanceToOut(const G4ThreeVector& p) const;

  G4GeometryType  GetEntityType() const { return G4String("G4Torus"); }
    // Naming method (pseudo-RTTI : run-time type identification)

  // Visualisation functions

  void                DescribeYourselfTo (G4VGraphicsScene& scene) const;
  G4Polyhedron*       CreatePolyhedron   () const;
  G4NURBS*            CreateNURBS        () const;

protected:

  G4int SolveBiQuadratic(G4double c[], G4double s[]  ) const ;
  G4int SolveCubic(G4double c[], G4double s[]  ) const ;

  G4int SolveBiQuadraticNew(G4double c[], G4double s[]  ) const ;
  G4int SolveCubicNew(G4double c[], G4double s[], G4double& cd  ) const ;

  G4int SolveQuadratic(G4double c[], G4double s[]  ) const ;

  G4double SolveNumeric(const G4ThreeVector& p,
                        const G4ThreeVector& v,
			G4bool IsDistanceToIn) const;

  G4ThreeVectorList* CreateRotatedVertices(const G4AffineTransform& pTransform,
			                   G4int& noPolygonVertices) const;

  G4double fRmin,fRmax,fRtor,fSPhi,fDPhi;

  // Used by distanceToOut
  enum ESide {kNull,kRMin,kRMax,kSPhi,kEPhi};
  // used by normal
  enum ENorm {kNRMin,kNRMax,kNSPhi,kNEPhi};

private:

  G4double TorusEquation (G4double x, G4double y, G4double z,
                          G4double R0, G4double R1) const
  {
	/* R0 : Radius of all little circles
	   R1 : Radius of little circles
	*/
	/*
	  An interesting property is that the sign
	  tell if the point is inside or outside
	  or if > EPSILON on the surface
	*/
	G4double temp;

	temp = ((x*x + y*y + z*z) + R0*R0 - R1*R1) ;
	temp = temp*temp ;
	temp = temp - 4*R0*R0*(x*x + y*y) ;

	/*
	  > 0 Outside
	  < 0 Inside
	*/
	return temp ;
  }
  
  G4double TorusDerivativeX (G4double x, G4double y, G4double z,
                             G4double R0, G4double R1) const
  {
	return 4*x*(x*x + y*y + z*z +  R0*R0 - R1*R1) - 8*R0*R0*x ;
  }

  G4double TorusDerivativeY (G4double x, G4double y, G4double z,
                             G4double R0, G4double R1) const
  {
	return 4*y*(x*x + y*y + z*z +  R0*R0 - R1*R1) - 8*R0*R0*y ;
  }

  G4double TorusDerivativeZ (G4double x, G4double y, G4double z,
                             G4double R0, G4double R1) const
  {
	return 4*z*(x*x + y*y + z*z +  R0*R0 - R1*R1) ;
  }

  G4double TorusGradient(G4double dx, G4double dy, G4double dz,
			 G4double x, G4double y, G4double z,
			 G4double Rmax, G4double Rmin) const
  {
    /* This tell the normal at a surface point */
    G4double result;
    result = 0;
    result += dx*TorusDerivativeX(x,y,z,Rmax,Rmin); 
    result += dy*TorusDerivativeY(x,y,z,Rmax,Rmin); 
    result += dz*TorusDerivativeZ(x,y,z,Rmax,Rmin); 

    return result;
  }

  void BVMIntersection (G4double x, G4double y, G4double z,
			G4double dx, G4double dy, G4double dz,
			G4double Rmax, G4double Rmin,
			G4double *NewL, G4int *valid) const;
  
  void SortIntervals (G4double *SortL, G4double *NewL,
                      G4int *valid, G4int *NbIntersection) const;
  
  G4double DistanceToTorus (G4double x, G4double y, G4double z,
                            G4double dx, G4double dy, G4double dz,
                            G4double R0,G4double R1) const;
};


class TorusEquationClass
{
public:
  TorusEquationClass()
  {
    ;    
  }

  TorusEquationClass(G4double Rmax, G4double Rmin)		
  {
    R0 = Rmax;
    R1 = Rmin;
  }
  
  ~TorusEquationClass() {;}

  void setRadius (G4double Rmax, G4double Rmin)		
  {
    R0 = Rmax;
    R1 = Rmin;
  }
  
  
  void setPosition (G4double x,G4double y,G4double z)
  {
    Px = x;
    Py = y;
    Pz = z;
  }

  void setPosition (const G4ThreeVector& p)
  {
    Px = p.x();
    Py = p.y();
    Pz = p.z();
  }

  
  void setDirection (G4double dirx,G4double diry,G4double dirz)
  {
    dx = dirx;
    dy = diry;
    dz = dirz;    
  }

  void setDirection (const G4ThreeVector& v)
  {
    dx = v.x();
    dy = v.y();
    dz = v.z();    
  }

  

private:
  G4double R0;
  G4double R1;

  G4double Px,Py,Pz;
  G4double dx,dy,dz;
  
  
  G4double TorusEquation (G4double x, G4double y, G4double z) //const
  {
	/*
	  An interesting property is that the sign
	  tell if the point is inside or outside
	  or if > EPSILON on the surface
	*/
	G4double temp;

	temp = ((x*x + y*y + z*z) + R0*R0 - R1*R1) ;
	temp = temp*temp ;
	temp = temp - 4*R0*R0*(x*x + y*y) ;

	/*
	  > 0 Outside
	  < 0 Inside
	*/
	return temp ;
  }
  
  G4double TorusDerivativeX (G4double x, G4double y, G4double z) // const
  {
	return 4*x*(x*x + y*y + z*z +  R0*R0 - R1*R1) - 8*R0*R0*x ;
  }

  G4double TorusDerivativeY (G4double x, G4double y, G4double z) // const
  {
	return 4*y*(x*x + y*y + z*z +  R0*R0 - R1*R1) - 8*R0*R0*y ;
  }

  G4double TorusDerivativeZ (G4double x, G4double y, G4double z) // const
  {
	return 4*z*(x*x + y*y + z*z +  R0*R0 - R1*R1) ;
  }

public:  
  G4double Function (G4double value)
  {
    G4double Lx,Ly,Lz;
    G4double result;  
       
    Lx = Px + value*dx;
    Ly = Py + value*dy;
    Lz = Pz + value*dz;
       
    result = TorusEquation(Lx,Ly,Lz);
         
    return result ;  
  }

  G4double Derivative(G4double value)
  {
    G4double Lx,Ly,Lz;
    G4double result;

     Lx = Px + value*dx;
     Ly = Py + value*dy;
     Lz = Pz + value*dz;
      
     result = dx*TorusDerivativeX(Lx,Ly,Lz);
     result += dy*TorusDerivativeY(Lx,Ly,Lz);
     result += dz*TorusDerivativeZ(Lx,Ly,Lz);
   
     return result;
  }
} ;



#endif