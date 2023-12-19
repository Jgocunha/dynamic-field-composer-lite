// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "simulation/simulation.h"


namespace dnf_composer
{
	Simulation::Simulation(double deltaT, double tZero, double t)
		: deltaT(deltaT), tZero(tZero), t(t)
	{
		if (deltaT <= 0 || tZero > t)
			throw Exception(ErrorCode::SIM_INVALID_PARAMETER, "Invalid parameters for Simulation constructor");

		initialized = false;
		paused = false;
		elements = {};
	}

	void Simulation::init()
	{
		paused = false;
		t = tZero;
		for (const auto& element : elements)
			element->init();

		initialized = true;
		log(LogLevel::INFO, "Simulation initialized.\n");
	}

	void Simulation::step()
	{
		if (paused)
			return;
		t += deltaT;
		for (const auto& element : elements)
			element->step(t, deltaT);
	}

	void Simulation::close()
	{
		for (const auto& element : elements)
			element->close();
		
		initialized = false;
		log(LogLevel::INFO, "Simulation closed.\n");
	}

	void Simulation::pause()
	{
		paused = true;
		log(LogLevel::INFO, "Simulation paused.\n");
	}

	void Simulation::resume()
	{
		paused = false;
		log(LogLevel::INFO, "Simulation resumed.\n");
	}

	void Simulation::run(double runTime)
	{
		if (runTime <= 0)
			throw Exception(ErrorCode::SIM_RUNTIME_LESS_THAN_ZERO, static_cast<int>(runTime));

		const double simTime = t + runTime;

		if (!initialized)
			init();

		while (t < simTime)
			step();

		close();
	}

	void Simulation::addElement(const std::shared_ptr<element::Element>& element)
	{
		// Check if an element with the same id already exists
		const std::string newElementName = element->getUniqueName();
		for (const auto& existingElement : elements) {
			if (existingElement->getUniqueName() == newElementName) 
			{
				const std::string logMessage = "An element with the same unique name already exists '" + newElementName + "'! New element was not added.\n";
				log(LogLevel::WARNING, logMessage);
				return;
			}
		}

		elements.push_back(element);
		//element->init(); 

		const std::string logMessage = "Element '" + newElementName + "' was added to the simulation.\n";
		log(LogLevel::INFO, logMessage);
	}

	void Simulation::removeElement(const std::string& elementId)
	{
		for (const auto& element : elements)
			element->removeInput(elementId);

		for (int i = 0; i < static_cast<int>(elements.size()); i++)
		{
			if (elements[i]->getUniqueName() == elementId)
			{
				elements.erase(elements.begin() + i);
				const std::string logMessage = "Element '" + elementId + "' was removed from the simulation.\n";
				log(LogLevel::INFO, logMessage);
				return;
			}
		}
		const std::string logMessage = "Element '" + elementId + "' was not found and consequently not removed from the simulation.\n";
		log(LogLevel::FATAL, logMessage);
	}

	void Simulation::resetElement(const std::string& idOfElementToReset, const std::shared_ptr<element::Element>& newElement)
	{
		bool elementFound = false;

		for (auto& element : elements) 
		{
			if (element->getUniqueName() == idOfElementToReset) 
			{
				element = newElement;
				element->init();
				const std::string logMessage = "Element '" + idOfElementToReset + "' was reset in the simulation.\n";
				log(LogLevel::INFO, logMessage);
				elementFound = true;
				break;
			}
		}

		if (!elementFound)
		{
			const std::string logMessage = "Element '" + idOfElementToReset + "' was not found and consequently not reset.\n";
			log(LogLevel::FATAL, logMessage);
		}
	}

	void Simulation::createInteraction(const std::string& stimulusElementId, 
		const std::string& stimulusComponent, const std::string& receivingElementId) const
	{
		const std::shared_ptr<element::Element> stimulusElement = getElement(stimulusElementId);
		const std::shared_ptr<element::Element> receivingElement = getElement(receivingElementId);

		if (!stimulusElement)
		{
			const std::string logMessage = "Element '" + stimulusElementId + "' was not found and consequently no interaction was created.\n";
			log(LogLevel::FATAL, logMessage);
			return;
			//throw Exception(ErrorCode::SIM_ELEM_NOT_FOUND, stimulusElementId);
		}

		if (!receivingElement)
		{
			const std::string logMessage = "Element '" + receivingElementId + "' was not found and consequently no interaction was created.\n";
			log(LogLevel::FATAL, logMessage);
			return;
			//throw Exception(ErrorCode::SIM_ELEM_NOT_FOUND, receivingElementId);
		}

		receivingElement->addInput(stimulusElement, stimulusComponent);

		const std::string logMessage = "Interaction created: " + stimulusElementId + " -> " + receivingElementId + '\n';
		log(LogLevel::INFO, logMessage);

	}

	std::shared_ptr<element::Element> Simulation::getElement(const std::string& id) const
	{
		for (const auto& element : elements)
			if (element->getUniqueName() == id)
				return element;

		throw Exception(ErrorCode::SIM_ELEM_NOT_FOUND, id);
	}

	std::shared_ptr<element::Element> Simulation::getElement(const int index) const 
	{
		if (index < static_cast<int>(elements.size()))
			return elements[index];

		throw Exception(ErrorCode::SIM_ELEM_INDEX, index);
	}

	std::vector<double> Simulation::getComponent(const std::string& id, const std::string& componentName) const
	{
		const std::shared_ptr<element::Element> foundElement = getElement(id);
		return foundElement->getComponent(componentName);
	}

	std::vector<double>* Simulation::getComponentPtr(const std::string& id, const std::string& componentName) const
	{
		const std::shared_ptr<element::Element> foundElement = getElement(id);
		return foundElement->getComponentPtr(componentName);
	}

	int Simulation::getNumberOfElements() const
	{
		return static_cast<int>(elements.size());
	}

	std::vector<std::shared_ptr<element::Element>> Simulation::getElementsThatHaveSpecifiedElementAsInput(const std::string& specifiedElement, const std::string& inputComponent) const
	{
		std::vector<std::shared_ptr<element::Element>> elementsThatHaveSpecifiedElementAsInput;
		for (const auto& element : elements) 
		{
			if (element->hasInput(specifiedElement, inputComponent)) {
				elementsThatHaveSpecifiedElementAsInput.push_back(element);
			}
		}
		return elementsThatHaveSpecifiedElementAsInput;
	}

	bool Simulation::isInitialized() const
	{
		return initialized;
	}

	void Simulation::exportComponentToFile(const std::string& id, const std::string& componentName) const
	{

		const std::shared_ptr<element::Element> foundElement = getElement(id);
		const std::vector<double> component = foundElement->getComponent(componentName);

		const std::chrono::zoned_time localTime{std::chrono::current_zone(), std::chrono::system_clock::now()};
		const std::string timeSignature = std::format("{:%Y-%m-%d_%H-%M-%S}", localTime);

		// Add the time signature to the filename
		const std::string filename = std::string(OUTPUT_DIRECTORY) + "/exports/" + id + "_" + componentName + "_" + timeSignature + ".txt";

		const bool success = utilities::saveVectorToFile(component, filename);
		if (success)
		{
			const std::string logMessage = "Component '" + componentName + "' of element '" + id + "' was exported to file '" + filename + "'.\n";
			log(LogLevel::INFO, logMessage);
		}
		else
		{
			const std::string logMessage = "Component '" + componentName + "' of element '" + id + "' was not exported to file '" + filename + "'.\n";
			log(LogLevel::ERROR, logMessage);
		}

	}

}

