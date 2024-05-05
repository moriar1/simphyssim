#pragma once
#include "text.hpp"

#include <imgui.h>

class SimulationManager {
    sf::RenderWindow m_appWindow;
    ImGuiViewport* m_mainViewport; // may be replaced with m_appWindow.getSize(). in code
    sf::Clock m_clock;
    sf::Time m_elapsed;
    sf::Time m_frameTime;

    Text m_text;
    BasePhysicalSystem* m_chosenSystem;
    bool m_isShowPlotLines[3]; // 0 - coordinate, 1 - velocity, 2 - acceleration
    bool m_isShowAnimation;

    void Render();
    void ShowHarmonicOscillator(); // TODO: relocate into HarmonicOscillator class
public:
    SimulationManager();
    ~SimulationManager();
    void run();
};
