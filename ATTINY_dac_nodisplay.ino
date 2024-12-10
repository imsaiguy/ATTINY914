//ImsaiGuy 2024
//control DAC on ATTINY814
//push button to cycle through different waveforms
//
// based on code * ATtiny1614 Audio Function Generator (20Hz to 20kHz)
//               * John Bradnam (jbrad2089@gmail.com)

#define DAC_OUT 2     // DAC output pin (PA6)
#define Button 5      // push button
#define LED 1         // LED
#define LOOP_TIME 54  // Time loop duration to achieve 1kHz output at 1kHz frequency
#define SAMPLES 256   // Number of samples in the waveform
#define DACMAX 256    // Maximum DAC output value

byte waveform[SAMPLES];  // Array to store waveform data
int WaveType = 0;       // Waveform types
// 0 - SINE
// 1 - TRIANGLE
// 2 - RAMP_DOWN
// 3 - RAMP_UP
// 4 - RECTANGLE

long currentFrequency = 10;  // Initial frequency (valid up to ~20 kHz)

// Setup hardware and initialize
void setup() {
  pinMode(Button, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  // Configure DAC reference voltage (4.3V max output)
  VREF.CTRLA = VREF_DAC0REFSEL_4V34_gc;

  // Initialize DAC
  DAC0.DATA = 0;                    // Start with 0 output
  DAC0.CTRLA = DAC_RUNSTDBY_bm | DAC_OUTEN_bm | DAC_ENABLE_bm;
  setWave();                        // Populate waveform table
  runOscillator(currentFrequency);  // Start oscillator
}

// Main loop 
void loop() {
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  WaveType = (WaveType + 1) % 5;    // types 0 to 4
  setWave();                        // Populate waveform table
  runOscillator(currentFrequency);  // Start oscillator
}

// Generate waveform data
void setWave() {
  for (int s = 0; s < SAMPLES; ++s) {
    float phase = (s + 0.5) / SAMPLES;  // Normalize phase [0, 1)
    float angle = 2 * PI * phase;       // Convert to radians
    int val = 0;

    // Calculate waveform value based on selected type
    switch (WaveType) {
      case 0: val = (sin(angle) + 1.0) * DACMAX / 2; break;    //Sine
      case 1: val = abs(DACMAX * (1.0 - 2.0 * phase)); break;  //Triangle
      case 2: val = DACMAX * (1.0 - phase); break;             //Ramp down
      case 3: val = DACMAX * phase; break;                     //Ramp up
      case 4: val = (DACMAX - 1) * (phase > 0.5); break;       //Rectangle
    }

    // Clamp value to valid DAC range
    waveform[s] = (byte)min(max(val, 0), DACMAX - 1);
  }
}

// Oscillator function to generate the waveform
__attribute__((optimize("O0"))) 
void runOscillator(long freq) {
  unsigned long phase = 0;                                          // Phase accumulator
  unsigned long phaseInc = 0.2147483648 * LOOP_TIME * freq * 1000;  // Phase increment

  while (digitalRead(Button) == HIGH) {  // Infinite loop
    phase += phaseInc;                   // Increment phase
    int reducedPhase = phase >> 24;      // Extract upper 8 bits for indexing
    DAC0.DATA = waveform[reducedPhase];  // Output waveform value
  }
}
