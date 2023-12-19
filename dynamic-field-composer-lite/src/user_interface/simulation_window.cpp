// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "./user_interface/simulation_window.h"


namespace dnf_composer
{
	namespace user_interface
	{
		SimulationWindow::SimulationWindow(const std::shared_ptr<Simulation>& simulation)
			: simulation(simulation)
		{
		}

		void SimulationWindow::render()
		{
			if (ImGui::Begin("Simulation control"))
			{
				renderStartSimulationButton();
				renderAddElement();
				renderSetInteraction();
				renderRemoveElement();
				renderLogElementProperties();
				renderExportElementComponents();
			}
			ImGui::End();
		}

		void SimulationWindow::renderStartSimulationButton() const
		{
			if (ImGui::Button("Start simulation", { 200.00f, 35.00f }))
				simulation->init();

			ImGui::SameLine();

			if (ImGui::Button("Pause simulation", { 200.00f, 35.00f }))
				simulation->pause();

			ImGui::SameLine();

			if (ImGui::Button("Resume simulation", { 200.00f, 35.00f }))
				simulation->resume();
		}

		void SimulationWindow::renderAddElement() const
		{
			if (ImGui::CollapsingHeader("Add element"))
			{
				for (const auto& pair : element::ElementLabelToString)
					renderElementProperties(pair);
			}
		}

		void SimulationWindow::renderSetInteraction() const
		{
			if (ImGui::CollapsingHeader("Set interactions between elements"))
			{
				const int numberOfElementsInSimulation = simulation->getNumberOfElements();

				for (int i = 0; i < numberOfElementsInSimulation; i++)
				{
					const auto simulationElement = simulation->getElement(i);
					std::string targetElementId = simulationElement->getUniqueName();

					if (ImGui::TreeNode(targetElementId.c_str()))
					{
						static std::string selectedElementId{};
						static int currentElementIdx = 0;
						ImGui::Text("Select the element you want to define as input");
						if (ImGui::BeginListBox("##Element list available as inputs"))
						{
							for (int n = 0; n < numberOfElementsInSimulation; n++)
							{
								const auto element = simulation->getElement(n);
								std::string inputElementId = element->getUniqueName();
								const bool isSelected = (currentElementIdx == n);
								if (ImGui::Selectable(inputElementId.c_str(), isSelected))
								{
									selectedElementId = inputElementId;
									currentElementIdx = n;
								}

								if (isSelected)
									ImGui::SetItemDefaultFocus();
							}
							ImGui::EndListBox();
						}

						if (ImGui::Button("Add", { 100.0f, 30.0f }))
						{
							auto input = simulation->getElement(selectedElementId);
							simulationElement->addInput(input);
						}
						ImGui::TreePop();
					}
				}
			}
		}

		void SimulationWindow::renderRemoveElement() const
		{
			if (ImGui::CollapsingHeader("Remove elements from simulation"))
			{
				int numberOfElementsInSimulation = simulation->getNumberOfElements();

				for (int i = 0; i < numberOfElementsInSimulation; i++)
				{
					const auto simulationElement = simulation->getElement(i);
					std::string elementId = simulationElement->getUniqueName();

					if (ImGui::TreeNode(elementId.c_str()))
					{
						if (ImGui::Button("Remove", { 100.0f, 30.0f }))
						{
							simulation->removeElement(elementId);
							numberOfElementsInSimulation--;
						}
						ImGui::TreePop();
					}
				}
			}
		}

		void SimulationWindow::renderElementProperties(const std::pair<int, std::string>& elementId) const
		{
			if (ImGui::TreeNode(elementId.second.c_str()))
			{
				switch (elementId.first)
				{
				case element::ElementLabel::NEURAL_FIELD:
					addElementNeuralField();
					break;
				case element::ElementLabel::GAUSS_STIMULUS:
					addElementGaussStimulus();
					break;
				case element::ElementLabel::FIELD_COUPLING:
					addElementFieldCoupling();
					break;
				case element::ElementLabel::GAUSS_KERNEL:
					addElementGaussKernel();
					break;
				case element::ElementLabel::MEXICAN_HAT_KERNEL:
					addElementMexicanHatKernel();
					break;
				case element::ElementLabel::NORMAL_NOISE:
					addElementNormalNoise();
					break;
				case element::ElementLabel::GAUSS_FIELD_COUPLING:
					addElementGaussFieldCoupling();
					break;
				default:
					//log(LogLevel::ERROR, "There is a missing element in the TreeNode in simulation window.\n");
					break;
				}
				ImGui::TreePop();
			}
		}

		void SimulationWindow::renderLogElementProperties() const
		{
			if (ImGui::CollapsingHeader("Log element parameters"))
			{
				const int numberOfElementsInSimulation = simulation->getNumberOfElements();

				for (int i = 0; i < numberOfElementsInSimulation; i++)
				{
					const auto simulationElement = simulation->getElement(i);
					std::string elementId = simulationElement->getUniqueName();

					if (ImGui::TreeNode(elementId.c_str()))
					{
						if (ImGui::Button("Log", { 100.0f, 30.0f }))
						{
							simulationElement->printParameters();
						}
						ImGui::TreePop();
					}
				}
			}
		}

		void SimulationWindow::renderExportElementComponents() const
		{
			if (ImGui::CollapsingHeader("Export element components"))
			{
				const int numberOfElementsInSimulation = simulation->getNumberOfElements();

				for (int i = 0; i < numberOfElementsInSimulation; i++)
				{
					const auto simulationElement = simulation->getElement(i);
					std::string elementId = simulationElement->getUniqueName();

					if (ImGui::TreeNode(elementId.c_str()))
					{
						for(const auto& componentName : simulationElement->getComponentList())
						{
							if (ImGui::TreeNode(componentName.c_str()))
							{
								if (ImGui::Button("Export", { 100.0f, 30.0f }))
								{
									simulation->exportComponentToFile(elementId, componentName);
								}
								ImGui::TreePop();
							}
						}
						ImGui::TreePop();
					}
				}
			}
		}

		void SimulationWindow::addElementNeuralField() const
		{
			static char id[CHAR_SIZE] = "neural field u";
			ImGui::InputTextWithHint("id", "enter text here", id, IM_ARRAYSIZE(id));
			static int x_max = 100;
			ImGui::InputInt("x_max", &x_max, 1.0, 10.0);
			static double d_x = 0.1;
			ImGui::InputDouble("d_x", &d_x, 0.1, 0.5, "%.2f");
			static double tau = 25;
			ImGui::InputDouble("tau", &tau, 1.0f, 10.0f, "%.2f");
			static double sigmoidSteepness = 5.0f;
			ImGui::InputDouble("sigmoid steepness", &sigmoidSteepness, 1.0f, 10.0f, "%.2f");
			static double restingLevel = -10.0f;
			ImGui::InputDouble("resting level", &restingLevel, 1.0f, 10.0f, "%.2f");

			if (ImGui::Button("Add", { 100.0f, 30.0f }))
			{
				const element::SigmoidFunction activationFunction{ 0, sigmoidSteepness };
				const element::NeuralFieldParameters nfp = { tau, restingLevel, activationFunction };

				const element::ElementIdentifiers neuralFieldIdentifiers{id};
				const element::ElementSpatialDimensionParameters neuralFieldDimensions{ x_max, d_x };
				const element::ElementCommonParameters commonParameters{ neuralFieldIdentifiers, neuralFieldDimensions };

				const std::shared_ptr<element::NeuralField> neuralField(new element::NeuralField(commonParameters, nfp));
				simulation->addElement(neuralField);
			}
		}

		void SimulationWindow::addElementGaussStimulus() const
		{
			static char id[CHAR_SIZE] = "gauss stimulus a";
			ImGui::InputTextWithHint("id", "enter text here", id, IM_ARRAYSIZE(id));
			static int x_max = 100;
			ImGui::InputInt("x_max", &x_max, 1.0, 10.0);
			static double d_x = 0.1;
			ImGui::InputDouble("d_x", &d_x, 0.1, 0.5, "%.2f");
			static double sigma = 5;
			ImGui::InputDouble("sigma", &sigma, 1.0f, 10.0f, "%.2f");
			static double amplitude = 20;
			ImGui::InputDouble("amplitude", &amplitude, 1.0f, 10.0f, "%.2f");
			static double position = 50;
			ImGui::InputDouble("position", &position, 1.0f, 10.0f, "%.2f");


			if (ImGui::Button("Add", { 100.0f, 30.0f }))
			{
				const element::GaussStimulusParameters gsp = { sigma, amplitude, position };
				const element::ElementSpatialDimensionParameters dimensions{ x_max, d_x };
				const std::shared_ptr<element::GaussStimulus> gaussStimulus(new element::GaussStimulus ({id, dimensions}, gsp));
				simulation->addElement(gaussStimulus);
			}
		}

		void SimulationWindow::addElementNormalNoise() const
		{
			static char id[CHAR_SIZE] = "normal noise a";
			ImGui::InputTextWithHint("id", "enter text here", id, IM_ARRAYSIZE(id));
			static int x_max = 100;
			ImGui::InputInt("x_max", &x_max, 1.0, 10.0);
			static double d_x = 0.1;
			ImGui::InputDouble("d_x", &d_x, 0.1, 0.5, "%.2f");
			static double amplitude = 20;
			ImGui::InputDouble("amplitude", &amplitude, 1.0f, 10.0f, "%.2f");

			if (ImGui::Button("Add", { 100.0f, 30.0f }))
			{
				const element::NormalNoiseParameters nnp = { amplitude };
				const element::ElementSpatialDimensionParameters dimensions{ x_max, d_x };
				const std::shared_ptr<element::NormalNoise> normalNoise( new element::NormalNoise({ id, dimensions}, nnp));
				const element::GaussKernelParameters gkp = { 0.25, 0.2 };
				const std::shared_ptr<element::GaussKernel> gaussKernelNormalNoise(new element::GaussKernel({ std::string(id) + " gauss kernel", dimensions }, gkp));
				simulation->addElement(normalNoise);
				simulation->addElement(gaussKernelNormalNoise);
			}
		}

		void SimulationWindow::addElementFieldCoupling() const
		{
		}

		void SimulationWindow::addElementGaussFieldCoupling() const
		{
			
		}

		void SimulationWindow::addElementGaussKernel() const
		{
			static char id[CHAR_SIZE] = "gauss kernel u -> u";
			ImGui::InputTextWithHint("id", "enter text here", id, IM_ARRAYSIZE(id));
			static int x_max = 100;
			ImGui::InputInt("x_max", &x_max, 1.0, 10.0);
			static double d_x = 0.1;
			ImGui::InputDouble("d_x", &d_x, 0.1, 0.5, "%.2f");
			static double sigma = 20;
			ImGui::InputDouble("sigma", &sigma, 1.0f, 10.0f, "%.2f");
			static double amplitude = 2;
			ImGui::InputDouble("amplitude", &amplitude, 1.0f, 10.0f, "%.2f");

			if (ImGui::Button("Add", { 100.0f, 30.0f }))
			{
				const element::GaussKernelParameters gkp = { sigma, amplitude };
				const element::ElementSpatialDimensionParameters dimensions{ x_max, d_x };
				const std::shared_ptr<element::GaussKernel> gaussKernel(new element::GaussKernel({ id, dimensions }, gkp));
				simulation->addElement(gaussKernel);
			}
		}

		void SimulationWindow::addElementMexicanHatKernel() const
		{
			static char id[CHAR_SIZE] = "mexican hat kernel u -> u";
			ImGui::InputTextWithHint("id", "enter text here", id, IM_ARRAYSIZE(id));
			static int x_max = 100;
			ImGui::InputInt("x_max", &x_max, 1.0, 10.0);
			static double d_x = 0.1;
			ImGui::InputDouble("d_x", &d_x, 0.1, 0.5, "%.2f");
			static double sigmaExc = 5;
			ImGui::InputDouble("sigmaExc", &sigmaExc, 1.0f, 10.0f, "%.2f");
			static double amplitudeExc = 15;
			ImGui::InputDouble("amplitudeExc", &amplitudeExc, 1.0f, 10.0f, "%.2f");
			static double sigmaInh = 10;
			ImGui::InputDouble("sigmaInh", &sigmaInh, 1.0f, 10.0f, "%.2f");
			static double amplitudeInh = 15;
			ImGui::InputDouble("amplitudeInh", &amplitudeInh, 1.0f, 10.0f, "%.2f");
			static double amplitudeGlobal = 0;
			ImGui::InputDouble("global amplitude", &amplitudeGlobal, 1.0f, 10.0f, "%.2f");

			if (ImGui::Button("Add", { 100.0f, 30.0f }))
			{
				const element::MexicanHatKernelParameters mhkp = { sigmaExc, amplitudeExc, sigmaInh, amplitudeInh, amplitudeGlobal };
				const element::ElementSpatialDimensionParameters dimensions{ x_max, d_x };
				const std::shared_ptr<element::MexicanHatKernel> mexicanHatKernel(new element::MexicanHatKernel({ id, dimensions }, mhkp));
				simulation->addElement(mexicanHatKernel);
			}
		}

	}
}
