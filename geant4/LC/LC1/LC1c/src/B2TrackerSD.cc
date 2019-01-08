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
// $Id: B2TrackerSD.cc 66536 2012-12-19 14:32:36Z ihrivnac $
//
/// \file B2TrackerSD.cc
/// \brief Implementation of the B2TrackerSD class

#include "B2TrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2TrackerSD::B2TrackerSD(const G4String& name,
                         const G4String& hitsCollectionName) 
 : G4VSensitiveDetector(name),
   fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2TrackerSD::~B2TrackerSD() 
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2TrackerSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection

  fHitsCollection 
    = new B2TrackerHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce

  G4int hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool B2TrackerSD::ProcessHits(G4Step* aStep, 
                                     G4TouchableHistory*)
{  
// Correct position of charged particle

  auto charge = aStep->GetTrack()->GetDefinition()->GetPDGCharge();
  if ( charge == 0.0 ) return true;



  // energy deposit
  G4double edep = aStep->GetTotalEnergyDeposit();
  G4StepPoint *pre = aStep->GetPreStepPoint();
  G4StepPoint *post = aStep->GetPostStepPoint();
  G4TouchableHandle preTouch = pre->GetTouchableHandle();
  
  G4int copyno=preTouch->GetCopyNumber();
  auto worldPos = pre->GetPosition();

  //G4cerr << " copyno=" << copyno ;
  //G4cerr << " pre.status=" << pre->GetStepStatus();
  //G4cerr << " post.status=" << post->GetStepStatus();
  //G4cerr << G4endl;

  if ( copyno == 0 ) return true;
  // if ( pre->GetStepStatus() != G4StepStatus::fGeomBoundary ) return true;
  // G4cerr << " Hit accepted.  copyno=" << copyno << G4endl;

  B2TrackerHit* newHit = new B2TrackerHit();

  newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
  newHit->SetCopyNo(pre->GetTouchableHandle()->GetCopyNumber());
  newHit->SetEdep(edep);
  newHit->SetPDG(aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding());
  newHit->SetCharge(pre->GetCharge());
  newHit->SetPrePos (pre->GetPosition());
  newHit->SetPreStatus( pre->GetStepStatus() );
  newHit->SetPreTime( pre->GetGlobalTime() ) ;
  newHit->SetPreMom (pre->GetMomentum() );
  newHit->SetPreEnergy(pre->GetTotalEnergy() );

  newHit->SetPostPos (post->GetPosition());
  newHit->SetPostStatus( post->GetStepStatus() );
  newHit->SetPostTime( post->GetGlobalTime() ) ;
  newHit->SetPostMom (post->GetMomentum() );
  newHit->SetPostEnergy(post->GetTotalEnergy() );

  fHitsCollection->insert( newHit );

  // newHit->Print();

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2TrackerSD::EndOfEvent(G4HCofThisEvent*)
{
  if ( verboseLevel>1 ) { 
     G4int nofHits = fHitsCollection->entries();
     G4cout
       << G4endl 
       << "-------->Hits Collection: in this event they are " << nofHits 
       << " hits in the tracker chambers: " << G4endl;
     for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
