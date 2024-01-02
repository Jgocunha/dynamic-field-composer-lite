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

		void FieldCoupling::step(double t, double deltaT)
		{
			updateInput();

			// multiply the input by the weights to get output
			for (int i = 0; i < static_cast<int>(components["output"].size()); i++)
				for (int j = 0; j < static_cast<int>(components["input"].size()); j++)
					components["output"][i] += weights[j][i] * components["input"][j];

			// only the positive values of the output are considered
			//for (auto& value : components["output"])
				//if (value < 0)
					//value = 0;

			// Scale the output by parameter k
			for (auto& value : components["output"])
				value *= parameters.scalar;

		}

		void FieldCoupling::close()
		{
			
		}

		void FieldCoupling::updateWeights(const std::vector<double>& input, const std::vector<double>& output)
		{
			switch (parameters.learningParameters.learningRule)
			{
			case LearningRule::HEBBIAN:
				weights = mathtools::hebbLearningRule(input, output, parameters.learningParameters.learningRate);
				break;
			case LearningRule::DELTA_WIDROW_HOFF:
				weights = mathtools::deltaLearningRuleWidrowHoff(weights, input, output, parameters.learningParameters.learningRate);
				break;
			case LearningRule::DELTA_KROGH_HERTZ:
				weights = mathtools::deltaLearningRuleKroghHertz(weights, input, output, parameters.learningParameters.learningRate);
				break;
			}

			writeWeights();

		}


		void FieldCoupling::printParameters()
		{
			
		}


		bool FieldCoupling::readWeights()
		{
			const std::string weightsFile = std::string(OUTPUT_DIRECTORY) + "/couplings/" + commonParameters.identifiers.uniqueName + "_weights.txt";

			const auto result = utilities::readMatrixFromFile(weightsFile);

			if (result.second) 
			{
				if (result.first.size() == weights.size() &&
					(result.first.empty() || result.first[0].size() == weights[0].size()))
				{
					weights = result.first;
					const std::string message = "Weights '" + this->getUniqueName() + "' read successfully from: " + weightsFile + ". \n";
					log(LogLevel::INFO, message);
					return true;
				}
				const std::string message = "Mismatched matrix sizes while reading weights '" + this->getUniqueName() + "' from: " + weightsFile + ". \n";
				log(LogLevel::WARNING, message);
				return false;
			}

			const std::string message = "Failed to read weights '" + this->getUniqueName() + "' from: " + weightsFile + ". \n";
			log(LogLevel::WARNING, message);
			return false;
		}

		void FieldCoupling::writeWeights() const
		{
			const std::string weightsFile = std::string(OUTPUT_DIRECTORY) + "/couplings/" + commonParameters.identifiers.uniqueName + "_weights.txt";
			std::ifstream file(weightsFile);

			if (utilities::saveMatrixToFile(weights, weightsFile))
			{
				const std::string message = "Saved weights '" + this->getUniqueName() + "' to: " + weightsFile + ". \n";
				log(LogLevel::INFO, message);
				return;
			}
			const std::string message = "Failed to saved weights '" + this->getUniqueName() + "' to: " + weightsFile + ". \n";
			log(LogLevel::WARNING, message);
		}
	}
}
