/*  Max Lair Detect Entrance
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "Common/Compiler.h"
#include "CommonFramework/ImageTools/ImageBoxes.h"
#include "CommonFramework/ImageMatch/ImageDiff.h"
#include "CommonFramework/VideoPipeline/VideoOverlay.h"
#include "PokemonSwSh_MaxLair_Detect_Entrance.h"

#include <iostream>
using std::cout;
using std::endl;

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSwSh{
namespace MaxLairInternal{


EntranceDetector::EntranceDetector(const ImageViewRGB32& entrance_screen)
    : VisualInferenceCallback("EntranceDetector")
    , m_box0(0.020, 0.020, 0.500, 0.750)
    , m_entrance_screen(extract_box_reference(entrance_screen, m_box0).copy())
{}
void EntranceDetector::make_overlays(VideoOverlaySet& items) const{
    items.add(COLOR_DARKGREEN, m_box0);
}
bool EntranceDetector::process_frame(const ImageViewRGB32& frame, WallClock timestamp){
    return detect(frame);
}

bool EntranceDetector::detect(const ImageViewRGB32& screen){
    if (!screen){
        return false;
    }

    ImageRGB32 copy;

    ImageViewRGB32 image = extract_box_reference(screen, m_box0);
    if (image.width() != (size_t)m_entrance_screen.width() || image.height() != (size_t)m_entrance_screen.height()){
        copy = image.scale_to(m_entrance_screen.width(), m_entrance_screen.height());
        image = copy;
    }

    double diff = ImageMatch::pixel_RMSD(m_entrance_screen, image);
//    cout << diff << endl;

    return diff < 20;
}



}
}
}
}
