/*  PokemonBDSP Settings
 * 
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 * 
 */

#ifndef PokemonAutomation_PokemonBDSP_Settings_H
#define PokemonAutomation_PokemonBDSP_Settings_H

#include <stdbool.h>
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Advanced Options

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Menu Navigation Timings

//  Delay to bring up the menu when pressing X in the overworld.
extern uint16_t PokemonBDSP_OVERWORLD_TO_MENU_DELAY;

//  Delay to go from menu back to overworld.
extern uint16_t PokemonBDSP_MENU_TO_OVERWORLD_DELAY;

//  Delay from pressing home to entering the the Switch home menu.
extern uint16_t PokemonBDSP_GAME_TO_HOME_DELAY;

//  Delay to enter game from home menu.
extern uint16_t PokemonBDSP_HOME_TO_GAME_DELAY;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Start Game Timings

//  Delays to start and enter the game when it isn't running.
extern uint16_t PokemonBDSP_START_GAME_MASH;  //  1. Mash A for this long to start the game.
extern uint16_t PokemonBDSP_START_GAME_WAIT;  //  2. Wait this long for the game to load.
extern uint16_t PokemonBDSP_ENTER_GAME_MASH;  //  3. Mash A for this long to enter the game.
extern uint16_t PokemonBDSP_ENTER_GAME_WAIT;  //  4. Wait this long for the game to fully load into the overworld.


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Box Timings (for egg programs)

extern uint16_t PokemonBDSP_BOX_SCROLL_DELAY;         //  Delay to move the cursor.
extern uint16_t PokemonBDSP_BOX_CHANGE_DELAY;         //  Delay to change boxes.
extern uint16_t PokemonBDSP_BOX_PICKUP_DROP_DELAY;    //  Delay to pickup/drop pokemon.

extern uint16_t PokemonBDSP_MENU_TO_POKEMON_DELAY;    //  Delay to enter Pokemon menu.
extern uint16_t PokemonBDSP_POKEMON_TO_BOX_DELAY;     //  Delay to enter box system.
extern uint16_t PokemonBDSP_BOX_TO_POKEMON_DELAY;     //  Delay to exit box system.
extern uint16_t PokemonBDSP_POKEMON_TO_MENU_DELAY;    //  Delay to return to menu.

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
