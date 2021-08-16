// -*- C++ -*-
// ----------------------------------------------------------------------
// HEP coherent system of Units
//
// This file has been provided to CLHEP by Geant4 (simulation toolkit for HEP).
//
// The basic units are :
// millimeter              (millimeter)
// nanosecond              (nanosecond)
// Mega electron Volt      (MeV)
// positron charge         (eplus)
// degree Kelvin           (kelvin)
// the amount of substance (mole)
// luminous intensity      (candela)
// radian                  (radian)
// steradian               (steradian)
//
// Below is a non exhaustive list of derived and pratical units
// (i.e. mostly the SI units).
// You can add your own units.
//
// The SI numerical value of the positron charge is defined here,
// as it is needed for conversion factor : positron charge = e_SI (coulomb)
//
// The others physical constants are defined in the header file :
// PhysicalConstants.h
//
// Authors: M.Maire, S.Giani
//
// History:
//
// 06.02.96   Created.
// 28.03.96   Added miscellaneous constants.
// 05.12.97   E.Tcherniaev: Redefined pascal (to avoid warnings on WinNT)
// 20.05.98   names: meter, second, gram, radian, degree
//            (from Brian.Lasiuk@yale.edu (STAR)). Added luminous units.
// 05.08.98   angstrom, picobarn, microsecond, picosecond, petaelectronvolt
// 01.03.01   parsec    
// 31.01.06   kilogray, milligray, microgray    
// 29.04.08   use PDG 2006 value of e_SI
// 03.11.08   use PDG 2008 value of e_SI
// 19.08.15   added liter and its sub units (mma)
// 12.01.16   added symbols for microsecond (us) and picosecond (ps) (mma)
// 17.07.20   use PDG 2019 value of e_SI

#ifndef HEP_SYSTEM_OF_UNITS_H
#define HEP_SYSTEM_OF_UNITS_H

namespace CLHEP {

  //
  //
  //
  inline static constexpr double     pi  = 3.14159265358979323846;
  inline static constexpr double  twopi  = 2*pi;
  inline static constexpr double halfpi  = pi/2;
  inline static constexpr double     pi2 = pi*pi;

  // 
  // Length [L]
  //
  inline static constexpr double millimeter  = 1.;                        
  inline static constexpr double millimeter2 = millimeter*millimeter;
  inline static constexpr double millimeter3 = millimeter*millimeter*millimeter;

  inline static constexpr double centimeter  = 10.*millimeter;   
  inline static constexpr double centimeter2 = centimeter*centimeter;
  inline static constexpr double centimeter3 = centimeter*centimeter*centimeter;
    
  inline static constexpr double meter  = 1000.*millimeter;                  
  inline static constexpr double meter2 = meter*meter;
  inline static constexpr double meter3 = meter*meter*meter;

  inline static constexpr double kilometer = 1000.*meter;                   
  inline static constexpr double kilometer2 = kilometer*kilometer;
  inline static constexpr double kilometer3 = kilometer*kilometer*kilometer;

  inline static constexpr double parsec = 3.0856775807e+16*meter;

  inline static constexpr double micrometer = 1.e-6 *meter;             
  inline static constexpr double  nanometer = 1.e-9 *meter;
  inline static constexpr double  angstrom  = 1.e-10*meter;
  inline static constexpr double  fermi     = 1.e-15*meter;

  inline static constexpr double      barn = 1.e-28*meter2;
  inline static constexpr double millibarn = 1.e-3 *barn;
  inline static constexpr double microbarn = 1.e-6 *barn;
  inline static constexpr double  nanobarn = 1.e-9 *barn;
  inline static constexpr double  picobarn = 1.e-12*barn;

  // symbols
  inline static constexpr double nm  = nanometer;                        
  inline static constexpr double um  = micrometer;                        

  inline static constexpr double mm  = millimeter;                        
  inline static constexpr double mm2 = millimeter2;
  inline static constexpr double mm3 = millimeter3;

  inline static constexpr double cm  = centimeter;   
  inline static constexpr double cm2 = centimeter2;
  inline static constexpr double cm3 = centimeter3;

  inline static constexpr double liter = 1.e+3*cm3;
  inline static constexpr double  L = liter;
  inline static constexpr double dL = 1.e-1*liter;
  inline static constexpr double cL = 1.e-2*liter;
  inline static constexpr double mL = 1.e-3*liter;       
  
  inline static constexpr double m  = meter;                  
  inline static constexpr double m2 = meter2;
  inline static constexpr double m3 = meter3;

  inline static constexpr double km  = kilometer;                   
  inline static constexpr double km2 = kilometer2;
  inline static constexpr double km3 = kilometer3;

  inline static constexpr double pc = parsec;

  //
  // Angle
  //
  inline static constexpr double radian      = 1.;                  
  inline static constexpr double milliradian = 1.e-3*radian;
  inline static constexpr double degree = (pi/180.0)*radian;

  inline static constexpr double   steradian = 1.;
  
  // symbols
  inline static constexpr double rad  = radian;
  inline static constexpr double mrad = milliradian;
  inline static constexpr double sr   = steradian;
  inline static constexpr double deg  = degree;

  //
  // Time [T]
  //
  inline static constexpr double nanosecond  = 1.;
  inline static constexpr double second      = 1.e+9 *nanosecond;
  inline static constexpr double millisecond = 1.e-3 *second;
  inline static constexpr double microsecond = 1.e-6 *second;
  inline static constexpr double  picosecond = 1.e-12*second;

  inline static constexpr double hertz = 1./second;
  inline static constexpr double kilohertz = 1.e+3*hertz;
  inline static constexpr double megahertz = 1.e+6*hertz;

  // symbols
  inline static constexpr double ns = nanosecond;
  inline static constexpr double  s = second;
  inline static constexpr double ms = millisecond;
  inline static constexpr double us = microsecond;
  inline static constexpr double ps = picosecond;

  //
  // Electric charge [Q]
  //
  inline static constexpr double eplus = 1. ;// positron charge
  inline static constexpr double e_SI  = 1.602176634e-19;// positron charge in coulomb
  inline static constexpr double coulomb = eplus/e_SI;// coulomb = 6.24150 e+18 * eplus

  //
  // Energy [E]
  //
  inline static constexpr double megaelectronvolt = 1. ;
  inline static constexpr double     electronvolt = 1.e-6*megaelectronvolt;
  inline static constexpr double kiloelectronvolt = 1.e-3*megaelectronvolt;
  inline static constexpr double gigaelectronvolt = 1.e+3*megaelectronvolt;
  inline static constexpr double teraelectronvolt = 1.e+6*megaelectronvolt;
  inline static constexpr double petaelectronvolt = 1.e+9*megaelectronvolt;

  inline static constexpr double joule = electronvolt/e_SI;// joule = 6.24150 e+12 * MeV

  // symbols
  inline static constexpr double MeV = megaelectronvolt;
  inline static constexpr double  eV = electronvolt;
  inline static constexpr double keV = kiloelectronvolt;
  inline static constexpr double GeV = gigaelectronvolt;
  inline static constexpr double TeV = teraelectronvolt;
  inline static constexpr double PeV = petaelectronvolt;

  //
  // Mass [E][T^2][L^-2]
  //
  inline static constexpr double  kilogram = joule*second*second/(meter*meter);   
  inline static constexpr double      gram = 1.e-3*kilogram;
  inline static constexpr double milligram = 1.e-3*gram;

  // symbols
  inline static constexpr double  kg = kilogram;
  inline static constexpr double   g = gram;
  inline static constexpr double  mg = milligram;

  //
  // Power [E][T^-1]
  //
  inline static constexpr double watt = joule/second;// watt = 6.24150 e+3 * MeV/ns

  //
  // Force [E][L^-1]
  //
  inline static constexpr double newton = joule/meter;// newton = 6.24150 e+9 * MeV/mm

  //
  // Pressure [E][L^-3]
  //
#define pascal hep_pascal                          // a trick to avoid warnings 
  inline static constexpr double hep_pascal = newton/m2;   // pascal = 6.24150 e+3 * MeV/mm3
  inline static constexpr double bar        = 100000*pascal; // bar    = 6.24150 e+8 * MeV/mm3
  inline static constexpr double atmosphere = 101325*pascal; // atm    = 6.32420 e+8 * MeV/mm3

  //
  // Electric current [Q][T^-1]
  //
  inline static constexpr double      ampere = coulomb/second; // ampere = 6.24150 e+9 * eplus/ns
  inline static constexpr double milliampere = 1.e-3*ampere;
  inline static constexpr double microampere = 1.e-6*ampere;
  inline static constexpr double  nanoampere = 1.e-9*ampere;

  //
  // Electric potential [E][Q^-1]
  //
  inline static constexpr double megavolt = megaelectronvolt/eplus;
  inline static constexpr double kilovolt = 1.e-3*megavolt;
  inline static constexpr double     volt = 1.e-6*megavolt;

  //
  // Electric resistance [E][T][Q^-2]
  //
  inline static constexpr double ohm = volt/ampere;// ohm = 1.60217e-16*(MeV/eplus)/(eplus/ns)

  //
  // Electric capacitance [Q^2][E^-1]
  //
  inline static constexpr double farad = coulomb/volt;// farad = 6.24150e+24 * eplus/Megavolt
  inline static constexpr double millifarad = 1.e-3*farad;
  inline static constexpr double microfarad = 1.e-6*farad;
  inline static constexpr double  nanofarad = 1.e-9*farad;
  inline static constexpr double  picofarad = 1.e-12*farad;

  //
  // Magnetic Flux [T][E][Q^-1]
  //
  inline static constexpr double weber = volt*second;// weber = 1000*megavolt*ns

  //
  // Magnetic Field [T][E][Q^-1][L^-2]
  //
  inline static constexpr double tesla     = volt*second/meter2;// tesla =0.001*megavolt*ns/mm2

  inline static constexpr double gauss     = 1.e-4*tesla;
  inline static constexpr double kilogauss = 1.e-1*tesla;

  //
  // Inductance [T^2][E][Q^-2]
  //
  inline static constexpr double henry = weber/ampere;// henry = 1.60217e-7*MeV*(ns/eplus)**2

  //
  // Temperature
  //
  inline static constexpr double kelvin = 1.;

  //
  // Amount of substance
  //
  inline static constexpr double mole = 1.;

  //
  // Activity [T^-1]
  //
  inline static constexpr double becquerel = 1./second ;
  inline static constexpr double curie = 3.7e+10 * becquerel;
  inline static constexpr double kilobecquerel = 1.e+3*becquerel;
  inline static constexpr double megabecquerel = 1.e+6*becquerel;
  inline static constexpr double gigabecquerel = 1.e+9*becquerel;
  inline static constexpr double millicurie = 1.e-3*curie;
  inline static constexpr double microcurie = 1.e-6*curie;
  inline static constexpr double Bq = becquerel;
  inline static constexpr double kBq = kilobecquerel;
  inline static constexpr double MBq = megabecquerel;
  inline static constexpr double GBq = gigabecquerel;
  inline static constexpr double Ci = curie;
  inline static constexpr double mCi = millicurie;
  inline static constexpr double uCi = microcurie;

  //
  // Absorbed dose [L^2][T^-2]
  //
  inline static constexpr double      gray = joule/kilogram ;
  inline static constexpr double  kilogray = 1.e+3*gray;
  inline static constexpr double milligray = 1.e-3*gray;
  inline static constexpr double microgray = 1.e-6*gray;

  //
  // Luminous intensity [I]
  //
  inline static constexpr double candela = 1.;

  //
  // Luminous flux [I]
  //
  inline static constexpr double lumen = candela*steradian;

  //
  // Illuminance [I][L^-2]
  //
  inline static constexpr double lux = lumen/meter2;

  //
  // Miscellaneous
  //
  inline static constexpr double perCent     = 0.01 ;
  inline static constexpr double perThousand = 0.001;
  inline static constexpr double perMillion  = 0.000001;

}  // namespace CLHEP

#endif /* HEP_SYSTEM_OF_UNITS_H */
