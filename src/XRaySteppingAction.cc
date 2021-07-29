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
/// \file XRaySteppingAction.cc
/// \brief Implementation of the XRaySteppingAction class

#include "XRaySteppingAction.hh"
#include "XRayEventAction.hh"
#include "XRayDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4VProcess.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

XRaySteppingAction::XRaySteppingAction(
                      const XRayDetectorConstruction* detectorConstruction,
                      XRayEventAction* eventAction)
  : G4UserSteppingAction(),
    fDetConstruction(detectorConstruction),
    fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

XRaySteppingAction::~XRaySteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void XRaySteppingAction::UserSteppingAction(const G4Step* step)
{
// Collect energy and track length step by step

  // get volume of the current step
  auto volume = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume();
  
  // step length
  //G4double stepLength = 0.;
  //if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
  //  stepLength = step->GetStepLength();
  //}
  
  if ( volume == fDetConstruction->GetDetectorPV() ) {
    // Get photon energy
    G4double Etot = step->GetTrack()->GetTotalEnergy(); 
    // Get creator process name
    const G4VProcess* proc = step->GetTrack()->GetCreatorProcess();
    G4String procName;
    if(proc != nullptr)
      procName = proc->GetProcessName();
    else
      procName = "NULL";
    // Add the energy to the appropriate histograms
    fEventAction->AddDet(Etot);
    if(procName == "phot")
      fEventAction->AddDetFluo(Etot);
  }
  
  
  //if ( volume == fDetConstruction->GetGapPV() ) {
  //  fEventAction->AddGap(edep,stepLength);
  //}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
