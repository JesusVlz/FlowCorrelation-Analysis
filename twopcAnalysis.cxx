#include "src/ConfigurationCard.h"
#include "src/TwoParticleCorrelationAnalyzer.h"
#include <iostream>

int main(int argc, char *argv[]) {
    // Requerimos la tarjeta Y el archivo ROOT
    if (argc != 3) {
        std::cout << "Error: Faltan argumentos." << std::endl;
        std::cout << "Uso: ./twopcAnalysis <configurationCard.input> <input_forest.root>" << std::endl;
        return 1;
    }

    std::cout << "========================================" << std::endl;
    std::cout << "  Starting 2PC Analysis for pO collisions " << std::endl;
    std::cout << "========================================" << std::endl;

    ConfigurationCard *config = new ConfigurationCard(argv[1]);
    std::string inputFile = argv[2]; // Capturamos el nombre del archivo
    
    // Le pasamos el nombre del archivo al inicializar
    TwoParticleCorrelationAnalyzer *analyzer = new TwoParticleCorrelationAnalyzer(config, inputFile);
    
    analyzer->DoAnalysis();
    analyzer->WriteHistograms();

    delete analyzer;
    delete config;
    
    std::cout << "Analysis completed successfully!" << std::endl;
    return 0;
}