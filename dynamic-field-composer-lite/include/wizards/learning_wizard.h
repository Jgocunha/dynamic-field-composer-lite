#pragma once

#include "./application/application.h"
#include "./simulation/simulation.h"
#include "./elements/neural_field.h"
#include "./elements/field_coupling.h"
#include "./elements/gauss_stimulus.h"
#include "utilities/utilities.h"


namespace dnf_composer
{
	struct RelevantSimulationElements
	{
		std::shared_ptr<element::NeuralField> inputField;
		std::shared_ptr<element::NeuralField> targetField;
		std::shared_ptr<element::FieldCoupling> fieldCoupling;
	};

	class LearningWizard
	{
	private:
		std::shared_ptr<Simulation> simulation;
		RelevantSimulationElements elements;
		std::vector<std::vector<double>> peakLocationsForInputField;
		std::vector<std::vector<double>> peakLocationsForTargetField;
		element::GaussStimulusParameters gsp{ 5, 15, 0 };
	public:
		LearningWizard(const std::shared_ptr<Simulation>& simulation, const std::string& fieldCouplingName, bool activateUI = true);

		void run(int iterations);


		void setPeakLocationsForInputField(const std::vector<std::vector<double>>& peakLocations);
		void setPeakLocationsForTargetField(const std::vector<std::vector<double>>& peakLocations);


	private:
		void simulateAssociation();
		void trainWeights(int iterations);

		void findRelevantElements(const std::string& fieldCouplingName);
		void findInputField();
		void findTargetField();
	};
}
