// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "elements/field_coupling.h"


namespace dnf_composer
{
	namespace element
	{

		FieldCoupling::FieldCoupling(const ElementCommonParameters& elementCommonParameters, const FieldCouplingParameters& parameters)
			: Element(elementCommonParameters), parameters(parameters)
		{
			if (parameters.inputFieldSize <= 0)
				throw Exception(ErrorCode::ELEM_SIZE_NOT_ALLOWED, commonParameters.identifiers.uniqueName);

			commonParameters.identifiers.label = ElementLabel::FIELD_COUPLING;

			components["input"] = std::vector<double>(parameters.inputFieldSize);
			utilities::resizeMatrix(weights, static_cast<int>(components["input"].size()), static_cast<int>(components["output"].size()));

			utilities::fillMatrixWithZeros(weights);
		}

		void FieldCoupling::init()
		{
			//std::ranges::fill(components["input"], 0);
			//std::ranges::fill(components["output"], 0);
			if(!readWeights())
			{
				utilities::fillMatrixWithRandomValues(weights);
				writeWeights();
			}
		}

		bool FieldCoupling::readWeights()
		{
			const std::string weightsFilePath = std::string(OUTPUT_DIRECTORY) + "/field-coupling-weights/" + commonParameters.identifiers.uniqueName + "_weights.txt";
			std::ifstream file(weightsFilePath);




			if (file.is_open()) {
				utilities::resizeMatrix(weights, 0, 0);
				double element;
				std::vector<double> row;
				while (file >> element)
				{
					row.push_back(element);
					if (row.size() == components["output"].size())
					{
						weights.push_back(row);
						row.clear();
					}
				}
				file.close();
				const std::string message = "Weights '" + this->getUniqueName() + "' read successfully from: " + weightsFilePath + ". \n";
				log(LogLevel::INFO, message);
				return true;
			}

			const std::string message = "Failed to read weights '" + this->getUniqueName() + "' from: " + weightsFilePath + ". \n";
			log(LogLevel::ERROR, message);

			return false;
		}

		void FieldCoupling::writeWeights() const
		{
			const std::string weightsFilePath = std::string(OUTPUT_DIRECTORY) + "/field-coupling-weights/" + commonParameters.identifiers.uniqueName + "_weights.txt";
			std::ifstream file(weightsFilePath);

			if (file.is_open()) {
				for (const auto& row : weights) {
					for (const auto& element : row) {
						file << element << " ";
					}
					file << '\n';
				}
				file.close();
				const std::string message = "Saved weights '" + this->getUniqueName() + "' to: " + weightsFilePath + ". \n";
				log(LogLevel::INFO, message);
			}
			else
			{
				const std::string message = "Failed to saved weights '" + this->getUniqueName() + "' to: " + weightsFilePath + ". \n";
				log(LogLevel::ERROR, message);
			}
		}
	}
}
