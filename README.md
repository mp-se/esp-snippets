# ESP snippets

This project contains a number of snippets for testing various things with ESP. Copy the files from the snippets directory to the src and compilöe / upload.

## Snippets

### Voltage measurement from analog pin using a voltage divider.

- On esp8266 you need a 220k resistor between analog pin and + of your source (the resistor to gnd is built in the chip)
- On esp32 boards you the same as above but also a 220k between the analog pin and gnd

