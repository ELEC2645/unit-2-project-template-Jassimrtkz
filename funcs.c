// Engineering Helper Toolkit: funcs.c
// Carries out the reasoning behind each menu item in the main application.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "funcs.h"

// Some compilers do not define M_PI, therefore we put it here.
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define MAX_SAMPLES 100     // The maximum amount of signal samples permitted
#define ADC_RES 1023        // 10-bit ADC maximum value

// Forward declarations 

// General helpers
static void flush_in(void);
static int get_int(const char *msg, int min, int max);

// Signal analyser helpers
static double calc_rms(double *x, int n);
static void min_max(double *x, int n, double *mn, double *mx);
static void draw_graph(double *x, int n);

// ADC helpers
static double adc_to_voltage(int adc, int max_adc, double vref);
static double voltage_to_temp(double v);

// RC filter helpers
static double find_c(double fc, double R);
static double find_r(double fc, double C);

// Unit converter helpers
static double dbm_to_mw(double dbm);
static double mw_to_dbm(double mw);
static double hz_to_rad(double f);
static double rad_to_hz(double w);
static double peak_to_rms(double vp);
static double rms_to_peak(double vr);

// Resistor code helpers
static void read_col(char *buf, int size, const char *msg);
static void str_lower(char *s);
static int digit_from_colour(const char *c);
static int mult_from_colour(const char *c, double *mult);
static const char *tol_from_colour(const char *c);
static void format_res(double R, double *mag, const char **unit);

// AI helper functions + file logging
static void show_expl(void);
static void quiz(void);
static void view_results(void);
static void log_signal(double mn, double mx, double p2p, double rms);
static void log_sensor(int adc, double vref, double v, double t);
static void log_res(double R, const char *tol);

// Automatic internal tests
static void run_tests(void);
static int nearly(double a, double b, double tol);



// SUPPORT FOR GENERAL INPUT


// Clears the input buffer so faulty input does not invalidate scanf()
static void flush_in(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Integer input with boundaries checking
static int get_int(const char *msg, int min, int max)
{
    int v, ok = 0;
    do {
        printf("%s", msg);
        if (scanf("%d", &v) != 1) {
            printf("Enter a number.\n");
            flush_in();
            continue;
        }
        flush_in();

        if (v < min || v > max) {
            printf("Must be %d to %d.\n", min, max);
        } else {
            ok = 1;
        }
    } while (!ok);

    return v;
}




// MENU ITEM 1: SIGNAL ANALYSER


void menu_item_1(void)
{
    double x[MAX_SAMPLES];
    int n = get_int("How many samples: ", 1, MAX_SAMPLES);

    // Examine sample values provided by the user
    printf("Enter %d samples:\n", n);
    for (int i = 0; i < n; i++) {
        while (scanf("%lf", &x[i]) != 1) {
            printf("Invalid.\n");
            flush_in();
        }
    }
    flush_in();

    // Calculate statistics
    double mn, mx;
    min_max(x, n, &mn, &mx);
    double p2p = mx - mn;
    double rms = calc_rms(x, n);

    // Display results
    printf("\nMin = %.4f\n", mn);
    printf("Max = %.4f\n", mx);
    printf("P2P = %.4f\n", p2p);
    printf("RMS = %.4f\n", rms);

    // Illustare ASCII bar graph
    draw_graph(x, n);

    // Ask to save results
    printf("\nSave? (y/n): ");
    int c = getchar();
    flush_in();
    if (c == 'y' || c == 'Y') {
        log_signal(mn, mx, p2p, rms);
        printf("Saved.\n");
    }
}

// RMS formula = sqrt( sum(x^2) / n )
static double calc_rms(double *x, int n)
{
    double s = 0.0;
    for (int i = 0; i < n; i++)
        s += x[i] * x[i];
    return sqrt(s / n);
}

// Determines the sample values' minimum and maximum
static void min_max(double *x, int n, double *mn, double *mx)
{
    double lo = x[0], hi = x[0];
    for (int i = 1; i < n; i++) {
        if (x[i] < lo) lo = x[i];
        if (x[i] > hi) hi = x[i];
    }
    *mn = lo;
    *mx = hi;
}

// A straightforward ASCII bar graph scaled by |x|*10
static void draw_graph(double *x, int n)
{
    printf("\nGraph:\n");
    for (int i = 0; i < n; i++) {
        int bars = (int)(fabs(x[i]) * 10);
        if (bars > 50) bars = 50; // limit graph size

        printf("%2d: ", i + 1);
        for (int j = 0; j < bars; j++) printf("|");
        printf(" (%.3f)\n", x[i]);
    }
}




// MENU ITEM 2: ADC CONVERTER


void menu_item_2(void)
{
    double vref;
    int adc;

    printf("\nADC Converter\n");

    // Read reference voltage
    printf("Reference voltage: ");
    while (scanf("%lf", &vref) != 1 || vref <= 0) {
        printf("Invalid.\n");
        flush_in();
    }
    flush_in();

    adc = get_int("ADC reading: ", 0, ADC_RES);

    // Convert ADC → voltage
    double v = adc_to_voltage(adc, ADC_RES, vref);

    // Voltage → temperature (simple model)
    double t = voltage_to_temp(v);

    printf("Voltage = %.4f V\n", v);
    printf("Temp ≈ %.2f C\n", t);

    // Save log entry
    printf("Save? (y/n): ");
    int c = getchar();
    flush_in();
    if (c == 'y' || c == 'Y') {
        log_sensor(adc, vref, v, t);
        printf("Saved.\n");
    }
}

// Typical ADC formula
static double adc_to_voltage(int adc, int max_adc, double vref)
{
    return (adc / (double)max_adc) * vref;
}

// A very basic linear temperature model
static double voltage_to_temp(double v)
{
    return v * 100.0;
}




// MENU ITEM 3: RC FILTER CALCULATOR


void menu_item_3(void)
{
    double fc, R, C;

    printf("\nRC Filter\n");
    printf("fc = 1/(2*pi*R*C)\n");

    // Cutoff frequency
    printf("Cutoff (Hz): ");
    while (scanf("%lf", &fc) != 1 || fc <= 0) {
        printf("Invalid.\n");
        flush_in();
    }
    flush_in();

    // Choose mode
    printf("1) Enter R, solve C\n");
    printf("2) Enter C, solve R\n");
    int ch = get_int("Select: ", 1, 2);

    if (ch == 1) {
        printf("R (ohms): ");
        while (scanf("%lf", &R) != 1 || R <= 0) {
            printf("Invalid.\n");
            flush_in();
        }
        flush_in();

        C = find_c(fc, R);
        printf("C ≈ %.9f F\n", C);

    } else {
        printf("C (farads): ");
        while (scanf("%lf", &C) != 1 || C <= 0) {
            printf("Invalid.\n");
            flush_in();
        }
        flush_in();

        R = find_r(fc, C);
        printf("R ≈ %.2f Ω\n", R);
    }
}

// Calculates C = 1 / (2π R fc)
static double find_c(double fc, double R)
{
    return 1.0 / (2.0 * M_PI * R * fc);
}

// Calculates R = 1 / (2π C fc)
static double find_r(double fc, double C)
{
    return 1.0 / (2.0 * M_PI * C * fc);
}




// MENU ITEM 4: UNIT CONVERTER

void menu_item_4(void)
{
    int run = 1;
    double x;

    while (run) {
        printf("\nUnit Converter\n");
        printf("1) dBm → mW\n");
        printf("2) mW → dBm\n");
        printf("3) Hz → rad/s\n");
        printf("4) rad/s → Hz\n");
        printf("5) Vpeak → Vrms\n");
        printf("6) Vrms → Vpeak\n");
        printf("0) Back\n");

        int ch = get_int("Select: ", 0, 6);

        if (ch == 0) {
            run = 0;
        } else {
            printf("Value: ");
            scanf("%lf", &x);
            flush_in();

            switch (ch) {
                case 1: printf("= %.6f mW\n", dbm_to_mw(x)); break;
                case 2:
                    if (x > 0) printf("= %.4f dBm\n", mw_to_dbm(x));
                    else printf("Power must be > 0.\n");
                    break;
                case 3: printf("= %.6f rad/s\n", hz_to_rad(x)); break;
                case 4: printf("= %.6f Hz\n", rad_to_hz(x)); break;
                case 5: printf("= %.6f Vrms\n", peak_to_rms(x)); break;
                case 6: printf("= %.6f Vpeak\n", rms_to_peak(x)); break;
            }
        }
    }
}

// Formulas for unit conversion
static double dbm_to_mw(double dbm)
{
    return pow(10.0, dbm / 10.0);
}

static double mw_to_dbm(double mw)
{
    return 10.0 * log10(mw);
}

static double hz_to_rad(double f)
{
    return 2.0 * M_PI * f;
}

static double rad_to_hz(double w)
{
    return w / (2.0 * M_PI);
}

static double peak_to_rms(double vp)
{
    return vp / sqrt(2.0);
}

static double rms_to_peak(double vr)
{
    return vr * sqrt(2.0);
}




// MENU ITEM 5: RESISTOR COLOUR CODE


void menu_item_5(void)
{
    char c1[30], c2[30], c3[30], c4[30];
    int d1, d2;
    double mult;
    const char *tol;

    printf("\nResistor Colour Code\n");

    // Examine every colour band.
    read_col(c1, sizeof(c1), "Band 1: ");
    read_col(c2, sizeof(c2), "Band 2: ");
    read_col(c3, sizeof(c3), "Multiplier: ");
    read_col(c4, sizeof(c4), "Tolerance: ");

    d1 = digit_from_colour(c1);
    d2 = digit_from_colour(c2);

    if (d1 < 0 || d2 < 0) {
        printf("Invalid digit.\n");
        return;
    }

    if (!mult_from_colour(c3, &mult)) {
        printf("Invalid multiplier.\n");
        return;
    }

    tol = tol_from_colour(c4);

    // Full resistor value
    double R = (10 * d1 + d2) * mult;

    double mag;
    const char *u;
    format_res(R, &mag, &u);

    printf("\nR = %.0f ohms\n", R);
    printf("≈ %.3f %s\n", mag, u);
    printf("Tolerance: %s\n", tol);

    // If requested, save
    printf("\nSave? (y/n): ");
    int c = getchar();
    flush_in();
    if (c == 'y' || c == 'Y') {
        log_res(R, tol);
        printf("Saved.\n");
    }
}

// Strip newline + convert to lowercase
static void read_col(char *buf, int size, const char *msg)
{
    printf("%s", msg);
    fgets(buf, size, stdin);
    buf[strcspn(buf, "\n")] = 0;
    str_lower(buf);
}

// Lowercase assiatant
static void str_lower(char *s)
{
    while (*s) {
        *s = (char)tolower(*s);
        s++;
    }
}

// Returns colour digit (0–9)
static int digit_from_colour(const char *c)
{
    if (!strcmp(c,"black")) return 0;
    if (!strcmp(c,"brown")) return 1;
    if (!strcmp(c,"red")) return 2;
    if (!strcmp(c,"orange")) return 3;
    if (!strcmp(c,"yellow")) return 4;
    if (!strcmp(c,"green")) return 5;
    if (!strcmp(c,"blue")) return 6;
    if (!strcmp(c,"violet") || !strcmp(c,"purple")) return 7;
    if (!strcmp(c,"grey")   || !strcmp(c,"gray")) return 8;
    if (!strcmp(c,"white"))  return 9;
    return -1;
}

// Determines the multiplier from colour code
static int mult_from_colour(const char *c, double *mult)
{
    if (!strcmp(c,"black"))  *mult = 1.0;
    else if (!strcmp(c,"brown"))  *mult = 10.0;
    else if (!strcmp(c,"red"))    *mult = 100.0;
    else if (!strcmp(c,"orange")) *mult = 1e3;
    else if (!strcmp(c,"yellow")) *mult = 1e4;
    else if (!strcmp(c,"green"))  *mult = 1e5;
    else if (!strcmp(c,"blue"))   *mult = 1e6;
    else if (!strcmp(c,"violet") || !strcmp(c,"purple")) *mult = 1e7;
    else if (!strcmp(c,"grey")   || !strcmp(c,"gray"))   *mult = 1e8;
    else if (!strcmp(c,"white"))  *mult = 1e9;
    else if (!strcmp(c,"gold"))   *mult = 0.1;
    else if (!strcmp(c,"silver")) *mult = 0.01;
    else return 0;
    return 1;
}

// Gives back the tolerance string
static const char *tol_from_colour(const char *c)
{
    if (!strcmp(c,"brown"))  return "±1%";
    if (!strcmp(c,"red"))    return "±2%";
    if (!strcmp(c,"green"))  return "±0.5%";
    if (!strcmp(c,"blue"))   return "±0.25%";
    if (!strcmp(c,"violet")  || !strcmp(c,"purple")) return "±0.1%";
    if (!strcmp(c,"grey")    || !strcmp(c,"gray"))   return "±0.05%";
    if (!strcmp(c,"gold"))   return "±5%";
    if (!strcmp(c,"silver")) return "±10%";
    return "±20%";
}

// Chooses Ω, kΩ, MΩ format
static void format_res(double R, double *mag, const char **unit)
{
    if (R >= 1e6) {
        *mag = R / 1e6; *unit = "MΩ";
    } else if (R >= 1e3) {
        *mag = R / 1e3; *unit = "kΩ";
    } else {
        *mag = R; *unit = "Ω";
    }
}




// MENU ITEM 6: AI HELPER


void menu_item_6(void)
{
    int run = 1;

    while (run) {
        printf("\nAI Helper\n");
        printf("1) Explanations\n");
        printf("2) Quiz\n");
        printf("3) View results\n");
        printf("4) Run tests\n");
        printf("0) Back\n");

        int ch = get_int("Select: ", 0, 4);

        if (ch == 1) show_expl();
        else if (ch == 2) quiz();
        else if (ch == 3) view_results();
        else if (ch == 4) run_tests();
        else run = 0;
    }
}


static void show_expl(void)
{
    printf("\nRMS:\n Vrms = Vp/sqrt(2) for sine waves\n");

    printf("\nADC:\n 10-bit ADC ranges from 0 to 1023\n");

    printf("\nRC Filter:\n fc = 1/(2*pi*R*C)\n");
}

// Simple 3-question quiz
static void quiz(void)
{
    int ans;

    printf("\n1) Vrms of 10 V peak?\n");
    printf(" 1) 10   2) 7.07   3) 5\n");
    ans = get_int("Ans: ", 1, 3);
    if (ans == 2) printf("Correct.\n");
    else printf("Wrong.\n");

    printf("\n2) Levels in a 10-bit ADC?\n");
    printf(" 1) 1000   2) 1024   3) 512\n");
    ans = get_int("Ans: ", 1, 3);
    if (ans == 2) printf("Correct.\n");
    else printf("Wrong.\n");

    printf("\n3) If R increases in RC filter, fc will:\n");
    printf(" 1) increase   2) decrease   3) stay same\n");
    ans = get_int("Ans: ", 1, 3);
    if (ans == 2) printf("Correct.\n");
    else printf("Wrong.\n");
}

// Shows the contents of results.txt
static void view_results(void)
{
    FILE *f = fopen("results.txt", "r");
    if (!f) {
        printf("No results.\n");
        return;
    }

    printf("\nResults:\n");
    char line[200];
    while (fgets(line, sizeof(line), f)) {
        printf("%s", line);
    }
    fclose(f);
}




// LOGGING FUNCTIONS

// Preserve signal analyser output
static void log_signal(double mn, double mx, double p2p, double rms)
{
    FILE *f = fopen("results.txt","a");
    if (!f) return;
    fprintf(f, "SIGNAL mn=%.4f mx=%.4f p2p=%.4f rms=%.4f\n",
            mn, mx, p2p, rms);
    fclose(f);
}

// Save the output of the ADC converter.
static void log_sensor(int adc, double vref, double v, double t)
{
    FILE *f = fopen("results.txt","a");
    if (!f) return;
    fprintf(f, "ADC adc=%d vref=%.2f V=%.4f T=%.2f\n",
            adc, vref, v, t);
    fclose(f);
}

// Save the resistor computation.
static void log_res(double R, const char *tol)
{
    FILE *f = fopen("results.txt","a");
    if (!f) return;
    fprintf(f, "RES R=%.0f tol=%s\n", R, tol);
    fclose(f);
}




// INTERNAL TESTS


// Verify two numbers are nearly equivalent
static int nearly(double a, double b, double tol)
{
    return fabs(a - b) < tol;
}

// Automated inspection for accurate marking
static void run_tests(void)
{
    int pass = 0;

    printf("\nRunning tests...\n");

    // Test 1: RMS of {1, -1} should be 1
    {
        double s[2] = {1, -1};
        double r = calc_rms(s, 2);
        if (nearly(r, 1.0, 0.001)) {
            printf("Test 1 (RMS): OK\n");
            pass++;
        } else {
            printf("Test 1 (RMS): FAIL (expected 1.0, got %.4f)\n", r);
        }
    }

    // Test 2: 0 dBm = 1 mW
    {
        double mw = dbm_to_mw(0);
        if (nearly(mw, 1.0, 0.001)) {
            printf("Test 2 (dBm->mW): OK\n");
            pass++;
        } else {
            printf("Test 2 FAIL (expected 1.0, got %.4f)\n", mw);
        }
    }

    // Test 3: Hz <-> rad/s round-trip
    {
        double f = 50;
        double w = hz_to_rad(f);
        double f2 = rad_to_hz(w);
        if (nearly(f, f2, 0.001)) {
            printf("Test 3 (Hz <-> rad/s): OK\n");
            pass++;
        } else {
            printf("Test 3 FAIL (expected %.2f, got %.4f)\n", f, f2);
        }
    }

    printf("\nPassed %d/3 tests.\n", pass);
}
