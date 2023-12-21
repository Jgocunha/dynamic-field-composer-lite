#include "./wizards/learning_wizard.h"


namespace dnf_composer
{
    LearningWizard::LearningWizard(const std::shared_ptr<Simulation>& simulation, const std::string& fieldCouplingName, bool activateUI)
	    : simulation(simulation)
    {
		elements.fieldCoupling = std::dynamic_pointer_cast<element::FieldCoupling>(simulation->getElement(fieldCouplingName));
		log(LogLevel::INFO, "Learning wizard has found the target field coupling to train '" + fieldCouplingName + "'.\n");
		findRelevantElements(fieldCouplingName);

    }

	void LearningWizard::findRelevantElements(const std::string& fieldCouplingName)
    {
    	findInputField();
		findTargetField();
	}

	void LearningWizard::findInputField()
	{
		elements.inputField = std::dynamic_pointer_cast<element::NeuralField>(elements.fieldCoupling->getInputs().at(0));
		log(LogLevel::INFO, "Learning wizard has found the target field '" + elements.inputField->getUniqueName() + "'.\n");
	}

	void LearningWizard::findTargetField()
    {
		const std::shared_ptr<Simulation> simulation = application.getSimulation();
		elements.targetField = std::dynamic_pointer_cast<element::NeuralField>(simulation->getElementsThatHaveSpecifiedElementAsInput(elements.fieldCoupling->getUniqueName()).at(0));
		log(LogLevel::INFO, "Learning wizard has found the target field '" + elements.targetField->getUniqueName() + "'.\n");
	}

	void LearningWizard::setPeakLocationsForInputField(const std::vector<std::vector<double>>& peakLocations)
	{
		peakLocationsForInputField = peakLocations;
	}

	void LearningWizard::setPeakLocationsForTargetField(const std::vector<std::vector<double>>& peakLocations)
	{
		peakLocationsForOutputField = peakLocations;
	}

	void LearningWizard::run(int iterations)
	{
		simulateAssociation();
		trainWeights(iterations);
	}

	void LearningWizard::simulateAssociation()
	{
		log(INFO, "Started creating association between fields '" + elements.inputField->getUniqueName() +"' and '" + elements.targetField->getUniqueName() + "'");

		if(peakLocationsForInputField.size() != peakLocationsForTargetField.size())
		{
			log(WARNING, "The target peak locations for the input and target field are mismatched. \n");
			return;
		}
			
    	const size_t numberOfPeakLocations = peakLocationsForInputField.size();
		for (size_t i = 0; i < numberOfPeakLocations; i++)
		{
			for (size_t j = 0; j < numberOfPeakLocations; j++)
			{
				const std::string stimulusName = "Input Gaussian Stimulus " + std::to_string(i + 1) + std::to_string(j + 1);
				element::ElementSpatialDimensionParameters stimulusDimensions{ elements.inputField->getMaxSpatialDimension(), elements.inputField->getStepSize() };
				gsp.position = peakLocationsForInputField[i][j];
				std::shared_ptr<element::GaussStimulus> stimulus = std::make_shared<element::GaussStimulus>({stimulusName, stimulusDimensions}, gsp);

				simulation->addElement(stimulus);
				elements.inputField->addInput(stimulus);

				simulation->init();
				//elements.fieldCoupling->resetWeights();
				for (int k = 0; k < 100; k++)
					simulation->step();
			}

			for (size_t j = 0; j < numberOfPeakLocations; j++)
			{
				const std::string stimulusName = "Target Gaussian Stimulus " + std::to_string(i + 1) + std::to_string(j + 1);
				element::ElementSpatialDimensionParameters stimulusDimensions{ elements.targetField->getMaxSpatialDimension(), elements.targetField->getStepSize() };
				gsp.position = peakLocationsForTargetField[i][j];
				std::shared_ptr<element::GaussStimulus> stimulus = std::make_shared<element::GaussStimulus>({ stimulusName, stimulusDimensions }, gsp);

				simulation->addElement(stimulus);
				elements.targetField->addInput(stimulus);

				simulation->init();
				//elements.fieldCoupling->resetWeights();
				for (int k = 0; k < 100; k++)
					simulation->step();
			}

		}
	}

	void LearningWizard::trainWeights(int iterations)
	{
		log(INFO, "Started training the weights for field coupling: '" + elements.fieldCoupling->getUniqueName() + "'");

	}



}
