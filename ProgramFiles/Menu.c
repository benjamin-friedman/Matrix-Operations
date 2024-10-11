/*
  Author:       Benjamin G. Friedman
  Date:         05/20/2021
  File:         Menu.c
  Description:  Implementation file for the matrix operations menu interface.
*/


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Menu.h"

typedef struct menuOptionMessages {
	MenuOption menuOption;
	char message[100];
} MenuOptionMessages;

const MenuOptionMessages menuOptionMessages[] = {
	{ QUIT,  "Quit" },
	{ MULT,  "Multiplication" },
	{ ADD,   "Addition" },
	{ SUB,   "Subtraction" },
	{ POW,   "Power" },
	{ TRANS, "Transpose" },
	{ DET,   "Determinant" },
	{ INV,   "Inverse" },
};
const int menuOptionMessagesSize = sizeof(menuOptionMessages) / sizeof(*menuOptionMessages);

#define INPUT_BUF_CAP 500




/*********** Declarations for helper functions defined in Matrix.c **********/
int at2(int rows, int cols, int row, int col);




/*********** Declarations for helper functions defined in this file **********/
/*
FUNCTION
  - Name:     createOrdinalNum
  - Purpose:  Create an ordinal number as a string from a given integer.
              An ordinal number is a number with two characters appended to it to be used in a sentence.
              For example, the ordinal number of 1 is 1st.
PRECONDITION
  - ordNum
      Purpose:       Array of characters to hold the string representation of the ordinal number.
      Restrictions:  Capacity is large enough to hold the ordinal number as a string.
  - num
      Purpose:       Number from which to create the ordinal number.
      Restrictions:  N/A
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       Creates an ordinal number as a string from a given integer.
  - Return value:  N/A
  - ordNum:        The ordinal number of the number as a string is stored in the array.
Failure
  - N/A
*/
static void createOrdinalNum(char* ordNum, int num);


/*
FUNCTION
  - Name:     dimsCanBeMultiplied
  - Purpose:  Determine if the dimensions of two matrices can be multiplied.
              Two matrices can be multiplied if the columns of matrix 1 equal the rows of matrix 2.
PRECONDITION
  - cols1
      Purpose:       The columns of matrix 1.
	  Restrictions:  Any positive integer.
  - rows2
      Purpose:       The rows of matrix 2.
	  Restrictions:  Any positive integer. 
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       The correct value is returned accordingly.
  - Return value:  TRUE if the columns of matrix 1 equal the rows of matrix 2.
                   FALSE if otherwise.
Failure
  - N/A
*/
static Boolean dimsCanBeMultiplied(int cols1, int rows2);


/*
FUNCTION
  - Name:     displayDimsPrompt
  - Purpose:  Display the prompt for the user to enter the dimensions of a matrix.
PRECONDITION
  - op
      Purpose:       Indicate which matrix operation is being performed.
      Restrictions:  Not QUIT.
  - pMxNum
      Purpose:       Indicate if "1st" or "2nd" should be printed for multiplication.
      Restrictions:  If operation is multiplication, the integer it points to is 1 or 2.
					 NULL if otherwise.
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       Displays the prompt for the user to enter the dimensions of a matrix.
  - Return value:  N/A
Failure
  - N/A
*/
static void displayDimsPrompt(MenuOption op, const int* pMxNum);


/*
FUNCTION
  - Name:     displayEntriesPrompt
  - Purpose:  Display the prompt for the user to enter the entries of a matrix.
PRECONDITION
  - op
      Purpose:       Indicate which matrix operation is being performed.
      Restrictions:  Not QUIT.
  - pMxNum
      Purpose:       Indicate if "1st", "2nd", "3rd"... should be printed for addition and subtraction.
      Restrictions:  If operation is addition or subtraction, the integer it points to is any positive integer.
	                 NULL if otherwise.
  - rows
      Purpose:       The rows of the matrix for which the prompt is being displayed.
	  Restrictions:  Any positive integer.
  - cols
      Purpose:       The columns of the matrix for which the prompt is being displayed.
	  Restrictions:  Any positive integer.     
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       Displays the prompt for the user to enter the entries of a matrix.
  - Return value:  N/A
Failure
  - N/A
*/
static void displayEntriesPrompt(MenuOption op, const int* pMxNum, int rows, int cols);


/*
FUNCTION
  - Name:     displayNumMatricesPrompt
  - Purpose:  Display the prompt for the user to enter the number of matrices to add or subtract.
PRECONDITION
  - op
      Purpose:       Indicate if the matrix operation is addition or subtraction.
      Restrictions:  ADD or SUB
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       Displays the prompt for the user to enter the number of matrices to add or subtract.
  - Return value:  N/A
Failure
  - N/A
*/
static void displayNumMatricesPrompt(MenuOption op);


/*
FUNCTION
  - Name:     displayPowPrompt
  - Purpose:  Display the prompt for the user to enter the power for the matrix power operation.
PRECONDITION
  - N/A
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       Displays the prompt for the user to enter the power for the matrix power operation.
  - Return value:  N/A
Failure
  - N/A
*/
static void displayPowPrompt(void);


/*
FUNCTION
  - Name:     displayResultsMatrixOpAdd
  - Purpose:  Display the result of the matrix addition operation.
PRECONDITION
  - The matrix addition operation has happened.
  - hMxs
      Purpose:       The matrices that were added.
      Restrictions:  Array of handles to valid matrix objects.
  - hMxsSize
      Purpose:       The size of the array of matrices (the number of matrices that were added).
      Restrictions:  Equals the size of array of matrices.
  - hMxRes
      Purpose:       The result matrix that stores the result of the addition operation.
      Restrictions:  Handle to a valid matrix object.
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       Displays the result of the matrix addition operation.
  - Return value:  N/A
Failure
  - N/A
*/
static void displayResultsMatrixOpAdd(MATRIX* hMxs, int hMxsSize, MATRIX hMxRes);


/*
FUNCTION
  - Name:     displayResultsMatrixOpDet
  - Purpose:  Display the result of the matrix determinant operation.
PRECONDITION
  - The matrix determinant operation has happened.
  - hMx
      Purpose:       The matrix for which the determinant was calculated.
      Restrictions:  Handle to a valid matrix object.
  - res
      Purpose:       The result of the determinant operation.
      Restrictions:  N/A
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       Displays the result of the matrix determinant operation.
  - Return value:  N/A
Failure
  - N/A
*/
static void displayResultsMatrixOpDet(MATRIX hMx, double res);


/*
FUNCTION
  - Name:     displayResultsMatrixOpInv
  - Purpose:  Display the result of the matrix inverse operation.
PRECONDITION
  - The matrix inverse operation has happened.
  - hMxs
      Purpose:       The matrix that was inverted.
      Restrictions:  Handle to a valid matrix object.
  - mxIsInvertible
      Purpose:       Indicate if the matrix was invertible.
      Restrictions:  N/A
  - hMxRes
      Purpose:       The result matrix that stores the inverted matrix if it was vertible.
      Restrictions:  Handle to a valid matrix object.
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       Displays the result of the matrix inverse operation.
  - Return value:  N/A
Failure
  - N/A
*/
static void displayResultsMatrixOpInv(MATRIX hMx, Boolean mxIsInvertible, MATRIX hMxRes);


/*
FUNCTION
  - Name:     displayResultsMatrixOpMult
  - Purpose:  Display the result of the matrix multiplication operation.
PRECONDITION
  - The matrix multiplication operation has happened.
  - hMx1
      Purpose:       The first matrix that was multiplied.
      Restrictions:  Handle to a valid matrix object.
  - hMx2
      Purpose:       The second matrix that was multiplied.
      Restrictions:  Handle to a valid matrix object.
  - hMxRes
      Purpose:       The result matrix that is the result of the multiplication operation.
      Restrictions:  Handle to a valid matrix object.
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       Displays the result of the matrix multiplication operation.
  - Return value:  N/A
Failure
  - N/A
*/
static void displayResultsMatrixOpMult(MATRIX hMx1, MATRIX hMx2, MATRIX hMxRes);


/*
FUNCTION
  - Name:     displayResultsMatrixOpPow
  - Purpose:  Display the result of the matrix power operation.
PRECONDITION
  - The matrix power operation has happened.
  - hMx
      Purpose:       The matrix used in the power operation.
      Restrictions:  Handle to a valid matrix object.
  - power
      Purpose:       The power used in the power operation.
      Restrictions:  Any positive integer.
  - hMxRes
      Purpose:       The result matrix that is the result of the power operation.
      Restrictions:  Handle to a valid matrix object.
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       Displays the result of the matrix power operation.
  - Return value:  N/A
Failure
  - N/A
*/
static void displayResultsMatrixOpPow(MATRIX hMx, int power, MATRIX hMxRes);


/*
FUNCTION
  - Name:     displayResultsMatrixOpSub
  - Purpose:  Display the result of the matrix subtraction operation.
PRECONDITION
  - The matrix subtraction operation has happened.
  - hMxs
      Purpose:       The matrices that were subtracted.
      Restrictions:  Array of handles to valid matrix objects.
  - hMxsSize
      Purpose:       The size of the array of matrices (the number of matrices that were subtracted).
      Restrictions:  Equals the size of array of matrices.
  - hMxRes
      Purpose:       The result matrix that stores the result of the subtraction operation.
      Restrictions:  Handle to a valid matrix object.
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       Displays the result of the matrix subtraction operation.
  - Return value:  N/A
Failure
  - N/A
*/
static void displayResultsMatrixOpSub(MATRIX* hMxs, int hMxsSize, MATRIX hMxRes);


/*
FUNCTION
  - Name:     displayResultsMatrixOpTrans
  - Purpose:  Display the result of the matrix transpose operation.
PRECONDITION
  - The matrix addition transpose has happened.
  - hMx
      Purpose:       The matrix that was transposed.
      Restrictions:  Handle to a valid matrix object.
  - hMxRes
      Purpose:       The result matrix that is the result of the transpose operation.
      Restrictions:  Handle to a valid matrix object.
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       Displays the result of the matrix transpose operation.
  - Return value:  N/A
Failure
  - N/A
*/
static void displayResultsMatrixOpTrans(MATRIX hMx, MATRIX hMxRes);


/*
FUNCTION
  - Name:     displayRoundMessage
  - Purpose:  Display the menu each round prompting the user to select a matrix operation or quit.
PRECONDITION
  - menuOptionMessages
      Purpose:       Contains the messages to display for the various menu options.
      Restrictions:  N/A
  - menuOptionMessgesSize
      Purpose:       The size of menuOptionsMessages.
      Restrictions:  Equals the actual size of menuOptionsMessages.
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       Displays the menu each round prompting the user to select a matrix operation or quit.
  - Return value:  N/A
Failure
  - N/A
*/
static void displayRoundMessage(const MenuOptionMessages menuOptionMessages[], int menuOptionMessagesSize);


/*
FUNCTION
  - Name:     entriesInitDims
  - Purpose:  Initialize a new array of entries with a given amount of rows and columns.
PRECONDITION
  - rows
      Purpose:       Rows of the new entries.
      Restrictions:  Any positive integer
  - cols
      Purpose:       Columns of the entries.
      Restrictions:  Any positive integer.
POSTCONDITION
Success
  - Reason:        No memory allocation failure.
  - Summary:       Initializes and returns a new array of entries with a given amount of rows and columns.
                   The value of every entry is 0 by default.
  - Return value:  Array of entries as described above.
Failure
  - Reason:        Memory allocation failure.
  - Summary:       Doesn't initialize a new array of entries and nothing of significance happens.
  - Return value:  NULL
*/
static double* entriesInitDims(int rows, int cols);


/*
FUNCTION
  - Name:     entriesConvertRowStrToArr
  - Purpose:  Convert a row of entries as a string into a row of entries as an array of doubles.
PRECONDITION
  - entriesRowStr
      Purpose:       Row of entries as a string to be converted.
      Restrictions:  Valid string representation of a row of entries (all valid doubles).
  - size
      Purpose:       The amount of entries in the string.
	  Restrictions:  Must equal the actual size of entriesRowStr.
  - entriesRowArr
      Purpose:       Store the converted entries. 
      Restrictions:  Not NULL.
POSTCONDITION
Success
  - Reason:         The array of doubles is large enough to hold the amount of entries in the string.
  - Summary:        Converts a row of entries as a string into a row of entries as an array.
  - Return value:   SUCCESS
  - entriesRowArr:  The entries are stored in it. 
Failure
  - Reason:        The array of doubles isn't large enough to hold the amount of entries in the string.
  - Summary:       Doesn't convert the string and nothing of significance happens.
  - Return value:  FAILURE
*/
static Status entriesConvertRowStrToArr(const char* entriesRowStr, int size, double* entriesRowArr);


/*
FUNCTION
  - Name:     inputsAreValidDoubles
  - Purpose:  Checks if a string contains a specific amount of valid doubles.
PRECONDITION
  - input
      Purpose:       String to check if it contains only a specific amount of valid doubles.
      Restrictions:  None.
  - expectedNums
      Purpose:       The expected amount of doubles to be found in the input string.
      Restrictions:  Any positive integer.
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       The correct value is returned accordingly.
  - Return value:  TRUE if input only contains valid doubles and the amount of doubles equals expectedNums.
                   FALSE if otherwise.
Failure
  - N/A
*/
static Boolean inputsAreValidDoubles(const char* input, int expectedNums);


/*
FUNCTION
  - Name:     inputsAreValidInts
  - Purpose:  Checks if a string contains a specific amount of valid integers.
PRECONDITION
  - input
      Purpose:       String to check if it contains only a specific amount of valid integers.
      Restrictions:  None.
  - expectedNums
      Purpose:       The expected amount of integers to be found in the input string.
      Restrictions:  Any positive integer.
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       The correct value is returned accordingly.
  - Return value:  TRUE if input only contains valid integers and the amount of integers equals expectedNums.
                   FALSE if otherwise.
Failure
  - N/A
*/
static Boolean inputsAreValidInts(const char* input, int expectedNums);


/*
FUNCTION
  - Name:     inputsAreValidPositiveInts
  - Purpose:  Checks if a string contains a specific amount of valid positive integers.
PRECONDITION
  - input
      Purpose:       String to check if it contains only a specific amount of valid positive integers.
      Restrictions:  None.
  - expectedNums
      Purpose:       The expected amount of positive integers to be found in the input string.
      Restrictions:  Any positive integer.
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       The correct value is returned accordingly.
  - Return value:  TRUE if input only contains valid positive integers and the amount of positive integers equals expectedNums.
                   FALSE if otherwise.
Failure
  - N/A
*/
static Boolean inputsAreValidPositiveInts(const char* input, int expectedNums);


/*
FUNCTION
  - Name:     userInputGetDims
  - Purpose:  Get the dimensions of a matrix from user input for a matrix operation.
PRECONDITION
  - op
      Purpose:       Indicate which matrix operation is being performed.
      Restrictions:  Not QUIT.
  - pRows
      Purpose:       Store the rows.
      Restrictions:  Not NULL.
  - pCols
      Purpose:       Store the columns.
      Restrictions:  Not NULL.
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       Prompts the user to enter the dimensions.
                   Validates that the input is two valid positive integers, and for power, determinant, and inverse, that the rows equal the columns.
				   Stores the inputs in the row and column.
  - Return value:  N/A
  - pRows          The integer it points to is set to the rows.
  - pCols          The integer it points to is set to the columns.
Failure
  - N/A
*/
static void userInputGetDims(MenuOption op, int* pRows, int* pCols);


/*
FUNCTION
  - Name:     userInputGetEntries
  - Purpose:  Get the entries of a matrix from user input for a matrix operation.
PRECONDITION
  - entries
      Purpose:       Store the entries.
      Restrictions:  Its capacity must equal the actual amount of entries needed for the matrix.
  - rows
      Purpose:       Rows of the matrix.
      Restrictions:  Must equal the actual amount of rows of the matrix.
  - cols
      Purpose:       Columns of the matrix.
      Restrictions:  Must equal the actual amount of columns of the matrix.
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       Prompts the user to enter the entries.
                   Validates that the inputs are the correct amount of valid doubles.
				   Stores the entries in the array.
  - Return value:  N/A
  - entries:       Stores the entries.
Failure
  - N/A
*/
static void userInputGetEntries(double *entries, int rows, int cols);


/*
FUNCTION
  - Name:     userInputGetNumMatrices
  - Purpose:  Get the number of matrices to add or subtract.
PRECONDITION
  - op
      Purpose:       Indicate whether it's for addition or subtraction.
      Restrictions:  ADD or SUB.
  - pNumMxs
      Purpose:       Store the number of matrices to add or subtract.
      Restrictions:  Not NULL.
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       Prompts the user to enter the number of matrices.
                   Validates that the input is an integer >= 2.
				   Stores the input in the integer holding the number of matrices.
  - Return value:  N/A
  - pRows          The integer it points stores the number of matrices.
Failure
  - N/A
*/
static void userInputGetNumMatrices(MenuOption op, int* pNumMxs);


/*
FUNCTION
  - Name:     userInputGetPow
  - Purpose:  Get the power of the matrix for the power operation.
PRECONDITION
  - pPow
      Purpose:       Store the power.
      Restrictions:  Not NULL.
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       Prompts the user to enter the power.
                   Validates that the input is a positive integer.
				   Stores the input in the integer holding the power.
  - Return value:  N/A
  - pRows          The integer it points stores the power.
Failure
  - N/A
*/
static void userInputGetPow(int* pPow);




/***** Functions declared in Menu.h *****/
MenuOption menu_getUserChoice(void) {
	char userChoiceStr[INPUT_BUF_CAP];
	int userChoice;
	Boolean userChoiceIsValid = TRUE;
	MenuOption menuOption = QUIT;


	do {
		displayRoundMessage(menuOptionMessages, menuOptionMessagesSize);
		fgets(userChoiceStr, INPUT_BUF_CAP, stdin);
		userChoiceStr[strlen(userChoiceStr) - 1] = '\0';
		if (!inputsAreValidInts(userChoiceStr, 1) || atoi(userChoiceStr) < 0 || atoi(userChoiceStr) > (menuOptionMessagesSize - 1)) {
			printf("Error - you must enter an integer between 0 and %d.\n\n", menuOptionMessagesSize - 1);
			userChoiceIsValid = FALSE;
		}
		else
			userChoiceIsValid = TRUE;
	} while (!userChoiceIsValid);

	userChoice = atoi(userChoiceStr);

	for (int i = 0; i < menuOptionMessagesSize; ++i) {
		if (userChoice == (int)menuOptionMessages[i].menuOption) {
			menuOption = menuOptionMessages[i].menuOption;
			break;
		}
	}

	return menuOption;
}


Status menu_implementUserChoice(MenuOption userChoice) {
	Status status = SUCCESS;

	switch (userChoice) {
	case MULT:
		if (!menu_matrixOpMult())
			status = FAILURE;
		break;
	case ADD:
		if (!menu_matrixOpAdd())
			status = FAILURE;
		break;
	case SUB:
		if (!menu_matrixOpSub())
			status = FAILURE;
		break;
	case POW:
		if (!menu_matrixOpPow())
			status = FAILURE;
		break;
	case TRANS:
		if (!menu_matrixOpTrans())
			status = FAILURE;
		break;
	case DET:
		if (!menu_matrixOpDet())
			status = FAILURE;
		break;
	case INV:
		if (!menu_matrixOpInv())
			status = FAILURE;
		break;
	default: // QUIT
		break;
	}

	return status;
}


Status menu_matrixOpAdd(void) {
	MATRIX* hMxs;      // array of matrices to add
	MATRIX hMxRes;     // result matrix of the addition
	double* entries;   // entries of matrices being added
	int rows, cols;    // dimensions of the matrices being added
	int numMxs;        // number of matrices being added
	int mxNum;         // indicates which matrix is being used during user input


	// get the number of matrices to add from user input
	displayNumMatricesPrompt(ADD);
	userInputGetNumMatrices(ADD, &numMxs);

	// get the dimensions of each matrix (all are the same)
	displayDimsPrompt(ADD, NULL);
	userInputGetDims(ADD, &rows, &cols);

	// create the matrices being added and the result matrix
	if (!(hMxRes = matrix_initDims(rows, cols)))
		return FAILURE;
	if (!(hMxs = calloc(numMxs, sizeof(*hMxs)))) {
		matrix_destroy(&hMxRes);
		return FAILURE;
	}
	for (int i = 0; i < numMxs; ++i) {
		if (!(hMxs[i] = matrix_initDims(rows, cols))) {
			if (i > 0) {
				for (; i >= 0; --i)
					matrix_destroy(&hMxs[i]);
			}
			free(hMxs);
			matrix_destroy(&hMxRes);
			return FAILURE;
		}
	}

	// get the entries of the matrices from user input and fill up the matrices
	// matrix 1
	if (!(entries = entriesInitDims(rows, cols))) {
		for (int i = 0; i < numMxs; ++i)
			matrix_destroy(&hMxs[i]);
		free(hMxs);
		matrix_destroy(&hMxRes);
		return FAILURE;
	}

	for (int i = 0; i < numMxs; ++i) {
		mxNum = i + 1;
		displayEntriesPrompt(ADD, &mxNum, rows, cols);
		userInputGetEntries(entries, rows, cols);
		if (!matrix_newMatrix(hMxs[i], entries, rows, cols)) {
			for (int i = 0; i < numMxs; ++i)
				matrix_destroy(&hMxs[i]);
			free(hMxs);
			matrix_destroy(&hMxRes);
			free(entries);
			return FAILURE;
		}
	}

	// perform the addition
	if (!matrix_opAdd(hMxs, numMxs, &hMxRes)) {
		for (int i = 0; i < numMxs; ++i)
			matrix_destroy(&hMxs[i]);
		free(hMxs);
		matrix_destroy(&hMxRes);
		free(entries);
		return FAILURE;
	}

	// display results
	displayResultsMatrixOpAdd(hMxs, numMxs, hMxRes);

	// clean up memory
	for (int i = 0; i < numMxs; ++i)
		matrix_destroy(&hMxs[i]);
	free(hMxs);
	matrix_destroy(&hMxRes);
	free(entries);

	return SUCCESS;
}


Status menu_matrixOpInv(void) {
	MATRIX hMx = NULL;         // matrix to be inverted
	MATRIX hMxRes = NULL;      // result matrix - inverted matrix
	double* entries;           // entries of the matrix to be inverted
	int rows, cols;            // dimensions of the matrix to be inverted
	Boolean mxIsInvertible;    // indicate if the matrix is invertible during the inverse operation


	// get the dimensions of the matrix from user input
	displayDimsPrompt(INV, NULL);
	userInputGetDims(INV, &rows, &cols);

	// create the matrix from the dimensions
	if (!(hMx = matrix_initDims(rows, cols)))
		return FAILURE;

	// get the entries of the matrix from user input and fill up the matrix
	if (!(entries = entriesInitDims(rows, cols))) {
		matrix_destroy(&hMx);
		return FAILURE;
	}

	displayEntriesPrompt(INV, NULL, rows, cols);
	userInputGetEntries(entries, rows, cols);
	if (!matrix_newMatrix(hMx, entries, rows, cols)) {
		matrix_destroy(&hMx);
		free(entries);
		return FAILURE;
	}

	// perform the inverse
	if (!matrix_opInv(hMx, &mxIsInvertible, &hMxRes)) {
		// memory allocation failure - status would have returned FAILURE but invertible boolean would be FALSE
		if (!mxIsInvertible) {
			matrix_destroy(&hMx);
			matrix_destroy(&hMxRes);
			free(entries);
			return FAILURE;
		}
	}

	// display results
	displayResultsMatrixOpInv(hMx, mxIsInvertible, hMxRes);

	// clean up memory
	matrix_destroy(&hMx);
	matrix_destroy(&hMxRes);
	free(entries);

	return SUCCESS;
}


Status menu_matrixOpMult(void) {
	MATRIX hMx1;                       // matrix 1 being multiplied
	MATRIX hMx2;                       // matrix 2 being multiplied
	MATRIX hMxRes = NULL;              // result matrix of the multiplication
	double *entries1, *entries2;       // entries of matrices being multiplied
	int rows1, cols1, rows2, cols2;    // dimensions of matrices being multiplied
	int mxNum;                         // indicates which matrix is being used during user input
	Boolean canBeMultiplied;           // indicates if the matrices can be multiplied


	// get the dimensions of the matrices from user input
	do {
		mxNum = 1;
		displayDimsPrompt(MULT, &mxNum);
		userInputGetDims(MULT, &rows1, &cols1);

		mxNum = 2;
		displayDimsPrompt(MULT, &mxNum);
		userInputGetDims(MULT, &rows2, &cols2);

		canBeMultiplied = dimsCanBeMultiplied(cols1, rows2);
		if (!canBeMultiplied) {
			printf("Input error. The columns of matrix 1 must equal the rows of matrix 2 in order for two matrices to be multiplied.\n"
                   "Re-enter the dimensions starting with the first matrix.\n");
		}
	} while (!canBeMultiplied);


	// create the matrices being multiplied
	if (!(hMx1 = matrix_initDims(rows1, cols1)))
		return FAILURE;
	if (!(hMx2 = matrix_initDims(rows2, cols2))) {
		matrix_destroy(&hMx1);
		return FAILURE;
	}

	// get the entries of the matrices from user input and fill up the matrices
	// matrix 1
	if (!(entries1 = entriesInitDims(rows1, cols1))) {
		matrix_destroy(&hMx1);
		matrix_destroy(&hMx2);
		return FAILURE;
	}

	displayEntriesPrompt(MULT, NULL, rows1, cols1);
	userInputGetEntries(entries1, rows1, cols1);
	if (!matrix_newMatrix(hMx1, entries1, rows1, cols1)) {
		matrix_destroy(&hMx1);
		matrix_destroy(&hMx2);
		free(entries1);
		return FAILURE;
	}

	// matrix 2
	if (!(entries2 = entriesInitDims(rows2, cols2))) {
		matrix_destroy(&hMx1);
		matrix_destroy(&hMx2);
		free(entries1);
		return FAILURE;
	}

	displayEntriesPrompt(MULT, NULL, rows2, cols2);
	userInputGetEntries(entries2, rows2, cols2);
	if (!matrix_newMatrix(hMx2, entries2, rows2, cols2)) {
		matrix_destroy(&hMx1);
		matrix_destroy(&hMx2);
		free(entries1);
		free(entries2);
		return FAILURE;
	}

	// perform the multiplication
	if (!matrix_opMult(hMx1, hMx2, &hMxRes)) {
		matrix_destroy(&hMx1);
		matrix_destroy(&hMx2);
		matrix_destroy(&hMxRes);
		free(entries1);
		free(entries2);
		return FAILURE;
	}

	// display results
	displayResultsMatrixOpMult(hMx1, hMx2, hMxRes);

	// clean up memory
	matrix_destroy(&hMx1);
	matrix_destroy(&hMx2);
	matrix_destroy(&hMxRes);
	free(entries1);
	free(entries2);

	return SUCCESS;
}


Status menu_matrixOpPow(void) {
	MATRIX hMx;               // matrix to perform the power operation on
	MATRIX hMxRes = NULL;     // result matrix of the power operation
	double* entries;          // entries of the matrix used in the power operation
	int rows, cols;           // dimensions of the matrix the power operation is being performed on
	int power;                // power used in the power operation

	
	// get the matrix power from user input
	displayPowPrompt();
	userInputGetPow(&power);

	// get the dimensions of the matrix from user input
	displayDimsPrompt(POW, NULL);
	userInputGetDims(POW, &rows, &cols);

	// create the matrix to be used in the power operation
	if (!(hMx = matrix_initDims(rows, cols)))
		return FAILURE;

	// get the entries of the matrices from user input and fill up the matrix
	if (!(entries = entriesInitDims(rows, cols))) {
		matrix_destroy(&hMx);
		return FAILURE;
	}

	displayEntriesPrompt(POW, NULL, rows, cols);
	userInputGetEntries(entries, rows, cols);
	if (!matrix_newMatrix(hMx, entries, rows, cols)) {
		matrix_destroy(&hMx);
		free(entries);
		return FAILURE;
	}

	// perform the power operation
	if (!matrix_opPow(hMx, power, &hMxRes)) {
		matrix_destroy(&hMx);
		return FAILURE;
	}

	// display results
	displayResultsMatrixOpPow(hMx, power, hMxRes);

	// clean up memory
	matrix_destroy(&hMx);
	matrix_destroy(&hMxRes);
	free(entries);

	return SUCCESS;
}


Status menu_matrixOpSub(void) {
	MATRIX* hMxs;      // array of matrices to subtract
	MATRIX hMxRes;     // result matrix of the subtraction
	double* entries;   // entries of matrices being subtracted
	int rows, cols;    // dimensions of the matrices being subtracted
	int numMxs;        // number of matrices being subtracted
	int mxNum;         // indicates which matrix is being used during user input
	

	// get the number of matrices to subtract from user input
	displayNumMatricesPrompt(SUB);
	userInputGetNumMatrices(SUB, &numMxs);

	// get the dimensions of each matrix (all are the same)
	displayDimsPrompt(SUB, NULL);
	userInputGetDims(SUB, &rows, &cols);

	// create the result matrix and the matrix array
	if (!(hMxRes = matrix_initDims(rows, cols)))
		return FAILURE;
	if (!(hMxs = calloc(numMxs, sizeof(*hMxs)))) {
		matrix_destroy(&hMxRes);
		return FAILURE;
	}
	for (int i = 0; i < numMxs; ++i) {
		if (!(hMxs[i] = matrix_initDims(rows, cols))) {
			if (i > 0) {
				for (; i >= 0; --i)
					matrix_destroy(&hMxs[i]);
			}
			free(hMxs);
			matrix_destroy(&hMxRes);
			return FAILURE;
		}
	}

	// get the entries of the matrices from user input and fill up the matrices
	if (!(entries = entriesInitDims(rows, cols))) {
		for (int i = 0; i < numMxs; ++i)
			matrix_destroy(&hMxs[i]);
		free(hMxs);
		matrix_destroy(&hMxRes);
		return FAILURE;
	}

	for (int i = 0; i < numMxs; ++i) {
		mxNum = i + 1;
		displayEntriesPrompt(ADD, &mxNum, rows, cols);
		userInputGetEntries(entries, rows, cols);
		if (!matrix_newMatrix(hMxs[i], entries, rows, cols)) {
			for (int i = 0; i < numMxs; ++i)
				matrix_destroy(&hMxs[i]);
			free(hMxs);
			matrix_destroy(&hMxRes);
			free(entries);
			return FAILURE;
		}
	}

	// perform the subtraction
	if (!matrix_opSub(hMxs, numMxs, &hMxRes)) {
		for (int i = 0; i < numMxs; ++i)
			matrix_destroy(&hMxs[i]);
		free(hMxs);
		matrix_destroy(&hMxRes);
		free(entries);
		return FAILURE;
	}

	// display results
	displayResultsMatrixOpSub(hMxs, numMxs, hMxRes);

	// clean up memory
	for (int i = 0; i < numMxs; ++i)
		matrix_destroy(&hMxs[i]);
	free(hMxs);
	matrix_destroy(&hMxRes);
	free(entries);

	return SUCCESS;
}


Status menu_matrixOpTrans(void) {
	MATRIX hMx = NULL;       // matrix to be transposed
	MATRIX hMxRes = NULL;    // result matrix - tranposed matrix
	double* entries;         // entries of the matrix used in the transpose operation
	int rows, cols;          // dimensions of the matrix the tranpose operation is being performed on


	// get the dimensions of the matrix from user input
	displayDimsPrompt(TRANS, NULL);
	userInputGetDims(TRANS, &rows, &cols);

	// create the matrix to be tranposed
	if (!(hMx = matrix_initDims(rows, cols)))
		return FAILURE;

	// get the entries of the matrix from user input and fill up the matrix
	if (!(entries = entriesInitDims(rows, cols))) {
		matrix_destroy(&hMx);
		return FAILURE;
	}

	displayEntriesPrompt(TRANS, NULL, rows, cols);
	userInputGetEntries(entries, rows, cols);
	if (!matrix_newMatrix(hMx, entries, rows, cols)) {
		matrix_destroy(&hMx);
		free(entries);
		return FAILURE;
	}

	// perform the transpose operation
	if (!matrix_opTrans(hMx, &hMxRes)) {
		matrix_destroy(&hMx);
		free(entries);
		return FAILURE;
	}

	// display results 
	displayResultsMatrixOpTrans(hMx, hMxRes);

	// clean up memory
	matrix_destroy(&hMxRes);
	matrix_destroy(&hMx);
	free(entries);

	return SUCCESS;
}


Status menu_matrixOpDet(void) {
	MATRIX hMx = NULL;    // matrix to calculate the determinant of
	double* entries;      // entries of the matrix used in the determinant operation
	int rows, cols;       // dimensions of the matrix the determinant is calculated on
	double res;           // the result of the determinant operation
	Status mem;           // indicate if memory allocation fails


	// get the dimensions of the matrix from user input
	displayDimsPrompt(DET, NULL);
	userInputGetDims(DET, &rows, &cols);

	// create the matrix to calculate the determinant of
	if (!(hMx = matrix_initDims(rows, cols)))
		return FAILURE;

	// get the entries of the matrix from user input and fill up the matrix
	if (!(entries = entriesInitDims(rows, cols))) {
		matrix_destroy(&hMx);
		return FAILURE;
	}

	displayEntriesPrompt(DET, NULL, rows, cols);
	userInputGetEntries(entries, rows, cols);
	if (!matrix_newMatrix(hMx, entries, rows, cols)) {
		matrix_destroy(&hMx);
		free(entries);
		return FAILURE;
	}


	// calculate the determinant
	res = matrix_opDet(hMx, &mem);
	if (!mem) {
		matrix_destroy(&hMx);
		free(entries);
		return FAILURE;
	}

	// display results
	displayResultsMatrixOpDet(hMx, res);

	// clean up memory
	matrix_destroy(&hMx);
	free(entries);

	return SUCCESS;
}




/***** Helper functions used only in this file *****/
static void createOrdinalNum(char* ordNum, int num) {
	char ordInd[3] = { '\0' };    // ordinal indicator

	if (num >= 4 && num <= 19)    // special case of th - [4, 19] i.e 4th, 5th...19th
		strcpy(ordInd, "th");
	else if ((num - 1) % 10 == 0) // st i.e. 1st, 21st, 31st...
		strcpy(ordInd, "st");
	else if ((num - 2) % 10 == 0) // nd i.e. 2nd, 22nd, 32nd...
		strcpy(ordInd, "nd");
	else if ((num - 3) % 10 == 0) // rd i.e 3rd, 23rd, 33rd...
		strcpy(ordInd, "rd");
	else                          // th i.e. 20th, 2[4, 9]th, 30th, 3[4, 9]th...
		strcpy(ordInd, "th");
	
	sprintf(ordNum, "%d%s", num, ordInd);
}


static Boolean dimsCanBeMultiplied(int cols1, int rows2) {
	return cols1 == rows2;
}


static void displayDimsPrompt(MenuOption op, const int *pMxNum) {
	char opStr[15];      // matrix operation    
	int cols;            // columns to display in the prompt
	char ordNum[100];    // ordinal number of the number of matrices in the prompt


	printf("Enter the desired number rows and columns for the ");
	if (op == POW || op == TRANS || op == DET || op == INV)
		printf("matrix");
	else if (op == ADD || op == SUB)
		printf("matrices");
	else { // MULT
		createOrdinalNum(ordNum, *pMxNum);
		printf("%s matrix", ordNum);
	}
	printf(" separated by a space.\n");

	if (op == POW || op == DET || op == INV) {
		cols = 3;
		if (op == POW)
			strcpy(opStr, "power");
		else if (op == DET)
			strcpy(opStr, "determinant");
		else
			strcpy(opStr, "inverse");
		printf("For the matrix %s operation, the rows must equal the columns.\n", opStr);
	}
	else { // MULT, ADD, SUB, TRANS
		cols = 5;
		if (op == MULT)
			printf("For matrix multiplication, the columns of the first matrix must equal the rows of the second matrix.\n");
	}

	printf("For example, enter 3 %d to create a 3 x %d matrix.\n", cols, cols);
}


static void displayEntriesPrompt(MenuOption op, const int* pMxNum, int rows, int cols) {
	char ordinalNum[100];    // ordinal number of the number of matrices in the prompt

	printf("Enter values for the ");
	if (op == ADD || op == SUB) {
		createOrdinalNum(ordinalNum, *pMxNum);
		printf("%s ", ordinalNum);
	}
	printf("%d x %d matrix with each row separated by a newline.\n", rows, cols);
}


static void displayNumMatricesPrompt(MenuOption op) {
	printf("Enter the number of matrices to ");
	if (op == ADD)
		printf("add. ");
	else // SUB
		printf("subtract. ");
	printf("It must be an integer greater than or equal to 2.\n");
}


static void displayPowPrompt(void) {
	 printf("Enter the matrix power. It must be a single integer greater than or equal to 1.\n");
}


static void displayResultsMatrixOpAdd(MATRIX* hMxs, int hMxsSize, MATRIX hMxRes) {
	printf("\n\nThe %d matrices being added are\n", hMxsSize);
	for (int i = 0; i < hMxsSize; ++i) {
		matrix_print(hMxs[i]);
		printf("\n\n");
	}
	printf("The resulting matrix after addition is\n");
	matrix_print(hMxRes);
	printf("\n\n");
}


static void displayResultsMatrixOpDet(MATRIX hMx, double res) {
	printf("\n\nThe determinant of the following matrix is %g\n", res);
	matrix_print(hMx);
	printf("\n\n");
}


static void displayResultsMatrixOpInv(MATRIX hMx, Boolean mxIsInvertible, MATRIX hMxRes) {
	if (!mxIsInvertible) {
		printf("\n\nThe matrix is\n");
		matrix_print(hMx);
		printf("\n\n");
		printf("Its inverse is\n");
		matrix_print(hMxRes);
		printf("\n\n");
	}
	else {
		printf("\n\nThe determinant of the following matrix is 0. Therefore, it has no inverse.\n");
		matrix_print(hMx);
		printf("\n\n");
	}
}


static void displayResultsMatrixOpMult(MATRIX hMx1, MATRIX hMx2, MATRIX hMxRes) {
	printf("\n\nThe two matrices being multiplied are\n");
	matrix_print(hMx1);
	printf("\n\n");
	matrix_print(hMx2);
	printf("\n\n");
	printf("The resulting matrix after multiplication is\n");
	matrix_print(hMxRes);
	printf("\n\n");
}


static void displayResultsMatrixOpPow(MATRIX hMx, int power, MATRIX hMxRes) {
	printf("\n\nThe original matrix in the power operation is\n");
	matrix_print(hMx);
	printf("\n\n");
	printf("The result of the matrix raised to the power of %d is\n", power);
	matrix_print(hMxRes);
	printf("\n\n");
}


static void displayResultsMatrixOpSub(MATRIX* hMxs, int hMxsSize, MATRIX hMxRes) {
	printf("\n\nThe %d matrices being subtracted are\n", hMxsSize);
	for (int i = 0; i < hMxsSize; ++i) {
		matrix_print(hMxs[i]);
		printf("\n\n");
	}
	printf("The resulting matrix after subtraction is\n");
	matrix_print(hMxRes);
	printf("\n\n");
}


static void displayResultsMatrixOpTrans(MATRIX hMx, MATRIX hMxRes) {
	printf("\n\nThe matrix being transposed is\n");
	matrix_print(hMx);
	printf("\n\n");
	printf("The resulting transposed matrix is\n");
	matrix_print(hMxRes);
	printf("\n\n");
}


static void displayRoundMessage(const MenuOptionMessages menuOptionMessages[], int menuOptionMessagesSize) {
	printf("---------------------------------------------------------------------------------\n");
	printf("Enter the number of the matrix operation to perform or 0 to quit the program.\n");
	for (int i = 0; i < menuOptionMessagesSize; ++i) {
		if (menuOptionMessages[i].menuOption != QUIT)
			printf("%d) %s\n", (int)menuOptionMessages[i].menuOption, menuOptionMessages[i].message);
	}
	printf("%d) %s\n", (int)menuOptionMessages[0].menuOption, menuOptionMessages[0].message);
	printf("Enter choice: ");
}


static double* entriesInitDims(int rows, int cols) {
	double *entries = calloc(rows * cols, sizeof(*entries));
	return entries;
}


static Status entriesConvertRowStrToArr(const char* entriesRowStr,  int size, double* entriesRowArr) {
	char singleNum[100];    // single number from entriesRowStr
	int i = 0;              // index for entriesRowStr
	int j = 0;              // index for singleNum
	int k = 0;              // index for entriesRowArr

	while (entriesRowStr[i] != '\0') {
		if (k >= size)
			return FAILURE;
			
		while (entriesRowStr[i] != ' ' && entriesRowStr[i] != '\0')
			singleNum[j++] = entriesRowStr[i++];
		singleNum[j] = '\0';

		entriesRowArr[k++] = strtod(singleNum, NULL);
		++i;
		j = 0;
	}
	return SUCCESS;
}


static Boolean inputsAreValidDoubles(const char* input, int expectedNums) {
	Boolean negExists = FALSE;    // indicates if a negative sign has already been encountered in the same number
	Boolean decExists = FALSE;    // indicates if a decimal point has already been encountered in the same number
	int totalNums = 0;            // counts the amount of numbers in input to compare to expectedNums
	int i = 0;                    // index of input
	int j;                        // saves value of i to deal with an edge case


	// clear leading whitespace, space is the only valid whitespace character
	while (isspace(input[i])) {
		if (input[i] != ' ')
			return FALSE;
		++i;
	}
	if (input[i] == '\0') // user only pressed "enter" or whitespace followed by "enter" 
		return FALSE;

	// validate that all inputs are valid doubles
	while (input[i] != '\0') {
		// something other than a digit, negative sign, decimal point, or space is invalid
		if (!isdigit(input[i]) && input[i] != ' ' && input[i] != '-' && input[i] != '.')
			return FALSE;

		// repeat decimals points/negative signs are ok if they're in separate numbers
		// space indicates a new number or trailing whitespace, so reset them
		else if (input[i] == ' ') {
			negExists = FALSE;
			decExists = FALSE;
		}

		// negative sign
		else if (input[i] == '-') {
			if (negExists) // multiple negatives in a single number is invalid
				return FALSE;
			else
				negExists = TRUE;

			// negative sign is the first character of input
			if (i == 0) {
				// invalid if the next character isn't a digit or decimal point
				if (!isdigit(input[i + 1]) && input[i + 1] != '.')
					return FALSE;
			}
			// negative sign isn't the first character of input
			else {
				// invalid if the next character isn't a digit or decimal point, or if the previous character isn't a space
				if ((!isdigit(input[i + 1]) && input[i + 1] != '.') || input[i - 1] != ' ')
					return FALSE;
			}
		}

		// decimal point
		else if (input[i] == '.') {
			if (decExists) // multiple decimal points in a single number is invalid.
				return FALSE;
			else
				decExists = TRUE;

			// decimal point is the first character of input
			if (i == 0) {
				// invalid if the next character isn't a digit
				if (!isdigit(input[i + 1]))
					return FALSE;
			}
			// decimal point isn't the first character of input
			else {
				// invalid if the next character isn't a digit, or if the previous character isn't a space, negative sign, or number
				if (!isdigit(input[i + 1]) || (!isdigit(input[i - 1]) && input[i - 1] != ' ' && input[i - 1] != '-'))
					return FALSE;
			}
		}

		++i;
	}


	// all inputs are valid doubles, now verify the correct amount of numbers have been entered
	i = 0;
	while (input[i] != '\0') {
		while (input[i] == ' ')                     // clear whitespace
			++i;
		j = i;
		while (input[i] != ' ' && input[i] != '\0') // get the number
			++i;
		if (i != j)                                 // don't want to increment totalNums if user entered extra whitespace at end
			++totalNums;
	}

	return totalNums == expectedNums;
}


static Boolean inputsAreValidInts(const char* input, int expectedNums) {
	Boolean negExists = FALSE;    // indicates if a negative sign has already been encountered in the same number
	int totalNums = 0;            // counts the amount of numbers in input to compare to expectedNums
	int i = 0;                    // index of input
	int j;                        // saves value of i to deal with an edge case


	// clear leading whitespace, space is the only valid whitespace character
	while (isspace(input[i])) {
		if (input[i] != ' ')
			return FALSE;
		++i;
	}
	if (input[i] == '\0') // user only pressed "enter" or whitespace followed by "enter"
		return FALSE;

	// validate that all inputs are valid integers
	while (input[i] != '\0') {
		// something other than a digit, negative sign, or space is invalid
		if (!isdigit(input[i]) && input[i] != ' ' && input[i] != '-')
			return FALSE;

		// repeat negative signs are ok if they're in separate numbers
		// space indicates a new number or trailing whitespace, so reset it
		else if (input[i] == ' ')
			negExists = FALSE;

		// negative sign
		else if (input[i] == '-') {
			if (negExists) // multiple negatives in a single number is invalid
				return FALSE;
			else
				negExists = TRUE;

			// negative sign is the first character of input
			if (i == 0) {
				// invalid if the next character isn't a digit
				if (!isdigit(input[i + 1]))
					return FALSE;
			}
			// negative sign isn't the first character of input
			else {
				// invalid if the next character isn't a digit, or the previous character isn't a space
				if (!isdigit(input[i + 1]) || input[i - 1] != ' ')
					return FALSE;
			}
		}
		++i;
	}


	// all inputs are valid integers, now verify the correct amount of numbers have been entered
	i = 0;
	while (input[i] != '\0') {
		while (input[i] == ' ')                     // clear whitespace
			++i;
		j = i;
		while (input[i] != ' ' && input[i] != '\0') // get the number
			++i;
		if (i != j)                                 // don't want to incremement totalNums if user entered extra whitespace at end
			++totalNums;
	}

	return totalNums == expectedNums;
}


static Boolean inputsAreValidPositiveInts(const char* input, int expectedNums) {
	char singleNumStr[50];
	int idx = 0;
	int totalNums = 0;
	int i = 0;
	int j;


	// user only pressed "enter" or whitespace followed by "enter"
	// space is the only valid whitespace character
	while (isspace(input[i])) {
		if (input[i] != ' ')
			return FALSE;
		++i;
	}
	if (input[i] == '\0')
		return FALSE;


	// validate that all inputs are valid integers
	while (input[i] != '\0') {
		// something other than a digit or space is invalid
		if (!isdigit(input[i]) && input[i] != ' ')
			return FALSE;
		++i;
	}

	// all inputs are valid integers, now verify the correct amount of numbers have been entered and they're all > 0
	i = 0;
	while (input[i] != '\0') {
		while (input[i] == ' ')                     // clear whitespace
			++i;
		j = i;
		while (input[i] != ' ' && input[i] != '\0') // get the number
			singleNumStr[idx++] = input[i++];
		singleNumStr[idx] = '\0';
		idx = 0;

		if (i != j) {                               // don't want to increment totalNums or check the number if user entered extra whitespace at end
			if (atoi(singleNumStr) == 0)
				return FALSE;
			++totalNums;
		}

	}

	return totalNums == expectedNums;
}


static void userInputGetDims(MenuOption op, int* pRows, int* pCols) {
	char input[INPUT_BUF_CAP];
	Boolean areValidDims;

	do {
		fgets(input, INPUT_BUF_CAP, stdin);
		input[strlen(input) - 1] = '\0';

		// must be two valid integers
		if (!inputsAreValidPositiveInts(input, 2)) { 
			printf("Error - the dimensions entered are not valid. They must be two positive integers. Enter again.\n");
			areValidDims = FALSE;
		} else {
			sscanf(input, "%d%d", pRows, pCols);
			// if power, determinant, or inverse, the rows must equal the columns
			if ((op == POW || op == DET|| op == INV) && *pRows != *pCols) {
				printf("Error - the dimensions entered are not valid. The rows must equal the columns. Enter again.\n");
				areValidDims = FALSE;
			}
			else
				areValidDims = TRUE;
		}
	} while (!areValidDims);
}


static void userInputGetEntries(double* entries, int rows, int cols) {
	char input[INPUT_BUF_CAP];
	Boolean areValidEntries;

	do {
		areValidEntries = TRUE;
		for (int i = 0; i < rows && areValidEntries; ++i) {
			fgets(input, INPUT_BUF_CAP, stdin);
			input[strlen(input) - 1] = '\0';
			if (!inputsAreValidDoubles(input, cols)) {
				if (cols == 1)
					printf("Error - The entry for the row is invalid. It must be 1 number. Enter the entry again starting with the first row.\n");
				else
					printf("Error - The entries for the row are invalid. They must be %d numbers. Enter the entries again starting with the first row.\n", cols);
				areValidEntries = FALSE;
			}
			else
				entriesConvertRowStrToArr(input, cols, &entries[at2(rows, cols, i, 0)]);
		}
	} while (!areValidEntries);
}


static void userInputGetNumMatrices(MenuOption op, int* pNumMxs) {
	char input[INPUT_BUF_CAP];
	Boolean isValidNumMxs;

	do {
		fgets(input, INPUT_BUF_CAP, stdin);
		input[strlen(input) - 1] = '\0';
		if (!inputsAreValidPositiveInts(input, 1) || atoi(input) < 2) {
			printf("Error - the number of matrices to ");
			if (op == ADD)
				printf("add ");
			else // SUB
				printf("subtract ");
			printf("must be an integer greater than or equal to 2. Enter again.\n");
			isValidNumMxs = FALSE;
		} else {
			*pNumMxs = atoi(input);
			isValidNumMxs = TRUE;
		}
	} while (!isValidNumMxs);
}


static void userInputGetPow(int* pPow) {
	char input[INPUT_BUF_CAP];
	Boolean isValidPow;

	do {
		fgets(input, INPUT_BUF_CAP, stdin);
		input[strlen(input) - 1] = '\0';
		if (!inputsAreValidPositiveInts(input, 1)) {
			printf("Error - the power must be a single integer greater than or equal to 1. Enter again.\n");
			isValidPow = FALSE;
		} else {
			*pPow = atoi(input);
			isValidPow = TRUE;
		}
	} while (!isValidPow);
}