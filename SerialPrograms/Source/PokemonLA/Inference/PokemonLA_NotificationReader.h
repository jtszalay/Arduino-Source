/*  Notification Reader
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_PokemonLA_NotificationReader_H
#define PokemonAutomation_PokemonLA_NotificationReader_H

#include "CommonFramework/ImageTools/ImageBoxes.h"
#include "CommonFramework/OCR/OCR_SmallDictionaryMatcher.h"
#include "CommonFramework/Inference/VisualDetector.h"
#include "CommonFramework/Inference/VisualInferenceCallback.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonLA{


enum class Notification{
    NOTHING,
    DISTORTION_FORMING,
    DISTORTION_APPEARED,
    DISTORTION_FADED,
    CANNOT_GO_FURTHER,
};



class NotificationOCR : public OCR::SmallDictionaryMatcher{
public:
    static const NotificationOCR& instance();

private:
    NotificationOCR();
};



class NotificationReader{
public:
    NotificationReader(Logger& logger, Language language);

    void make_overlays(VideoOverlaySet& items) const;
    Notification detect(const QImage& screen) const;

private:
    Logger& m_logger;
    Language m_language;
    ImageFloatBox m_ocr_box;
};


class NotificationDetector : public VisualInferenceCallback{
public:
    NotificationDetector(Logger& logger, Language language);

    Notification result() const{
        return m_last.load(std::memory_order_acquire);
    }

    virtual void make_overlays(VideoOverlaySet& items) const override;

    virtual bool process_frame(
        const QImage& frame,
        std::chrono::system_clock::time_point timestamp
    ) override;

private:
    NotificationReader m_reader;
    std::atomic<Notification> m_last;
};





}
}
}
#endif
