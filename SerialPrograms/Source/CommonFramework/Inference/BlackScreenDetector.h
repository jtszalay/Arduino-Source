/*  Black Screen Detector
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 *
 *      Returns true after a black screen has been detected and
 * the black screen has ended.
 *
 */

#ifndef PokemonAutomation_CommonFramework_BlackScreenDetector_H
#define PokemonAutomation_CommonFramework_BlackScreenDetector_H

#include "Common/Cpp/Color.h"
#include "CommonFramework/ImageTools/ImageBoxes.h"
#include "CommonFramework/InferenceInfra/VisualInferenceCallback.h"
#include "CommonFramework/Inference/VisualDetector.h"

namespace PokemonAutomation{


class BlackScreenDetector : public StaticScreenDetector{
public:
    BlackScreenDetector(
        Color color = COLOR_RED,
        const ImageFloatBox& box = {0.1, 0.1, 0.8, 0.8},
        double max_rgb_sum = 100,
        double max_stddev_sum = 10
    );

    virtual void make_overlays(VideoOverlaySet& items) const override;
    virtual bool detect(const ImageViewRGB32& screen) const override;

private:
    Color m_color;
    ImageFloatBox m_box;
    double m_max_rgb_sum;
    double m_max_stddev_sum;
};


class BlackScreenWatcher : public BlackScreenDetector, public VisualInferenceCallback{
public:
    BlackScreenWatcher(
        Color color = COLOR_RED,
        const ImageFloatBox& box = {0.1, 0.1, 0.8, 0.8},
        double max_rgb_sum = 100,
        double max_stddev_sum = 10
    );

    virtual void make_overlays(VideoOverlaySet& items) const override;
    virtual bool process_frame(const ImageViewRGB32& frame, WallClock timestamp) override;
};

// Detect when a perido of black screen is over
class BlackScreenOverWatcher : public VisualInferenceCallback{
public:
    BlackScreenOverWatcher(
        Color color = COLOR_RED,
        const ImageFloatBox& box = {0.1, 0.1, 0.8, 0.8},
        double max_rgb_sum = 100,
        double max_stddev_sum = 10
    );

    bool black_is_over(const ImageViewRGB32& frame);

    virtual void make_overlays(VideoOverlaySet& items) const override;

    virtual bool process_frame(const ImageViewRGB32& frame, WallClock timestamp) override;

private:
    BlackScreenDetector m_detector;
    bool m_has_been_black = false;
};



}
#endif
