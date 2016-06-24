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
  /// The class constructor.
  /**
   * The function passed to the constructor must return "true", if sounds
   * should be played, or "false" if all sound should be muted. If muting
   * control isn't required, provide a pointer to a function that always
   * returns "true".
   * This function will be called from the timer interrupt service routine,
   * at the start of each score note, so it should be as fast as possible.
   */
  ArduboyPlaytune(boolean (*outEn)());

  /// Assign an output pin to a score channel.
  /**
   * Each time this function is called the next score channel is assigned
   * to the provided pin number, so it should be called once or twice.
   * If the tone() function is to be used, the second channel must be
   * initialised since tones are alway played on it.
   *
   * The same pin number can be used for both channels, in which case only the
   * first score channel will be played and tones will play on the same pin.
   * Function toneMutesScore(true) can be use to prevent the strange sounds
   * that occur from using the same pin for both the score and tones.
   */
  void static initChannel(byte pin);

  /// Start playing the provided score.
  /**
   * Any notes for channels above the first two will be ignored.
   */
  void playScore(const byte *score);

  /// Stop playing the score.
  void stopScore();

  /// Close both channels.
  /**
   * After calling this function, use function initChannel() must be called to
   * reassign pins to channels.
   */
  void closeChannels();

  /// Returns "true" if a score is playing.
  boolean playing();

  /// Play a tone of a given frequency and duration on the second channel.
  /**
   * If a score is playing that uses the second channel, the notes for that
   * channel stop for the duration of the tone. Score notes on the
   * first channel continue to play unless toneMutesScore(true) has been
   * called.
   */
  void tone(unsigned int frequency, unsigned long duration);

  /// Control whether playing a tone mutes the first score channel.
  /**
   * If called with "true" the first channel will be muted when a tone
   * is playing. (The second channel is always muted since the tone plays
   * on it.)
   *
   * If called with "false" (the default) the first channel will continue to
   * play when a tone is playing.
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
