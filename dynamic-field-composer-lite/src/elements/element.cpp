// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "elements/element.h"

namespace dnf_composer
{
	namespace element
	{
		Element::Element(const ElementCommonParameters& parameters)
		{
			if(parameters.dimensionParameters.size <= 0)
				throw Exception(ErrorCode::ELEM_INVALID_SIZE, commonParameters.identifiers.uniqueName);
			commonParameters = parameters;
		}

		void Element::addInput(const std::shared_ptr<Element>& inputElement, const std::string& inputComponent)
		{
			if (!inputElement)
				throw Exception(ErrorCode::ELEM_INPUT_IS_NULL, this->getUniqueIdentifier());

			const auto existingInput = inputs.find(inputElement);
			if (existingInput != inputs.end())
				throw Exception(ErrorCode::ELEM_INPUT_ALREADY_EXISTS, existingInput->first->getUniqueIdentifier());

			if (inputElement->getComponentPtr("output")->size() != this->getComponentPtr("input")->size())
				if (inputElement->getComponentPtr("output")->size() != this->getSize())
					throw Exception(ErrorCode::ELEM_INPUT_SIZE_MISMATCH, inputElement->getUniqueIdentifier());

			inputs[inputElement] = inputComponent;

			const std::string logMessage = "Input '" + inputElement->getUniqueName() +"' added successfully to '" +  this->getUniqueName() + "." ;
			user_interface::LoggerWindow::addLog(user_interface::LogLevel::_INFO, logMessage.c_str());
		}

		void Element::removeInput(const std::string& inputElementId)
		{
			for (auto& key : inputs | std::views::keys)
			{
				if (key->commonParameters.identifiers.uniqueName == inputElementId) {
					inputs.erase(key);
					return;
				}
			}
		}

		void Element::removeInput(int uniqueId)
		{
			for (auto& key : inputs | std::views::keys)
			{
				if (key->commonParameters.identifiers.uniqueIdentifier == uniqueId) {
					inputs.erase(key);
					return;
				}
			}
		}

		bool Element::hasInput(const std::string& inputElementName, const std::string& inputComponent)
		{
			const bool found = std::ranges::any_of(inputs, [&](const auto& pair) {
				const auto& [key, value] = pair;
				return key->identifiers.uniqueName == inputElementName && value == inputComponent;
				});
			if (found)
				return true;
			return false;
		}

		bool Element::hasInput(int inputElementId, const std::string& inputComponent)
		{
			const bool found = std::ranges::any_of(inputs, [&](const auto& pair) {
				const auto& [key, value] = pair;
				return key->commonParameters.identifiers.uniqueIdentifier == inputElementId && value == inputComponent;
				});
			if (found)
				return true;
			return false;
		}

		void Element::updateInput()
		{
			std::ranges::fill(components["input"], 0);

			for (const auto& input_pair : inputs) {
				const auto inputElement = input_pair.first;
				auto inputElementComponent = input_pair.second;
				auto& inputElementComponents = inputElement->components;
				const auto& inputElementComponentValue = inputElementComponents.at(inputElementComponent);

				for (int i = 0; i < inputElementComponentValue.size(); i++)
				{
					components["input"][i] += inputElementComponentValue[i];
				}
			}
		}

		int Element::getMaxSpatialDimension() const
		{
			return commonParameters.dimensionParameters.size;
		}

		double Element::getStepSize() const
		{
			return commonParameters.dimensionParameters.d_x;
		}

		int Element::getSize() const
		{
			return commonParameters.dimensionParameters.size;
		}

		std::string Element::getUniqueName() const
		{
			return commonParameters.identifiers.uniqueName;
		}

		int Element::getUniqueIdentifier() const
		{
			return commonParameters.identifiers.uniqueIdentifier;
		}

		ElementLabel Element::getLabel() const
		{
			return commonParameters.identifiers.label;
		}

		std::vector<double> Element::getComponent(const std::string& componentName)
		{
			if (components.contains(componentName))
				return components.at(componentName);
			throw Exception(ErrorCode::ELEM_COMP_NOT_FOUND, commonParameters.identifiers.uniqueName, componentName);
		}

		std::vector<double>* Element::getComponentPtr(const std::string& componentName)
		{
			if (components.contains(componentName))
				return &components.at(componentName);
			throw Exception(ErrorCode::ELEM_COMP_NOT_FOUND, commonParameters.identifiers.uniqueName, componentName);
		}

		std::vector<std::string> Element::getComponentList() const
		{

			std::vector<std::string> componentNames;
			componentNames.reserve(components.size());

			for (const auto& pair : components)
			{
				const std::string& componentName = pair.first;
				componentNames.push_back(componentName);
			}

			return componentNames;
		
		}

		std::vector<std::shared_ptr<Element>> Element::getInputs()
		{
			std::vector<std::shared_ptr<Element>> inputVec;
			inputVec.reserve(inputs.size());

			for (const auto& key : inputs | std::views::keys)
				inputVec.push_back(key);

			return inputVec;
		}
	}
}
