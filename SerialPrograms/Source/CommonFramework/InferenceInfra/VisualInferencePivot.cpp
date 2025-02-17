/*  Visual Inference Pivot
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "Common/Cpp/Exceptions.h"
#include "Common/Cpp/PrettyPrint.h"
#include "CommonFramework/VideoPipeline/VideoFeed.h"
#include "VisualInferencePivot.h"

#include <iostream>
using std::cout;
using std::endl;

namespace PokemonAutomation{



struct VisualInferencePivot::PeriodicCallback{
    Cancellable& scope;
    std::atomic<InferenceCallback*>* set_when_triggered;
    VisualInferenceCallback& callback;
    std::chrono::milliseconds period;
    StatAccumulatorI32 stats;
    uint64_t last_seqnum;

    PeriodicCallback(
        Cancellable& p_scope,
        std::atomic<InferenceCallback*>* p_set_when_triggered,
        VisualInferenceCallback& p_callback,
        std::chrono::milliseconds p_period
    )
        : scope(p_scope)
        , set_when_triggered(p_set_when_triggered)
        , callback(p_callback)
        , period(p_period)
        , last_seqnum(0)
    {}
};



VisualInferencePivot::VisualInferencePivot(CancellableScope& scope, VideoFeed& feed, AsyncDispatcher& dispatcher)
    : PeriodicRunner(dispatcher)
    , m_feed(feed)
{
    attach(scope);
}
VisualInferencePivot::~VisualInferencePivot(){
    detach();
    stop_thread();
}
void VisualInferencePivot::add_callback(
    Cancellable& scope,
    std::atomic<InferenceCallback*>* set_when_triggered,
    VisualInferenceCallback& callback,
    std::chrono::milliseconds period
){
    SpinLockGuard lg(m_lock);
    auto iter = m_map.find(&callback);
    if (iter != m_map.end()){
        throw InternalProgramError(nullptr, PA_CURRENT_FUNCTION, "Attempted to add the same callback twice.");
    }
    iter = m_map.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(&callback),
        std::forward_as_tuple(scope, set_when_triggered, callback, period)
    ).first;
    try{
        PeriodicRunner::add_event(&iter->second, period);
    }catch (...){
        m_map.erase(iter);
        throw;
    }
}
StatAccumulatorI32 VisualInferencePivot::remove_callback(VisualInferenceCallback& callback){
    SpinLockGuard lg(m_lock);
    auto iter = m_map.find(&callback);
    if (iter == m_map.end()){
        return StatAccumulatorI32();
    }
    StatAccumulatorI32 stats = iter->second.stats;
    PeriodicRunner::remove_event(&iter->second);
    m_map.erase(iter);
    return stats;
}
void VisualInferencePivot::run(void* event, bool is_back_to_back) noexcept{
    PeriodicCallback& callback = *(PeriodicCallback*)event;
    try{
        //  Reuse the cached screenshot.
        if (!is_back_to_back || callback.last_seqnum == m_seqnum){
//            cout << "back-to-back" << endl;
            m_last = m_feed.snapshot();
            m_seqnum++;
        }

        WallClock time0 = current_time();
        bool stop = callback.callback.process_frame(m_last.frame, m_last.timestamp);
        WallClock time1 = current_time();
        callback.stats += (uint32_t)std::chrono::duration_cast<std::chrono::microseconds>(time1 - time0).count();
        callback.last_seqnum = m_seqnum;
        if (stop){
            if (callback.set_when_triggered){
                InferenceCallback* expected = nullptr;
                callback.set_when_triggered->compare_exchange_strong(expected, &callback.callback);
            }
            callback.scope.cancel(nullptr);
        }
    }catch (...){
        callback.scope.cancel(std::current_exception());
    }
}


OverlayStatSnapshot VisualInferencePivot::get_current() const{
    double utilization = this->current_utilization();
    Color color = COLOR_WHITE;
    if (utilization > 0.90){
        color = COLOR_RED;
    }else if (utilization > 0.80){
        color = COLOR_ORANGE;
    }else if (utilization > 0.50){
        color = COLOR_YELLOW;
    }
    return OverlayStatSnapshot{
        "Video Pivot Utilization: " + tostr_fixed(utilization * 100, 2) + " %",
        color
    };
}




}
