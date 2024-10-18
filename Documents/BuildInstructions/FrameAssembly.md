# Frame Assembly
This section will explain the design considerations for the development of the muon scanner. Design influences drawn from Computer Numeric Control (CNC) machining methods to PMT calibrations are utilized with the aid of rapid prototyping methods, such as Fused Deposition Modelling (FMD).

  | Finalized Design |
  | :---: |
  |<img src="../../Figures/setup_pmt1.jpg" alt="muon frame image" width="600" />|

The entire scanner was mounted directly onto a spared table. This was done not only due to the availability of tables but it also allows the unit to be quite portable and fit through a standard doorway.

## Axis Movement
Once the table was selected, a method for xy-axis scanning was needed. The movement was heavily inspired by CNC routers and uses a combination of belt-driven and gear-driven movement.
+ [ ] **Y-axis:** uses a large spiral gear that runs along the long length of the table.
+ [ ] **X-axis:** uses a belt drive that spans over the top of the table.

### Y-axis
Mounted onto the spiral gear there is a stepper motor that is controlled by an Arduino. The rotation of the stepper motor is transferred to the spiral gear through a small belt. As the spiral gear turns, a large vertical aluminum profile follows the gearing. This mechanism determines the y-axis movement.

  | Y-Axis Spiral Gear & Stepper Motor|
  | :---: |
  |<img src="../../Figures/setup_pmt5_a.jpg" alt="muon frame image" width="300" />|

### X-axis
Mounted onto the larger vertical aluminum profile, a second stepper motor can be found. This stepper motor directly drives a belt. The belt is then attached to two horizontal scanning arms that will eventually house the coincidence PMTs.

  | X-Axis Belt Drive & Stepper | Belt Drive & Scanning Arms |
  | :---: | :--:|
  |<img src="../../Figures/setup_pmt6.jpg" alt="muon frame image" width="600" />|<img src="../../Figures/beltdrive.jpg" alt="muon frame image" width="600" />

To hold up the far end of the x-axis and reduce the chance of the arm flexing from the movement of the scanner, a supporting wheel was installed. This wheel simply runs along the length of the table and maintains a constant scan height.

  | Axis Support Wheel |
  | :---: |
  |<img src="../../Figures/wheel.jpg" alt="muon frame image" width="300" />|

## Aluminum Profile
Now that both the x- and y-axis controlling hardware are mounted, the frame that suspends the large plate scintillator was then constructed. This was accomplished by vertically mounting four aluminum profiles (each 2.5 cm X 38.5 cm). L-brackets mounted on each vertical profile, allow the user to adjust the height of the plate scintillator.

  | L-Bracket | L-Bracket Holding the Scintillator |
  | :---: | :---: |
  |<img src="../../Figures/bracket.jpg" alt="muon frame image" width="300" />|<img src="../../Figures/bracket1.jpg" alt="muon frame image" width="300" />|
  
