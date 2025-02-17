/*  Visual Inference Routines
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "Common/Cpp/Exceptions.h"
#include "ClientSource/Connection/BotBase.h"
#include "InferenceSession.h"
#include "InferenceRoutines.h"

#include <iostream>
using std::cout;
using std::endl;

namespace PokemonAutomation{



int wait_until(
    ConsoleHandle& console, BotBaseContext& context,
    WallClock deadline,
    const std::vector<PeriodicInferenceCallback>& callbacks,
    std::chrono::milliseconds default_video_period,
    std::chrono::milliseconds default_audio_period
){
    BotBaseContext subcontext(context, console.botbase());
    InferenceSession session(
        subcontext, console,
        callbacks,
        default_video_period, default_audio_period
    );

    try{
        subcontext.wait_until(deadline);
    }catch (OperationCancelledException&){}

    context.throw_if_cancelled();

    return session.triggered_index();
}




int run_until(
    ConsoleHandle& console, BotBaseContext& context,
    std::function<void(BotBaseContext& context)>&& command,
    const std::vector<PeriodicInferenceCallback>& callbacks,
    std::chrono::milliseconds default_video_period,
    std::chrono::milliseconds default_audio_period
){
    BotBaseContext subcontext(context, console.botbase());
    InferenceSession session(
        subcontext, console,
        callbacks,
        default_video_period, default_audio_period
    );

    try{
        command(subcontext);
        subcontext.wait_for_all_requests();
    }catch (OperationCancelledException&){}

    context.throw_if_cancelled();

    return session.triggered_index();
}



}



