Requirements:
- Partial Arduino Core implementing interfaces Print, Printable and Stream -> Used for generic interfaces and avoid duplicates between Arduino and non-Arduino projects
- ArduinoJson library, despite it's name it's fully portable
- If using Arduino Core:
  + If using Grove RGB LED: ChainableLED library
