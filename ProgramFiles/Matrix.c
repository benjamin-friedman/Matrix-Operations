/*
  Author:       Benjamin G. Friedman
  Date:         05/20/2021
  File:         Matrix.c
  Description:  Implementation file for the matrix opaque object interface.
*/


#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Matrix.h"

typedef struct matrix {
	double* entries;    // 1D array implementation fo 2D matrix
	int rows;           // total rows
	int cols;           // total columns
	int maxLength;      // max width of a number out of the entire array i.e -425.73 has a width of 7 (5 numbers, '.', and '-')
} Matrix;




/*********** Declarations for helper functions defined in this file **********/
/*
FUNCTION
  - Name:     adjustMatrixDims
  - Purpose:  Adjust the dimensions of a matrix with a given amount of rows and columns.
PRECONDITION
  - ppMX
      Purpose:       Matrix to be adjusted.
      Restrictions:  Pointer to a pointer to a valid matrix object, or pointer to a NULL pointer.
  - rows
      Purpose:       New rows.
      Restrictions:  Any positive integer.
  - cols
      Purpose:       New columns.
      Restrictions:  Any positive integer.
POSTCONDITION
Success
  - Reason:        No memory allocation failure.
  - Summary:       Adjusts the dimensions of a matrix with a given amount of rows and columns.
                   If the matrix doesn't exist it is created first.
				   If the matrix exists, its dimensions are adjusted.
				   The values of all the entries are set to 0.
  - Return value:  SUCCESS
  - ppMx:          If it is a pointer to a pointer to a valid matrix object, the matrix's dimensions are adjusted.
                   If is is pointer to a NULL pointer, a new matrix is created with those dimensions and the pointer it points to stores the address of the new matrix.
Failure
  - Reason:        Memory allocation failure.
  - Summary:       Doesn't adjust the dimensions of a matrix and nothing of signifiance happens.
                   If the matrix doesn't exist it is created first.
				   If the matrix exists, its dimensions are adjusted.
				   The values of all the entries are set to 0.
  - Return value:  SUCCESS
  - ppMx:          If it was a pointer to a pointer to a valid matrix object, the state of the matrix before the function call is preserved.
                   If is was pointer to a NULL pointer, the pointer remains NULL.
*/
static Status adjustMatrixDims(Matrix** ppMx, int rows, int cols);


/*
FUNCTION
  - Name:     at
  - Purpose:  Get the actual index in the 1D array using the row-column coordinates of the 2D matrix.
              For example:
			  2D matrix      2D array       1D array      
			  [R1C1] [R1C2]  [0][0] [0][1]  [0]   [1]   [2]   [3]
			  [R2C1] [R2C2]  [1][0] [1][1]  [R1C1][R1C2][R2C1][R2C2]
PRECONDITION
  - pMX
      Purpose:       Matrix to get the index of.
      Restrictions:  Pointer to a valid matrix object.
  - row
      Purpose:       Index of the row of the entry in the 2D matrix (i.e. row 1 = index 0).
      Restrictions:  Any integer >= 0.
  - col
      Purpose:       Index of the column of the entry in the 2D matrix (i.e. column 1 = index 0).
      Restrictions:  Any integer >= 0.
POSTCONDITION
Success
  - Reason:        Row-column coordinate is in bounds.
  - Summary:       Returns the index in the 1D array of the row-column coordinate in the 2D matrix.
  - Return value:  The index in the 1D array of the row-column coordinate in the 2D matrix.
Failure
  - Reason:        Row-column coordinate is out of bounds
  - Summary:       Returns a special value to indicate out of bounds.
  - Return value:  -1
*/
static int at(Matrix* pMx, int row, int col);


/*
FUNCTION
  - Name:     at2
  - Purpose:  Get the actual index in the 1D array using the row-column coordinates of the 2D matrix.
              This function is a modified version of the at function for when the entries are stored in an array by itself and not within a matrix object
              For example:
			  2D matrix      2D array       1D array      
			  [R1C1] [R1C2]  [0][0] [0][1]  [R1C1][R1C2][R2C1][R2C2]
			  [R2C1] [R2C2]  [1][0] [1][1]
PRECONDITION
  - rows
      Purpose:       Rows of the entries array (2D matrix not within a matrix object).
      Restrictions:  Any positive integer.
  - cols
      Purpose:       Columns of the entries array (2D matrix not within a matrix object).
      Restrictions:  Any positive integer.
  - row
      Purpose:       Index of the row of the entry in the 2D matrix (i.e. row 1 = index 0).
      Restrictions:  Any integer >= 0.
  - col
      Purpose:       Index of the column of the entry in the 2D matrix (i.e. column 1 = index 0).
      Restrictions:  Any integer >= 0.
POSTCONDITION
Success
  - Reason:        Row-column coordinate is in bounds.
  - Summary:       Returns the index in the 1D array of the row-column coordinate in the 2D matrix.
  - Return value:  The index in the 1D array of the row-column coordinate in the 2D matrix.
Failure
  - Reason:        Row-column coordinate is out of bounds
  - Summary:       Returns a special value to indicate out of bounds.
  - Return value:  -1
*/
int at2(int rows, int cols, int row, int col);


/*
FUNCTION
  - Name:     calcEntryLength
  - Purpose:  Calculate the length of an entry not including trailing 0s.
                - If the entry is equivalent to an integer, it will be the length of the integer part (i.e. 1024.0 = length 4).
				- If otherwise,it will not include trailing 0s (i.e. 1.5000 = length 3).
PRECONDITION
  - entry
      Purpose:       Entry to calculate the length of.
      Restrictions:  N/A
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       Calculates and returns the length of an entry.
  - Return value:  Returns the number as described above.
Failure
  - N/A
*/
static int calcEntryLength(double entry);


/*
FUNCTION
  - Name:     getSize
  - Purpose:  Calculate the size of the 1D array of entries using the number of rows and columns in the 2D matrix.
PRECONDITION
  - rows
      Purpose:       Rows of the matrix.
      Restrictions:  Any positive integer.
  - cols
      Purpose:       Columns of the matrix
      Restrictions:  Any positive integer.
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       Calculates and returns the size of the 1D array of entries using the number of rows and columns in the 2D matrix.
  - Return value:  Returns the number as described above.
Failure
  - N/A
*/
static int getSize(int rows, int cols);


/*
FUNCTION
  - Name:     opAdjugate
  - Purpose:  Perform the matrix adjugate operation.
              The adjugate of a matrix is the matrix that is the transpose of the matrix of its cofactors.
			  Used in the matrix inverse operation.
PRECONDITION
  - pMx
      Purpose:       Matrix to calculate the adjugate of.
      Restrictions:  Pointer to a valid matrix object.
                     The rows equal the columns.
  - ppMxRes
      Purpose:       Store the result matrix that is the adjugate.
      Restrictions:  Pointer to a pointer to a valid matrix object or pointer to a NULL pointer.
POSTCONDITION
Success
  - Reason:        No memory allocation failure.
  - Summary:       Performs the matrix adjugate operation and stores the result that is the adjugate in the result matrix.
  - Return value:  SUCCESS.
  - pMx:           The state of the matrix before the function call is preserved.
  - ppMx:          The pointer it points to stores the result matrix that is the adjugate.
                   If it was a pointer to a handle to a valid matrix object before the function call, the matrix's dimensions are adjusted if necessary.
                   If it was a pointer to a NULL handle before the function call, a new matrix gets created to store the result.
Failure
  - Reason:        Memory allocation failure.
  - Summary:       The adjugate operation is not performed and nothing of significance happens.
  - Return value:  FAILURE.
  - pMx:           The state of the matrix before the function call is preserved.
  - ppMxRes:       If it was a pointer to a handle to a valid matrix object before the function call, the state of the matrix before the function call is not guaranteed to be preserved.
                   If it was a pointer to a NULL handle before the function call, the handle is not guaranteed to still be NULL.
*/
static Status opAdjugate(Matrix* pMx, Matrix** ppMxRes);


/*
FUNCTION
  - Name:     opDet
  - Purpose:  Perform the matrix determinant operation.
              Recursive helper function used in matrix_opDet.
PRECONDITION
  - pMx
      Purpose:       Matrix to calculate the determinant of.
      Restrictions:  Pointer to a valid matrix object.
                     The rows equal the columns.
  - pMem
      Purpose:       Indicate if memory allocation fails.
      Restrictions:  Not NULL.
POSTCONDITION
Success
  - Reason:        No memory allocation failure.
  - Summary:       Performs the matrix determinant operation and returns the result.
  - Return value:  Result of the determinant operation.
  - hMx:           The state of the matrix before the function call is preserved.
  - pMem:          The Boolean it points to is set to SUCCESS.
Failure
  - Reason:        Memory allocation failure.
  - Summary:       The determinant operation is not performed and nothing of significance happens.
  - Return value:  0
  - hMx:           The state of the matrix before the function call is preserved.
  - pMem:          The Boolean it points to is set to FAILURE.
*/
static double opDet(Matrix* pMx, Status* pMem);


/*
FUNCTION
  - Name:     opDet2x2
  - Purpose:  Perform the matrix determinant operation for a 2 x 2 matrix.
              Recursive helper function to deal with the base case used in opDet.
PRECONDITION
  - a11
      Purpose:       Entry in row 1 column 1.
      Restrictions:  N/A
  - a12
      Purpose:       Entry in row 1 column 2.
      Restrictions:  N/A
  - a21
      Purpose:       Entry in row 2 column 1.
      Restrictions:  N/A
  - a22
      Purpose:       Entry in row 2 column 2.
      Restrictions:  N/A
POSTCONDITION
Success
  - Reason:        All cases
  - Summary:       Performs the matrix determinant operation for a 2 x 2 matrix and returns the result.
  - Return value:  Result of the 2 x 2 determinant operation.
Failure
  - N/A
*/
static double opDet2x2(double a11, double a12, double a21, double a22);


/*
FUNCTION
  - Name:     removeTrailingZeroes
  - Purpose:  Remove trailing zeroes from a string representation of an entry which is a floating point number.
PRECONDITION
  - entryStr
      Purpose:       String representation of the entry to remove the trailing zeroes from.
      Restrictions:  Any valid string.
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       Removes trailing zeroes from the string representation of an entry.
  - Return value:  N/A
Failure
  - N/A
*/
static void removeTrailingZeroes(char* entryStr);




/********** Definitions for matrix interface functions declared in Matrix.h **********/
Boolean matrix_canBeAdd(MATRIX hMx1, MATRIX hMx2) {
	Matrix* pMx1 = hMx1;
	Matrix* pMx2 = hMx2;
	return (pMx1->rows == pMx2->rows && pMx1->cols == pMx2->cols);
}


Boolean matrix_canBeInv(MATRIX hMx, Status* pMem) {
	double det = matrix_opDet(hMx, pMem);
	return (*pMem) ? det != 0 : FALSE;
}


Boolean matrix_canBeMult(MATRIX hMx1, MATRIX hMx2) {
	Matrix* pMx1 = hMx1;
	Matrix* pMx2 = hMx2;
	return pMx1->cols == pMx2->rows;
}


Boolean matrix_canBeSub(MATRIX hMx1, MATRIX hMx2) {
	Matrix* pMx1 = hMx1;
	Matrix* pMx2 = hMx2;
	return (pMx1->rows == pMx2->rows && pMx1->cols == pMx2->cols);
}


Status matrix_copy(MATRIX* phMxDest, MATRIX hMxSrc) {
	Matrix* pMxSrc = hMxSrc;
	Matrix* pMxDest = *phMxDest;
	double* entries;

	// destination matrix doesn't exist, create new matrix
	if (!(*phMxDest)) {
		if (!(*phMxDest = matrix_initDims(pMxSrc->rows, pMxSrc->cols)))
			return FAILURE;
		pMxDest = *phMxDest;
	}
	// destination matrix exists
	else {
		// resize if necessary
		if (getSize(pMxDest->rows, pMxDest->cols) < getSize(pMxSrc->rows, pMxSrc->cols)) {
			if (!(entries = calloc(getSize(pMxSrc->rows, pMxSrc->cols), sizeof(*entries))))
				return FAILURE;
			free(pMxDest->entries);
			pMxDest->entries = entries;
		}
		pMxDest->rows = pMxSrc->rows;
		pMxDest->cols = pMxSrc->cols;
	}
	
	// in either case, set the max length and copy the entries
	pMxDest->maxLength = pMxSrc->maxLength;
	int size = getSize(pMxSrc->rows, pMxSrc->cols);
	for (int i = 0; i < size; ++i)
		pMxDest->entries[i] = pMxSrc->entries[i];
	
	return SUCCESS;
}


Status matrix_destroy(MATRIX* phMx) {
	Matrix* pMx = *phMx;
	if (pMx) {
		free(pMx->entries);
		free(pMx);
		*phMx = NULL;
		return SUCCESS;
	}
	return FAILURE;
}


Status matrix_getEntry(MATRIX hMx, int row, int col, double* pEntry) {
	Matrix* pMx = hMx;
	int idx;

	idx = at(hMx, row, col);
	if (idx != -1) {
		*pEntry = pMx->entries[idx];
		return SUCCESS;
	}
	else {
		*pEntry = 0;
		return FAILURE;
	}
}


MATRIX matrix_initCopy(MATRIX hMxSrc) {
	Matrix* pMxSrc = hMxSrc;

	Matrix* pMx = malloc(sizeof(*pMx));
	if (pMx) {
		int srcSize = getSize(pMxSrc->rows, pMxSrc->cols);
		if (!(pMx->entries = calloc(srcSize, sizeof(*(pMx->entries))))) {
			free(pMx);
			return NULL;
		}
		pMx->rows = pMxSrc->rows;
		pMx->cols = pMxSrc->cols;
		pMx->maxLength = pMxSrc->maxLength;
		for (int i = 0; i < srcSize; ++i)
			pMx->entries[i] = pMxSrc->entries[i];
	}

	return pMx;
}


MATRIX matrix_initDims(int rows, int cols) {
	Matrix* pMx = malloc(sizeof(*pMx));
	if (pMx) {
		if (!(pMx->entries = calloc(getSize(rows, cols), sizeof(*(pMx->entries))))) {
			free(pMx);
			return NULL;
		}
		pMx->rows = rows;
		pMx->cols = cols;
		pMx->maxLength = 1;
	}

	return pMx;
}


MATRIX matrix_initMove(MATRIX* phMxSrc) {
	Matrix* pMx = *phMxSrc;
	if (pMx)
		*phMxSrc = NULL;
	return pMx;
}


Status matrix_move(MATRIX* phMxDest, MATRIX* phMxSrc) {
	// source matrix doesn't exist
	if (!(*phMxSrc))
		return FAILURE;

	// source matrix exists
	// destroy matrix in the destination handle if it exists in preparation for the move
	matrix_destroy(phMxDest);

	// move the source matrix into the destination handle
	*phMxDest = *phMxSrc;

	// remove ownership of the matrix from the source handle
	*phMxSrc = NULL;

	return SUCCESS;
}


Status matrix_newMatrix(MATRIX hMx, const double* entries, int rows, int cols) {
	Matrix* pMx = hMx;
	int maxLength = 1;     // max length of the numbers found
	int numLength;         // length of a single number
	double* entriesResize;

	// resize if necessary
	if (getSize(pMx->rows, pMx->cols) < getSize(rows, cols)) {
		if (!(entriesResize = malloc(sizeof(*entries) * getSize(rows, cols))))
			return FAILURE;
		free(pMx->entries);
		pMx->entries = entriesResize;
	}

	// copy the entries
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			pMx->entries[at(pMx, i, j)] = entries[at2(rows, cols, i, j)];
			numLength = calcEntryLength(entries[at2(rows, cols, i, j)]);
			if (i == 0 && j == 0)
				maxLength = numLength;
			else if (numLength > maxLength)
				maxLength = numLength;
		}
	}

	// sets rows, columns, and max length
	pMx->rows = rows;
	pMx->cols = cols;
	pMx->maxLength = maxLength;

	return SUCCESS;
}


Status matrix_opAdd(MATRIX* hMxs, int hMxsSize, MATRIX* phMxRes) {
	Matrix* pMxToAdd = hMxs[0];        // first matrix being added
	Matrix* pMxRes;                    // result matrix, not initialized b/c phMxRes isn't guaranteed to have a matrix
	double sum = 0;                    // sum for each new individual term
	int maxLength = 1;                 // max length of the new matrix (same as in the matrix structure)
	int numLength;                     // gets the length of each number to be compared to max length
	Boolean isFirstNewEntry = TRUE;    // indicates if the first number being calculated in the whole result matrix
	

	// recreate the result matrix if its dimensions aren't appropriate for the addition or it's NULL
	if (!adjustMatrixDims((Matrix**)phMxRes, pMxToAdd->rows, pMxToAdd->cols))
		return FAILURE;
	pMxRes = *phMxRes;


	// perform the addition
	// outer 2 loops for result matrix
	for (int resRow = 0; resRow < pMxToAdd->rows; ++resRow) {
		for (int resCol = 0; resCol < pMxToAdd->cols; ++resCol) {
			// inner loop to get the sum
			sum = 0;
			for (int i = 0; i < hMxsSize; ++i) {
				pMxToAdd = hMxs[i];
				sum += pMxToAdd->entries[at(pMxToAdd, resRow, resCol)];
			}
			numLength = calcEntryLength(sum);
			if (isFirstNewEntry) {
				maxLength = numLength;
				isFirstNewEntry = FALSE;
			}
			else if (numLength > maxLength)
				maxLength = numLength;
			pMxRes->entries[at(pMxRes, resRow, resCol)] = sum;
		}
	}
	pMxRes->maxLength = maxLength;

	return SUCCESS;
}


double matrix_opDet(MATRIX hMx, Status* pMem) {
	Matrix* pMx = hMx;
	*pMem = SUCCESS;  // assume memory allocation failure won't happen

	// the determinant of a 1 x 1 matrix is just the single number in the matrix
	if (pMx->rows == 1 && pMx->cols == 1)
		return pMx->entries[0];

	// all other matrices - 2 x 2, 3 x 3 etc.
	return opDet(pMx, pMem);
}


Status matrix_opInv(MATRIX hMx, Boolean* pMxIsInvertible, MATRIX* phMxRes) {
	Matrix* pMx = hMx;
	Matrix* pMxRes;       // result matrix, not initialized b/c phMxRes isn't guaranteed to have a matrix
	Status mem;           // indicates if there's memory allocation failure
	double det;           // determinant
	double newEntry;      // each new term in result matrix after multiply by 1 / determinant
	int maxLength = 1;    // max length of the new matrix (same as in the matrix structure)
	int numLength;        // gets the length of each number to be compared to max length
	

	*pMxIsInvertible = FALSE;    // assume the matrix isn't invertible

	// recreate the result matrix if its dimensions aren't appropriate for the inverse or it's NULL
	if (!adjustMatrixDims((Matrix**)phMxRes, pMx->rows, pMx->cols))
		return FAILURE;
	pMxRes = *phMxRes;

	// implement the inverse operation
	// 1: calculate the determinant - if it's 0 the matrix is invertible and the inverse can't be calculated
	det = matrix_opDet(hMx, &mem);
	if (!mem)
		return FAILURE;
	else if (det == 0) {
		*pMxIsInvertible = TRUE;
		return FAILURE;
	}

	// 2: calculate the adjugate matrix
	if (!opAdjugate(pMx, &pMxRes))
		return FAILURE;

	// 3: multiply every term in the adjugate matrix by 1 / determinant - the result of this is the inverse
	int size = getSize(pMxRes->rows, pMxRes->cols);
	for (int i = 0; i < size; ++i) {
		newEntry = pMxRes->entries[i] / det;
		numLength = calcEntryLength(newEntry);
		if (i == 0)
			maxLength = numLength;
		else if (numLength > maxLength)
			maxLength = numLength;
		pMxRes->entries[i] = newEntry;
	}
	pMxRes->maxLength = maxLength;

	return SUCCESS;
}


Status matrix_opMult(MATRIX hMx1, MATRIX hMx2, MATRIX* phMxRes) {
	Matrix* pMx1 = hMx1;               // matrix 1 being multiplied
	Matrix* pMx2 = hMx2;               // matrix 2 being multiplied
	Matrix* pMxRes;                    // result matrix, not initialized b/c phMxRes isn't guaranteed to have a matrix
	double sum = 0;                    // the sum for each new entry in the result matrix
	int resRow = 0;                    // index tracker of the result matrix's row
	int resCol = 0;                    // index tracker of the result matrix's column
	int maxLength = 1;                 // max length of the new matrix (same as in the matrix structure)
	int numLength;                     // gets the length of each number to be compared to max length
	Boolean isFirstNewEntry = TRUE;    // indicates if the first number being calculated in the whole result matrix


	// recreate the result matrix if its dimensions aren't appropriate for the multiplication or it's NULL
	if (!adjustMatrixDims((Matrix**)phMxRes, pMx1->rows, pMx2->cols))
		return FAILURE;
	pMxRes = *phMxRes;


	// perform the multiplication
	for (int m1row = 0; m1row < pMx1->rows; ++m1row) {
		resCol = 0;
		for (int m2col = 0; m2col < pMx2->cols; ++m2col) {
			sum = 0;
			for (int m1col = 0, m2row = 0; m1col < pMx1->cols; ++m1col, ++m2row)
				sum += pMx1->entries[at(pMx1, m1row, m1col)] * pMx2->entries[at(pMx2, m2row, m2col)];
			numLength = calcEntryLength(sum);
			if (isFirstNewEntry) {
				maxLength = numLength;
				isFirstNewEntry = FALSE;
			}
			else if (numLength > maxLength)
				maxLength = numLength;
			pMxRes->entries[at(pMxRes, resRow, resCol)] = sum;
			++resCol;
		}
		++resRow;
	}
	pMxRes->maxLength = maxLength;

	return SUCCESS;
}


Status matrix_opPow(MATRIX hMx, int power, MATRIX* phMxRes) {
	MATRIX hMxToMult = NULL;        // handles used for power operation
	MATRIX hMxResTemp = NULL;

	// special cases: power = 1 or 2
	if (power == 1) {
		if (!matrix_copy(phMxRes, hMx))
			return FAILURE;
		return SUCCESS;
	}
	else if (power == 2) {
		if (!matrix_opMult(hMx, hMx, phMxRes))
			return FAILURE;
		return SUCCESS;
	}

	// all other cases
	// do the first multiplication in the power operation
	if (!matrix_opMult(hMx, hMx, &hMxToMult))
		return FAILURE;
	// do all other multiplications thereafter
	for (int i = 3; i <= power; ++i) {
		if (!matrix_opMult(hMx, hMxToMult, &hMxResTemp)) {
			matrix_destroy(&hMxToMult);
			return FAILURE;
		}
		matrix_destroy(&hMxToMult);
		hMxToMult = hMxResTemp;
		hMxResTemp = NULL;
	}

	// store the result of the power operation
	matrix_move(phMxRes, &hMxToMult);
	return SUCCESS;
}


Status matrix_opSub(MATRIX* hMxs, int hMxsSize, MATRIX* phMxRes) {
	Matrix* pMxToSub = hMxs[0];        // first matrix, the matrix being subtracted from
	Matrix* pMxRes;                    // result matrix, not initialized b/c phMxRes isn't guaranteed to have a matrix
	double sum = 0;                    // sum for each new individual term
	int maxLength = 1;                 // max length of the new matrix (same as in the matrix structure)
	int numLength;                     // gets the length of each number to be compared to max length
	Boolean isFirstNewEntry = TRUE;    // indicates if the first number being calculated in the whole result matrix
	

	// recreate the result matrix if its dimensions aren't appropriate for the subtraction or it's NULL
	if (!adjustMatrixDims((Matrix**)phMxRes, pMxToSub->rows, pMxToSub->cols))
		return FAILURE;
	pMxRes = *phMxRes;  // result of subtraction
	

	// perform the subtraction
	// outer 2 loops for result matrix
	for (int resRow = 0; resRow < pMxToSub->rows; ++resRow) {
		for (int resCol = 0; resCol < pMxToSub->cols; ++resCol) {
			// inner loop to get the sum
			sum = 0;
			for (int i = 0; i < hMxsSize; ++i) {
				pMxToSub = hMxs[i];
				if (i == 0)
					sum += pMxToSub->entries[at(pMxToSub, resRow, resCol)];
				else
					sum -= pMxToSub->entries[at(pMxToSub, resRow, resCol)];
			}
			numLength = calcEntryLength(sum);
			if (isFirstNewEntry) {
				maxLength = numLength;
				isFirstNewEntry = FALSE;
			}
			else if (numLength > maxLength)
				maxLength = numLength;
			pMxRes->entries[at(pMxRes, resRow, resCol)] = sum;
		}
	}
	pMxRes->maxLength = maxLength;

	return SUCCESS;
}


Status matrix_opTrans(MATRIX hMx, MATRIX* phMxRes) {
	Matrix* pMx = hMx;
	Matrix* pMxRes;    // result matrix, not initialized b/c phMxRes isn't guaranteed to have a matrix

	// recreate the result matrix if its dimensions aren't appropriate for the transpose or it's NULL
	if (!adjustMatrixDims((Matrix**)phMxRes, pMx->cols, pMx->rows))
		return FAILURE;
	pMxRes = *phMxRes;

	// calculate the transpose
	for (int i = 0; i < pMx->rows; ++i) {
		for (int j = 0; j < pMx->cols; ++j)
			pMxRes->entries[at(pMxRes, j, i)] = pMx->entries[at(pMx, i, j)];
	}
	pMxRes->maxLength = pMx->maxLength;

	return SUCCESS;
}


void matrix_print(MATRIX hMx) {
	Matrix* pMx = hMx;
	char entryStr[500];    // buffer to hold each entry as a string
	double entry;          // one entry in the matrix
	int extraSpaces;       // count how many extra spaces to print for each entry
	int spacesPerNum;      // each entry occupies the same fixed space                          
	int totalSpaces;       // the total spaces horizontally the matrix takes up so it's known how many dashes to print
	

	spacesPerNum = pMx->maxLength + 2;
	totalSpaces = spacesPerNum * pMx->cols + pMx->cols + 1;
	
	// print the matrix
	// print top level of dashes
	for (int i = 0; i < totalSpaces; ++i)
		printf("-");
	printf("\n");

	for (int i = 0; i < pMx->rows; ++i) {
		for (int j = 0; j < pMx->cols; ++j) {
			entry = pMx->entries[at(hMx, i, j)];
			sprintf(entryStr, "%f", entry);
			removeTrailingZeroes(entryStr);
			printf("|");
			printf("%s", entryStr);
			extraSpaces = pMx->maxLength - strlen(entryStr);

			while (extraSpaces > 0) {
				printf(" ");
				--extraSpaces;
			}
			printf("  ");
			if (j == pMx->cols - 1)
				printf("|");
		}
		printf("\n");

		// print level of dashes for each row
		for (int i = 0; i < totalSpaces; ++i)
			printf("-");
		printf("\n");
	}
}


Status matrix_setEntry(MATRIX hMx, int row, int col, double entry) {
	Matrix* pMx = hMx;
	int idx;

	idx = at(hMx, row, col);
	if (idx != -1) {
		pMx->entries[idx] = entry;
		return SUCCESS;
	}
	else
		return FAILURE;
}




/********** Helper function definitions **********/
static Status adjustMatrixDims(Matrix** ppMx, int rows, int cols) {
	Matrix* pMx = *ppMx;
	double* entries;


	// matrix doesn't exist
	if (!(*ppMx)) {
		if (!(*ppMx = matrix_initDims(rows, cols)))
			return FAILURE;
	}
	// matrix exists
	else {
		// needs resizing
		if (getSize(pMx->rows, pMx->cols) < getSize(rows, cols)) {
			if (!(entries = calloc(getSize(rows, cols), sizeof(*entries))))
				return FAILURE;
			free(pMx->entries);
			pMx->entries = entries;
		}
		else { // doesn't need resizing, 0 out new entries
			int size = getSize(rows, cols);
			for (int i = 0; i < size; ++i)
				pMx->entries[i] = 0;
		}

		// in either case, set the new rows, columns, and max length
		pMx->rows = rows;
		pMx->cols = cols;
		pMx->maxLength = 1;
	}

	return SUCCESS;
}


static int at(Matrix* pMx, int row, int col) {
	return (row >= pMx->rows || col >= pMx->cols) ? -1 : (row * pMx->cols + col);
}


int at2(int rows, int cols, int row, int col) {
	return (row >= rows || col >= cols) ? -1 : row * cols + col;
}


static int calcEntryLength(double entry) {
	char entryStr[100];    // string representation of entry
	int entryInt;          // integer part of entry
	int length;

	// if entry is equivalent to an integer (i.e. 2.0) its length is just the integer part
	entryInt = (int)floor(entry);
	if (entryInt == entry) {
		sprintf(entryStr, "%d", entryInt);
		length =  strlen(entryStr);
	}
	// entry isn't equivalent to an integer, must get characters after decimal part, exclude trailing 0s
	else {
		sprintf(entryStr, "%f", entry);
		length = strlen(entryStr);
		for (int i = strlen(entryStr) - 1; entryStr[i] == '0' || entryStr[i] == '.'; --i)
			--length;
	}

	return length;
}


static int getSize(int rows, int cols) {
	return rows * cols;
}


static Status opAdjugate(Matrix* pMx, Matrix** ppMxRes) {
	Matrix* pMxRes;    // result matrix, not initialized b/c ppMxRes isn't guaranteed to have a matrix
	

	// recreate the result matrix if its dimensions aren't appropriate for the adjugate or it's NULL
	if (!adjustMatrixDims(ppMxRes, pMx->rows, pMx->cols))
		return FAILURE;
	pMxRes = *ppMxRes;

	// special case for 1 x 1 matrix
	if (pMxRes->rows == 1 && pMxRes->cols == 1) {
		pMxRes->entries[0] = (pMx->entries[0] != 0) ? 1 : 0;
		return SUCCESS;
	}
	// special case for a 2 x 2 matrix
	else if (pMxRes->rows == 2 && pMxRes->cols == 2) {
		int size = getSize(pMxRes->rows, pMxRes->cols);
		for (int i = 0; i < size; ++i)
			pMxRes->entries[i] = pMx->entries[i];

		pMxRes->entries[at(pMxRes, 0, 1)] *= -1;
		pMxRes->entries[at(pMxRes, 1, 0)] *= -1;
		double temp = pMxRes->entries[at(pMxRes, 0, 0)];
		pMxRes->entries[at(pMxRes, 0, 0)] = pMxRes->entries[at(pMxRes, 1, 1)];
		pMxRes->entries[at(pMxRes, 1, 1)] = temp;
		int length1 = calcEntryLength(pMxRes->entries[at(pMxRes, 0, 0)]);
		int length2 = calcEntryLength(pMxRes->entries[at(pMxRes, 0, 1)]);
		int length3 = calcEntryLength(pMxRes->entries[at(pMxRes, 1, 0)]);
		int length4 = calcEntryLength(pMxRes->entries[at(pMxRes, 1, 1)]);
		int maxLength = length1;
		if (length2 > maxLength)
			maxLength = length2;
		if (length3 > maxLength)
			maxLength = length3;
		if (length4 > maxLength)
			maxLength = length4;
		pMxRes->maxLength = maxLength;
		return SUCCESS;
	}

	// all other matrices - 3 x 3, 4 x 4 etc.
	// create the matrix of cofactors
	MATRIX hMxOfCofactors;     // the adjugate matrix is the transpose of the matrix of cofactors
	Matrix* pMxOfCofactors;    
	if (!(hMxOfCofactors = matrix_initDims(pMx->cols, pMx->rows)))
		return FAILURE;
	pMxOfCofactors = hMxOfCofactors;

	// calculate each entry for the matrix of cofactors
	Status mem;                    // indicate if there is memory allocation failure
	int maxLength = 1;             // max length of the new matrix (same as in the matrix structure).
	int numLength;                 // gets the length of each number to be compared to max length
	Boolean firstNewNum = TRUE;    // TRUE = first number being calculated in the whole result matrix
	double newTerm;                // each new term after multiplying by 1 / determinant

	for (int row = 0; row < pMx->rows; ++row) {
		for (int col = 0; col < pMx->cols; ++col) {
			// create the recursive submatrix
			MATRIX hMxSub = matrix_initDims(pMx->rows - 1, pMx->cols - 1);
			if (!hMxSub) {
				matrix_destroy(&hMxOfCofactors);
				return FAILURE;
			}
			Matrix* pMxSub = hMxSub;

			// get the submatrix
			int mxSub_row = 0;
			int mxSub_col = 0;
			for (int _row = 0; _row < pMx->rows; ++_row) {
				if (_row == row)
					continue;
				mxSub_col = 0;
				for (int _col = 0; _col < pMx->cols; ++_col) {
					if (_col == col)
						continue;
					pMxSub->entries[at(pMxSub, mxSub_row, mxSub_col)] = pMx->entries[at(pMx, _row, _col)];
					++mxSub_col;
				}
				++mxSub_row;
			}

			// get the i, jth entry in the matrix of cofactors
			mem = SUCCESS;
			newTerm = opDet(pMxSub, &mem);
			// memory allocation failure
			if (!mem) {
				matrix_destroy(&hMxOfCofactors);
				return FAILURE;
			}
			// multiply by -1 or not
			if (newTerm != 0 && (row + col) % 2 != 0)
				newTerm *= -1;

			numLength = calcEntryLength(newTerm);

			if (firstNewNum) {
				maxLength = numLength;
				firstNewNum = FALSE;
			}
			else if (numLength > maxLength)
				maxLength = numLength;
			
			pMxOfCofactors->entries[at(pMxOfCofactors, row, col)] = newTerm;

			matrix_destroy(&hMxSub);
		}
	}
	pMxOfCofactors->maxLength = maxLength;


	// matrix of cofactors has been created
	// now calculate the transpose of the matrix of cofactors, this is the adjugate matrix
	if (!matrix_opTrans(hMxOfCofactors, (MATRIX*)ppMxRes)) {
		matrix_destroy(&hMxOfCofactors);
		return FAILURE;
	}

	matrix_destroy(&hMxOfCofactors);

	return SUCCESS;
}


static double opDet(Matrix* pMx, Status* pMem) {
	MATRIX hMxSub;        // recursive submatrix in non-base case
	Matrix* pMxSub;       // non-handle version of recursive submatrix in non-base case
	double sum;           // sum in non-base case
	double entry;         // entry used in non-base case
	int mxSub_row = 0;    // submatrix row in non-base case
	int mxSub_col = 0;    // submatrix column in non-base case


	// base case: 2 x 2 matrix
	if (pMx->rows == 2 && pMx->cols == 2) {
		double a11 = pMx->entries[at(pMx, 0, 0)];
		double a21 = pMx->entries[at(pMx, 0, 1)];
		double a12 = pMx->entries[at(pMx, 1, 0)];
		double a22 = pMx->entries[at(pMx, 1, 1)];
		return opDet2x2(a11, a12, a21, a22);
	}

	// all other cases: recursive calculation
	sum = 0;
	for (int col = 0; col < pMx->cols; ++col) {
		// create the recursive submatrix
		hMxSub = matrix_initDims(pMx->rows - 1, pMx->cols - 1);
		if (!hMxSub) {
			*pMem = FAILURE;
			return 0;
		}
		pMxSub = hMxSub;

		// get one part of the recursive sum
		// get the a_1_jth entry
		entry = pMx->entries[at(pMx, 0, col)];
		mxSub_row = 0;
		mxSub_col = 0;
		// get the recursive submatrix
		for (int row = 1; row < pMx->rows; ++row) {
			mxSub_col = 0;
			for (int _col = 0; _col < pMx->cols; ++_col) {
				if (_col == col)
					continue;
				pMxSub->entries[at(hMxSub, mxSub_row, mxSub_col)] = pMx->entries[at(pMx, row, _col)];
				++mxSub_col;
			}
			++mxSub_row;
		}

		if (col % 2 == 0)
			sum += entry * opDet(pMxSub, pMem);
		else
			sum -= entry * opDet(pMxSub, pMem);

		matrix_destroy((MATRIX*)(&pMxSub));
	}

	return sum;
}


static double opDet2x2(double a11, double a12, double a21, double a22) {
	return a11 * a22 - a21 * a12;
}


static void removeTrailingZeroes(char* entryStr) {
	Boolean reachedDecimalPoint = FALSE;

	for (int i = strlen(entryStr) - 1; i >= 0 && (entryStr[i] == '0' || entryStr[i] == '.'); --i) {
		if (reachedDecimalPoint == TRUE)
			break;
		if (entryStr[i] == '.')
			reachedDecimalPoint = TRUE;
		entryStr[i] = '\0';
	}
}