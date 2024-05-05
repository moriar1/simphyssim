#include "base.hpp"

BasePhysicalSystem::BasePhysicalSystem() :
    stepTime(0.005f), m_animationIterator(0), chosenStepTime(0), endTime(25.f) {
}

BasePhysicalSystem::~BasePhysicalSystem() {
}
