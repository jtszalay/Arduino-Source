/*  Pokemon Home Generate Name OCR
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_PokemonHome_GenerateNameOCR_H
#define PokemonAutomation_PokemonHome_GenerateNameOCR_H

#include "Common/Cpp/Options/TimeExpressionOption.h"
#include "CommonFramework/Options/LanguageOCROption.h"
#include "NintendoSwitch/NintendoSwitch_SingleSwitchProgram.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonHome{


class GenerateNameOCRData_Descriptor : public SingleSwitchProgramDescriptor{
public:
    GenerateNameOCRData_Descriptor();
};



class GenerateNameOCRData : public SingleSwitchProgramInstance{

public:
    GenerateNameOCRData();

    virtual void program(SingleSwitchProgramEnvironment& env, BotBaseContext& context) override;

private:
    OCR::LanguageOCR LANGUAGE;
    TimeExpressionOption<uint16_t> DELAY;
};



}
}
}
#endif
