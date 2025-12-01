# Engineering Helper Toolkit  
ELEC2645 – Unit 2 Individual Project  
Author: Jassim Khalid I S AlMuhaiza 
Academic Year: 2025/26

---

## Overview

A command-line tool created for the ELEC2645 Unit 2 project is called the Engineering Helper Toolkit.
 The objective is to offer a small collection of engineering utilities that are commonly needed in:


 - ELEC2101: Design of Electronic Circuits and Systems

 - General electronics laboratory tasks

 - Signal processing fundamentals

 - Sensor-to-ADC conversion job

 The software extends on the Unit Version 2.1 template by providing new tools, greater input validation, file logging, and internal automated testing.

---

## Features

### **1. Signal Analyser**

- Accepts input from users samples

- Determines: minimum, maximum, peak-to-peak, RMS

- Produces ASCII bar graphs

- The ability to save results into results.txt

---

### **2. ADC Converter**

- Produces voltage from 10-bit ADC data (0–1023).

- Makes use of a variable reference voltage

- Uses a proportionate model (100°C/V) to estimate temperature.

- Enables file logging

---

### **3. RC Filter Calculator**
Uses the standard cutoff formula:

```
fc = 1 / (2πRC)

```

Two modes:

- Enter R → calculate C

- Enter C → calculate R

Important for ELEC2101 labs and filter computation.

---

### **4. Unit Converter**
Quick conversions:

- dBm → mW  
- mW → dBm  
- Hz → rad/s  
- rad/s → Hz  
- Vpeak → Vrms  
- Vrms → Vpeak  

Every conversion uses the appropriate technical formulas.

---

### **5. Resistor Colour Code Calculator**
- Takes into account every common resistor band. 
- Calculates:
  - Recognises typical resistor colour bands
  - Produces accurate resistance and scaled values (Ω, kΩ, MΩ)  
  - Tolerance
  - Results can be saved to results.txt.
  

---

### **6. AI Helper**
Includes:
- Short overviews in engineering (RMS, ADC, RC filters)  
- A multi-choice test 
- Display of stored results
- Automated internal testing (three tests)

Example output:
```
Passed 3/3 tests.
```

---

## File Logging

The outcomes are added to a text file and saved:

```
results.txt
```

Example entries:
```
SIGNAL mn=0.0000 mx=98.0000 p2p=98.0000 rms=50.3310
ADC adc=512 vref=3.30 V=1.6500 T=165.00
RES R=1000 tol=±5%
```

The following improves project completion and makes traceability easier.

---

## Way to Create

Compile using gcc:

```bash
gcc main.c funcs.c -o main.out -lm
```

`-lm` links the math library.

---

## Way to Run

```bash
./main.out
```

You will see:

```
1. Signal analyser
2. ADC converter
3. RC filter calculator
4. Unit converter
5. Resistor colour code calculator
6. AI helper
7. Exit
```

---

## Project Structure

```
│── main.c          # The menu navigation
│── funcs.c         # Full utilisation of each tool
│── funcs.h         # Definitions of functions
│── results.txt     # Logs 
│── README.md       # Recordkeeping
│── Makefile        # Program for extra setups
```

---

## Testing

The AI Assistant has a built-in test harness.

To run tests:
```
Menu → 6 → 4
```

Expected output:
```
Passed 3/3 tests.
```

The result validates the accuracy of the Hzrad/s round-trip calculation, dBm conversion, and RMS calculation.

---

## Notes

- Strong input validation is employed throughout the application.

- Extra functions (logging, quiz, explanations, testing) go beyond the bare requirements.

- The toolbox is flexible and easy to extend

---

## License
This task was built for use in ELEC2645 Unit 2 at the University of Leeds.
