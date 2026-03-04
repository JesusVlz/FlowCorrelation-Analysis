#ifndef FORESTREADER_H
#define FORESTREADER_H

// C++ includes
#include <vector>
#include <iostream>

// Root includes
#include <TTree.h>
#include <TChain.h>
#include <TFile.h>

class ForestReader {
  
public:
  
  // Enumeration for data types
  enum enumDataTypes {kPp, kPbPb, kPpMC, kPbPbMC, kPPb, kPPbPlus, kPPb5TeV, kPPbMC, kPPbMCPlus};
  
  // Constructors and destructors
  ForestReader();                                              // Default constructor
  ForestReader(Int_t dataType, Int_t useJetTrigger, Int_t jetType, Int_t jetAxis, Int_t matchJets, Bool_t readTrackTree = true, Bool_t mixingMode = false, Bool_t megaSkimMode = false); // Custom constructor
  ForestReader(const ForestReader& in);                        // Copy constructor
  virtual ~ForestReader();                                     // Destructor
  ForestReader& operator=(const ForestReader& obj);            // Equal sign operator
  
  // Methods
  virtual void ReadForestFromFile(TFile* inputFile) = 0;             // Read the forest from a file
  virtual void ReadForestFromFileList(std::vector<TString> fileList) = 0; // Read the forest from a file list
  virtual void BurnForest() = 0;                                     // Burn the forest
  virtual void GetEvent(Int_t nEvent) = 0;                           // Get the nEventh event from the file
  virtual bool CheckFileProblems() = 0;                              // Check if there are problems in the file list
  
  // Getters for event information
  Int_t GetNEvents() const;            // Getter for number of events in the tree
  Float_t GetVz() const;               // Getter for vertex z-position
  ULong64_t GetEventNumber() const;    // Getter for event number
  Int_t GetNTracks() const;            // Getter for number of tracks
  Bool_t HasPrimaryVertex() const;     // Does the event have a primary vertex?
  Bool_t PassesNoiseFilter() const;    // Does the event pass HCAL noise filter?
  
  // Getters for track trees
  virtual Float_t GetTrackPt(Int_t iTrack) const = 0;                    // Getter for track pT
  virtual Float_t GetTrackPtError(Int_t iTrack) const = 0;               // Getter for track pT error
  virtual Float_t GetTrackPhi(Int_t iTrack) const = 0;                   // Getter for track phi
  virtual Float_t GetTrackEta(Int_t iTrack) const = 0;                   // Getter for track eta
  virtual Bool_t GetTrackHighPurity(Int_t iTrack) const = 0;             // Getter for the high purity of the track
  virtual Float_t GetTrackVertexDistanceZ(Int_t iTrack) const = 0;       // Getter for track distance from primary vertex in z-direction
  virtual Float_t GetTrackVertexDistanceZError(Int_t iTrack) const = 0;  // Getter for error of track distance from primary vertex in z-direction
  virtual Float_t GetTrackVertexDistanceXY(Int_t iTrack) const = 0;      // Getter for track distance from primary vertex in xy-direction
  virtual Float_t GetTrackVertexDistanceXYError(Int_t iTrack) const = 0; // Getter for error of track distance from primary vertex in xy-direction

protected:
  
  // Input parameters
  Int_t fDataType;       // 0 = pp, 1 = PbPb, 2 = pp MC, 3 = PbPb MC, 4 = pPb p -> -eta, 5 = pPb p -> +eta, 6 = pPb 5 TeV
  Int_t fUseJetTrigger;  // 0 = No trigger, 1 = CaloJet80, 2 = CaloJet100 ...
  Int_t fJetType;        // 0 = Calo jets, 1 = PF jets
  Int_t fJetAxis;        // 0 = Anti-kT axis, 1 = WTA axis
  Int_t fMatchJets;      // 0 = No jet matching, 1 = Match reco jets to gen jets
  Bool_t fReadTrackTree; // True = read track tree, False = do not read track tree
  Bool_t fMixingMode;    // True = do mixed events, false = do regular events
  Bool_t fIsMC;          // True = We are looking at monte carlo, False = Real data
  Bool_t fIsPPb;         // True = Analysis is run on pPb data, False = Not pPb data
  Bool_t fIsMiniAOD;     // True = Read from MiniAOD files, False = Read from AOD files
  Bool_t fMegaSkimMode;  // True = Files contain only strictly necessary info for event mixing

  // Variables for event information
  Float_t fVertexZ;                 // Vertex z-position
  ULong64_t fEventNumber;           // Event number
  Int_t fnTracks;                   // Number of tracks
  
  // Variables for Event selection
  Int_t fPrimaryVertexFilterBit;          // Event selection filter for primary vertex
  Int_t fHBHENoiseFilterBit;              // Event selection filter for HBHE noise
  
  // Common branches for all types of trees
  TBranch* fHiVzBranch;             // Branch for vertex z-position
  TBranch* fEventNumberBranch;      // Branch for event number
  
  // Branches for event selection
  TBranch* fPrimaryVertexBranch;          // Branch for primary vertex filter
  TBranch* fHBHENoiseBranch;              // Branch for HBHE noise filter

  // Common track tree branches
  TBranch* fTrackPtBranch;                    // Branch for track pT:s
  TBranch* fTrackPtErrorBranch;               // Branch for track pT errors
  TBranch* fTrackPhiBranch;                   // Branch for track phis
  TBranch* fTrackEtaBranch;                   // Branch for track etas
  TBranch* fHighPurityTrackBranch;            // Branch for the high purity of tracks
  TBranch* fTrackVertexDistanceZBranch;       // Branch for track distance from primary vertex in z-direction
  TBranch* fTrackVertexDistanceZErrorBranch;  // Branch for error for track distance from primary vertex in z-direction
  TBranch* fTrackVertexDistanceXYBranch;      // Branch for track distance from primary vertex in xy-direction
  TBranch* fTrackVertexDistanceXYErrorBranch; // Branch for error for track distance from primary vertex in xy-direction
};

#endif