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
/// \file XRayEventAction.hh
/// \brief Definition of the XRayEventAction class

#ifndef XRayEventAction_h
#define XRayEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

/// Event action class
///
/// It defines data members to hold the energy deposit and track lengths
/// of charged particles in Absober and Gap layers:
/// - fEnergyAbs, fEnergyGap, fTrackLAbs, fTrackLGap
/// which are collected step by step via the functions
/// - AddAbs(), AddGap()

class XRayEventAction : public G4UserEventAction
{
  public:
    XRayEventAction();
    virtual ~XRayEventAction();

    virtual void  BeginOfEventAction(const G4Event* event);
    virtual void    EndOfEventAction(const G4Event* event);
    
    void AddDet(G4double E);
    void AddDetFluo(G4double E);
    
  private:
    G4double  fEnergyDet;       // Energy incident on detector
    G4double  fEnergyDetFluo;   // Energy incident on detector from fluorescence photon
};

// inline functions

inline void XRayEventAction::AddDet(G4double E) {
  if(fEnergyDet == 0.)
    fEnergyDet = E;
}

inline void XRayEventAction::AddDetFluo(G4double E) {
  if(fEnergyDetFluo == 0.)
    fEnergyDetFluo = E;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
