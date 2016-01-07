/*
 * LEDBlinker.h
 *
 *  Created on: Jan 6, 2016
 *      Author: Bruce Williams
 */

#ifndef LEDBlinker_H_
#define LEDBlinker_H_
#include "Arduino.h"
// The minimum time (milliseconds) the program will wait between LED adjustments
// adjust this to modify performance.
#define LED_BLINKER_MIN_INTERVAL 20

// Example setup/running code:
//    LEDBlinker led(13);  // Use pin 13
//    void setup() {
//      pinMode(13, OUTPUT);  // Must setup pin as output
//    }
//    void loop() { 
//      led.update(); // Must be called in loop.  Do not block (use delays) in loop
//    }
//
// Example control code:
//   Turn LED on indefinitely:
//     led.on();
//
//   Turn LED off indefinitely:
//     led.off();
//
//   Turn LED on for 5 seconds, then write to serial console
//     void done() { 
//       led.off(); // ensure LED is off
//       Serial.println("I'm done!"); 
//     }
//     led.onForDuration(5000, done);
//
//   Blink LED on twice for 200ms, off for 300ms between, then pause for 700ms.
//   Repeat the sequence a 2nd time, and then write to serial console
//     void done() { 
//       led.off(); // not needed in this case, but ensure anyway
//       Serial.println("I'm done!"); 
//     }
//     led.blink(200, 300, 2, 700, 2, done);
//
//   Blink LED on three times for 500ms, off for 300ms between, then pause for 1 second.
//   Repeat the sequence forever.  No callback because this runs forever.
//     led.blink(500, 300, 3, 1000, 0, NULL);

class LEDBlinker {
private:
  uint8_t mPin;
  uint8_t mNumBlinks;
  uint16_t mBlinkOnDuration; 
  uint16_t mBlinkOffDuration;
  uint16_t mPauseDuration;
  uint16_t mNumSequences;  // 0 to run forever
  unsigned long mStartTime;
  unsigned long mLastRunTime;
  void (*mCallbackFunction)();

protected:
  LEDBlinker(); // Hide this (force pin to be set)
  
public:
  // Create a new LED Fader for a pin
  LEDBlinker(uint8_t const pin): mPin(pin), mNumBlinks(0){};
  
  // Restarts at first sequence (if one was running) on new pin
  void setPin(uint8_t const pin);
  
  // This is the workhorse function.  It starts a sequence of LED blinking, followed
  // by a pause (with the LED off), and repeats the sequence a specified number of times,
  // then calls a callback function (if provided) when completely done.  The maximum
  // time for a single sequence (blinks plus pause) is 65,535 milliseconds.  At the end
  // of the last sequence, the LED will be left in it's final state (off, unless the 
  // pause duration is 0)
  // Arguments:
  //   blinkOnDuration - time (in ms) that LED will blink on
  //   blinkOffDuration - time (in ms) that LED will blink off (between "on"
  //       blinks, not before pause)
  //   numBlinks - number of times the LED should blink on.  The LED will 
  //       blink off numBlinks - 1 times.  Must be greater than 0.
  //   pauseDuration - duration to leave the LED off after the last blink on until
  //       the end of the sequence
  //   numSequences - number of times the blink/pause sequence should repeat.  You
  //      can specify 0 to indicate infinite sequences.
  //   callbackFunction - a function you want to have run when all sequences have
  //      completed.  Ignored if numSequences is 0 (infinite sequences).
  void blink(uint16_t const blinkOnDuration,
             uint16_t const blinkOffDuration,
             uint8_t const numBlinks,
             uint16_t const pauseDuration,
             uint16_t const numSequences,
             void (*callbackFunction)());
  
  // Turn on, leave on for duration, then call callback
  void onForDuration(uint16_t const onDuration,
                     void (*callbackFunction)()) {
    blink(onDuration, 0, 1, 0, 1, callbackFunction);        
  }
  
  // Turn off, leave off for duration, then call callback
  void offForDuration(uint16_t const offDuration,
                      void (*callbackFunction)()) {
    blink(0, offDuration, 1, 0, 1, callbackFunction);        
  }
  
  // Turn on, leave on forever
  void on() { blink(1, 0, 1, 0, 0, NULL); }

  // Turn off, leave off forever
  void off() { blink(0, 1, 1, 0, 0, NULL); }
  
  // Blink for one sequence, with a fixed number of blinks in the sequence, using default blink/pause durations
  void blink(uint8_t const numBlinks, void (*callbackFunction)()) {
    blink(250, 250, numBlinks, 750, 1, callbackFunction); 
  }

  // This must be called in your loop, and your loop should be non-blocking (no delays)
  void update();
};

#endif /* LEDBlinker_H_ */
