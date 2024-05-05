#include "text.hpp"
#include "pendulum.hpp"
#include "spring.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

// clang-format off
const std::unordered_map<MagnitudesFlags, std::string>Text::s_Naming = {
	{flag_none, ""},
	{flag_time, "Time"},
	{flag_stepTime, "Internal time step"},
	{flag_chosenStepTime, "Time step"},
	{flag_endTime, "End time"},
	{flag_initialCoordinate, "Initial coordinate"},
	{flag_coordinate, "Coordinate"},
	{flag_velocity, "Velocity"},
	{flag_acceleration, "Acceleration"},
	{flag_mass, "Block mass"}, // or Block's mass
	{flag_frictionCoefficient, "Friction coefficient"},
	{flag_springConstant, "Spring constant"},
	{flag_length, "String length"},
	{flag_angularFrequency, "Angular frequency"},
	{flag_period, "Period"},
	{flag_mEnergies, "Full mechanical energy"},
	{flag_mEnergyDeviations, "Energy deviation"},
	{flag_meanMEnergy, "Arithmetic mean of full mechanical energy"},
	{flag_standardDeviationMEnergy, "Standard deviation of arithmetic mean of full mechanical energy"}	
};

const std::unordered_map<MagnitudesFlags, std::string>Text::s_SymbolNaming = {
	{flag_none, ""},
	{flag_time, "t"},
	{flag_stepTime, "dt_in"},
	{flag_chosenStepTime, "dt"},
	{flag_endTime, "t_end"},
	{flag_initialCoordinate, "x0"}, // maybe should add teta0
	{flag_coordinate, "x(t)"},
	{flag_velocity, "v(t)"},
	{flag_acceleration, "a(t)"},
	{flag_mass, "m"},
	{flag_frictionCoefficient, "nu"},
	{flag_springConstant, "k"},
	{flag_length, "L"},
	{flag_angularFrequency, "w0"},
	{flag_period, "T"},
	{flag_mEnergies, "W"},
	{flag_mEnergyDeviations, "W - <W>"},
	{flag_meanMEnergy, "<W>"},
	{flag_standardDeviationMEnergy, "S_<W>"}
};
// clang-format on

Text::Text() : m_precision(3), m_isUseSymbols(false) {
}

Text::~Text() {
}

bool& Text::GetIsUseSymbolsRef() {
    return m_isUseSymbols;
}

int& Text::GetPrecisionRef() {
    return m_precision;
}

int Text::GetPrecision() const {
    return m_precision;
}

std::string Text::GetMagnitudes(BasePhysicalSystem* l_chosenSystem, MagnitudesFlags l_flags) {
    std::string l_stringOutput;

    if (!((l_flags & flag_value) || (l_flags & flag_string))) { // if none of general flags chosen
        if (!m_isUseSymbols) {
            return std::move(s_Naming.at(l_flags));
        } else {
            return std::move(s_SymbolNaming.at(l_flags));
        }
    }
    if (!m_isUseSymbols) {
        if (l_flags & flag_all) {
            BuildAllMagnitudes(l_chosenSystem);
            return std::move(m_AllMagnitudes);
        }
        //!! Perhaps should be removed until the end of function  !!
        // ---------- String ----------
        if (l_flags & flag_string) {
            if (l_flags & flag_mass) l_stringOutput += s_Naming.at(flag_mass);
            if (l_flags & flag_time) l_stringOutput += s_Naming.at(flag_time);
            if (l_flags & flag_stepTime) l_stringOutput += s_Naming.at(flag_stepTime);
            if (l_flags & flag_chosenStepTime) l_stringOutput += s_Naming.at(flag_chosenStepTime);
            if (l_flags & flag_endTime) l_stringOutput += s_Naming.at(flag_endTime);
            if (l_flags & flag_initialCoordinate) l_stringOutput += s_Naming.at(flag_initialCoordinate);
            if (l_flags & flag_coordinate) l_stringOutput += s_Naming.at(flag_coordinate);
            if (l_flags & flag_velocity) l_stringOutput += s_Naming.at(flag_velocity);
            if (l_flags & flag_acceleration) l_stringOutput += s_Naming.at(flag_acceleration);
            if (l_flags & flag_frictionCoefficient) l_stringOutput += s_Naming.at(flag_frictionCoefficient);
            if (l_flags & flag_springConstant) l_stringOutput += s_Naming.at(flag_springConstant);
            if (l_flags & flag_length) l_stringOutput += s_Naming.at(flag_length);
            if (l_flags & flag_angularFrequency) l_stringOutput += s_Naming.at(flag_angularFrequency);
            if (l_flags & flag_period) l_stringOutput += s_Naming.at(flag_period);
            if (l_flags & flag_mEnergies) l_stringOutput += s_Naming.at(flag_mEnergies);
            if (l_flags & flag_mEnergyDeviations) l_stringOutput += s_Naming.at(flag_mEnergyDeviations);
            if (l_flags & flag_meanMEnergy) l_stringOutput += s_Naming.at(flag_meanMEnergy);
            if (l_flags & flag_standardDeviationMEnergy)
                l_stringOutput += s_Naming.at(flag_standardDeviationMEnergy);
        }
    } else { // if m_isUseSymbols
        if (l_flags & flag_time) l_stringOutput += s_SymbolNaming.at(flag_time);
        if (l_flags & flag_stepTime) l_stringOutput += s_SymbolNaming.at(flag_stepTime);
        if (l_flags & flag_chosenStepTime) l_stringOutput += s_SymbolNaming.at(flag_chosenStepTime);
        if (l_flags & flag_endTime) l_stringOutput += s_SymbolNaming.at(flag_endTime);
        if (l_flags & flag_initialCoordinate) l_stringOutput += s_SymbolNaming.at(flag_initialCoordinate);
        if (l_flags & flag_coordinate) l_stringOutput += s_SymbolNaming.at(flag_coordinate);
        if (l_flags & flag_velocity) l_stringOutput += s_SymbolNaming.at(flag_velocity);
        if (l_flags & flag_acceleration) l_stringOutput += s_SymbolNaming.at(flag_acceleration);
        if (l_flags & flag_mass) l_stringOutput += s_SymbolNaming.at(flag_mass);
        if (l_flags & flag_frictionCoefficient) l_stringOutput += s_SymbolNaming.at(flag_frictionCoefficient);
        if (l_flags & flag_springConstant) l_stringOutput += s_SymbolNaming.at(flag_springConstant);
        if (l_flags & flag_length) l_stringOutput += s_SymbolNaming.at(flag_length);
        if (l_flags & flag_angularFrequency) l_stringOutput += s_SymbolNaming.at(flag_angularFrequency);
        if (l_flags & flag_period) l_stringOutput += s_SymbolNaming.at(flag_period);
        if (l_flags & flag_mEnergies) l_stringOutput += s_SymbolNaming.at(flag_mEnergies);
        if (l_flags & flag_mEnergyDeviations) l_stringOutput += s_SymbolNaming.at(flag_mEnergyDeviations);
        if (l_flags & flag_meanMEnergy) l_stringOutput += s_SymbolNaming.at(flag_meanMEnergy);
        if (l_flags & flag_standardDeviationMEnergy)
            l_stringOutput += s_SymbolNaming.at(flag_standardDeviationMEnergy);
    }
    // Add separator
    if (l_flags & (flag_value | flag_string)) {
        if (m_isUseSymbols) {
            l_stringOutput += " = ";
        } else {
            l_stringOutput += ": ";
        }
    }

    // ---------- Value ----------
    if (l_flags & flag_value) {
        if (l_flags & (flag_time | flag_coordinate | flag_velocity | flag_acceleration)) {
            std::cerr << "Text::GetMagnitudes(). Attempt to take time/coordintae/acceleration value. Text class "
                         "should be used to take value from array (vector etc.)\n";
            throw(false);
        }
        if (l_flags & flag_chosenStepTime)
            l_stringOutput += to_string_with_precision(l_chosenSystem->chosenStepTime);
        if (l_flags & flag_endTime) l_stringOutput += to_string_with_precision(l_chosenSystem->endTime);
        if (HarmonicOscillator* l_harmonic = dynamic_cast<HarmonicOscillator*>(l_chosenSystem)) {
            if (l_flags & flag_mass) l_stringOutput += to_string_with_precision(l_harmonic->mass);
            if (l_flags & flag_meanMEnergy)
                l_stringOutput += to_string_with_precision(l_harmonic->meanMEnergy) + " J";
            if (l_flags & flag_standardDeviationMEnergy)
                l_stringOutput += to_string_with_precision(l_harmonic->standardDeviationMEngergy) + " J";
            if (l_flags & flag_period) l_stringOutput += to_string_with_precision(l_harmonic->period) + " s";
            if (l_flags & flag_angularFrequency)
                l_stringOutput += to_string_with_precision(l_harmonic->angularFrequency) + " rad/s";
            // optional TODO: add rest show values
        }
    }
    return std::move(l_stringOutput);
}

std::string Text::to_string_with_precision(const float a_value) {
    std::stringstream out{};
    out << std::fixed << std::setprecision(m_precision) << a_value;
    return out.str();
}

void Text::BuildAllMagnitudes(BasePhysicalSystem* l_chosenSystem) {
    if (!m_isUseSymbols) {
        if (HarmonicOscillator* l_harmonic = dynamic_cast<HarmonicOscillator*>(l_chosenSystem)) {
            m_AllMagnitudes = s_Naming.at(flag_chosenStepTime) + ": " +
                              to_string_with_precision(l_harmonic->chosenStepTime) + " s\n" +
                              s_Naming.at(flag_endTime) + ": " + to_string_with_precision(l_harmonic->endTime) +
                              " s\n" + s_Naming.at(flag_initialCoordinate) + ": " +
                              to_string_with_precision(l_harmonic->initialCoordinate) + " s\n" +
                              s_Naming.at(flag_mass) + ": " + to_string_with_precision(l_harmonic->mass) + " kg\n";
            if (Pendulum* l_pendulum = dynamic_cast<Pendulum*>(l_harmonic)) {
                m_AllMagnitudes +=
                    s_Naming.at(flag_length) + ": " + to_string_with_precision(l_pendulum->length) + " m\n";
            } else {
                SpringMassSystem* l_springMassSystem = dynamic_cast<SpringMassSystem*>(l_harmonic);
                m_AllMagnitudes += s_Naming.at(flag_length) + ": " +
                                   to_string_with_precision(l_springMassSystem->springConstant) + " N/kg\n";
            }
        }
    }
}

// TODO: try to write new function using csv-parser
template<typename T>
void Text::SaveFile(FileOutputFlags l_flags) {
    if (!(l_flags & flag_manyFiles)) {
        if (l_flags & flag_csv) {
            std::ofstream("data.csv");
        }
    }
}
