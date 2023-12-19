#pragma once

#include "exceptions/exception.h"
#include "simulation/simulation.h"
#include "simulation/visualization.h"
#include "user_interface/user_interface.h"

namespace dnf_composer
{
	class Application
	{
	private:
		std::shared_ptr<Simulation> simulation;
		std::shared_ptr<user_interface::UserInterface> userInterface;
		bool activateUserInterface;
	public:
		Application(const std::shared_ptr<Simulation>& simulation, bool activateUserInterface = true);
		Application(const Application&) = delete;             
		Application& operator=(const Application&) = delete;  
		Application(Application&&) = delete;                  
		Application& operator=(Application&&) = delete;       

		void init() const;
		void step() const;
		void close() const;

		void activateUserInterfaceWindow(const std::shared_ptr<user_interface::UserInterfaceWindow>& window) const;
		void setActivateUserInterfaceAs(bool activateUI);

		bool getCloseUI() const;
		bool getActivateUserInterface() const;
		std::shared_ptr<Simulation> getSimulation() const;

		~Application() = default;
	};
}

