/*
 * LEDBlinker.cpp
 *
 *  Created on: Jan 6, 2016
 *      Author: Bruce Williams
 */

#include "LEDBlinker.h"

void LEDBlinker::setPin(uint8_t const pin)
{
   mPin = pin; 
   blink(mBlinkOnDuration, 
         mBlinkOffDuration, 
         mNumBlinks,
         mPauseDuration,
         mNumSequences); 
}

LEDBlinker::update() {
  unsigned long curTime = millis();
  if (curTime - mLastRunTime < LED_BLINKER_MIN_INTERVAL) {
    return;
  }
  mLastRunTime = curTime;
  if (!mNumBlinks || !mPin) {
    return;
  }
  
  unsigned long elapsedTime = curTime - mStartTime;
  uint16_t blinkDuration = mBlinkOnDuration + mBlinkOffDuration;
  uint16_t sequenceDuration = blinkDuration + mPauseDuration - mBlinkOffDuration;
  if (mNumSequences != 0 && elapsedTime / sequenceDuration >= mNumSequences) {
    mNumBlinks = 0;
    if (mCallbackFunction) {
      mCallbackFunction();
    }
    return;
  }
  
  // Figure out where we are within a sequence
  uint16_t blinkingDuration = blinkDuration * mNumBlinks;
  uint16_t timeInSequence = elapsedTime % sequenceDuration;
  if (timeInSequence < blinkingDuration && timeInSequence % blinkDuration < mBlinkOnDuration) {
    digitalWrite(mPin, HIGH);    
  } else {
    digitalWrite(mPin, LOW);    
  }
}

void LEDBlinker::blink(uint16_t const blinkOnDuration,
                       uint16_t const blinkOffDuration,
                       uint8_t const numBlinks,
                       uint16_t const pauseDuration,
                       uint16_t const numSequences,
                       void (*callbackFunction)()) {
  mBlinkOnDuration = blinkOnDuration ? MAX(LED_BLINKER_MIN_INTERVAL, blinkOnDuration) : 0;
  mBlinkOffDuration = blinkOffDuration ? MAX(LED_BLINKER_MIN_INTERVAL, blinkOffDuration) : 0;
  mNumBlinks = numBlinks;
  mPauseDuration = pauseDuration;
  mNumSequences = numSequences;
  mCallbackFunction = callbackFunction;
  mStartTime = millis();
  mLastRunTime = 0;
  if !(blinkOnDuration || blinkOffDuration) {
    mNumBlinks = 0;
  }
  update();             
}