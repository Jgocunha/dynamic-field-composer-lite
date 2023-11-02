#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "elements/gauss_stimulus.h"

TEST_CASE("GaussStimulus class tests", "[GaussStimulus]")
{
    // Create a GaussStimulus object for testing
    std::string id = "testGaussStimulus";
    uint8_t size = 3;
    double sigma = 1.0;
    double amplitude = 2.0;
    double position = 0.5;
    GaussStimulusParameters gsp{sigma, amplitude, position};

    SECTION("GaussStimulus constructor getParameters() method")
    {
        GaussStimulus gaussStimulus(id, size, gsp);

        REQUIRE(gaussStimulus.getLabel() == ElementLabel::GAUSS_STIMULUS);
        REQUIRE(gaussStimulus.getUniqueName() == id);
        REQUIRE(gaussStimulus.getSize() == size);
        REQUIRE(gaussStimulus.getComponent("output").size() == size);

        REQUIRE(gaussStimulus.getParameters() == gsp);

        // invalid position
        gsp.position = position + size;
        REQUIRE_THROWS_AS(GaussStimulus::GaussStimulus(id, size, gsp), Exception);
    }

    SECTION("init() method")
    {
        GaussStimulus gaussStimulus(id, size, gsp);

        // Call init() method
        gaussStimulus.init();

        // Check if components["output"] has correct values
        REQUIRE(gaussStimulus.getComponent("output")[0] == Catch::Approx(amplitude * mathtools::circularGauss(size, sigma, position)[0]));
        REQUIRE(gaussStimulus.getComponent("output")[1] == Catch::Approx(amplitude * mathtools::circularGauss(size, sigma, position)[1]));
        REQUIRE(gaussStimulus.getComponent("output")[2] == Catch::Approx(amplitude * mathtools::circularGauss(size, sigma, position)[2]));
    }

    SECTION("setParameters() method")
    {
        GaussStimulus gaussStimulus(id, size, gsp);
        double newSigma = 1.1;
        double newAmplitude = 3.0;
        double newPosition = 1.5;
        GaussStimulusParameters newgsp{ newSigma, newAmplitude, newPosition};

        gaussStimulus.setParameters(newgsp);

        // Check if parameters were updated and init() was called
        REQUIRE(gaussStimulus.getParameters() == newgsp);
        REQUIRE(gaussStimulus.getComponent("output")[0] == Catch::Approx(newAmplitude * mathtools::circularGauss(size, newSigma, newPosition)[0]));
        REQUIRE(gaussStimulus.getComponent("output")[1] == Catch::Approx(newAmplitude * mathtools::circularGauss(size, newSigma, newPosition)[1]));
        REQUIRE(gaussStimulus.getComponent("output")[2] == Catch::Approx(newAmplitude * mathtools::circularGauss(size, newSigma, newPosition)[2]));
    }
}