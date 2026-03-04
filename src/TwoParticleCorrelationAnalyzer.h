#ifndef TWOPARTICLECORRELATIONANALYZER_H
#define TWOPARTICLECORRELATIONANALYZER_H

#include <iostream>
#include <vector>
#include <deque>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TMath.h"

#include "ConfigurationCard.h"
#include "HighForestReader.h"

// Estructura simplificada sin pesos (weight = 1 para todas las trazas)
struct EventTracksNode {
    std::vector<double> phi;
    std::vector<double> eta;
    std::vector<double> pt;
};

class TwoParticleCorrelationAnalyzer {

private:
    ConfigurationCard *config;
    HighForestReader *forestReader;

    std::deque<EventTracksNode> mixingBuffer;
    int mixingDepth;

    TFile *outputFile;
    std::string outputFileName;

    // --- Variables de configuración de la tarjeta ---
    double minPtCut;
    double maxPtCut;
    double etaCut;
    double maxPtRelError;
    double vertexMaxDist;
    int requireHighPurity;
    // ------------------------------------------------

    const double PI = TMath::Pi();
    const double DphiMax = PI * 3.0/2.0;
    const double DphiMin = -PI / 2.0;

    // Histogramas
    TH1D *hist_multiplicity;
    TH1D *NTrigger_Signal;
    TH1D *NTrigger_Background;
    
    TH2D *hc; // Signal
    TH2D *hm; // Background

    TH1D *trk_eta;
    TH1D *trk_pT;

public:
    TwoParticleCorrelationAnalyzer(ConfigurationCard *newConfig, std::string inputFile);
    ~TwoParticleCorrelationAnalyzer();

    void InitializeHistograms();
    void DoAnalysis();
    void WriteHistograms();
};

#endif