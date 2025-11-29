# Engineering Helper Toolkit  
ELEC2645 – Unit 2 Individual Project  
Author: Jassim Khalid I S Almuhaiza  
Academic Year: 2025/26

---

## Overview

A command-line tool created for the ELEC2645 Unit 2 project is called the Engineering Helper Toolkit.  
The software aims to offer a small collection of tools that are frequently needed in computer engineering and electronics.  
Calculations pertinent to modules like these are included.

- **ELEC2101 – Electronic Circuits & Systems Design**
- Work at the general electronics lab
- Basics of signal processing
- Converting sensors to ADCs

The program adds new features, enhanced input validation, file logging, and internal automated testing to the Unit Version 2.1 basic menu template.

---

## Features

### **1. Signal Analyser**
- Enter a list of examples 
- calculates:
  - Minimal quantity  
  - Maximum amount  
  - Peak-to-Peak  
  - RMS  
- shows a sample magnitude bar graph in ASCII.  
- The choice to store every result in `results.txt`

---

### **2. ADC Converter**
- 10-bit ADC (0–1023) values are converted to voltage  
- includes a reference voltage that the user can adjust. 
- uses a basic linear model to estimate temperature. (100°C/V)  
- able to record outcomes in a file

---

### **3. RC Filter Calculator**
Uses the standard cutoff formula:

```
fc = 1 / (2πRC)
```

Two modes:
- Enter R → calculate C  
- Enter C → calculate R  

helpful for lab tasks and ELEC2101 filter design.

---

### **4. Unit Converter**
Quick conversions:

- dBm → mW  
- mW → dBm  
- Hz → rad/s  
- rad/s → Hz  
- Vpeak → Vrms  
- Vrms → Vpeak  

The right engineering formulas are used in each conversion.

---

### **5. Resistor Colour Code Calculator**
- accepts all common resistor bands. 
- Calculates:
  - Precise resistance 
  - Scaled value (Ω, kΩ, MΩ)  
  - Tolerance
  - Records outcomes in `results.txt`  
  

---

### **6. AI Helper**
Includes:
- Brief explanations in engineering (RMS, ADC, RC filters)  
- A multi-choice test 
- Viewer of stored results 
- Internal testing that is automated (3 tests)

Example output:
```
Passed 3/3 tests.
```

---

## File Logging

Results are saved and added to a text file:

```
results.txt
```

Example entries:
```
SIGNAL mn=0.0000 mx=98.0000 p2p=98.0000 rms=50.3310
ADC adc=512 vref=3.30 V=1.6500 T=165.00
RES R=1000 tol=±5%
```

The following enhances project completeness and facilitates traceability.

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
│── main.c          # Menu handling 
│── funcs.c         # Complete use of every tool
│── funcs.h         # Definitions of functions
│── results.txt     # Logs 
│── README.md       # Recordkeeping
│── Makefile        # Script for optional builds
```

---

## Testing

There is a test harness built into the AI Assistant.

To run tests:
```
Menu → 6 → 4
```

Expected output:
```
Passed 3/3 tests.
```

The result confirms the accuracy of the Hzrad/s round-trip calculation, dBm conversion, and RMS calculation.

---

## Notes

- To avoid crashes, input validation was applied throughout.  
- Extra features (testing + explanations + logging)  exhibit imagination and comprehension that goes beyond the necessities.  

---

## License
This project was created for use in ELEC2645 Unit 2 at the University of Leeds.
