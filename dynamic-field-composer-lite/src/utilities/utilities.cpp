// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "utilities/utilities.h"


namespace dnf_composer::utilities
{
	int countNumOfLinesInFile(const std::string& filename)
	{
		std::ifstream file(filename);
		if (file.is_open()) {
			int lineCount = 0;
			std::string line;
			while (std::getline(file, line)) {
				lineCount++;
			}
			file.close();
			return lineCount;
		}
		
		return -1; // Return -1 to indicate an error
	}

	bool saveVectorToFile(const std::vector<double>& vector, const std::string& filename)
	{
		std::ofstream file(filename);
		if (file.is_open())
		{
			for (auto& element : vector)
				file << element << " ";
			file.close();
			return true;
		}
		return false;
	}

	bool saveMatrixToFile(const std::vector<std::vector<double>>& matrix, const std::string& filename)
	{
		std::ofstream file(filename);

		if (file.is_open()) 
		{
			// Write the number of rows and columns to the file
			file << matrix.size() << " " << (matrix.empty() ? 0 : matrix[0].size()) << "\n";

			for (const auto& row : matrix) {
				for (const auto& element : row) {
					file << element << " ";
				}
				file << '\n';
			}
			file.close();
			return true;
		}
		return false;
	}

	std::pair<std::vector<std::vector<double>>, bool> readMatrixFromFile(const std::string& filename) {
		std::ifstream file(filename);

		// Check if the file is open successfully
		if (!file.is_open()) {
			return { {}, false };
		}

		std::vector<std::vector<double>> matrix;
		int rows, cols;

		// Read the number of rows and columns
		file >> rows >> cols;

		// Check if the matrix dimensions are valid
		if (rows <= 0 || cols <= 0) {
			return { {}, false };
		}

		// Resize the matrix
		matrix.resize(rows, std::vector<double>(cols));

		// Read matrix elements
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				if (!(file >> matrix[i][j])) {
					return { {}, false };
				}
			}
		}

		// Close the file
		file.close();

		// Return the matrix and success flag
		return { matrix, true };
	}
}

