/**
 * @file ArduboyPlaytune.h
 * \brief An Arduino library that plays a one or two part musical score and
 * generates tones. Intended for the Arduboy game system.
 */

/*****************************************************************************
* ArduboyPlaytune
*
* Plays a one or two part musical score and generates tones.
*
* Derived from:
* Playtune: An Arduino tune player library
* https://github.com/LenShustek/arduino-playtune
*
* Modified to work well with the Arduboy game system
* https://www.arduboy.com/
*
*  (C) Copyright 2016, Chris J. Martinez, Kevin Bates, Josh Goebel, Scott Allen
*  Based on work (C) Copyright 2011, 2015, Len Shustek
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of version 3 of the GNU General Public License as
*  published by the Free Software Foundation at http://www.gnu.org/licenses,
*  with Additional Permissions under term 7(b) that the original copyright
*  notice and author attibution must be preserved and under term 7(c) that
*  modified versions be marked as different from the original.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  This was inspired by and adapted from "A Tone Generator Library",
*  written by Brett Hagman, http://www.roguerobotics.com/
*
*****************************************************************************/

#ifndef ARDUBOY_PLAYTUNE_H
#define ARDUBOY_PLAYTUNE_H

#include <Arduino.h>
#include <avr/pgmspace.h>

#define AVAILABLE_TIMERS  2
#define TUNE_OP_PLAYNOTE  0x90  /* play a note: low nibble is generator #, note is next byte */
#define TUNE_OP_STOPNOTE  0x80  /* stop a note: low nibble is generator # */
#define TUNE_OP_RESTART   0xe0  /* restart the score from the beginning */
#define TUNE_OP_STOP      0xf0  /* stop playing */

class ArduboyPlaytune
{
public:
  /**
   * The ArduboyPlaytune class constructor.
   * \param outEn
   * \parblock
   * The function passed to the constructor must return `true`, if
   * sounds should be played, or `false` if all sound should be muted.
   *
   * If muting control isn't required, provide a pointer to a function that
   * always returns `true`.
   *
   * The provided function will be called from a timer interrupt service
   * routine, at the start of each score note, so it should be as fast as
   * possible.
   * \endparblock
   * \details When muting is in effect, scores and tones are still processed
   * as usual but no sound is produced.
   * \note If using the Arduboy library or a library derived from it,
   * `audio.enabled()` is appropriate to use as the mute function
   */
  ArduboyPlaytune(boolean (*outEn)());

  /**
   * Assign an output pin to a score channel.
   * \param pin The pin number to be used to produce sound for a score channel.
   * \details
   * \parblock
   * Each time this function is called the next score channel is assigned
   * to the provided pin number, so it should be called once or twice.
   *
   * If the `tone()` function is to be used, the second channel must be
   * initialized since tones are alway played on it.
   *
   * The same pin number can be used for both channels, in which case only the
   * first score channel will be played and tones will play on the same pin.
   * Function `toneMutesScore(true)` can be use to prevent the strange sounds
   * that occur from using the same pin for both the score and tones.
   * \endparblock
   * \note If using the Arduboy library or a library derived from it,
   * the defined values `PIN_SPEAKER_1` and `PIN_SPEAKER_2` should be used for
   * the `pin` parameter.
   */
  void static initChannel(byte pin);

  /**
   * Start playing the provided score.
   * \param score A pointer to an array of bytes containing the score data.
   * The array must be placed in code space using `PROGMEM`.
   * \details The score will be played in the background until an
   * "End of score: stop playing" command is read or the `stopScore()` function
   *  is called. Any notes in the score for channels above the one or two that
   *  have been initialized will be ignored.
   */
  void playScore(const byte *score);

  /**
   * Stop playing a score started using `playScore()`.
   * \details If a score is playing, it will stop. If nothing is playing,
   * this function will do nothing.
   */
  void stopScore();

  /**
   * Close all (one or two) initialized channels.
   * \details After calling this function, function `initChannel()` must be
   * called, to reassign pins to channels, if more sound is to be produced.
   */
  void closeChannels();

  /**
   * Check if a score is currently playing.
   * \return boolean `true` if playing (even if sound is muted).
   */
  boolean playing();

  /**
   * Play a tone of a given frequency and duration on the second channel.
   * \param frequency The frequency of the tone in hertz (cycles per second).
   * \param duration The duration of the tone in milliseconds.
   * \details If a score is playing that uses the second channel, the notes for
   * that channel are muted for the duration of the tone. Score notes on the
   * first channel continue to play unless `toneMutesScore(true)` has been
   * called.
   */
  void tone(unsigned int frequency, unsigned long duration);

  /**
   * Set a mode to determine whether playing a tone mutes the first score
   * channel.
   * \param mute
   * \parblock
   * If `true` a score part on the first channel will be muted when a tone
   * is playing. (A score part playing on the second channel is always muted
   * since the tone plays on it.)
   *
   * If `false` (the default) the first channel will continue to
   * play when a tone is playing.
   * \endparblock
   */
  void toneMutesScore(boolean mute);

private:
  void static playNote(byte chan, byte note);
  void static stopNote(byte chan);

public:
  // called via interrupt. Should not be called by a program.
  void static step();
};

#endif
