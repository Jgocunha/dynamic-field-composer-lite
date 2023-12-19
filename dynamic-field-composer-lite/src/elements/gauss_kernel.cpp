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
				const std::string message = "Tried to initialize a non-circular Gaussian kernel '" + this->getUniqueName() + "'. That is not supported yet.\n";
				log(LogLevel::ERROR, message);
			}

			int rangeXsize = kernelRange[0] + kernelRange[1] + 1;
			std::vector<int> rangeX(rangeXsize);
			const int startingValue = static_cast<int>(kernelRange[0]);
			std::iota(rangeX.begin(), rangeX.end(), -startingValue);
			std::vector<double> gauss(commonParameters.dimensionParameters.size);
			if (!normalized)
				gauss = mathtools::gauss(rangeX, 0.0, parameters.sigma);
			else
			{
				const std::string message = "Tried to initialize a normalized Gaussian kernel '" + this->getUniqueName() + "'. That is not supported yet.\n";
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
				components["output"][i] = convolution[i] + parameters.amplitudeGlobal * fullSum;

			//for (int i = 0; i < components["output"].size(); i++)
				//components["output"][i] = (convolution[i] + parameters.amplitudeGlobal) * commonParameters.dimensionParameters.d_x;

		}

		void GaussKernel::close()
		{
		}

		void GaussKernel::printParameters()
		{
			printCommonParameters();

			std::ostringstream logStream;

			logStream << "Logging specific element parameters" << std::endl;
			logStream << "Amplitude: " << parameters.amplitude << std::endl;
			logStream << "Amplitude Global: " << parameters.amplitudeGlobal << std::endl;
			logStream << "Sigma: " << parameters.sigma << std::endl;
			logStream << "Cut-Off Factor: " << cutOfFactor << std::endl;
			logStream << "Circular: " << circular << std::endl;
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
