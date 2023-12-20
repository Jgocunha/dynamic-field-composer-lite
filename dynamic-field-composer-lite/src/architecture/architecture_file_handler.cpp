// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "architecture/architecture_file_handler.h"


namespace dnf_composer
{
	ArchitectureFileHandler::ArchitectureFileHandler(const std::string& identifier)
	{
	    architectureFileLocation = std::string(OUTPUT_DIRECTORY)
	        + "/architectures/architecture-" + identifier + "/architecture-" + identifier + "-dnfs.txt";
	    fieldCouplingFileLocation = std::string(OUTPUT_DIRECTORY)
	        + "/architectures/architecture-" + identifier + "/architecture-" + identifier + "-fcs.txt";

	    unparsedDynamicNeuralFieldParameters = {};
	    unparsedDynamicNeuralFieldCouplingsParameters = {};

	    dynamicNeuralFieldParametersToSave = {};
	    dynamicNeuralFieldCouplingsParametersToSave = {};
	}

	void ArchitectureFileHandler::readArchitectureFile()
	{
	    readDynamicNeuralFieldParameters();
	    readDynamicNeuralFieldCouplingsParameters();
	}

	void ArchitectureFileHandler::saveArchitectureFile()
	{
	    architectureFileLocation = architectureFileLocation + "-saved.txt";
	    fieldCouplingFileLocation = fieldCouplingFileLocation + "-saved.txt";

	    saveDynamicNeuralFieldParameters();
	    saveDynamicNeuralFieldCouplingsParameters();
	}

	std::deque<std::string> ArchitectureFileHandler::getUnparsedDynamicNeuralFieldParameters()
	{
	    return unparsedDynamicNeuralFieldParameters;
	}

	std::deque<std::string> ArchitectureFileHandler::getUnparsedDynamicNeuralFieldCouplingsParameters()
	{
	    return unparsedDynamicNeuralFieldCouplingsParameters;
	}

	void ArchitectureFileHandler::setDynamicNeuralFieldParametersToSave(const std::string& parameters)
	{
	    dynamicNeuralFieldParametersToSave = parameters;
	}

	void ArchitectureFileHandler::setDynamicNeuralFieldCouplingsParametersToSave(const std::string& parameters)
	{
	    dynamicNeuralFieldCouplingsParametersToSave = parameters;
	}

	void ArchitectureFileHandler::readDynamicNeuralFieldParameters()
	{
	    std::ifstream architectureFile(architectureFileLocation);
	    if (architectureFile.is_open())
	    {
	        std::string line;
	        while (std::getline(architectureFile, line))
	            unparsedDynamicNeuralFieldParameters.push_back(line);

	        architectureFile.close();
			log(LogLevel::INFO, "Dynamic neural field parameters read successfully from " + architectureFileLocation + ". \n");
	    }
	    else
	    {
			const std::string message = "Unable to open file to read dynamic neural field parameters " + architectureFileLocation + ". \n";
			log(LogLevel::ERROR, message);
	    }
	}

	void ArchitectureFileHandler::readDynamicNeuralFieldCouplingsParameters()
	{
	    std::ifstream fieldCouplingsFile(fieldCouplingFileLocation);
	    if (fieldCouplingsFile.is_open())
	    {
	        std::string line;
	        while (std::getline(fieldCouplingsFile, line))
	            unparsedDynamicNeuralFieldCouplingsParameters.push_back(line);

	        fieldCouplingsFile.close();
			log(LogLevel::INFO, "Field coupling parameters read successfully from " + architectureFileLocation + ". \n");
	    }
	    else
	    {
			const std::string message = "Unable to open file to read field coupling parameters " + architectureFileLocation + ". \n";
			log(LogLevel::ERROR, message);
	    }
	}

	void ArchitectureFileHandler::saveDynamicNeuralFieldParameters() const
	{
	    std::ofstream architectureFile(architectureFileLocation);
	    if (architectureFile.is_open())
	    {
	        architectureFile << dynamicNeuralFieldParametersToSave;
			log(LogLevel::INFO, "Dynamic neural field parameters saved successfully to " + architectureFileLocation + ". \n");
	    }
	    else
	    {
			const std::string message = "Unable to open file to save dynamic neural field parameters " + architectureFileLocation + "\n";
			log(LogLevel::ERROR, message);
	    }

	    architectureFile.close();
	}

	void ArchitectureFileHandler::saveDynamicNeuralFieldCouplingsParameters() const 
	{
	    std::ofstream fieldCouplingFile(fieldCouplingFileLocation);
	    if (fieldCouplingFile.is_open())
	    {
	        fieldCouplingFile << dynamicNeuralFieldCouplingsParametersToSave;
			log(LogLevel::INFO, "Field coupling parameters saved successfully to " + architectureFileLocation + ". \n");
	    }
		else
		{
			const std::string message = "Unable to open file to save field coupling parameters " + architectureFileLocation + "\n";
			log(LogLevel::ERROR, message);
		}

		fieldCouplingFile.close();
	}

}

