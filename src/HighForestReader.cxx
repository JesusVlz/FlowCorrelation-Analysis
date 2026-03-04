// Implementation for HighForestReader
// Optimized for Two-Particle Correlation in pO collisions

// Own includes
#include "HighForestReader.h"

// ROOT includes para CheckFileProblems
#include <TChainElement.h>
#include <TObjArray.h>

/*
 * Default constructor
 */
HighForestReader::HighForestReader() :
  ForestReader(),
  fHeavyIonTree(0),
  fSkimTree(0),
  fTrackTree(0),
  fnTracksBranch(0),
  fTrackPtArray(),
  fTrackPtErrorArray(),
  fTrackPhiArray(),
  fTrackEtaArray(),
  fHighPurityTrackArray(),
  fTrackVertexDistanceZArray(),
  fTrackVertexDistanceZErrorArray(),
  fTrackVertexDistanceXYArray(),
  fTrackVertexDistanceXYErrorArray(),
  fTrackPtVector(0),
  fTrackPtErrorVector(0),
  fTrackPhiVector(0),
  fTrackEtaVector(0),
  fHighPurityTrackVector(0),
  fTrackVertexDistanceZVector(0),
  fTrackVertexDistanceZErrorVector(0),
  fTrackVertexDistanceXYVector(0),
  fTrackVertexDistanceXYErrorVector(0)
{
  // Default constructor
}

/*
 * Custom constructor (Mantenemos la firma para compatibilidad con ForestReader)
 */
HighForestReader::HighForestReader(Int_t dataType, Int_t useJetTrigger, Int_t jetType, Int_t jetAxis, Int_t matchJets, Bool_t readTrackTree, Bool_t mixingMode, Bool_t megaSkimMode) :
  ForestReader(dataType,useJetTrigger,jetType,jetAxis,matchJets,readTrackTree,mixingMode,megaSkimMode),
  fHeavyIonTree(0),
  fSkimTree(0),
  fTrackTree(0),
  fnTracksBranch(0),
  fTrackPtVector(0),
  fTrackPtErrorVector(0),
  fTrackPhiVector(0),
  fTrackEtaVector(0),
  fHighPurityTrackVector(0),
  fTrackVertexDistanceZVector(0),
  fTrackVertexDistanceZErrorVector(0),
  fTrackVertexDistanceXYVector(0),
  fTrackVertexDistanceXYErrorVector(0)
{
  // Custom constructor
}

/*
 * Copy constructor
 */
HighForestReader::HighForestReader(const HighForestReader& in) :
  ForestReader(in),
  fHeavyIonTree(in.fHeavyIonTree),
  fSkimTree(in.fSkimTree),
  fTrackTree(in.fTrackTree),
  fnTracksBranch(in.fnTracksBranch),
  fTrackPtVector(in.fTrackPtVector),
  fTrackPtErrorVector(in.fTrackPtErrorVector),
  fTrackPhiVector(in.fTrackPhiVector),
  fTrackEtaVector(in.fTrackEtaVector),
  fHighPurityTrackVector(in.fHighPurityTrackVector),
  fTrackVertexDistanceZVector(in.fTrackVertexDistanceZVector),
  fTrackVertexDistanceZErrorVector(in.fTrackVertexDistanceZErrorVector),
  fTrackVertexDistanceXYVector(in.fTrackVertexDistanceXYVector),
  fTrackVertexDistanceXYErrorVector(in.fTrackVertexDistanceXYErrorVector)
{
  // Copy constructor
  for(Int_t i = 0; i < fnMaxTrack; i++){
    fTrackPtArray[i] = in.fTrackPtArray[i];
    fTrackPtErrorArray[i] = in.fTrackPtErrorArray[i];
    fTrackPhiArray[i] = in.fTrackPhiArray[i];
    fTrackEtaArray[i] = in.fTrackEtaArray[i];
    fHighPurityTrackArray[i] = in.fHighPurityTrackArray[i];
    fTrackVertexDistanceZArray[i] = in.fTrackVertexDistanceZArray[i];
    fTrackVertexDistanceZErrorArray[i] = in.fTrackVertexDistanceZErrorArray[i];
    fTrackVertexDistanceXYArray[i] = in.fTrackVertexDistanceXYArray[i];
    fTrackVertexDistanceXYErrorArray[i] = in.fTrackVertexDistanceXYErrorArray[i];
  }
}

/*
 * Assignment operator
 */
HighForestReader& HighForestReader::operator=(const HighForestReader& in){
  if (&in==this) return *this;
  ForestReader::operator=(in);
  
  fHeavyIonTree = in.fHeavyIonTree;
  fSkimTree = in.fSkimTree;
  fTrackTree = in.fTrackTree;
  fnTracksBranch = in.fnTracksBranch;
  
  for(Int_t i = 0; i < fnMaxTrack; i++){
    fTrackPtArray[i] = in.fTrackPtArray[i];
    fTrackPtErrorArray[i] = in.fTrackPtErrorArray[i];
    fTrackPhiArray[i] = in.fTrackPhiArray[i];
    fTrackEtaArray[i] = in.fTrackEtaArray[i];
    fHighPurityTrackArray[i] = in.fHighPurityTrackArray[i];
    fTrackVertexDistanceZArray[i] = in.fTrackVertexDistanceZArray[i];
    fTrackVertexDistanceZErrorArray[i] = in.fTrackVertexDistanceZErrorArray[i];
    fTrackVertexDistanceXYArray[i] = in.fTrackVertexDistanceXYArray[i];
    fTrackVertexDistanceXYErrorArray[i] = in.fTrackVertexDistanceXYErrorArray[i];
  }
  
  fTrackPtVector = in.fTrackPtVector;
  fTrackPtErrorVector = in.fTrackPtErrorVector;
  fTrackPhiVector = in.fTrackPhiVector;
  fTrackEtaVector = in.fTrackEtaVector;
  fHighPurityTrackVector = in.fHighPurityTrackVector;
  fTrackVertexDistanceZVector = in.fTrackVertexDistanceZVector;
  fTrackVertexDistanceZErrorVector = in.fTrackVertexDistanceZErrorVector;
  fTrackVertexDistanceXYVector = in.fTrackVertexDistanceXYVector;
  fTrackVertexDistanceXYErrorVector = in.fTrackVertexDistanceXYErrorVector;
  
  return *this;
}

/*
 * Destructor
 */
HighForestReader::~HighForestReader(){
}

/*
 * Initialization, meaning that the branches are connected to the tree
 */
void HighForestReader::Initialize(){

  // Connect the branches of the heavy ion tree
  fHeavyIonTree->SetBranchStatus("*", 0);
  fHeavyIonTree->SetBranchStatus("vz", 1);
  fHeavyIonTree->SetBranchAddress("vz", &fVertexZ, &fHiVzBranch);
  fHeavyIonTree->SetBranchStatus("evt", 1);
  fHeavyIonTree->SetBranchAddress("evt", &fEventNumber, &fEventNumberBranch);

  // Connect the branches to the skim tree (pO specific)
  fSkimTree->SetBranchStatus("*", 0);
  //fSkimTree->SetBranchStatus("pPAprimaryVertexFilter", 1);
  //fSkimTree->SetBranchAddress("pPAprimaryVertexFilter", &fPrimaryVertexFilterBit, &fPrimaryVertexBranch);
  //fSkimTree->SetBranchStatus("HBHENoiseFilterResultRun2Loose", 1);
  //fSkimTree->SetBranchAddress("HBHENoiseFilterResultRun2Loose", &fHBHENoiseFilterBit, &fHBHENoiseBranch);
  
  // Connect the branches to the track tree
  fTrackTree->SetBranchStatus("*", 0);
  
  if(fIsMiniAOD){
    fTrackTree->SetBranchStatus("trkPt", 1);
    fTrackTree->SetBranchAddress("trkPt", &fTrackPtVector, &fTrackPtBranch);
    fTrackTree->SetBranchStatus("trkPhi", 1);
    fTrackTree->SetBranchAddress("trkPhi", &fTrackPhiVector, &fTrackPhiBranch);
    fTrackTree->SetBranchStatus("trkEta", 1);
    fTrackTree->SetBranchAddress("trkEta", &fTrackEtaVector, &fTrackEtaBranch);
    fTrackTree->SetBranchStatus("nTrk", 1);
    fTrackTree->SetBranchAddress("nTrk", &fnTracks, &fnTracksBranch);
    fTrackTree->SetBranchStatus("trkPtError", 1);
    fTrackTree->SetBranchAddress("trkPtError", &fTrackPtErrorVector, &fTrackPtErrorBranch);
    fTrackTree->SetBranchStatus("highPurity", 1);
    fTrackTree->SetBranchAddress("highPurity", &fHighPurityTrackVector, &fHighPurityTrackBranch);
    fTrackTree->SetBranchStatus("trkDzFirstVtx", 1);
    fTrackTree->SetBranchAddress("trkDzFirstVtx", &fTrackVertexDistanceZVector, &fTrackVertexDistanceZBranch);
    fTrackTree->SetBranchStatus("trkDzErrFirstVtx", 1);
    fTrackTree->SetBranchAddress("trkDzErrFirstVtx", &fTrackVertexDistanceZErrorVector, &fTrackVertexDistanceZErrorBranch);
    fTrackTree->SetBranchStatus("trkDxyFirstVtx", 1);
    fTrackTree->SetBranchAddress("trkDxyFirstVtx", &fTrackVertexDistanceXYVector, &fTrackVertexDistanceXYBranch);
    fTrackTree->SetBranchStatus("trkDxyErrFirstVtx", 1);
    fTrackTree->SetBranchAddress("trkDxyErrFirstVtx", &fTrackVertexDistanceXYErrorVector, &fTrackVertexDistanceXYErrorBranch);
  } else { 
    // Standard AOD for pO
    fTrackTree->SetBranchStatus("trkPt", 1);
    fTrackTree->SetBranchAddress("trkPt", &fTrackPtArray, &fTrackPtBranch);
    fTrackTree->SetBranchStatus("trkPhi", 1);
    fTrackTree->SetBranchAddress("trkPhi", &fTrackPhiArray, &fTrackPhiBranch);
    fTrackTree->SetBranchStatus("trkEta", 1);
    fTrackTree->SetBranchAddress("trkEta", &fTrackEtaArray, &fTrackEtaBranch);
    fTrackTree->SetBranchStatus("nTrk", 1);
    fTrackTree->SetBranchAddress("nTrk", &fnTracks, &fnTracksBranch);
    fTrackTree->SetBranchStatus("trkPtError", 1);
    fTrackTree->SetBranchAddress("trkPtError", &fTrackPtErrorArray, &fTrackPtErrorBranch);
    fTrackTree->SetBranchStatus("highPurity", 1);
    fTrackTree->SetBranchAddress("highPurity", &fHighPurityTrackArray, &fHighPurityTrackBranch);
    fTrackTree->SetBranchStatus("trkDz1", 1);
    fTrackTree->SetBranchAddress("trkDz1", &fTrackVertexDistanceZArray, &fTrackVertexDistanceZBranch);
    fTrackTree->SetBranchStatus("trkDzError1", 1);
    fTrackTree->SetBranchAddress("trkDzError1", &fTrackVertexDistanceZErrorArray, &fTrackVertexDistanceZErrorBranch);
    fTrackTree->SetBranchStatus("trkDxy1", 1);
    fTrackTree->SetBranchAddress("trkDxy1", &fTrackVertexDistanceXYArray, &fTrackVertexDistanceXYBranch);
    fTrackTree->SetBranchStatus("trkDxyError1", 1);
    fTrackTree->SetBranchAddress("trkDxyError1", &fTrackVertexDistanceXYErrorArray, &fTrackVertexDistanceXYErrorBranch);
  }
  
  GetEvent(0);
}

/*
 * Connect a new tree to the reader
 */
void HighForestReader::ReadForestFromFile(TFile* inputFile){
  std::vector<TString> fileList;
  fileList.push_back(inputFile->GetName());
  ReadForestFromFileList(fileList);
}

/*
 * Connect a new tree to the reader
 */
void HighForestReader::ReadForestFromFileList(std::vector<TString> fileList){

  // Check if MiniAOD or AOD
  TFile* inputFile = TFile::Open(fileList.at(0));
  TTree* miniAODcheck = (TTree*)inputFile->Get("ppTracks/trackTree");
  fIsMiniAOD = !(miniAODcheck == NULL);
  inputFile->Close();

  // Trees para pO
  fHeavyIonTree = new TChain("hiEvtAnalyzer/HiTree");
  fSkimTree = new TChain("skimanalysis/HltTree"); 

  if(fIsMiniAOD){
    fTrackTree = new TChain("ppTracks/trackTree");
  } else {
    fTrackTree = new TChain("ppTrack/trackTree"); // Típicamente usado para pO/pPb en CMS
  }

  for(std::vector<TString>::iterator listIterator = fileList.begin(); listIterator != fileList.end(); listIterator++){
    fHeavyIonTree->Add(*listIterator);
    fSkimTree->Add(*listIterator);
    fTrackTree->Add(*listIterator);
  }
  
  Initialize();
}

/*
 * Burn the current forest.
 */
void HighForestReader::BurnForest(){
  fHeavyIonTree->Delete();
  fSkimTree->Delete();
  fTrackTree->Delete();
}

/*
 * Check if there are problems in the file list
 */
bool HighForestReader::CheckFileProblems(){
  TObjArray *fileElements = fHeavyIonTree->GetListOfFiles();
  for (TObject *op: *fileElements) {
    auto chainElement = static_cast<TChainElement *>(op);
    TFile *testFile = TFile::Open(chainElement->GetTitle());
    if(!testFile->IsOpen() || testFile->IsZombie()) return true;
    testFile->Close();
  }
  return false;
}

/*
 * Load an event to memory
 */
void HighForestReader::GetEvent(Int_t nEvent){
  fHeavyIonTree->GetEntry(nEvent);
  fSkimTree->GetEntry(nEvent);
  fTrackTree->GetEntry(nEvent);
  if(fIsMiniAOD){
    fnTracks = fTrackPtVector->size();
  }
}

// -------------------------------------------------------------
// GETTERS (Solo los necesarios para trazas)
// -------------------------------------------------------------

Float_t HighForestReader::GetTrackPt(Int_t iTrack) const{
  if(fIsMiniAOD) return fTrackPtVector->at(iTrack);
  return fTrackPtArray[iTrack];
}

Float_t HighForestReader::GetTrackPtError(Int_t iTrack) const{
  if(fIsMiniAOD) return fTrackPtErrorVector->at(iTrack);
  return fTrackPtErrorArray[iTrack];
}

Float_t HighForestReader::GetTrackPhi(Int_t iTrack) const{
  if(fIsMiniAOD) return fTrackPhiVector->at(iTrack);
  return fTrackPhiArray[iTrack];
}

Float_t HighForestReader::GetTrackEta(Int_t iTrack) const{
  if(fIsMiniAOD) return fTrackEtaVector->at(iTrack);
  return fTrackEtaArray[iTrack];
}

Bool_t HighForestReader::GetTrackHighPurity(Int_t iTrack) const{
  if(fIsMiniAOD) return fHighPurityTrackVector->at(iTrack);
  return fHighPurityTrackArray[iTrack];
}

Float_t HighForestReader::GetTrackVertexDistanceZ(Int_t iTrack) const{
  if(fIsMiniAOD) return fTrackVertexDistanceZVector->at(iTrack);
  return fTrackVertexDistanceZArray[iTrack];
}

Float_t HighForestReader::GetTrackVertexDistanceZError(Int_t iTrack) const{
  if(fIsMiniAOD) return fTrackVertexDistanceZErrorVector->at(iTrack);
  return fTrackVertexDistanceZErrorArray[iTrack];
}

Float_t HighForestReader::GetTrackVertexDistanceXY(Int_t iTrack) const{
  if(fIsMiniAOD) return fTrackVertexDistanceXYVector->at(iTrack);
  return fTrackVertexDistanceXYArray[iTrack];
}

Float_t HighForestReader::GetTrackVertexDistanceXYError(Int_t iTrack) const{
  if(fIsMiniAOD) return fTrackVertexDistanceXYErrorVector->at(iTrack);
  return fTrackVertexDistanceXYErrorArray[iTrack];
}