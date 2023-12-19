// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "application/application.h"

namespace dnf_composer
{
	Application::Application(const std::shared_ptr<Simulation>& simulation, bool activateUserInterface)
		:simulation(simulation), activateUserInterface(activateUserInterface)
	{
		if (activateUserInterface)
			userInterface = std::make_shared<user_interface::UserInterface>();
	}


	void Application::init() const
	{
		simulation->init();
		if (activateUserInterface)
			userInterface->init();
	}

	void Application::step() const
	{
		simulation->step();
		if (activateUserInterface)
			userInterface->step();
	}

	void Application::close() const
	{
		simulation->close();
		if (activateUserInterface)
			userInterface->close();
	}

	void Application::setActivateUserInterfaceAs(bool activateUI)
	{
		activateUserInterface = activateUI;
		if (activateUserInterface)
			userInterface = std::make_shared<user_interface::UserInterface>();
	}

	void Application::activateUserInterfaceWindow(const std::shared_ptr<user_interface::UserInterfaceWindow>& window) const
	{
		if (activateUserInterface)
			userInterface->activateWindow(window);
	}

	bool Application::getCloseUI() const
	{
		if (activateUserInterface)
			return userInterface->getCloseUI();
		return false;
	}

	bool Application::getActivateUserInterface() const
	{
		return activateUserInterface;
	}

	std::shared_ptr<Simulation> Application::getSimulation() const
	{
		return simulation;
	}
}
