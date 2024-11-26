# Electronics & Wiring
This section will simply show how the PMTs, Arduino, and Oscilloscope were all connected together to log a muon pulse. 
Since this section only covers how to wire everything up, further discussion of the logic can be found on their respective pages:
  - [ ] [Arduino](../OperationalInstructions/Arduino.md) logic
  - [ ] [Oscilloscope](../OperationalInstructions/Oscilloscope.md) logging logic

The basic NIM rack layout is shown below. The NIM modules that were used are (from right to left) are:
  - [ ] Octal Discriminator
  - [ ] Quad Coincidence
  - [ ] NIM -> TTL Converter
  - [ ] Counter
  
| NIM Rack | 
| :--: |
|<img src="../../Figures/electronics1.jpg" alt="photo of NIM rack" width="900" />|

A simplified drawing demonstrates the logic needed to take the signal from the PMTs and send it to the oscilloscope.
| Simplified Schematic| 
| :--: |
|<img src="../../Figures/NIM_diagram.jpg" alt="schematic of wiring of entire system" width="900" />|


