#include "manager.hpp"
#include "pendulum.hpp"
#include "spring.hpp"
#include "text.hpp"

#include <imgui-SFML.h>
#include <imgui.h>
#include <implot.h>

void SimulationManager::Render() {
    m_appWindow.clear(sf::Color::Black);

    // ---------- Main menu ----------
    ImGui::BeginMainMenuBar();
    if (ImGui::Button("Spring-Mass System")) {
        delete m_chosenSystem;
        m_chosenSystem = new SpringMassSystem;
        m_chosenSystem->TEST(); // TODO: remove test
    } else if (ImGui::Button("Pendulum")) {
        delete m_chosenSystem;
        m_chosenSystem = new Pendulum;
        m_chosenSystem->TEST(); // TODO: remove test
    }
    ImGui::EndMainMenuBar();

    // ---------- System config ----------
    ImGui::SetNextWindowPos(ImVec2(m_mainViewport->WorkPos.x + 10,
                                   m_mainViewport->WorkPos.y + 10) /*, ImGuiCond_FirstUseEver*/); // TODO:
                                                                                                  // uncomment flag
    ImGui::SetNextWindowSize(
        ImVec2(m_mainViewport->Size.x / 2 - 20, 180) /*, ImGuiCond_FirstUseEver*/); // TODO:
                                                                                    // uncomment
                                                                                    // flag
    ImGui::Begin("Configuration");
    ImGui::Checkbox("Show animation", &m_isShowAnimation);
    ImGui::SameLine();
    bool dummy = 0;
    // Perhaps should be added:
    // ImGui::Checkbox("Show details", &dummy); ImGui::SameLine();
    // ImGui::Checkbox("Show formulas", &dummy); ImGui::SameLine();
    ImGui::Checkbox("Use physical symbols", &m_text.GetIsUseSymbolsRef());
    ImGui::SameLine();
    ImGui::SetNextItemWidth(100);
    ImGui::SliderInt("Precision", &(m_text.GetPrecisionRef()), 0, 6, "%d", ImGuiSliderFlags_AlwaysClamp);
    if (dynamic_cast<HarmonicOscillator*>(m_chosenSystem)) {
        ShowHarmonicOscillator();
    }
    // TODO:
    //	add file output
    //	runtime change plot limits
    //	restart animation button
    // ImGui::SetNextItemWidth(120);
    // ImGui::InputFloat("Time Step", &(dynamic_cast<HarmonicOscillator*>(m_chosenSystem)->chosenStepTime), 0.001f,
    // 0.001f, ("%." + std::to_string(m_precision) + "f").c_str());
    ImGui::TextWrapped("Tip: press ctrl and click on slider line to enter a value");
    ImGui::End();

    // ---------- Details ----------
    ImGui::SetNextWindowPos(
        ImVec2(m_mainViewport->WorkPos.x + 10,
               m_mainViewport->WorkPos.y + 10 + 560) /*, ImGuiCond_FirstUseEver*/); // TODO:
                                                                                    // uncomment
                                                                                    // flag
    ImGui::SetNextWindowSize(
        ImVec2(m_mainViewport->Size.x / 2 - 20, 120) /*, ImGuiCond_FirstUseEver*/); // TODO:
                                                                                    // uncomment
                                                                                    // flag
    if (dynamic_cast<HarmonicOscillator*>(m_chosenSystem)) {
        ImGui::Begin("Data");
        ImGui::SetWindowCollapsed(true, ImGuiCond_FirstUseEver);
        // ImGui::TextWrapped("End time / 0.005 = %d\nNote: if you chose more than one plot, (End time / Step time)
        // must "
        //                    "be no more than 5000 to show plot\n"
        //                    "Hide plot if (End time / 0.005) > 5000\n\n",
        //                    static_cast<int>(m_chosenSystem->endTime / 0.005));

        // Uncomment if show values in Data window is required
        // ImGui::TextWrapped((m_text.GetMagnitudes(m_chosenSystem, flag_all)).c_str());
        ImGui::TextWrapped("%s", m_text.GetMagnitudes(m_chosenSystem, flag_period | flag_full).c_str());
        ImGui::TextWrapped("%s", m_text.GetMagnitudes(m_chosenSystem, flag_angularFrequency | flag_full).c_str());
        ImGui::TextWrapped("%s", m_text.GetMagnitudes(m_chosenSystem, flag_meanMEnergy | flag_full).c_str());
        ImGui::TextWrapped("%s", m_text.GetMagnitudes(m_chosenSystem, flag_standardDeviationMEnergy | flag_full)
                                     .c_str());
        ImGui::TextWrapped("\nContainer size: %d",
                           static_cast<int>(m_chosenSystem->endTime / m_chosenSystem->stepTime));
        ImGui::TextWrapped("Table size: %d",
                           static_cast<int>(m_chosenSystem->endTime / m_chosenSystem->chosenStepTime));
        if (ImGui::InputFloat(m_text.GetMagnitudes(m_chosenSystem, flag_stepTime).c_str(),
                              &(m_chosenSystem->stepTime), 0.000001f, 0.00001f, "%.6f m")) {
            m_chosenSystem->CalculateMagnitudes();
        }

        ImGui::End();
    }
    // ---------- Animation ----------
    if (m_isShowAnimation) {
        m_chosenSystem->ShowAnimation(m_appWindow, m_frameTime);
    }
    // ImGui::Render(); dunno what it does
    ImGui::SFML::Render(m_appWindow);
    m_appWindow.display();
}

void SimulationManager::ShowHarmonicOscillator() {
    HarmonicOscillator* l_harmonic = dynamic_cast<HarmonicOscillator*>(m_chosenSystem);
    // dynamic first slider
    const char* l_tempUnits = "nullptr"; // coordinate units

    bool isSpecialValueChanged = false;
    if (SpringMassSystem* l_sprMass = dynamic_cast<SpringMassSystem*>(l_harmonic)) {
        if (ImGui::SliderFloat(m_text.GetMagnitudes(m_chosenSystem, flag_springConstant).c_str(),
                               &l_sprMass->springConstant, 0.0f, 10.0f,
                               ("%." + std::to_string(m_text.GetPrecision()) + "f N/m").c_str()))
            isSpecialValueChanged = true;
        l_tempUnits = "m";

    } else if (Pendulum* l_pend = dynamic_cast<Pendulum*>(l_harmonic)) {
        if (ImGui::SliderFloat(m_text.GetMagnitudes(m_chosenSystem, flag_length).c_str(), &l_pend->length, 0.0f,
                               10.0f, ("%." + std::to_string(m_text.GetPrecision()) + "f m").c_str()))
            isSpecialValueChanged = true;
        l_tempUnits = "rad";
    }

    // ---------- Sliders and Buttons ----------
    if (isSpecialValueChanged ||
        ImGui::SliderFloat(m_text.GetMagnitudes(l_harmonic, flag_mass).c_str(), &(l_harmonic->mass), 0.0f, 10.0f,
                           ("%." + std::to_string(m_text.GetPrecision()) + "f kg").c_str()) ||
        ImGui::SliderFloat(m_text.GetMagnitudes(l_harmonic, flag_initialCoordinate).c_str(),
                           &(l_harmonic->initialCoordinate), 0.0f, 1.570796f,
                           ("%." + std::to_string(m_text.GetPrecision()) + "f " + l_tempUnits).c_str()) ||
        ImGui::SliderFloat(m_text.GetMagnitudes(l_harmonic, flag_frictionCoefficient).c_str(),
                           &(l_harmonic->frictionCoefficient), 0.0f, 1.f,
                           ("%." + std::to_string(m_text.GetPrecision()) + "f").c_str())
        // TODO
    ) {
        l_harmonic->CalculateMagnitudes();
    }

    // ---------- Plot ----------
    size_t l_limStep = static_cast<size_t>(m_chosenSystem->endTime / m_chosenSystem->stepTime);
    if (l_limStep * m_isShowPlotLines[0] + l_limStep * m_isShowPlotLines[1] + l_limStep * m_isShowPlotLines[2] <
        16383) { // if sum amount of points of plot >= 16383 program crashes:
        ImGui::SetNextWindowPos(
            ImVec2(m_mainViewport->WorkPos.x + 10,
                   m_mainViewport->WorkPos.y + 10 + 190) /*, ImGuiCond_FirstUseEver*/); // TODO:
                                                                                        // uncomment
                                                                                        // flag
        ImGui::SetNextWindowSize(
            ImVec2(m_mainViewport->Size.x / 2 - 20, 360) /*, ImGuiCond_FirstUseEver*/); // TODO:
                                                                                        // uncomment
                                                                                        // flag
        ImPlot::CreateContext();
        if (ImGui::Begin("Plot")) {
            ImGui::SetWindowCollapsed(true, ImGuiCond_FirstUseEver);
            ImGui::Checkbox(m_text.GetMagnitudes(m_chosenSystem, flag_coordinate).c_str(), &m_isShowPlotLines[0]);
            ImGui::SameLine();
            ImGui::Checkbox(m_text.GetMagnitudes(m_chosenSystem, flag_velocity).c_str(), &m_isShowPlotLines[1]);
            ImGui::SameLine();
            ImGui::Checkbox(m_text.GetMagnitudes(m_chosenSystem, flag_acceleration).c_str(),
                            &m_isShowPlotLines[2]);

            ImPlot::BeginPlot("The dependence of the coordinate on time");
            ImPlot::SetupAxes("Time", "" /*"Coordinate", ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_AutoFit*/);
            // !TODO: l_axesLimits = ...
            // TODO: set SetupAxesLimits autoFit
            if (m_isShowPlotLines[0]) {
                ImPlot::SetupAxesLimits(0, l_harmonic->endTime, -l_harmonic->initialCoordinate,
                                        l_harmonic->initialCoordinate);
            }
            if (m_isShowPlotLines[1]) {
                float l_maxVelocity = l_harmonic->angularFrequency > 1 ?
                                          l_harmonic->initialCoordinate * l_harmonic->angularFrequency :
                                          l_harmonic->initialCoordinate;
                ImPlot::SetupAxesLimits(0, l_harmonic->endTime, -l_maxVelocity, l_maxVelocity);
            }
            if (m_isShowPlotLines[2]) {
                ImPlot::SetupAxesLimits(0, l_harmonic->endTime, -l_harmonic->accelerations[0],
                                        l_harmonic->accelerations[0]);
                ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(1, 0, 0, 1));
                ImPlot::PlotLine("a(t)", l_harmonic->timePoints.data(), l_harmonic->accelerations.data(),
                                 static_cast<int>(l_harmonic->timePoints.size()));
                ImPlot::PopStyleColor();
            }
            if (m_isShowPlotLines[0]) {
                ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(0, 0, 0, -1));
                ImPlot::PlotLine("x(t)", l_harmonic->timePoints.data(), l_harmonic->coordinates.data(),
                                 static_cast<int>(l_harmonic->timePoints.size()));
                ImPlot::PopStyleColor();
            }
            if (m_isShowPlotLines[1]) {
                ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(1, 0, 1, 1));
                ImPlot::PlotLine("v(t)", l_harmonic->timePoints.data(), l_harmonic->velocities.data(),
                                 static_cast<int>(l_harmonic->timePoints.size()));
                ImPlot::PopStyleColor();
            }
            ImPlot::EndPlot();
        }
        ImGui::End();
        ImPlot::DestroyContext(); // because of this plot mouse input won't work
    }
    // ---------- Table ----------
    ImGui::SetNextWindowSize(
        ImVec2(m_mainViewport->Size.x / 2 - 10, 460) /*, ImGuiCond_FirstUseEver*/); // TODO:
                                                                                    // uncomment
                                                                                    // flag
    ImGui::SetNextWindowPos(ImVec2(m_mainViewport->Size.x / 2,
                                   m_mainViewport->WorkPos.y + 10) /*, ImGuiCond_FirstUseEver*/); // TODO:
                                                                                                  // uncomment flag
    ImGui::Begin("Table");
    ImGui::SetWindowCollapsed(true, ImGuiCond_FirstUseEver);
    ImGui::SetNextItemWidth(250);
    // Time sliders
    if (ImGui::SliderFloat(m_text.GetMagnitudes(l_harmonic, flag_chosenStepTime).c_str(),
                           &(l_harmonic->chosenStepTime), 0.005f, 1.0f,
                           ("%." + std::to_string(m_text.GetPrecision()) + "f s").c_str())) {
        l_harmonic->CalculateMagnitudes();
        // ImGui::End();
        // return;
    }
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    if (ImGui::SliderFloat(m_text.GetMagnitudes(l_harmonic, flag_endTime).c_str(), &(l_harmonic->endTime), 0.005f,
                           60.0f, ("%." + std::to_string(m_text.GetPrecision()) + "f s").c_str())) {
        l_harmonic->CalculateMagnitudes();
    }

    // Table
    if (ImGui::BeginTable("split", 6,
                          ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable |
                              ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_Reorderable |
                              ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn(m_text.GetMagnitudes(m_chosenSystem, flag_time).c_str());
        ImGui::TableSetupColumn(m_text.GetMagnitudes(m_chosenSystem, flag_coordinate).c_str());
        ImGui::TableSetupColumn(m_text.GetMagnitudes(m_chosenSystem, flag_velocity).c_str());
        ImGui::TableSetupColumn(m_text.GetMagnitudes(m_chosenSystem, flag_acceleration).c_str());
        ImGui::TableSetupColumn(m_text.GetMagnitudes(m_chosenSystem, flag_mEnergies).c_str());
        ImGui::TableSetupColumn(m_text.GetMagnitudes(m_chosenSystem, flag_mEnergyDeviations).c_str());

        ImGui::TableHeadersRow();

        size_t l_chosenSetSize = static_cast<size_t>(l_harmonic->endTime / l_harmonic->chosenStepTime);
        if (l_chosenSetSize == 0) {
            ImGui::EndTable();
            ImGui::End();
            return;
        }
        size_t l_iteratorStep = l_harmonic->coordinates.size() / l_chosenSetSize;
        for (size_t i = 0; i <= l_harmonic->coordinates.size() - l_iteratorStep; i += l_iteratorStep) {
            ImGui::TableNextColumn();
            ImGui::Text(("%." + std::to_string(m_text.GetPrecision()) + "f ").c_str(), l_harmonic->timePoints[i]);
            ImGui::TableNextColumn();
            ImGui::Text(("%." + std::to_string(m_text.GetPrecision()) + "f ").c_str(), l_harmonic->coordinates[i]);
            ImGui::TableNextColumn();
            ImGui::Text(("%." + std::to_string(m_text.GetPrecision()) + "f ").c_str(), l_harmonic->velocities[i]);
            ImGui::TableNextColumn();
            ImGui::Text(("%." + std::to_string(m_text.GetPrecision()) + "f ").c_str(),
                        l_harmonic->accelerations[i]);
            ImGui::TableNextColumn();
            ImGui::Text(("%." + std::to_string(m_text.GetPrecision()) + "f ").c_str(), l_harmonic->mEnergies[i]);
            // ImGui::Text("d");
            ImGui::TableNextColumn();

            // ImGui::Text("d");
            ImGui::Text(("%." + std::to_string(m_text.GetPrecision()) + "f ").c_str(),
                        l_harmonic->mEnergyDeviations[i]);
        }
        ImGui::EndTable();
    }
    ImGui::End();
}

// Welcome Screen implementation attempt
// void SimulationManager::ShowWelcomeScreen() {
//	ImGui::SetNextWindowPos(ImVec2(m_mainViewport->WorkPos.x + m_mainViewport->Size.x / 2 - 100,
// m_mainViewport->WorkPos.y + m_mainViewport->Size.y /2 - 100));
//	ImGui::SetNextWindowSize(ImVec2(m_mainViewport->Size.x / 2, m_mainViewport->Size.y / 2));
//	if (ImGui::Begin("Welcome")) {
//		ImGui::SetCursorPos(ImVec2(200, 200));
//		if (ImGui::Button("Spring-Mass system ", ImVec2(200, 100))) {
//			m_isShowWelcomeScreen = false;
//		}
//
//		ImGui::End();
//	}
// }
/*
Render():
if (m_isShowWelcomeScreen) {
        ShowWelcomeScreen();
        ImGui::SFML::Render(m_appWindow);
        m_appWindow.display();
        return;
        }
*/
//
/////////////////////////////////////////////////////////

SimulationManager::SimulationManager() :
    m_appWindow(sf::VideoMode(1280, 720), "Simulation"),
    m_text(),
    m_chosenSystem(new SpringMassSystem),
    m_isShowPlotLines{},
    m_isShowAnimation(false) {
    if (!ImGui::SFML::Init(m_appWindow)) {
        throw("manager.cpp constructor error");
    }
    m_elapsed = sf::Time::Zero;
    m_frameTime = sf::seconds(1.f / 75.f);
    m_mainViewport = ImGui::GetMainViewport(); // do not move to initialize list
    m_isShowPlotLines[0] = true;
}

SimulationManager::~SimulationManager() {
    delete m_chosenSystem;
}

void SimulationManager::run() {
    m_chosenSystem->TEST(); // TODO: remove test

    while (m_appWindow.isOpen()) {
        // ---------- Process Event ----------
        sf::Event event;
        while (m_appWindow.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(m_appWindow, event);
            if (event.type == sf::Event::Closed) {
                m_appWindow.close();
            }
        }
        if (m_elapsed > m_frameTime) {
            m_elapsed -= m_frameTime;
            ImGui::SFML::Update(m_appWindow, m_frameTime);
            // ImGui::ShowDemoWindow();
            Render();
        }
        m_elapsed += m_clock.restart();
        if (!m_isShowAnimation) sf::sleep(sf::seconds(0.013f));
        // Comment "if (!m_isShowAnimation)" to reduce CPU Utilization
    }
    ImGui::SFML::Shutdown();
}
