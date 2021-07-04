/*  Panic Dumping
 * 
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 * 
 */

#ifndef PokemonAutomation_PanicDumping_H
#define PokemonAutomation_PanicDumping_H

#include <string>
#include <functional>
#include "ClientSource/Connection/BotBase.h"

namespace PokemonAutomation{


void panic_dump(const char* location, const char* message);

void run_with_catch(const char* location, std::function<void()>&& lambda);


}
#endif

