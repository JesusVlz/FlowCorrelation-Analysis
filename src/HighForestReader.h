// Reader for track trees from CMS data
// Optimized for Two-Particle Correlation in pO collisions
//
//===========================================================
// HighForestReader.h
//===========================================================

#ifndef HIGHFORESTREADER_H
#define HIGHFORESTREADER_H

// Own includes
#include "ForestReader.h"
#include <vector>

using namespace std;

class HighForestReader : public ForestReader{
  
private:
  static const Int_t fnMaxTrack = 60000;    // Maximum number of tracks in an event
  
public:
  
  // Constructors and destructors
  HighForestReader();                                              // Default constructor
  HighForestReader(Int_t dataType, Int_t useJetTrigger, Int_t jetType, Int_t jetAxis, Int_t matchJets, Bool_t readTrackTree = true, Bool_t mixingMode = false, Bool_t megaSkimMode = false); // Custom constructor
  HighForestReader(const HighForestReader& in);                    // Copy constructor
  virtual ~HighForestReader();                                     // Destructor
  HighForestReader& operator=(const HighForestReader& obj);        // Equal sign operator
  
  // Methods
  void ReadForestFromFile(TFile* inputFile);  // Read the forest from a file
  void ReadForestFromFileList(std::vector<TString> fileList);  // Read the forest from a file list
  void BurnForest();                           // Burn the forest  
  void GetEvent(Int_t nEvent);                 // Get the nEventh event from the file
  bool CheckFileProblems();                    // Check if there are problems in the file list
  
  // Getters for leaves in the track tree
  Float_t GetTrackPt(Int_t iTrack) const;                    // Getter for track pT
  Float_t GetTrackPtError(Int_t iTrack) const;               // Getter for track pT error
  Float_t GetTrackPhi(Int_t iTrack) const;                   // Getter for track phi
  Float_t GetTrackEta(Int_t iTrack) const;                   // Getter for track eta
  Bool_t GetTrackHighPurity(Int_t iTrack) const;             // Getter for the high purity of the track
  Float_t GetTrackVertexDistanceZ(Int_t iTrack) const;       // Getter for track distance from primary vertex in z-direction
  Float_t GetTrackVertexDistanceZError(Int_t iTrack) const;  // Getter for error of track distance from primary vertex in z-direction
  Float_t GetTrackVertexDistanceXY(Int_t iTrack) const;      // Getter for track distance from primary vertex in xy-direction
  Float_t GetTrackVertexDistanceXYError(Int_t iTrack) const; // Getter for error of track distance from primary vertex in xy-direction
  
private:
  
  // Methods
  void Initialize();       // Connect the branches to the tree

  // Trees in the forest
  TChain* fHeavyIonTree;    // Tree for heavy ion event information
  TChain* fSkimTree;        // Tree for event selection information
  TChain* fTrackTree;       // Tree for tracks.
  
  // Branches
  TBranch* fnTracksBranch;       // Branch for number of tracks
  
  // Leaves for the track tree in AOD forests
  Float_t fTrackPtArray[fnMaxTrack] = {0};                    // Array for track pT:s
  Float_t fTrackPtErrorArray[fnMaxTrack] = {0};               // Array for track pT errors
  Float_t fTrackPhiArray[fnMaxTrack] = {0};                   // Array for track phis
  Float_t fTrackEtaArray[fnMaxTrack] = {0};                   // Array for track etas
  Bool_t fHighPurityTrackArray[fnMaxTrack] = {0};             // Array for the high purity of tracks
  Float_t fTrackVertexDistanceZArray[fnMaxTrack] = {0};       // Array for track distance from primary vertex in z-direction
  Float_t fTrackVertexDistanceZErrorArray[fnMaxTrack] = {0};  // Array for error for track distance from primary vertex in z-direction
  Float_t fTrackVertexDistanceXYArray[fnMaxTrack] = {0};      // Array for track distance from primary vertex in xy-direction
  Float_t fTrackVertexDistanceXYErrorArray[fnMaxTrack] = {0}; // Array for error for track distance from primary vertex in xy-direction
  
  // Leaves for the track tree in MiniAOD forests
  vector<float>* fTrackPtVector;                    // Vector for track pT:s
  vector<float>* fTrackPtErrorVector;               // Vector for track pT errors
  vector<float>* fTrackPhiVector;                   // Vector for track phis
  vector<float>* fTrackEtaVector;                   // Vector for track etas
  vector<bool>* fHighPurityTrackVector;             // Vector for the high purity of tracks
  vector<float>* fTrackVertexDistanceZVector;       // Vector for track distance from primary vertex in z-direction
  vector<float>* fTrackVertexDistanceZErrorVector;  // Vector for error for track distance from primary vertex in z-direction
  vector<float>* fTrackVertexDistanceXYVector;      // Vector for track distance from primary vertex in xy-direction
  vector<float>* fTrackVertexDistanceXYErrorVector; // Vector for error for track distance from primary vertex in xy-direction

};

#endif