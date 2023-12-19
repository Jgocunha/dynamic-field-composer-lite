#include "./wizards/learning_wizard.h"


namespace dnf_composer
{
    LearningWizard::LearningWizard(const std::shared_ptr<Simulation>& simulation, const std::string& fieldCouplingName, bool activateUI)
	    : application(simulation, activateUI)
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


	void LearningWizard::run()
    {
		applicationThread = std::thread([this]() { runApplication(); });
    }

	int LearningWizard::runLearning() const
    {
	    return 0;
    }

	int LearningWizard::runApplication() const
    {
		try
		{
			elements.fieldCoupling->resetWeights();
			application.init();

			bool userRequestClose = false;
			while (!userRequestClose)
			{
				application.step();
				userRequestClose = application.getCloseUI();
			}
			application.close();
			return 0;
		}
		catch (const dnf_composer::Exception& ex)
		{
			const std::string errorMessage = "Exception: " + std::string(ex.what()) + " ErrorCode: " + std::to_string(static_cast<int>(ex.getErrorCode())) + ". \n";
			log(dnf_composer::LogLevel::FATAL, errorMessage, dnf_composer::LogOutputMode::CONSOLE);
			return static_cast<int>(ex.getErrorCode());
		}
		catch (const std::exception& ex)
		{
			log(dnf_composer::LogLevel::FATAL, "Exception caught: " + std::string(ex.what()) + ". \n", dnf_composer::LogOutputMode::CONSOLE);
			return 1;
		}
		catch (...)
		{
			log(dnf_composer::LogLevel::FATAL, "Unknown exception occurred. \n", dnf_composer::LogOutputMode::CONSOLE);
			return 1;
		}
	}
}
