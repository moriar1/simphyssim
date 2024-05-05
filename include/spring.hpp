#pragma once
#include "harmonic.hpp"

class SpringMassSystem : public HarmonicOscillator {
private:
    sf::RectangleShape m_blockRect;

public:
    float springConstant;
    SpringMassSystem();
    ~SpringMassSystem();
    void CalculateMagnitudes() override;
    void ShowAnimation(sf::RenderWindow& l_appWindow, sf::Time l_frameTime) override;
    void TEST() override;
};
