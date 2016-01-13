/*
 * LEDBlinker.cpp
 *
 *  Created on: Jan 6, 2016
 *      Author: Bruce Williams
 */

#include "LEDBlinker.h"

void LEDBlinker::setPin(byte const pin)
{
   mPin = pin;
   pinMode(pin, OUTPUT);
   mStartTime = 0;
}

void LEDBlinker::update() {
  unsigned long curTime = millis();
  if (curTime - mLastRunTime < LED_BLINKER_MIN_INTERVAL) {
    return;
  }
  mLastRunTime = curTime;
  if (!mStartTime || !mPin) {
    return;
  }

  unsigned long elapsedTime = curTime - mStartTime;
  unsigned int blinkDuration = mBlinkOnDuration + mBlinkOffDuration;
  unsigned int sequenceDuration = blinkDuration * mNumBlinks + mPauseDuration - mBlinkOffDuration;
  if (!sequenceDuration ||
     (mNumSequences != 0 && elapsedTime / sequenceDuration >= mNumSequences)) {
    mStartTime = 0;
    if (mCallbackFunction) {
      mCallbackFunction();
    }
    return;
  }
  // Figure out where we are within a sequence
  unsigned int blinkingDuration = blinkDuration * mNumBlinks;
  unsigned int timeInSequence = elapsedTime % sequenceDuration;
  if (timeInSequence < blinkingDuration && timeInSequence % blinkDuration < mBlinkOnDuration) {
    digitalWrite(mPin, HIGH);
  } else {
    digitalWrite(mPin, LOW);
  }
}

void LEDBlinker::blink(unsigned int const blinkOnDuration,
                       unsigned int const blinkOffDuration,
                       byte const numBlinks,
                       unsigned int const pauseDuration,
                       unsigned int const numSequences,
                       void (*callbackFunction)()) {
  mBlinkOnDuration = blinkOnDuration ? max(LED_BLINKER_MIN_INTERVAL, blinkOnDuration) : 0;
  mBlinkOffDuration = blinkOffDuration ? max(LED_BLINKER_MIN_INTERVAL, blinkOffDuration) : 0;
  mNumBlinks = numBlinks;
  mPauseDuration = pauseDuration ? max(LED_BLINKER_MIN_INTERVAL, pauseDuration) : 0;;
  mNumSequences = numSequences;
  mCallbackFunction = callbackFunction;
  mStartTime = max(millis(), 1);
  mLastRunTime = 0;
  update();
}
