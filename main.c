// Unit 2 Project for ELEC2645
// Code for handling menus and the main menu

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "funcs.h"

// Functionality prototypes 
static void main_menu(void);
static void print_main_menu(void);
static int get_user_input(void);
static void select_menu_item(int input);
static void go_back_to_main(void);
static int is_integer(const char *s);

int main(void)
{
    // Until the user selects Exit, the software continues.
    // A basic infinite loop continually showing the menu.
    for (;;) {
        main_menu();
    }
    return 0;
}

static void main_menu(void)
{
    // Present menu and get input
    print_main_menu();
    int choice = get_user_input();

    // Launch the functionality that the user chose.
    select_menu_item(choice);
}

static int get_user_input(void)
{
    // The number of menu items that are valid (1–7)
    enum { MENU_ITEMS = 7 };

    char buf[128];
    int value = 0;
    int ok = 0;

    while (!ok) {
        printf("\nSelect item: ");

        // Read input as a string (safer than scanf)
        if (!fgets(buf, sizeof(buf), stdin)) {
            printf("Input error.\n");
            exit(1);
        }

        // Remove newline
        buf[strcspn(buf, "\n")] = 0;

        // Verify that it is a valid integer.
        if (!is_integer(buf)) {
            printf("Enter a valid number.\n");
            continue;
        }

        value = (int)strtol(buf, NULL, 10);

        // Verify that the number is inside the menu range.
        if (value < 1 || value > MENU_ITEMS) {
            printf("Invalid menu item.\n");
        } else {
            ok = 1;
        }
    }

    return value;
}

static void select_menu_item(int input)
{
    // Based on the menu selection, call the appropriate module.
    switch (input) {
        case 1:
            menu_item_1();   // Signal analyser
            break;
        case 2:
            menu_item_2();   // ADC converter
            break;
        case 3:
            menu_item_3();   // RC filter calculator
            break;
        case 4:
            menu_item_4();   // Unit converter
            break;
        case 5:
            menu_item_5();   // Resistor colour code calculator
            break;
        case 6:
            menu_item_6();   // AI helper (explain + quiz)
            break;
        default:
            // Option 7 = exit program
            printf("Bye!\n");
            exit(0);
    }

    // Upon completing any tool, return to menu
    go_back_to_main();
}

static void print_main_menu(void)
{
    // Simple text-based menu
    printf("\n----------- Main Menu -----------\n\n");
    printf(" 1. Signal analyser\n");
    printf(" 2. ADC converter\n");
    printf(" 3. RC filter calculator\n");
    printf(" 4. Unit converter\n");
    printf(" 5. Resistor colour code calculator\n");
    printf(" 6. AI helper (explain & quiz)\n");
    printf(" 7. Exit\n");
    printf("---------------------------------\n");
}

static void go_back_to_main(void)
{
    // Requires the user to hit "b" in order to return
    char buf[32];

    do {
        printf("\nEnter 'b' to go back: ");

        if (!fgets(buf, sizeof(buf), stdin)) {
            printf("Input error.\n");
            exit(1);
        }

        buf[strcspn(buf, "\n")] = 0;

    } while (!(buf[0] == 'b' || buf[0] == 'B') || buf[1] != '\0');
}

static int is_integer(const char *s)
{
    // Verifies if a string includes an acceptable integer (no letters)
    if (!s || !*s) return 0;

    // Optional leading sign
    if (*s == '+' || *s == '-') s++;

    // A digit must be the first character.
    if (!isdigit((unsigned char)*s)) return 0;

    // Verify the remaining characters
    while (*s) {
        if (!isdigit((unsigned char)*s)) return 0;
        s++;
    }

    return 1;
}
