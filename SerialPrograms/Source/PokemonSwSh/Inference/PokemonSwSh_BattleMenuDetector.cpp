/*  Battle Menu Detector
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 *
 *      Drop a wishing piece and determine if it is red or purple.
 *
 */

#include "Common/Compiler.h"
#include "CommonFramework/Inference/ImageTools.h"
#include "CommonFramework/Inference/ColorClustering.h"
#include "PokemonSwSh_BattleMenuDetector.h"

#include <iostream>
using std::cout;
using std::endl;

//#define DEBUG_StandardBattleMenuDetector

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSwSh{


StandardBattleMenuDetector::StandardBattleMenuDetector(VideoFeed& feed)
    : m_icon_fight   (feed, 0.923, 0.576 + 0 * 0.1075, 0.05, 0.080)
    , m_icon_pokemon (feed, 0.923, 0.576 + 1 * 0.1075, 0.05, 0.080)
    , m_icon_bag     (feed, 0.923, 0.576 + 2 * 0.1075, 0.05, 0.080)
    , m_icon_run     (feed, 0.923, 0.576 + 3 * 0.1075, 0.05, 0.080)
    , m_text_fight   (feed, 0.830, 0.576 + 0 * 0.1075, 0.08, 0.080)
    , m_text_pokemon (feed, 0.830, 0.576 + 1 * 0.1075, 0.08, 0.080)
    , m_text_bag     (feed, 0.830, 0.576 + 2 * 0.1075, 0.08, 0.080)
    , m_text_run     (feed, 0.830, 0.576 + 3 * 0.1075, 0.08, 0.080)
{}
bool StandardBattleMenuDetector::on_frame(
    const QImage& frame,
    std::chrono::system_clock::time_point timestamp
){
    return detect(frame);
}
bool StandardBattleMenuDetector::detect(const QImage& image) const{
    bool fight;

#ifdef DEBUG_StandardBattleMenuDetector
    cout << "=============> Fight Text 0" << endl;
#endif
    fight = false;
    fight |= !fight && cluster_fit_2(
        extract_box(image, m_text_fight),
        qRgb(0, 0, 0), 0.9,
        qRgb(255, 255, 255), 0.1,
        0.2, 50, 0.1
    );
    fight |= !fight && cluster_fit_2(
        extract_box(image, m_text_fight),
        qRgb(0, 0, 0), 0.1,
        qRgb(255, 255, 255), 0.9,
        0.2, 50, 0.1
    );
    if (!fight){
        return false;
    }

#ifdef DEBUG_StandardBattleMenuDetector
    cout << "=============> Pokemon Text" << endl;
#endif
    fight = false;
    fight |= !fight && cluster_fit_2(
        extract_box(image, m_text_pokemon),
        qRgb(0, 0, 0), 0.1,
        qRgb(255, 255, 255), 0.9,
        0.2, 50, 0.1
    );
    fight |= !fight && cluster_fit_2(
        extract_box(image, m_text_pokemon),
        qRgb(0, 0, 0), 0.9,
        qRgb(255, 255, 255), 0.1,
        0.2, 50, 0.1
    );
    if (!fight){
        return false;
    }

#ifdef DEBUG_StandardBattleMenuDetector
    cout << "=============> Bag Text" << endl;
#endif
    fight = false;
    fight |= !fight && cluster_fit_2(
        extract_box(image, m_text_bag),
        qRgb(0, 0, 0), 0.1,
        qRgb(255, 255, 255), 0.9,
        0.2, 50, 0.1
    );
    fight |= !fight && cluster_fit_2(
        extract_box(image, m_text_bag),
        qRgb(0, 0, 0), 0.9,
        qRgb(255, 255, 255), 0.1,
        0.2, 50, 0.1
    );
    if (!fight){
        return false;
    }

#ifdef DEBUG_StandardBattleMenuDetector
    cout << "=============> Run Text" << endl;
#endif
    fight = false;
    fight |= !fight && cluster_fit_2(
        extract_box(image, m_text_run),
        qRgb(0, 0, 0), 0.1,
        qRgb(255, 255, 255), 0.9,
        0.2, 50, 0.1
    );
    fight |= !fight && cluster_fit_2(
        extract_box(image, m_text_run),
        qRgb(0, 0, 0), 0.9,
        qRgb(255, 255, 255), 0.1,
        0.2, 50, 0.1
    );
    if (!fight){
        return false;
    }


#if 1
#ifdef DEBUG_StandardBattleMenuDetector
    cout << "=============> Fight Symbol 0" << endl;
#endif
    fight = false;
    fight |= !fight && cluster_fit_2(
        extract_box(image, m_icon_fight),
        qRgb(255, 255, 255), 1.7,
        qRgb(153, 75, 112), 1.0
    );
#ifdef DEBUG_StandardBattleMenuDetector
    cout << "=============> Fight Symbol 1" << endl;
#endif
    fight |= !fight && cluster_fit_2(
        extract_box(image, m_icon_fight),
        qRgb(0, 0, 0), 1.4,
        qRgb(185, 6, 40), 1.0
    );
    fight |= !fight && cluster_fit_2(   //  Max raid Fight button is a bit different.
        extract_box(image, m_icon_fight),
        qRgb(0, 0, 0), 1.7,
        qRgb(182, 33, 82), 1.0
    );
    if (!fight){
        return false;
    }

#ifdef DEBUG_StandardBattleMenuDetector
    cout << "=============> Pokemon Symbol" << endl;
#endif
    bool pokemon = false;
    pokemon |= !pokemon && cluster_fit_2(
        extract_box(image, m_icon_pokemon),
        qRgb(255, 255, 255), 3.1,
        qRgb(126, 224, 142), 1.0
    );
    pokemon |= !pokemon && cluster_fit_2(
        extract_box(image, m_icon_pokemon),
        qRgb(0, 0, 0), 2.7,
        qRgb(8, 158, 18), 1.0
    );
    if (!pokemon){
        return false;
    }

#ifdef DEBUG_StandardBattleMenuDetector
    cout << "=============> Bag Symbol" << endl;
#endif
    bool bag = false;
    bag |= !bag && cluster_fit_2(
        extract_box(image, m_icon_bag),
        qRgb(255, 255, 255), 2.4,
        qRgb(236, 192, 124), 1.0
    );
    bag |= !bag && cluster_fit_2(
        extract_box(image, m_icon_bag),
        qRgb(0, 0, 0), 1.9,
        qRgb(215, 120, 11), 1.0
    );
    if (!bag){
        return false;
    }

#ifdef DEBUG_StandardBattleMenuDetector
    cout << "=============> Run Symbol" << endl;
#endif
    bool run = false;
    run |= !run && cluster_fit_2(
        extract_box(image, m_icon_run),
        qRgb(255, 255, 255), 2.3,
        qRgb(216, 150, 230), 1.0
    );
    run |= !run && cluster_fit_2(
        extract_box(image, m_icon_run),
        qRgb(0, 0, 0), 1.9,
        qRgb(179, 15, 195), 1.0
    );
    if (!run){
        return false;
    }
#endif

//    image.save("battle-menu.png");
    return true;
}




}
}
}
