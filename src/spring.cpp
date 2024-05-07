#include "spring.hpp"
#include <cmath>
// 1clang-format off
SpringMassSystem::SpringMassSystem() : HarmonicOscillator(), springConstant(0.f) {
    CalculateMagnitudes();
    m_blockRect.setSize(sf::Vector2f(80.f, 60.f));
    m_stringLine.setRotation(180.f);
}

SpringMassSystem::~SpringMassSystem() {
}

void SpringMassSystem::CalculateMagnitudes() {
    angularFrequency = std::sqrt(springConstant / mass);
    period = 2.f * 3.141593f / angularFrequency;

    HarmonicOscillator::CalculateMagnitudes(); // clear vectors and push first zeros
    const size_t l_vectorSize = static_cast<size_t>(endTime / stepTime);
    accelerations[0] = -springConstant * coordinates[0] / mass;

    float l_time = 0.f;
    for (size_t i = 1; i < l_vectorSize; i++) {
        accelerations.push_back(-springConstant * coordinates[i - 1] / mass -
                                frictionCoefficient * velocities[i - 1]); // a = - w0^2 * x - mu * v;

        velocities.push_back(velocities[i - 1] + accelerations[i] * stepTime); // v += a * dt;
        coordinates.push_back(coordinates[i - 1] + velocities[i] * stepTime); // x += v * dt;
        timePoints.push_back(timePoints[i - 1] + stepTime);
        l_time += stepTime; // t += dt;
    }

    // Calculate standard deviation
    double l_sumEnergy = 0.;
    for (size_t i = 0; i != l_vectorSize; i++) {
        float l_kEnergy = mass * velocities[i] * velocities[i] / 2;
        float l_pEnergy = springConstant * coordinates[i] * coordinates[i] / 2;
        mEnergies.push_back(l_kEnergy + l_pEnergy);
        l_sumEnergy += static_cast<double>(mEnergies[i]);
    }
    meanMEnergy = l_sumEnergy / l_vectorSize;

    double l_sumSquaredDeviations = 0.;

    for (size_t i = 0; i != l_vectorSize; i++) {
        float l_deviation = mEnergies[i] - meanMEnergy;
        mEnergyDeviations.push_back(l_deviation);
        l_sumSquaredDeviations += l_deviation * l_deviation;
    }
    standardDeviationMEngergy =
        static_cast<float>(std::sqrt(l_sumSquaredDeviations / (l_vectorSize * (l_vectorSize - 1))));
}

void SpringMassSystem::ShowAnimation(sf::RenderWindow& l_appWindow, sf::Time l_frameTime) {
    float l_animationSpeed = coordinates.size() * l_frameTime.asSeconds() / 13.f; // may remove "animation_speed"
    if (m_animationIterator * l_animationSpeed >= coordinates.size() - 1ULL) m_animationIterator = 0;
    float l_scale = 320.f / initialCoordinate / (1.4f);

    m_blockRect.setPosition(
        sf::Vector2f(coordinates[static_cast<size_t>(l_animationSpeed * m_animationIterator++)] * l_scale + 960.f,
                     600.f));
    l_appWindow.draw(m_blockRect);

    l_appWindow.draw(m_horizontalSplitLine);
    l_appWindow.draw(m_verticalSplitLine);

    m_stringLine.setSize(sf::Vector2f(640.f - m_blockRect.getPosition().x, 5.f));
    m_stringLine.setPosition(sf::Vector2f(640.f, 600.f + m_blockRect.getSize().y / 2.f));
    l_appWindow.draw(m_stringLine);
}

//////////////////////////////////////////////////////////////////
// #include <iomanip>
// #include <iostream>
void SpringMassSystem::TEST() {
    springConstant = 2.f;
    endTime = 25.f;
    mass = 1.f;
    frictionCoefficient = 0.2f;
    stepTime = 0.005f;
    chosenStepTime = 0.05f;
    initialCoordinate = 1.f;
    CalculateMagnitudes();

    // for (size_t i = 0; i != endTime / stepTime; i++) {
    // std::cout << std::fixed << std::setprecision(6) << coordinates[i] << std::endl;
    //}
}
// 1clang-format on
