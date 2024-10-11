/*
  Author:       Benjamin G. Friedman
  Date:         05/20/2021
  File:         Main.c
  Description:  The main function for the matrix operations program.
*/


#include <stdio.h>
#include <stdlib.h>
#include "Menu.h"


int main(void)
{
	MenuOption userChoice;

	do {
		userChoice = menu_getUserChoice();
		if (!menu_implementUserChoice(userChoice)) {
			printf("Memory allocation failure. Exiting the program.\n");
			exit(1);
		}
	} while (userChoice);

	return 0;
}
