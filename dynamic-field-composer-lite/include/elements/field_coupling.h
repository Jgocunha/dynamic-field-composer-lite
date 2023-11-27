#pragma once

#include "mathtools/mathtools.h"

#include "element.h"

#include <set>

struct FieldCouplingParameters
{
	double scalar;
	double learningRate;
};

enum class LearningRule
{
	HEBBIAN = 0,
	DELTA_WIDROW_HOFF,
	DELTA_KROGH_HERTZ
};

class FieldCoupling : public Element
{
protected:
	FieldCouplingParameters parameters;
	std::vector<std::vector<double>> weights;
	bool trained;
	bool updateAllWeights;
	LearningRule learningRule;
	std::string weightsFilePath;
public:
	FieldCoupling(const std::string& id, int outputSize, int inputSize, FieldCouplingParameters parameters, LearningRule learningRule);

	void init() override;
	void step(double t, double deltaT) override;
	void close() override;

	void setWeightsFilePath(const std::string& filePath);
	bool readWeights();
	void resetWeights();
	void saveWeights() const;
	virtual void updateWeights(const std::vector<double>& input, const std::vector<double>& output);

	void setLearningRate(double learningRate);
	void setUpdateAllWeights(bool updateAllWeights);

	const std::vector<std::vector<double>>& getWeights() const;

	~FieldCoupling() = default;

protected:
	void getInputFunction();
	void computeOutput();
	void scaleOutput();

	void writeWeights() const;

	std::vector<std::vector<double>> degeneratedLearningRule(std::vector<std::vector<double>>& weights,
		const std::vector<double>& input, const std::vector<double>& targetOutput, const double& learningRate);
};