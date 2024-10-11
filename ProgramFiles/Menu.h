/*
  Author:       Benjamin G. Friedman
  Date:         05/20/2021
  File:         Menu.h
  Description:  Header file for the matrix operations menu interface.
*/


#ifndef MENU_H
#define MENU_H


#include "Matrix.h"

typedef enum menuOption {
    QUIT,   // quit
    MULT,   // multiplication
    ADD,    // addition
    SUB,    // subtraction
    POW,    // power
    TRANS,  // transpose
    DET,    // determinant
    INV     // inverse
} MenuOption;




/***** Functions defined in Menu.c *****/
/*
NOTES
  - Name:     menu_getUserChoice
  - Purpose:  Get the user's choice from the menu (matrix operation to perform or quit).
PRECONDITION
  - A new round has started in the program.
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       Prompts the user to enter one of the numerical menu options, validates the input, and returns the user's choice.
  - Return value:  User's choice.
Failure
  - N/A
*/
MenuOption menu_getUserChoice(void);


/*
NOTES
  - Name:     menu_implementUserChoice
  - Purpose:  Implements the user's choice from the menu (matrix operation to perform or quit).
PRECONDITION
  - userChoice
      Purpose:       Stores the user's choice for which matrix operation to implement or quit the program.
      Restrictions:  N/A
POSTCONDITION
Success
  - Reason:        No memory allocation failure.
  - Summary:       Implement the user's choice.
  - Return value:  SUCCESS
Failure
  - Reason:        Memory allocation falure.
  - Summary:       Doesn't implement the user's choice.
                   Much or little could've happened up until the point of failure because it can happen on numerous occasions.
  - Return value:  FAILURE
*/
Status menu_implementUserChoice(MenuOption userChoice);


/*
NOTES
  - Name:     menu_matrixOpAdd
  - Purpose:  Implements the matrix addition operation.
              A + B + C... for matrices A, B, C... whose dimensions are all the same.
PRECONDITION
  - User has selected to perform the matrix addition operation.
POSTCONDITION
Success
  - Reason:        No memory allocation failure.
  - Summary:       Implements the matrix addition operation.
                     - Prompts the user to enter the number of matrices to add, their dimensions, and entries.
                     - Performs the matrix addition operation.
                     - Displays the results.
  - Return value:  SUCCESS
Failure
  - Reason:        Memory allocation failure.
  - Summary:       Doesn't implement the matrix addition operation.
                   Much or little could've happened up until the point of failure because it can happen on numerous occasions.
  - Return value:  FAILURE
*/
Status menu_matrixOpAdd(void);


/*
NOTES
  - Name:     menu_matrixOpDet
  - Purpose:  Implements the matrix determinant operation.
              det(A) for matrix A.
PRECONDITION
  - User has selected to perform the matrix determinant operation.
POSTCONDITION
Success
  - Reason:        No memory allocation failure.
  - Summary:       Implements the matrix determinant operation.
                     - Prompts the user to enter the dimensions and entries of a matrix.
                     - Performs the matrix determinant operation.
                     - Displays the results.
  - Return value:  SUCCESS
Failure
  - Reason:        Memory allocation failure.
  - Summary:       Doesn't implement the matrix determinant operation.
                   Much or little could've happened up until the point of failure because it can happen on numerous occasions.
  - Return value:  FAILURE
*/
Status menu_matrixOpDet(void);


/*
NOTES
  - Name:     menu_matrixOpInv
  - Purpose:  Implements the matrix inverse operation.
              A^-1 for matrix A.
PRECONDITION
  - User has selected to perform the matrix inverse operation.
POSTCONDITION
Success
  - Reason:        No memory allocation failure.
  - Summary:       Implements the matrix inverse operation.
                     - Prompts the user to enter the dimensions and entries of a matrix.
                     - Performs the matrix inverse operation.
                     - Displays the results.
  - Return value:  SUCCESS
Failure
  - Reason:        Memory allocation failure.
  - Summary:       Doesn't implement the matrix inverse operation.
                   Much or little could've happened up until the point of failure because it can happen on numerous occasions.
  - Return value:  FAILURE
*/
Status menu_matrixOpInv(void);


/*
NOTES
  - Name:     menu_matrixOpMult
  - Purpose:  Implements the matrix multiplication operation.
              A x B for two matrices A and B where the columns of A equal the rows of B.
PRECONDITION
  - User has selected to perform the matrix multiplication operation.
POSTCONDITION
Success
  - Reason:        No memory allocation failure.
  - Summary:       Implements the matrix multiplication operation.
                     - Prompts the user to enter the dimensions and entries of the two matrices being multiplied.
                     - Performs the matrix multiplication operation.
                     - Displays the results.
  - Return value:  SUCCESS
Failure
  - Reason:        Memory allocation failure.
  - Summary:       Doesn't implement the matrix multiplication operation.
                   Much or little could've happened up until the point of failure because it can happen on numerous occasions.
  - Return value:  FAILURE
*/
Status menu_matrixOpMult(void);


/*
NOTES
  - Name:     menu_matrixOpPow
  - Purpose:  Implements the matrix power operation.
              A^P for matrix A with power P.
PRECONDITION
  - User has selected to perform the matrix power operation.
POSTCONDITION
Success
  - Reason:        No memory allocation failure.
  - Summary:       Implements the matrix power operation.
                     - Prompts the user to enter the dimensions and entries of a matrix.
                     - Performs the matrix power operation.
                     - Displays the results.
  - Return value:  SUCCESS
Failure
  - Reason:        Memory allocation failure.
  - Summary:       Doesn't implement the matrix power operation.
                   Much or little could've happened up until the point of failure because it can happen on numerous occasions.
  - Return value:  FAILURE
*/
Status menu_matrixOpPow(void);


/*
NOTES
  - Name:     menu_matrixOpSub
  - Purpose:  Implements the matrix subtraction operation.
              A - B - C... for matrices A, B, C... whose dimensions are all the same.
PRECONDITION
  - User has selected to perform the matrix subtraction operation.
POSTCONDITION
Success
  - Reason:        No memory allocation failure.
  - Summary:       Implements the matrix subtraction operation.
                     - Prompts the user to enter the number of matrices to subtract, their dimensions, and entries.
                     - Performs the matrix subtraction operation.
                     - Displays the results.
  - Return value:  SUCCESS
Failure
  - Reason:        Memory allocation failure.
  - Summary:       Doesn't implement the matrix subtraction operation.
                   Much or little could've happened up until the point of failure because it can happen on numerous occasions.
  - Return value:  FAILURE
*/
Status menu_matrixOpSub(void);


/*
NOTES
  - Name:     menu_matrixOpTrans
  - Purpose:  Implements the matrix transpose operation.
              A^T for matrix A.
PRECONDITION
  - User has selected to perform the matrix tranpose operation.
POSTCONDITION
Success
  - Reason:        No memory allocation failure.
  - Summary:       Implements the matrix transpose operation.
                     - Prompts the user to enter the dimensions and entries of a matrix.
                     - Performs the matrix transpose operation.
                     - Displays the results.
  - Return value:  SUCCESS
Failure
  - Reason:        Memory allocation failure.
  - Summary:       Doesn't implement the matrix transpose operation.
                   Much or little could've happened up until the point of failure because it can happen on numerous occasions.
  - Return value:  FAILURE
*/
Status menu_matrixOpTrans(void);


#endif