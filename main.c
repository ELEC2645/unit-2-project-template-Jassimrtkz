// ELEC2645 Unit 2 Project
// Main menu and menu handling code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "funcs.h"

// prototypes
static void main_menu(void);
static void print_main_menu(void);
static int get_user_input(void);
static void select_menu_item(int input);
static void go_back_to_main(void);
static int is_integer(const char *s);

int main(void)
{
    // run forever until exit is selected
    for (;;) {
        main_menu();
    }
    return 0;
}

static void main_menu(void)
{
    print_main_menu();
    int choice = get_user_input();
    select_menu_item(choice);
}

static int get_user_input(void)
{
    enum { MENU_ITEMS = 7 };  // 1..6 + exit

    char buf[128];
    int value = 0;
    int ok = 0;

    while (!ok) {
        printf("\nSelect item: ");

        if (!fgets(buf, sizeof(buf), stdin)) {
            printf("Input error.\n");
            exit(1);
        }

        buf[strcspn(buf, "\n")] = 0;

        if (!is_integer(buf)) {
            printf("Enter a valid number.\n");
            continue;
        }

        value = (int)strtol(buf, NULL, 10);

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
    switch (input) {
        case 1:
            menu_item_1();   // signal analyser
            break;
        case 2:
            menu_item_2();   // adc converter
            break;
        case 3:
            menu_item_3();   // rc filter
            break;
        case 4:
            menu_item_4();   // unit converter
            break;
        case 5:
            menu_item_5();   // resistor code
            break;
        case 6:
            menu_item_6();   // ai helper
            break;
        default:
            printf("Bye!\n");
            exit(0);
    }

    go_back_to_main();
}

static void print_main_menu(void)
{
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
    if (!s || !*s) return 0;

    if (*s == '+' || *s == '-') s++;

    if (!isdigit((unsigned char)*s)) return 0;

    while (*s) {
        if (!isdigit((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}
