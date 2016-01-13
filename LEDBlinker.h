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
//    LEDBlinker led(13);  // Use LED on pin 13
//    void loop() {
//      led.update(); // Must be called in loop.  Do not block (use delays) in loop
//      // Do other stuff, call LED control code below to control LED
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
//   Use the LED to display the value of an unsigned long
//   For example, to display the value 25, blink 2 times, then short pause, then blink 5 times, then long pause, then repeat
//     unsigned long numberToDisplay = 10934; // Could be whatever you want
//     unsigned long divisor = 1; // Used to extract each digit (10, 100, 1000, etc)
//     void displayDigit() {
//       if (divisor == 1) {
//         divisor = 10;
//         while (numberToDisplay / divisor != 0) {
//           divisor *= 10;
//         }
//         led.offForDuration(1000, displayDigit); // long pause between 1's digit and highest digit
//       } else {
//         unsigned int numBlinks = (numberToDisplay % divisor) / (divisor / 10);
//         divisor /= 10; 
//         if (numBlinks == 0) {
//           led.blink(1000, 0, 1, 700, 1, displayDigit);  // Do a long single blink if digit is zero
//         } else {
//           led.blink(200, 200, numBlinks, 700, 1, displayDigit); // blink for non-zero digit
//         }
//       }
//     }
//     displayDigit();

class LEDBlinker {
private:
  byte mPin;
  byte mNumBlinks;
  unsigned int mBlinkOnDuration;
  unsigned int mBlinkOffDuration;
  unsigned int mPauseDuration;
  unsigned int mNumSequences;  // 0 to run forever
  unsigned long mStartTime;
  unsigned long mLastRunTime;
  void (*mCallbackFunction)();

protected:
  LEDBlinker(); // Hide this (force pin to be set)

public:
  // Create a new LED Fader for a pin.  This automatically sets the pin mode
  LEDBlinker(byte const pin): mPin(pin), mStartTime(0){
    pinMode(pin, OUTPUT);
  };

  // Use new pin.  This automatically sets the pin mode
  void setPin(byte const pin);

  // This starts a sequence of LED blinking, followed by a pause
  // (with the LED off), and repeats the sequence a specified number of times,
  // then calls a callback function (if provided) when completely done.  The maximum
  // time for a single sequence (blinks plus pause) is 65,535 milliseconds.  At the end
  // of the last sequence, the LED will be left in it's final state (off, unless the
  // pause duration is 0)
  // Arguments:
  //   blinkOnDuration - time (in ms) that LED will blink on
  //   blinkOffDuration - time (in ms) that LED will blink off (between "on"
  //       blinks, not before pause)
  //   numBlinks - number of times the LED should blink on.  The LED will
  //       blink off numBlinks - 1 times.
  //   pauseDuration - duration to leave the LED off after the last blink on until
  //       the end of the sequence
  //   numSequences - number of times the blink/pause sequence should repeat.  You
  //      can specify 0 to indicate infinite sequences.
  //   callbackFunction - a function you want to have run when all sequences have
  //      completed.  Ignored if numSequences is 0 (infinite sequences).
  void blink(unsigned int const blinkOnDuration,
             unsigned int const blinkOffDuration,
             byte const numBlinks,
             unsigned int const pauseDuration,
             unsigned int const numSequences,
             void (*callbackFunction)());

  // Turn on, leave on for duration, then call callback
  void onForDuration(unsigned int const onDuration,
                     void (*callbackFunction)()) {
    blink(onDuration, 0, 1, 0, 1, callbackFunction);
  }

  // Turn off, leave off for duration, then call callback
  void offForDuration(unsigned int const offDuration,
                      void (*callbackFunction)()) {
    blink(0, 0, 0, offDuration, 1, callbackFunction);
  }

  // Turn on, leave on indefinitely
  void on() { blink(1, 0, 1, 0, 0, NULL); }

  // Turn off, leave off indefinitely
  void off() { blink(0, 0, 0, 1, 0, NULL); }

  // This is the workhorse function.  It must be called in your loop,
  // and your loop should be non-blocking (no delays)
  void update();
};

#endif /* LEDBlinker_H_ */
