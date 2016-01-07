# arduino-led-blinker
Arduino library allowing non-blocking LED blinking patterns in repeating sequences


## Example setup/running code:
```
LEDBlinker led(13);  // Use pin 13
 void setup() {
   pinMode(13, OUTPUT);  // Must setup pin as output
 }
 void loop() { 
   led.update(); // Must be called in loop.  Do not block (use delays) in loop
 }
```

## Example control code:
### Turn LED on indefinitely:
```
led.on();
```
### Turn LED off indefinitely:
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
