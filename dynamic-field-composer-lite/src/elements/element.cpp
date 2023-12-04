// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "elements/element.h"

#include <iostream>


namespace dnf_composer
{
	namespace element
	{

		Element::Element()
		{
			label = ElementLabel::UNINITIALIZED;

			uniqueIdentifier = uniqueIdentifierCounter++;
			uniqueName.clear();

			size = 0;
			components["output"] = {};
			components["input"] = {};
			inputs = {};
		}

		void Element::addInput(const std::shared_ptr<Element>& inputElement, const std::string& inputComponent)
		{
			if (!inputElement)
				throw Exception(ErrorCode::ELEM_INPUT_IS_NULL);

			const auto existingInput = inputs.find(inputElement);
			if (existingInput != inputs.end())
				throw Exception(ErrorCode::ELEM_INPUT_ALREADY_EXISTS);

			// check if input element has the same size as the element
			if (inputElement->getComponentPtr("output")->size() != this->getComponentPtr("input")->size())
				if (inputElement->getComponentPtr("output")->size() != this->getSize())
					throw Exception(ErrorCode::ELEM_INPUT_SIZE_MISMATCH, inputElement->getUniqueIdentifier());

			inputs[inputElement] = inputComponent;
		}

		void Element::removeInput(const std::string& inputElementId)
		{
			for (auto& key : inputs | std::views::keys)
			{
				if (key->uniqueName == inputElementId) {
					inputs.erase(key);
					return;
				}
			}
			//throw Exception(ErrorCode::ELEM_INPUT_NOT_FOUND, inputElementId);
		}

		void Element::removeInput(int uniqueId)
		{
			for (auto& key : inputs | std::views::keys)
			{
				if (key->uniqueIdentifier == uniqueId) {
					inputs.erase(key);
					return;
				}
			}
			//throw Exception(ErrorCode::ELEM_INPUT_NOT_FOUND, inputElementId);
		}

		bool Element::hasInput(const std::string& inputElementName, const std::string& inputComponent)
		{
			const bool found = std::ranges::any_of(inputs, [&](const auto& pair) {
				const auto& [key, value] = pair;
				return key->uniqueName == inputElementName && value == inputComponent;
				});
			if (found)
				return true;
			return false;

		}

		bool Element::hasInput(int inputElementId, const std::string& inputComponent)
		{
			const bool found = std::ranges::any_of(inputs, [&](const auto& pair) {
				const auto& [key, value] = pair;
				return key->uniqueIdentifier == inputElementId && value == inputComponent;
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

		void Element::setUniqueIdentifier(int uniqueIdentifier)
		{
			//this->uniqueIdentifier = uniqueIdentifier;

			// for now, element renaming can be potentially damaging for the simulation
			throw Exception(ErrorCode::ELEM_RENAME_NOT_ALLOWED, uniqueIdentifier);
		}

		void Element::setSize(int size) const
		{
			//this->size = size;
			//components.at("output").resize(size);
			//components.at("input").resize(size);

			// for now, element resizing can be potentially damaging for the simulation
			throw Exception(ErrorCode::ELEM_SIZE_NOT_ALLOWED, uniqueIdentifier);
		}

		int Element::getSize() const
		{
			return size;
		}

		std::string Element::getUniqueName() const
		{
			return uniqueName;
		}

		int Element::getUniqueIdentifier() const
		{
			return uniqueIdentifier;
		}

		ElementLabel Element::getLabel() const
		{
			return label;
		}

		std::vector<double> Element::getComponent(const std::string& componentName)
		{
			if (components.contains(componentName))
				return components.at(componentName);
			throw Exception(ErrorCode::ELEM_COMP_NOT_FOUND, uniqueName, componentName);
		}

		std::vector<double>* Element::getComponentPtr(const std::string& componentName)
		{
			if (components.contains(componentName))
				return &components.at(componentName);
			throw Exception(ErrorCode::ELEM_COMP_NOT_FOUND, uniqueName, componentName);
		}

		std::vector<std::shared_ptr<Element>> Element::getInputs()
		{
			std::vector<std::shared_ptr<Element>> inputVec;
			inputVec.reserve(inputs.size());

			for (const auto& key : inputs | std::views::keys)
				inputVec.push_back(key);

			return inputVec;
		}

		void Element::printParameters() const
		{
			std::cout << "Unique Identifier: " << uniqueIdentifier << std::endl;
			std::cout << "Unique Name: " << uniqueName << std::endl;
			std::cout << "Label: " << ElementLabelToString.at(label) << std::endl;
			std::cout << "Size: " << size << std::endl;

			std::cout << "Components:" << std::endl;
			for (const auto& pair : components)
			{
				const std::string& componentName = pair.first;
				const std::vector<double>& componentValues = pair.second;

				std::cout << componentName << ": ";
				for (const auto& value : componentValues)
				{
					std::cout << value << " ";
				}
				std::cout << std::endl;
			}

			std::cout << "Inputs:" << std::endl;
			for (const auto& inputPair : inputs)
			{
				const std::shared_ptr<Element>& inputElement = inputPair.first;
				const std::string& inputComponent = inputPair.second;

				std::cout << "Input Element Unique Identifier: " << inputElement->getUniqueName()
					<< ", Input Component: " << inputComponent << std::endl;
			}
		}
	}
}
