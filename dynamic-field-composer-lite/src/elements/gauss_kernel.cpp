// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "elements/gauss_kernel.h"

namespace dnf_composer
{
	namespace element
	{
		GaussKernel::GaussKernel(const ElementCommonParameters& elementCommonParameters, const GaussKernelParameters& gk_parameters)
			: Kernel(elementCommonParameters), parameters(gk_parameters)
		{
			commonParameters.identifiers.label = ElementLabel::GAUSS_KERNEL;
			components["kernel"] = std::vector<double>(commonParameters.dimensionParameters.size);
		}

		void GaussKernel::init()
		{
			kernelRange = mathtools::computeKernelRange(parameters.sigma, cutOfFactor, commonParameters.dimensionParameters.size, circular);

			if (circular)
				extIndex = mathtools::createExtendedIndex(commonParameters.dimensionParameters.size, kernelRange);
			else
			{
				const std::string message = "Tried to initialize a non-circular Mexican hat kernel '" + this->getUniqueName() + "'. That is not supported yet.";
				log(LogLevel::ERROR, message);
			}

			int rangeXsize = kernelRange[0] + kernelRange[1] + 1;
			std::vector<int> rangeX(rangeXsize);
			const int startingValue = static_cast<int>(kernelRange[0]);
			std::iota(rangeX.begin(), rangeX.end(), -startingValue);
			std::vector<double> gauss(commonParameters.dimensionParameters.size);
			if (!normalized)
				gauss = mathtools::gaussNorm(rangeX, 0.0, parameters.sigma);
			else
			{
				const std::string message = "Tried to initialize a normalized Mexican hat kernel '" + this->getUniqueName() + "'. That is not supported yet.";
				log(LogLevel::ERROR, message);
			}

			components["kernel"].resize(rangeX.size());
			for (int i = 0; i < components["kernel"].size(); i++)
				components["kernel"][i] = parameters.amplitude * gauss[i];

			components["input"].resize(extIndex.size());
			fullSum = 0;
			std::ranges::fill(components["input"], 0.0);
		}

		void GaussKernel::step(double t, double deltaT)
		{

			updateInput();

			fullSum = std::accumulate(components["input"].begin(), components["input"].end(), (double)0.0);

			std::vector<double> convolution(commonParameters.dimensionParameters.size);
			const std::vector<double> subDataInput = mathtools::obtainCircularVector(extIndex, components["input"]);


			if (circular)
				convolution = mathtools::conv_valid(subDataInput, components["kernel"]);
			else
				convolution = mathtools::conv(subDataInput, components["kernel"]);

			for (int i = 0; i < components["output"].size(); i++)
				components["output"][i] = (convolution[i] + parameters.amplitudeGlobal) * commonParameters.dimensionParameters.d_x;

		}

		void GaussKernel::close()
		{
		}

		void GaussKernel::printParameters()
		{
			std::ostringstream logStream;

			logStream << std::left;

			logStream << "Logging element parameters" << std::endl;
			logStream << "Unique Identifier: " << commonParameters.identifiers.uniqueIdentifier << std::endl;
			logStream << "Unique Name: " << commonParameters.identifiers.uniqueName << std::endl;
			logStream << "Label: " << ElementLabelToString.at(commonParameters.identifiers.label) << std::endl;
			logStream << "Maximum spatial dimension size: " << commonParameters.dimensionParameters.x_max << std::endl;
			logStream << "Spatial dimension step size: " << commonParameters.dimensionParameters.d_x << std::endl;
			logStream << "Number of samples in spatial dimension: " << commonParameters.dimensionParameters.size << std::endl;

			logStream << "Components: ";
			for (const auto& pair : components)
			{
				const std::string& componentName = pair.first;
				const std::vector<double>& componentValues = pair.second;

				logStream << componentName << " | ";
			}

			logStream << std::endl << "Inputs: ";
			for (const auto& inputPair : inputs)
			{
				const std::shared_ptr<Element>& inputElement = inputPair.first;
				const std::string& inputComponent = inputPair.second;

				logStream << inputElement->getUniqueName() << "->" << inputComponent << " | ";
			}

			logStream << std::endl << "GaussKernelParameters: ";
			logStream << "Amplitude: " << parameters.amplitude << " | ";
			logStream << "Amplitude Global: " << parameters.amplitudeGlobal << " | ";
			logStream << "Sigma: " << parameters.sigma << " | ";
			logStream << "Cut-Off Factor: " << cutOfFactor << " | ";
			logStream << "Circular: " << circular << " | ";
			logStream << "Normalized: " << normalized << std::endl;

			log(LogLevel::INFO, logStream.str());
		}

		void GaussKernel::setParameters(const GaussKernelParameters& gk_parameters)
		{
			parameters = gk_parameters;
			init();
		}

		GaussKernelParameters GaussKernel::getParameters() const
		{
			return parameters;
		}
	}
}
