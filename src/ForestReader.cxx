#include "ForestReader.h"

// Default constructor
ForestReader::ForestReader() :
  fDataType(0),
  fUseJetTrigger(0),
  fJetType(0),
  fJetAxis(0),
  fMatchJets(0),
  fReadTrackTree(true),
  fMixingMode(false),
  fIsMC(false),
  fIsPPb(false),
  fIsMiniAOD(false),
  fMegaSkimMode(false),
  fVertexZ(0),
  fEventNumber(0),
  fnTracks(0),
  fPrimaryVertexFilterBit(0),
  fHBHENoiseFilterBit(0),
  fHiVzBranch(0),
  fEventNumberBranch(0),
  fPrimaryVertexBranch(0),
  fHBHENoiseBranch(0),
  fTrackPtBranch(0),
  fTrackPtErrorBranch(0),
  fTrackPhiBranch(0),
  fTrackEtaBranch(0),
  fHighPurityTrackBranch(0),
  fTrackVertexDistanceZBranch(0),
  fTrackVertexDistanceZErrorBranch(0),
  fTrackVertexDistanceXYBranch(0),
  fTrackVertexDistanceXYErrorBranch(0)
{
}

// Custom constructor
ForestReader::ForestReader(Int_t dataType, Int_t useJetTrigger, Int_t jetType, Int_t jetAxis, Int_t matchJets, Bool_t readTrackTree, Bool_t mixingMode, Bool_t megaSkimMode) :
  fDataType(dataType),
  fUseJetTrigger(useJetTrigger),
  fJetType(jetType),
  fJetAxis(jetAxis),
  fMatchJets(matchJets),
  fReadTrackTree(readTrackTree),
  fMixingMode(mixingMode),
  fIsMiniAOD(false),
  fMegaSkimMode(megaSkimMode),
  fVertexZ(0),
  fEventNumber(0),
  fnTracks(0),
  fPrimaryVertexFilterBit(0),
  fHBHENoiseFilterBit(0),
  fHiVzBranch(0),
  fEventNumberBranch(0),
  fPrimaryVertexBranch(0),
  fHBHENoiseBranch(0),
  fTrackPtBranch(0),
  fTrackPtErrorBranch(0),
  fTrackPhiBranch(0),
  fTrackEtaBranch(0),
  fHighPurityTrackBranch(0),
  fTrackVertexDistanceZBranch(0),
  fTrackVertexDistanceZErrorBranch(0),
  fTrackVertexDistanceXYBranch(0),
  fTrackVertexDistanceXYErrorBranch(0)
{
  if(fDataType == kPpMC || fDataType == kPbPbMC || fDataType == kPPbMC || fDataType == kPPbMCPlus){
    fIsMC = true;
  } else {
    fIsMC = false;
  }
  
  if(fDataType >= kPPb){
    fIsPPb = true;
  } else {
    fIsPPb = false;
  }
}

// Copy constructor
ForestReader::ForestReader(const ForestReader& in) :
  fDataType(in.fDataType),
  fUseJetTrigger(in.fUseJetTrigger),
  fJetType(in.fJetType),
  fJetAxis(in.fJetAxis),
  fMatchJets(in.fMatchJets),
  fReadTrackTree(in.fReadTrackTree),
  fMixingMode(in.fMixingMode),
  fIsMC(in.fIsMC),
  fIsPPb(in.fIsPPb),
  fIsMiniAOD(in.fIsMiniAOD),
  fMegaSkimMode(in.fMegaSkimMode),
  fVertexZ(in.fVertexZ),
  fEventNumber(in.fEventNumber),
  fnTracks(in.fnTracks),
  fPrimaryVertexFilterBit(in.fPrimaryVertexFilterBit),
  fHBHENoiseFilterBit(in.fHBHENoiseFilterBit),
  fHiVzBranch(in.fHiVzBranch),
  fEventNumberBranch(in.fEventNumberBranch),
  fPrimaryVertexBranch(in.fPrimaryVertexBranch),
  fHBHENoiseBranch(in.fHBHENoiseBranch),
  fTrackPtBranch(in.fTrackPtBranch),
  fTrackPtErrorBranch(in.fTrackPtErrorBranch),
  fTrackPhiBranch(in.fTrackPhiBranch),
  fTrackEtaBranch(in.fTrackEtaBranch),
  fHighPurityTrackBranch(in.fHighPurityTrackBranch),
  fTrackVertexDistanceZBranch(in.fTrackVertexDistanceZBranch),
  fTrackVertexDistanceZErrorBranch(in.fTrackVertexDistanceZErrorBranch),
  fTrackVertexDistanceXYBranch(in.fTrackVertexDistanceXYBranch),
  fTrackVertexDistanceXYErrorBranch(in.fTrackVertexDistanceXYErrorBranch)
{
}

// Assignment operator
ForestReader& ForestReader::operator=(const ForestReader& in){
  if (&in==this) return *this;
  fDataType = in.fDataType;
  fUseJetTrigger = in.fUseJetTrigger;
  fJetType = in.fJetType;
  fJetAxis = in.fJetAxis;
  fMatchJets = in.fMatchJets;
  fReadTrackTree = in.fReadTrackTree;
  fMixingMode = in.fMixingMode;
  fIsMC = in.fIsMC;
  fIsPPb = in.fIsPPb;
  fIsMiniAOD = in.fIsMiniAOD;
  fMegaSkimMode = in.fMegaSkimMode;
  fVertexZ = in.fVertexZ;
  fEventNumber = in.fEventNumber;
  fnTracks = in.fnTracks;
  fPrimaryVertexFilterBit = in.fPrimaryVertexFilterBit;
  fHBHENoiseFilterBit = in.fHBHENoiseFilterBit;
  fHiVzBranch = in.fHiVzBranch;
  fEventNumberBranch = in.fEventNumberBranch;
  fPrimaryVertexBranch = in.fPrimaryVertexBranch;
  fHBHENoiseBranch = in.fHBHENoiseBranch;
  fTrackPtBranch = in.fTrackPtBranch;
  fTrackPtErrorBranch = in.fTrackPtErrorBranch;
  fTrackPhiBranch = in.fTrackPhiBranch;
  fTrackEtaBranch = in.fTrackEtaBranch;
  fHighPurityTrackBranch = in.fHighPurityTrackBranch;
  fTrackVertexDistanceZBranch = in.fTrackVertexDistanceZBranch;
  fTrackVertexDistanceZErrorBranch = in.fTrackVertexDistanceZErrorBranch;
  fTrackVertexDistanceXYBranch = in.fTrackVertexDistanceXYBranch;
  fTrackVertexDistanceXYErrorBranch = in.fTrackVertexDistanceXYErrorBranch;
  return *this;
}

ForestReader::~ForestReader(){}

// Getters for general event info
Int_t ForestReader::GetNEvents() const{
  return fHiVzBranch->GetEntries();
}

Float_t ForestReader::GetVz() const{
  return fVertexZ;
}

ULong64_t ForestReader::GetEventNumber() const{
  return fEventNumber;
}

Int_t ForestReader::GetNTracks() const{
  return fnTracks;
}

Bool_t ForestReader::HasPrimaryVertex() const{
  return fPrimaryVertexFilterBit == 1;
}

Bool_t ForestReader::PassesNoiseFilter() const{
  return fHBHENoiseFilterBit == 1;
}