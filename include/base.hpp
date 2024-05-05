#pragma once
#include <SFML/Graphics.hpp>

class BasePhysicalSystem {
protected:
    size_t m_animationIterator; // needs to deifine position of block of mass on
                                // screen at particular flag_time
public:
    float stepTime; // dt to build vectors<float>
    float chosenStepTime; // dt to show on screen
    float endTime;
    std::vector<float> timePoints; // TODO: remove
    BasePhysicalSystem();
    virtual ~BasePhysicalSystem() = 0;
    virtual void CalculateMagnitudes() = 0;
    virtual void ShowAnimation(sf::RenderWindow&, sf::Time) = 0; // optional TODO: check
                                                                 // ImPlot->CustomRendering()
    virtual void TEST() = 0;
};
