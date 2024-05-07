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
    const size_t l_vectorSize = static_cast<size_t>(endTime / stepTime);
    // clang-format off
    // or just auto l_vectors = {..
    std::vector<std::reference_wrapper<std::vector<float>>> l_vectors = {std::ref(timePoints),
        std::ref(coordinates), std::ref(velocities), std::ref(accelerations),
        std::ref(mEnergies),std::ref(mEnergyDeviations)
    };
    // clang-format on

    for (auto& it : l_vectors) {
        it.get().clear();
        it.get().reserve(l_vectorSize);
        it.get().push_back(0.f);
    }
    coordinates[0] = initialCoordinate;
}
