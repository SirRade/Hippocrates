#pragma once
#include "neural_network.h"
#include "body.h"
#include "training_parameters.h"
#include "genome.h"
#include "organism.h"
#include "species.h"
#include "trained_neural_network.h"
#include "logger.h"
#include <vector>
#include <memory>

namespace JNF_NEAT {

class NeuralNetworkTrainer {
public:
	bool loggingEnabled = false;

private:
	std::vector<Species> species;
	TrainingParameters parameters;
	size_t populationSize = 0U;
	std::vector<std::shared_ptr<IBody>> bodies;
	size_t generationsPassed = 0;
	Logger logger;

public:
	NeuralNetworkTrainer(std::vector<std::shared_ptr<IBody>> population, TrainingParameters parameters);
	NeuralNetworkTrainer(const NeuralNetworkTrainer& other) = default;
	NeuralNetworkTrainer(NeuralNetworkTrainer&& other) = default;
	~NeuralNetworkTrainer() = default;

	auto operator=(const NeuralNetworkTrainer&) -> NeuralNetworkTrainer& = default;
	auto operator=(NeuralNetworkTrainer&&) -> NeuralNetworkTrainer& = default;

	auto TrainUntilFitnessEquals(double fitnessToReach) -> void;
	auto TrainUntilGenerationEquals(size_t generationsToTrain) -> void;
	auto GetTrainedNeuralNetwork() -> TrainedNeuralNetwork;
	auto GetJSON() const->std::string;

private:
	auto CreateInitialOrganisms() -> void;
	auto TrainGenerationAndLog() -> void;

	auto ResetPopulationToTeachableState() -> void;
	auto Repopulate() -> void;
	auto LetGenerationLive() -> void;
	auto PrepareSpeciesForPopulation() -> void;
	auto FillOrganismIntoSpecies(Organism&& organism) -> void;
	auto AnalyzeAndClearSpeciesPopulation() -> void;
	auto DeleteStagnantSpecies() -> void;
	auto DeleteEmptySpecies() -> void;

	auto SelectSpeciesToBreed() -> Species&;
	auto GetFittestOrganism() -> Organism&;
};
}
