#include "pendulum.hpp"
#include <cmath>

Pendulum::Pendulum() : HarmonicOscillator(), length(1.f) {
    m_stringLine.setSize(sf::Vector2f(150.f, 10.f));
    m_stringLine.setOrigin(0, m_stringLine.getSize().y / 2);
    m_stringLine.setPosition(960, 503);
    m_bobCircle.setRadius(20.f);
    m_bobCircle.setOrigin(m_bobCircle.getRadius(), m_bobCircle.getRadius());
}

void Pendulum::CalculateMagnitudes() {
    angularFrequency = std::sqrt(9.80665f / length); // w0 = sqrt(g/L)
    period = 2.f * 3.141593f / angularFrequency;

    HarmonicOscillator::CalculateMagnitudes(); // clear vectors and push first zeros
    const size_t l_vectorSize = static_cast<size_t>(endTime / stepTime);

    float l_time = 0.f;
    float l_beta = frictionCoefficient / (2 * mass); // beta = b/2m
    accelerations[0] = -9.80665f / length * sin(coordinates[0]);
    for (size_t i = 1; i < l_vectorSize; i++) {
        accelerations.push_back(-2.f * l_beta * velocities[i - 1] -
                                (9.80665f / length) * sin(coordinates[i - 1])); // a = -2*beta*w - w0^2sin(teta)

        velocities.push_back(velocities[i - 1] + accelerations[i] * stepTime); // v += a * dt;
        coordinates.push_back(coordinates[i - 1] + velocities[i] * stepTime); // x += v * dt;
        timePoints.push_back(timePoints[i - 1] + stepTime);
        l_time += stepTime; // t += dt;
    }

    // Calculate standard deviation
    double l_sumEnergy = 0.;
    for (size_t i = 0; i != l_vectorSize; i++) {
        float l_kEnergy = mass * velocities[i] * velocities[i] / 2.f;
        float l_pEnergy = mass * 9.80665f * (length - length * std::cos(coordinates[i])); // mgh = mg(l-lcos(teta))
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

void Pendulum::ShowAnimation(sf::RenderWindow& l_appWindow, sf::Time l_frameTime) {
    float l_animationSpeed = coordinates.size() * l_frameTime.asSeconds() / 13.f; // may remove "animation_speed"
    if (m_animationIterator * l_animationSpeed >= coordinates.size() - 1ULL) m_animationIterator = 0;

    m_stringLine.setRotation(
        90.f + 57.2957795f * coordinates[static_cast<size_t>(l_animationSpeed * m_animationIterator++)]);
    l_appWindow.draw(m_stringLine);

    m_bobCircle.setPosition(sf::Vector2f(m_stringLine.getPosition().x +
                                             m_stringLine.getSize().x *
                                                 std::cos(0.017453f * (m_stringLine.getRotation())), // x +
                                                                                                     // L*cos(teta)
                                         m_stringLine.getPosition().y +
                                             m_stringLine.getSize().x *
                                                 std::sin(0.017453f * (m_stringLine.getRotation())) // y +
                                                                                                    // L*cos(teta)
                                         ));
    l_appWindow.draw(m_bobCircle);

    l_appWindow.draw(m_horizontalSplitLine);
    l_appWindow.draw(m_verticalSplitLine);
}

///////////////////////////////////////
// #include <iomanip>
// #include <iostream>

void Pendulum::TEST() {
    length = 1.f;
    endTime = 25.f;
    mass = 1.f;
    frictionCoefficient = 0.2f;
    stepTime = 0.005f;
    chosenStepTime = 0.05f;
    initialCoordinate = 1.f;
    CalculateMagnitudes();

    /*for (size_t i = 0; i != endTime / stepTime; i++) {
            std::cout << std::fixed << std::setprecision(6) << coordinates[i] << std::endl;
    }*/
}
