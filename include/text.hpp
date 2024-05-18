#pragma once
#include "base.hpp"

#include <string>
#include <unordered_map>

/*
 *
 * NOTE: flag_string automatically chooses if chosen flag_magnitude single flag
 * (i.e. if none of these flags: flag_all, flag_string, flag_value, flag_full chosen, flag_string adds implicitly
 * Example:
 * - get(flag_time) means get(flag_time | flag_string)
 * - get(length) means get(length | flag_string) etc.
 *
 */
// clang-format off
typedef int MagnitudesFlags;
enum MagnitudesFlags_ {
	flag_none                      = 0,
	flag_time                      = 1 << 0,
	flag_stepTime                  = 1 << 1,
	flag_chosenStepTime            = 1 << 2,
	flag_endTime                   = 1 << 3,
	flag_initialCoordinate         = 1 << 4,
	flag_coordinate                = 1 << 5,
	flag_velocity                  = 1 << 6,
	flag_acceleration              = 1 << 7,
	flag_mass                      = 1 << 8, 
	flag_frictionCoefficient       = 1 << 9,
	flag_springConstant            = 1 << 10,
	flag_length                    = 1 << 11,
	flag_angularFrequency          = 1 << 12,
	flag_period                    = 1 << 13,
	flag_mEnergies                 = 1 << 14,
	flag_mEnergyDeviations         = 1 << 15,
	flag_meanMEnergy               = 1 << 16,
	flag_standardDeviationMEnergy  = 1 << 17,

	flag_all                       = 1 << 26, // Return flag_all magnitudes in one flag_string: name/symbol + flag_value separated by '\n'
	flag_string                    = 1 << 27, // Return (name/symbol) without flag_value.
	flag_value                     = 1 << 28, // Return flag_value of quantity without (name/symbol)
	//flag_units
	flag_full = flag_string | flag_value // Return (name/symbol) and value of target quantity with units
};

typedef int FileOutputFlags;
enum FileOutputFlags_ {
	flag_csv                       = 1 << 29,
	flag_txt                       = 1 << 30,
	flag_manyFiles                 = 1 << 31
};
// clang-format on
class Text {
    int m_precision;
    bool m_isUseSymbols;
    std::string m_AllMagnitudes;
    std::string m_AllMagnitudesSymbols;

    void BuildAllMagnitudes(BasePhysicalSystem* l_chosenSystem);
    std::string to_string_with_precision(const float a_value);
    const static std::unordered_map<MagnitudesFlags, std::string> s_Naming;
    const static std::unordered_map<MagnitudesFlags, std::string> s_SymbolNaming;

public:
    Text();
    ~Text();
    bool& GetIsUseSymbolsRef();
    int& GetPrecisionRef();
    int GetPrecision() const;
    std::string GetMagnitudes(BasePhysicalSystem* l_chosenSystem, MagnitudesFlags l_flags);

    void SaveFile(BasePhysicalSystem* l_chosenSystem, FileOutputFlags l_flags = 0);
};
