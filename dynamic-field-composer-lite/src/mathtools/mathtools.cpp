// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "mathtools/mathtools.h"

namespace dnf_composer
{
	namespace mathtools
	{
		std::array<int, 2> computeKernelRange(double sigma, int cutOfFactor, int fieldSize, bool circular)
		{
			const int ceilSigmaCutOfFactor = static_cast<int>(std::ceil(sigma * cutOfFactor));
			const int fieldSizeMinus1 = (fieldSize - 1);

			if (circular)
			{
				const double halfFieldSizeMinus1 = (static_cast<double>(fieldSize) - 1) / 2;
				const int floorHalfFieldSizeMinus1 = static_cast<int>(std::floor(halfFieldSizeMinus1));
				const int ceilHalfFieldSizeMinus1 = static_cast<int>(std::ceil(halfFieldSizeMinus1));

				return std::min(std::array<int, 2>{ceilSigmaCutOfFactor, ceilSigmaCutOfFactor},
					std::array<int, 2>{floorHalfFieldSizeMinus1, ceilHalfFieldSizeMinus1});
			}

			return std::min(std::array<int, 2>{ceilSigmaCutOfFactor, ceilSigmaCutOfFactor},
					std::array<int, 2>{fieldSizeMinus1, fieldSizeMinus1});
		}

		std::vector<int> createExtendedIndex(int fieldSize, const std::array<int, 2>& kernelRange)
		{
			const int startingValue = fieldSize - kernelRange[1] + 1;
			const int initialVectorSize = fieldSize - startingValue + 1;

			std::vector<int> initialVector(initialVectorSize);
			std::iota(initialVector.begin(), initialVector.end(), startingValue);

			std::vector<int> secondVector(fieldSize);
			std::iota(secondVector.begin(), secondVector.end(), 1);

			std::vector<int> thirdVector(kernelRange[0]);
			std::iota(thirdVector.begin(), thirdVector.end(), 1);

			std::vector<int> extendedVector;
			extendedVector.reserve(initialVector.size() + secondVector.size() + thirdVector.size());

			extendedVector.insert(extendedVector.end(), initialVector.begin(), initialVector.end());
			extendedVector.insert(extendedVector.end(), secondVector.begin(), secondVector.end());
			extendedVector.insert(extendedVector.end(), thirdVector.begin(), thirdVector.end());

			return extendedVector;
		}

		std::vector<double> generateNormalVector(int size)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::normal_distribution<> dist(0, 1);

			std::vector<double> vec(size);
			for (int i = 0; i < size; ++i)
				vec[i] = dist(gen);

			return vec;
		}

		std::vector<double> normalizeVector(const std::vector<double>& vec)
		{
			// Make a copy of the input vector
			std::vector<double> normalizedVec = vec;

			constexpr double safetyFactor = 0.0;

			for (double& val : normalizedVec)
				if (val < 0.01)
					val = 0;

			// Find the minimum and maximum values in the vector
			const double maxVal = *std::max_element(normalizedVec.begin(), normalizedVec.end()) + safetyFactor;
			const double minVal = *std::min_element(normalizedVec.begin(), normalizedVec.end()) - safetyFactor;

			// Normalize the vector
			for (double& val : normalizedVec)
			{
				if (val != 0.0)
				{
					// Uncomment the following line if you want to normalize between -1 and 1
					// val = (val - minVal) / (maxVal - minVal) * 2.0 - 1.0;

					// Normalize between 0 and 1
					val = (val - minVal) / (maxVal - minVal);
				}
			}

			return normalizedVec;
		}
	}
}
