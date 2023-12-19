#pragma once

#include <thread>

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
		Application application;
		RelevantSimulationElements elements;
		std::thread applicationThread;
	public:
		LearningWizard(const std::shared_ptr<Simulation>& simulation, const std::string& fieldCouplingName, bool activateUI = true);
		void run();
	private:
		int runApplication() const;
		int runLearning() const;
		void findRelevantElements(const std::string& fieldCouplingName);
		void findInputField();
		void findTargetField();
	};
}
