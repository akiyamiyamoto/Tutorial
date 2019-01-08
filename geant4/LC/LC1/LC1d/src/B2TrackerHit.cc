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
// $Id: B2TrackerHit.cc 69505 2013-05-07 01:57:27Z asaim $
//
/// \file B2TrackerHit.cc
/// \brief Implementation of the B2TrackerHit class

#include "B2TrackerHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<B2TrackerHit>* B2TrackerHitAllocator=0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2TrackerHit::B2TrackerHit()
 : G4VHit(),
   fTrackID(-1),
   fCopyNo(-1),
   fEdep(0.), fPDG(0), fCharge(0.0),
   fPreStatus(-1), fPrePos(G4ThreeVector()), fPreTime(0.0), 
   fPreMom(G4ThreeVector()), fPreEnergy(0.0),
   fPostStatus(-1), fPostPos(G4ThreeVector()), fPostTime(0.0), 
   fPostMom(G4ThreeVector()), fPostEnergy(0.0)

{}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2TrackerHit::~B2TrackerHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2TrackerHit::B2TrackerHit(const B2TrackerHit& right)
  : G4VHit()
{
  fTrackID   = right.fTrackID;
  fCopyNo = right.fCopyNo;
  fEdep   = right.fEdep;
  fPDG    = right.fPDG; fCharge = right.fCharge;
  fPreStatus = right.fPreStatus; fPreTime = right.fPreTime; 
  fPrePos = right.fPrePos; fPreMom = right.fPreMom;
  fPreEnergy = right.fPreEnergy;
  fPostStatus = right.fPostStatus; fPostTime = right.fPostTime; 
  fPostPos = right.fPostPos; fPostMom = right.fPostMom;
  fPostEnergy = right.fPostEnergy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const B2TrackerHit& B2TrackerHit::operator=(const B2TrackerHit& right)
{
  fTrackID   = right.fTrackID;
  fCopyNo = right.fCopyNo;
  fEdep   = right.fEdep;
  fPDG    = right.fPDG; fCharge = right.fCharge;
  fPreStatus = right.fPreStatus; fPreTime = right.fPreTime; 
  fPrePos = right.fPrePos; fPreMom = right.fPreMom;
  fPreEnergy = right.fPreEnergy;
  fPostStatus = right.fPostStatus; fPostTime = right.fPostTime; 
  fPostPos = right.fPostPos; fPostMom = right.fPostMom;
  fPostEnergy = right.fPostEnergy;

  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int B2TrackerHit::operator==(const B2TrackerHit& right) const
{
  return ( this == &right ) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2TrackerHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(fPrePos);
    circle.SetScreenSize(4.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2TrackerHit::Print()
{
  G4cout
     << "  trackID: " << fTrackID << " coptyNb: " << fCopyNo
     << " status: " << fPreStatus 
     << "Edep: "
     << std::setw(7) << G4BestUnit(fEdep,"Energy")
     << " Position: "
     << std::setw(7) << G4BestUnit( fPrePos,"Length")
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
