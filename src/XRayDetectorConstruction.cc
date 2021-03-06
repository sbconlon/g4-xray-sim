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
// 
/// \file XRayDetectorConstruction.cc
/// \brief Implementation of the XRayDetectorConstruction class

#include "XRayDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
//#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//G4ThreadLocal 
//G4GlobalMagFieldMessenger* XRayDetectorConstruction::fMagFieldMessenger = nullptr; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

XRayDetectorConstruction::XRayDetectorConstruction()
 : G4VUserDetectorConstruction(),
   fTargetPV(nullptr),
   fDetectorPV(nullptr),
   fCheckOverlaps(true)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

XRayDetectorConstruction::~XRayDetectorConstruction()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* XRayDetectorConstruction::Construct()
{
  // Define materials 
  DefineMaterials();
  
  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void XRayDetectorConstruction::DefineMaterials()
{ 
  // Lead material defined using NIST Manager
  auto nistManager = G4NistManager::Instance();

  // Add World material
  nistManager->FindOrBuildMaterial("G4_AIR");

  // Add Target material
  nistManager->FindOrBuildMaterial("G4_Ti");

  // Add Detector material
  nistManager->FindOrBuildMaterial("G4_AIR");

  //nistManager->FindOrBuildMaterial("G4_Pb");
  
  // Liquid argon material
  //G4double a;  // mass of a mole;
  //G4double z;  // z=mean number of protons;  
  //G4double density; 
  //new G4Material("liquidArgon", z=18., a= 39.95*g/mole, density= 1.390*g/cm3);
         // The argon by NIST Manager is a gas with a different density

  // Vacuum
  //new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
  //                kStateGas, 2.73*kelvin, 3.e-18*pascal);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* XRayDetectorConstruction::DefineVolumes()
{
  // Geometry parameters
  //G4int nofLayers = 10;
  G4double targetThickness = .001*mm;
  G4double detectorThickness =  1.*nm;
  G4double targetSizeXY = 5.*cm;
  G4double detectorSizeXY = 2.*cm;

  G4double worldSizeXYZ = 20.*cm;
  
  // Get materials
  auto defaultMaterial = G4Material::GetMaterial("G4_AIR");
  auto targetMaterial = G4Material::GetMaterial("G4_Ti");
  auto detectorMaterial = G4Material::GetMaterial("G4_AIR");
  
  if ( ! defaultMaterial || ! targetMaterial || ! detectorMaterial ) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined."; 
    G4Exception("XRayDetectorConstruction::DefineVolumes()",
      "MyCode0001", FatalException, msg);
  }  
   
  //     
  // World
  //
  auto worldS 
    = new G4Box("World",           // its name
                 worldSizeXYZ/2, worldSizeXYZ/2, worldSizeXYZ/2); // its size
                         
  auto worldLV
    = new G4LogicalVolume(
                 worldS,           // its solid
                 defaultMaterial,  // its material
                 "World");         // its name
                                   
  auto worldPV
    = new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(),  // at (0,0,0)
                 worldLV,          // its logical volume                         
                 "World",          // its name
                 0,                // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps
  
  //                               
  // Target
  //
  auto targetS 
    = new G4Box("Target",            // its name
                 targetSizeXY/2, targetSizeXY/2, targetThickness/2); // its size
                         
  auto targetLV
    = new G4LogicalVolume(
                 targetS,        // its solid
                 targetMaterial, // its material
                 "Target");          // its name
                                   
  fTargetPV
    = new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(0., 0., -3.*cm),
                 targetLV,       // its logical volume                         
                 "Target",           // its name
                 worldLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 

  //                               
  // Detector
  //
  auto detectorS 
    = new G4Box("Detector",             // its name
                 detectorSizeXY/2, detectorSizeXY/2, detectorThickness/2); // its size
                         
  auto detectorLV
    = new G4LogicalVolume(
                 detectorS,             // its solid
                 detectorMaterial,      // its material
                 "Detector");           // its name
                                   
  fDetectorPV
    = new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(3.*cm, 0., 0.),
                 detectorLV,            // its logical volume                         
                 "Detector",            // its name
                 worldLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 
  
  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*
void XRayDetectorConstruction::ConstructSDandField()
{ 
  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue;
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
  
  // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);
}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......