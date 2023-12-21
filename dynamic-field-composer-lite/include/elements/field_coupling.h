#pragma once

#include <set>

#include "element.h"
#include "mathtools/mathtools.h"
#include "utilities/utilities.h"


namespace dnf_composer
{
	enum class LearningRule
	{
		HEBBIAN = 0,
		DELTA_WIDROW_HOFF,
		DELTA_KROGH_HERTZ
	};

	namespace element
	{
		struct LearningParameters
		{
			double learningRate;
			LearningRule learningRule;
		};

		struct FieldCouplingParameters
		{
			int inputFieldSize;
			double scalar;
			LearningParameters learningParameters;
		};

		class FieldCoupling : public Element
		{
		protected:
			FieldCouplingParameters parameters;
			std::vector<std::vector<double>> weights;
		public:
			FieldCoupling(const ElementCommonParameters& elementCommonParameters, const FieldCouplingParameters& fc_parameters);

			void init() override;
			void step(double t, double deltaT) override;
			void close() override;
			void printParameters() override;

			~FieldCoupling() override = default;
		private:
			bool readWeights();
			void writeWeights();
		};
	}
}
