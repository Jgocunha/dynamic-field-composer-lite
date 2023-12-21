// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "dynamic-neural-field-composer.h"

// This .cpp file is an example of how you can use the library to create your own DNF simulation.
// This setup runs the application with a GUI.
// This .exe creates a simulation object programatically defining its elements.
// This .exe creates a neural architecture with two coupled neural fields.

using namespace dnf_composer;

std::shared_ptr<Simulation> getSimulation()
{
	auto simulation = std::make_shared<Simulation>(5, 0, 0);

	const element::ElementSpatialDimensionParameters spatialDimensions{ 100, 1 };

	const element::HeavisideFunction activationFunction{ 0.2 };
	const element::NeuralFieldParameters nfp{ 25, -10, activationFunction };
	const std::shared_ptr<element::NeuralField> nf1 (new element::NeuralField({"input field", spatialDimensions}, nfp));
	const std::shared_ptr<element::NeuralField> nf2 (new element::NeuralField({ "target field", spatialDimensions }, nfp));
	simulation->addElement(nf1);
	simulation->addElement(nf2);

	constexpr element::GaussKernelParameters gkp{5, 20, 0};
	const std::shared_ptr<element::GaussKernel> gk1 (new element::GaussKernel({"input field kernel", spatialDimensions}, gkp));
	simulation->addElement(gk1);

	const std::shared_ptr<element::GaussKernel> gk2(new element::GaussKernel({ "target field kernel", spatialDimensions }, gkp));
	simulation->addElement(gk2);

	simulation->createInteraction("input field kernel", "output", "input field");
	simulation->createInteraction("target field kernel", "output", "target field");
	simulation->createInteraction("input field", "output", "input field kernel");
	simulation->createInteraction("target field", "output", "target field kernel");

	constexpr element::GaussStimulusParameters gsp{ 5, 11, 22 };
	const std::shared_ptr<element::GaussStimulus> gs1(new element::GaussStimulus({"gauss stimulus input field", spatialDimensions}, gsp));
	simulation->addElement(gs1);

	simulation->createInteraction("gauss stimulus input field", "output", "input field");

	const element::FieldCouplingParameters fcp{ spatialDimensions.size, 0.1, {0.1, LearningRule::DELTA_WIDROW_HOFF} };
	const std::shared_ptr<element::FieldCoupling> fc(new element::FieldCoupling({"input - target", spatialDimensions}, fcp));
	simulation->addElement(fc);

	simulation->createInteraction("input field", "output", "input - target");
	simulation->createInteraction("input - target", "output", "target field");

	return simulation;
}

int main(int argc, char* argv[])
{
	try
	{
		// After defining the simulation, we can create the application.
		auto simulation = getSimulation();
		// You can run the application without the user interface by setting the second parameter to false.
		constexpr bool activateUserInterface = true;
		const Application app{ simulation, activateUserInterface };

		// After creating the application, we can add the windows we want to display.
		app.activateUserInterfaceWindow(std::make_shared<user_interface::SimulationWindow>(simulation));
		app.activateUserInterfaceWindow(std::make_shared<user_interface::LoggerWindow>());

		user_interface::PlotParameters plotParameters;
		plotParameters.annotations = { "Neural field activation of two coupled fields", "Spatial dimension", "Activation" };
		plotParameters.dimensions = { 0, 100, -30, 40 };
		const std::shared_ptr<Visualization> visualization = std::make_shared<Visualization>(simulation);
		visualization->addPlottingData("input field", "activation");
		visualization->addPlottingData("target field", "activation");
		app.activateUserInterfaceWindow(std::make_shared<user_interface::PlotWindow>(visualization, plotParameters));

		app.init();

		bool userRequestClose = false;
		while (!userRequestClose)
		{
			app.step();
			userRequestClose = app.getCloseUI();
		}
		app.close();
		return 0;
	}
	catch (const Exception& ex)
	{
		const std::string errorMessage = "Exception: " + std::string(ex.what()) + " ErrorCode: " + std::to_string(static_cast<int>(ex.getErrorCode())) + ". \n";
		log(LogLevel::FATAL, errorMessage, LogOutputMode::CONSOLE);
		return static_cast<int>(ex.getErrorCode());
	}
	catch (const std::exception& ex)
	{
		log(LogLevel::FATAL, "Exception caught: " + std::string(ex.what()) + ". \n", LogOutputMode::CONSOLE);
		return 1;
	}
	catch (...)
	{
		log(LogLevel::FATAL, "Unknown exception occurred. \n", LogOutputMode::CONSOLE);
		return 1;
	}
}