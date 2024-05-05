#pragma once
#include "harmonic.hpp"

class Pendulum : public HarmonicOscillator {
private:
    sf::CircleShape m_bobCircle; // weight on end of cord
public:
    float length;
    Pendulum();
    void CalculateMagnitudes() override;
    virtual void ShowAnimation(sf::RenderWindow& l_appWindow, sf::Time l_frameTime) override;
    void TEST() override;
};
