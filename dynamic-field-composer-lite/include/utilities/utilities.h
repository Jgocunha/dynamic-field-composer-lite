#pragma once

#include <string>
#include <vector>
#include <random>
#include <sstream>
#include <fstream>

namespace dnf_composer
{
	namespace utilities
	{
		int countNumOfLinesInFile(const std::string& filename);

		bool saveVectorToFile(const std::vector<double>& vector, const std::string& filename);

		template <typename T>
		void resizeMatrix(std::vector<std::vector<T>>& matrix, int newRowSize, int newColSize)
		{
			matrix.resize(newRowSize);
			for (int i = 0; i < newRowSize; i++)
				matrix[i].resize(newColSize);
		}

		template <typename T>
		T generateRandomNumber(const T& min, const T& max)
		{
			// Seed the random number generator with a random device
			std::random_device rd;
			std::mt19937 gen(rd());
			// Create a uniform distribution from 1 to 2 (inclusive)
			std::uniform_real_distribution<> dis(min, max);
			// Generate a random integer between 1 and 2
			T randomNum = dis(gen);
			return randomNum;
		}

		template <typename T>
		void fillMatrixWithRandomValues(std::vector<std::vector<T>>& matrix, double minRange = -1.0, double maxRange = 1.0) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(minRange, maxRange);
			for (auto& row : matrix)
				for (auto& element : row)
					element = dis(gen);
		}

		template <typename T>
		std::string matrixToString(const std::vector<std::vector<T>>& matrix)
		{
			std::stringstream ss;
			ss << "[";
			for (const auto& row : matrix)
			{
				ss << "[";
				for (const auto& value : row)
				{
					ss << value << " ";
				}
				ss << "] ";
			}
			ss << "]";
			return ss.str();
		}

		template <typename T>
		std::string vectorToString(const std::vector<T>& vec)
		{
			std::stringstream ss;
			ss << "[";
			for (const auto& value : vec)
			{
				ss << value << " ";
			}
			ss << "]";
			return ss.str();
		}
	}
}
