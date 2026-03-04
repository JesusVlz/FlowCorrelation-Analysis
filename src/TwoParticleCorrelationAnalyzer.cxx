#include "TwoParticleCorrelationAnalyzer.h"
#include <fstream> // Necesario para leer archivos .txt

TwoParticleCorrelationAnalyzer::TwoParticleCorrelationAnalyzer(ConfigurationCard *newConfig, std::string inputFile) {
    config = newConfig;
    
    outputFileName    = config->GetStr("OutputFileName");
    mixingDepth       = config->Get("MixingDepth");
    
    minPtCut          = config->Get("MinTrackPtCut");
    maxPtCut          = config->Get("MaxTrackPtCut");
    etaCut            = config->Get("TrackEtaCut");
    maxPtRelError     = config->Get("MaxTrackPtRelativeError");
    vertexMaxDist     = config->Get("VertexMaxDistance");
    requireHighPurity = config->Get("HighPurity");

    outputFile = new TFile(outputFileName.c_str(), "RECREATE");

    int dataType = config->Get("DataType", 0);
    forestReader = new HighForestReader(dataType, 0, 0, 0, 0, true, false, false);

    // ---> NUEVO: Cargar los archivos al ForestReader <---
    std::vector<TString> fileList;
    if (inputFile.find(".root") != std::string::npos) {
        fileList.push_back(inputFile);
    } else if (inputFile.find(".txt") != std::string::npos) {
        std::ifstream fileStream(inputFile);
        std::string line;
        while (std::getline(fileStream, line)) {
            if(!line.empty()) fileList.push_back(line);
        }
    } else {
        std::cerr << "Error: Formato de archivo no reconocido (.root o .txt)" << std::endl;
    }
    
    // Conectar los árboles de ROOT
    if(!fileList.empty()) {
        forestReader->ReadForestFromFileList(fileList);
    }

    InitializeHistograms();
}

TwoParticleCorrelationAnalyzer::~TwoParticleCorrelationAnalyzer() {
    delete forestReader;
}

void TwoParticleCorrelationAnalyzer::InitializeHistograms() {
    outputFile->cd();

    int DPhiBins = 32;
    int DEtaBins = 32;

    hist_multiplicity = new TH1D("hist_multiplicity", "Track Multiplicity; N_{trk}; Events", 500, 0, 500);
    NTrigger_Signal     = new TH1D("NTrigger_Signal", "NTrigger_Signal", 1, 0, 2);
    NTrigger_Background = new TH1D("NTrigger_Background", "NTrigger_Background", 1, 0, 2);

    hc = new TH2D("hc", "Signal Distribution; #Delta#phi; #Delta#eta", DPhiBins, DphiMin, DphiMax, DEtaBins, -4.8, 4.8);
    hm = new TH2D("hm", "Mixed Background; #Delta#phi; #Delta#eta", DPhiBins, DphiMin, DphiMax, DEtaBins, -4.8, 4.8);
    
    hc->Sumw2();
    hm->Sumw2();

    // Ampliamos un poco los rangos de control
    trk_eta = new TH1D("trk_eta", "Track Eta; #eta; Counts", 50, -3.0, 3.0);
    trk_pT  = new TH1D("trk_pT", "Track pT; p_{T} [GeV/c]; Counts", 100, 0, 10);
}

void TwoParticleCorrelationAnalyzer::DoAnalysis() {
    int numberOfEvents = forestReader->GetNEvents();
    std::cout << "Starting analysis over " << numberOfEvents << " events..." << std::endl;
    
    double zVtxCut = config->Get("ZVertexCut");

    for (int iEvent = 0; iEvent < numberOfEvents; ++iEvent) {
        forestReader->GetEvent(iEvent);

        if (iEvent % 1000 == 0) std::cout << "Processing event: " << iEvent << std::endl;

        // 1. Cortes de Evento
        // Se usa GetVz() que proviene de la clase base ForestReader
        if (fabs(forestReader->GetVz()) > zVtxCut) continue; 

        EventTracksNode currentEventTracks;

        // 2. Loop de Trazas
        int nTracksInEvent = forestReader->GetNTracks();
        
        for (int iTrk = 0; iTrk < nTracksInEvent; ++iTrk) {
            
            // Usamos los Getters oficiales de HighForestReader
            double pt  = forestReader->GetTrackPt(iTrk);
            double eta = forestReader->GetTrackEta(iTrk);
            double phi = forestReader->GetTrackPhi(iTrk);

            // Cortes cinemáticos leídos de la tarjeta
            if (pt < minPtCut || pt > maxPtCut) continue;
            if (fabs(eta) > etaCut) continue; 

            // Cortes de calidad de traza
            if (requireHighPurity == 1 && !forestReader->GetTrackHighPurity(iTrk)) continue;
            
            double dz = forestReader->GetTrackVertexDistanceZ(iTrk);
            double dzErr = forestReader->GetTrackVertexDistanceZError(iTrk);
            if (dzErr > 0 && fabs(dz / dzErr) >= vertexMaxDist) continue;
            
            double dxy = forestReader->GetTrackVertexDistanceXY(iTrk);
            double dxyErr = forestReader->GetTrackVertexDistanceXYError(iTrk);
            if (dxyErr > 0 && fabs(dxy / dxyErr) >= vertexMaxDist) continue;
            
            double ptErr = forestReader->GetTrackPtError(iTrk);
            if ((ptErr / pt) >= maxPtRelError) continue;

            // Guardar traza que pasó los cortes
            currentEventTracks.phi.push_back(phi);
            currentEventTracks.eta.push_back(eta);
            currentEventTracks.pt.push_back(pt);

            trk_eta->Fill(eta);
            trk_pT->Fill(pt);
        }

        int nTrkSelected = currentEventTracks.phi.size();
        
        if (nTrkSelected < 2) continue; 
        hist_multiplicity->Fill(nTrkSelected);

        double nTrkWeight = (double)nTrkSelected;

        // 3. Signal (Same Event)
        for (int i = 0; i < nTrkSelected; ++i){
            NTrigger_Signal->Fill(1);

            for (int j = 0; j < nTrkSelected; ++j) {
                if (i == j) continue;

                double dPhi = currentEventTracks.phi[i] - currentEventTracks.phi[j];
                double dEta = currentEventTracks.eta[i] - currentEventTracks.eta[j];

                while (dPhi > DphiMax) dPhi -= 2.0 * PI;
                while (dPhi < DphiMin) dPhi += 2.0 * PI;

                hc->Fill(dPhi, dEta, 1.0 / nTrkWeight);
            }

            // 4. Background (Mixed Events)
            for (size_t mixIdx = 0; mixIdx < mixingBuffer.size(); ++mixIdx) {
                NTrigger_Background->Fill(1);
                
                for (size_t j = 0; j < mixingBuffer[mixIdx].phi.size(); ++j) {
                    double dPhi = currentEventTracks.phi[i] - mixingBuffer[mixIdx].phi[j];
                    double dEta = currentEventTracks.eta[i] - mixingBuffer[mixIdx].eta[j];

                    while (dPhi > DphiMax) dPhi -= 2.0 * PI;
                    while (dPhi < DphiMin) dPhi += 2.0 * PI;

                    hm->Fill(dPhi, dEta, 1.0 / nTrkWeight);
                }
            }
        }

        // 5. Actualizar Buffer
        mixingBuffer.push_front(currentEventTracks);
        if (mixingBuffer.size() > (size_t)mixingDepth) {
            mixingBuffer.pop_back();
        }
    }
}

void TwoParticleCorrelationAnalyzer::WriteHistograms() {
    outputFile->cd();
    hist_multiplicity->Write();
    NTrigger_Signal->Write();
    NTrigger_Background->Write();
    hc->Write();
    hm->Write();
    trk_eta->Write();
    trk_pT->Write();
    outputFile->Close();
}