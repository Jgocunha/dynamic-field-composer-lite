#include <catch2/catch_test_macros.hpp>

#include "elements/element.h"
#include "elements/neural_field.h"

// Helper function to create a sample Element object for testing
std::shared_ptr<NeuralField> createSampleNeuralField(const std::string& elementId, uint8_t size = 1)
{
    NeuralFieldParameters nfp{ 1, -5};
    ActivationFunctionParameters afp{ ActivationFunctionType::Sigmoid, 1, 0 };
    return std::make_shared<NeuralField>(elementId, size, nfp);
}

// Mock class for testing Element
class MockElement : public Element
{
public:
    void init() override {}
    void step(const double& t, const double& deltaT) override {}
    void close() override {}
};

TEST_CASE("Element class tests", "[element]")
{
    SECTION("addInput() hasInput() methods")
    {
        // Create a mock Element object for testing
        auto element = createSampleNeuralField("Element1");

        // Add input
        auto inputElement = createSampleNeuralField("inputElement");
        element->addInput(inputElement, "output");

        // Add nullptr element, should throw exception
        REQUIRE_THROWS_AS(element->addInput(nullptr, "component"), Exception);

        // Check if input was added
        REQUIRE(element->hasInput("inputElement", "output") == true);
        REQUIRE(element->hasInput("inputElement", "input") == false);
        REQUIRE(element->hasInput("nonexistentElement", "output") == false);

        // Add input with same unique identifier, should throw exception
        REQUIRE_THROWS_AS(element->addInput(inputElement, "output"), Exception);

        // Add input with different sizes, should throw exception
        auto inputElement2 = createSampleNeuralField("inputElement2", 2);
        REQUIRE_THROWS_AS(element->addInput(inputElement, "output"), Exception);
    }
    SECTION("removeInput() method")
    {
        // Create a mock Element object for testing
        auto element = createSampleNeuralField("Element1");

        // Add input
        auto inputElement = createSampleNeuralField("inputElement");
        REQUIRE(inputElement->getUniqueIdentifier() == "inputElement");
        element->addInput(inputElement, "output");

        // Remove input
        element->removeInput("inputElement");

        // Check if input was removed
        REQUIRE(element->hasInput("inputElement", "output") == false);
    }
    SECTION("updateInput() getComponent() getComponentPtr() methods")
    {
        uint8_t size = 5;
        std::vector<double> expectedInput(size, 0.0);
          
        // Create a mock Element object for testing
        auto element = createSampleNeuralField("Element1", size);

        // Add input
        auto inputElement1 = createSampleNeuralField("inputElement", size);
        element->addInput(inputElement1, "output");

        // Add another input
        auto inputElement2 = createSampleNeuralField("inputElement2", size);
        element->addInput(inputElement2, "output");

        // Update input
        element->updateInput();

        // Check if input was updated
        std::vector<double> actualInput = element->getComponent("input");
        REQUIRE(actualInput == expectedInput);
        std::vector<double> actualInputPtr = *element->getComponentPtr("input");
        REQUIRE(actualInputPtr == expectedInput);

    }
    SECTION("getUniqueIdentifier() method")
    {
        // Create a mock Element object for testing
        auto element = std::make_shared<MockElement>();
        // Check unique identifier
        REQUIRE(element->getUniqueIdentifier() == "");
    }
    SECTION("getLabel() method")
    {
        // Create a mock Element object for testing
        auto element = std::make_shared<MockElement>();
        // Check element label
        REQUIRE(element->getLabel() == ElementLabel::UNINITIALIZED);
    }
    SECTION("setUniqueIdentifier() method")
    {
        // Create a mock Element object for testing
        auto element = std::make_shared<MockElement>();
        // Set unique identifier
        REQUIRE_THROWS_AS(element->setUniqueIdentifier("randomId"), Exception);
    }
    SECTION("setSize() method")
    {
        // Create a mock Element object for testing
        auto element = std::make_shared<MockElement>();
        // Set size
        REQUIRE_THROWS_AS(element->setSize(10), Exception);
    }
    SECTION("getSize() method")
    {
        // Create a mock Element object for testing
        auto element = std::make_shared<MockElement>();
        REQUIRE(element->getSize() == 0);
    }
}