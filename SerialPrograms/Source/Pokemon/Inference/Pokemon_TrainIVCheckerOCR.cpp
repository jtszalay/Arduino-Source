/*  Train IV Checker OCR Data
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "Common/Cpp/Concurrency/ParallelTaskRunner.h"
#include "CommonFramework/Globals.h"
#include "CommonFramework/OCR/OCR_TrainingTools.h"
#include "CommonFramework/Tools/ProgramEnvironment.h"
#include "Pokemon/Pokemon_Strings.h"
#include "Pokemon_IVCheckerReader.h"
#include "Pokemon_TrainIVCheckerOCR.h"

namespace PokemonAutomation{
namespace Pokemon{


TrainIVCheckerOCR_Descriptor::TrainIVCheckerOCR_Descriptor()
    : ComputerProgramDescriptor(
        "PokemonSwSh:TrainIVCheckerOCR",
        STRING_POKEMON, "Train IV Checker OCR",
        "",
        "Train IV Checker OCR"
    )
{}



TrainIVCheckerOCR::TrainIVCheckerOCR()
    : DIRECTORY(
        false,
        LockWhileRunning::LOCKED,
        "<b>Training Data Directory:</b> (Relative to \"TrainingData/\")",
        "IVCheckerOCR/",
        "IVCheckerOCR/"
    )
    , THREADS(
        "<b>Worker Threads:</b>",
        LockWhileRunning::LOCKED,
        std::thread::hardware_concurrency()
    )
{
    PA_ADD_OPTION(DIRECTORY);
    PA_ADD_OPTION(MODE);
    PA_ADD_OPTION(THREADS);
}



void TrainIVCheckerOCR::program(ProgramEnvironment& env, CancellableScope& scope){
    OCR::TrainingSession session(env.logger(), scope, DIRECTORY);
    session.generate_small_dictionary(
        "Pokemon/IVCheckerOCR.json",
        "IVCheckerOCR.json",
        MODE == TrainOCRMode::Incremental, THREADS,
        OCR::BLACK_TEXT_FILTERS(),
        IVCheckerReader::MAX_LOG10P,
        IVCheckerReader::MAX_LOG10P_SPREAD
    );
}



}
}
