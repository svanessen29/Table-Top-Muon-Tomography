# Electronics & Wiring
This section will discuss how the PMTs, Arduino, and Oscilloscope were all connected together. 

Since this section only covers how to wire everything up, further discussion of the logic can be found on their respective pages:
  - [ ] [Arduino](../OperationalInstructions/Arduino.md) logic
  - [ ] [Oscilloscope](../OperationalInstructions/Oscilloscope.md) logging logic

| NIM Rack | 
| :--: |
|<img src="../../Figures/electronics1.jpg" alt="photo of NIM rack" width="900" />|

A simplified drawing demonstrates the logic needed totake the signal from the PMTs and send it to the oscilloscope.
| Simplified Schematic| 
| :--: |
|<img src="../../Figures/NIM_diagram.jpg" alt="schematic of wiring of entire system" width="900" />|

## Basic Logic
The main logic of the system depends on muon coincidences and a pseudo arduino pulse. To begin, for a coincidence to be measured, the two PMTs mounted on the scanning arms must both trigger with a muon pulse. Once this occurs the data from each PMT mounted on the plate scintillator is saved by the oscilloscope. The scanning arms sit at a location for a specified amount of time then move to the next grid location. The data is separated by grid location through the use of the arduino that moves the stepper motors.


