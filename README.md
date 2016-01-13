# arduino-led-blinker
Arduino library allowing non-blocking LED blinking patterns in repeating sequences


## Example setup/running code
```
LEDBlinker led(13);  // Use pin 13
void loop() { 
  led.update(); // Must be called in loop.  Do not block (use delays) in loop
  // Do other stuff, including calling LED control functions to control the LED
}
```

## Example control code
### Turn LED on indefinitely
```
led.on();
```
### Turn LED off indefinitely
```
led.off();
```
### Turn LED on for 5 seconds, then write to serial console
```
void done() { 
  led.off(); // ensure LED is off
  Serial.println("I'm done!"); 
}
led.onForDuration(5000, done);
```
### Blink LED on twice for 200ms, off for 300ms between, then pause for 700ms.  Repeat the sequence a 2nd time, and then write to serial console
```
void done() { 
  led.off(); // not needed in this case, but ensure anyway
  Serial.println("I'm done!"); 
}
led.blink(200, 300, 2, 700, 2, done);
```
### Blink LED on three times for 500ms, off for 300ms between, then pause for 1 second. Repeat the sequence forever.
No callback because this runs forever.
```
led.blink(500, 300, 3, 1000, 0, NULL);
```
### Use the LED to display the value of an unsigned long value
For example, to display the value 25, blink 2 times, then short pause, then blink 5 times, then long pause, then repeat
```
unsigned long numberToDisplay = 10934; // Could be whatever you want
unsigned long divisor = 1; // Used to extract each digit (10, 100, 1000, etc)
void displayDigit() {
  if (divisor == 1) {
    divisor = 10;
    while (numberToDisplay / divisor != 0) {
      divisor *= 10;
    }
    led.offForDuration(1000, displayDigit); // long pause between 1's digit and highest digit
  } else {
    unsigned int numBlinks = (numberToDisplay % divisor) / (divisor / 10);
    divisor /= 10; 
    if (numBlinks == 0) {
      led.blink(1000, 0, 1, 700, 1, displayDigit);  // Do a long single blink if digit is zero
    } else {
      led.blink(200, 200, numBlinks, 700, 1, displayDigit); // blink for non-zero digit
    }
  }
}

displayDigit();
```
