#include "harmonic.hpp"

HarmonicOscillator::HarmonicOscillator() :
    BasePhysicalSystem(), mass(0), initialCoordinate(1.f), frictionCoefficient(0), angularFrequency(0) {
    m_verticalSplitLine.setSize(sf::Vector2f(5.f, 220.f));
    m_verticalSplitLine.setPosition(sf::Vector2f(640.f, 500.f));
    m_horizontalSplitLine.setSize(sf::Vector2f(640.f, 5.f));
    m_horizontalSplitLine.setPosition(sf::Vector2f(640.f, 500.f));
}

HarmonicOscillator::~HarmonicOscillator() {
}

void HarmonicOscillator::CalculateMagnitudes() {
    // TODO: relocate into basePhiscicalSystem::CalculateMagnitudes()
    // TODO: REMOVE THAT CODE DUPLICATION
    const size_t l_vectorSize = static_cast<size_t>(endTime / stepTime);
    timePoints.clear();
    timePoints.reserve(l_vectorSize);
    timePoints.push_back(0.f);
    coordinates.clear();
    coordinates.reserve(l_vectorSize);
    coordinates.push_back(initialCoordinate);
    velocities.clear();
    velocities.reserve(l_vectorSize);
    velocities.push_back(0.f);
    accelerations.clear();
    accelerations.reserve(l_vectorSize);

    mEnergies.clear();
    mEnergies.reserve(l_vectorSize);

    mEnergyDeviations.clear();
    mEnergyDeviations.reserve(l_vectorSize);
}
