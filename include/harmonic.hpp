#pragma once
#include "base.hpp"
#include <vector>

class HarmonicOscillator : public BasePhysicalSystem {
protected:
    sf::RectangleShape m_verticalSplitLine;
    sf::RectangleShape m_horizontalSplitLine;
    sf::RectangleShape m_stringLine; // cord or spring

public:
    // input variables
    float mass;
    float initialCoordinate;
    float frictionCoefficient;
    // calculate variables
    float angularFrequency;
    float period;

    std::vector<float> coordinates; // angular coordinate (for pendulum) or abscissa (for spring-mass system)
    std::vector<float> velocities;
    std::vector<float> accelerations;
    std::vector<float> mEnergies; // mechanical Energy
    std::vector<float> mEnergyDeviations;
    float meanMEnergy;
    float standardDeviationMEngergy; // of arithmetic mean

    HarmonicOscillator();
    virtual ~HarmonicOscillator() = 0;

    virtual void CalculateMagnitudes() override;
    virtual void ShowAnimation(sf::RenderWindow& l_appWindow, sf::Time l_frameTime) override = 0;
    virtual void TEST() override = 0;
};
