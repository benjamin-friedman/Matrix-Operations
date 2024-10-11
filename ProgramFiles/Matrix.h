/*
  Author:       Benjamin G. Friedman
  Date:         05/20/2021
  File:         Matrix.h
  Description:  Header file for the matrix opaque object interface.
*/


#ifndef MATRIX_H
#define MATRIX_H

#include "Status.h"

typedef void* MATRIX;    // opaque object handle for matrix objects




/*
FUNCTION
  - Name:     matrix_canBeAdd
  - Purpose:  Determine if two matrices can be added.
              The dimensions of the matrices must be equal.
PRECONDITION
  - hMx1
      Purpose:       Check if it can be added with matrix 2.
      Restrictions:  Handle to a valid matrix object.
  - hMx2
      Purpose:       Check if it can be added with matrix 1.
      Restrictions:  Handle to a valid matrix object.
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       The correct value is returned accordingly.
  - Return value:  TRUE if the two matrices can be added.
                   FALSE if otherwise.
  - hMx1:          The state of the matrix before the function call is preserved.
  - hMx2:          The state of the matrix before the function call is preserved.
Failure
  - N/A
*/
Boolean matrix_canBeAdd(MATRIX hMx1, MATRIX hMx2);


/*
FUNCTION
  - Name:     matrix_canBeInv
  - Purpose:  Determine if a matrix can be inverted (is vertible).
              A matrix is vertible if its determinant isn't 0.
PRECONDITION
  - hMx
      Purpose:       Check if it can be inverted.
      Restrictions:  Handle to a valid matrix object.
  - pMem:
      Purpose:       Indicate if there is memory allocattion failure.
      Restrictions:  Not NULL.
POSTCONDITION
Success
  - Reason:        No memory allocation failure
  - Summary:       The correct value is returned accordingly.
  - Return value:  TRUE if the matrix can be inverted.
                   FALSE if otherwise.
  - hMx:           The state of the matrix before the function call is preserved.
  - pMem:          The status it points to is set to SUCCESS.
Failure
  - Reason:        Memory allocation failure
  - Summary:       Communicates the memory allocation failure.
  - Return value:  FALSE
  - hMx:           The state of the matrix before the function call is preserved.
  - pMem:          The status it points to is set to FAILURE.
*/
Boolean matrix_canBeInv(MATRIX hMx, Status* pMem);


/*
FUNCTION
  - Name:     matrix_canBeMult
  - Purpose:  Determine if two matrices can be multiplied.
              The columns of matrix 1 must equal the rows of matrix 2.
PRECONDITION
  - hMx1
      Purpose:       Check if it can be multiplied with matrix 2.
      Restrictions:  Handle to a valid matrix object.
  - hMx2
      Purpose:       Check if it can be multiplied with matrix 1.
      Restrictions:  Handle to a valid matrix object.
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       The correct value is returned accordingly.
  - Return value:  TRUE if the two matrices can be multiplied.
                   FALSE if otherwise.
  - hMx1:          The state of the matrix before the function call is preserved.
  - hMx2:          The state of the matrix before the function call is preserved.
Failure
  - N/A
*/
Boolean matrix_canBeMult(MATRIX hMx1, MATRIX hMx2);


/*
FUNCTION
  - Name:     matrix_canBeSub
  - Purpose:  Determine if two matrices can be subtracted.
              The dimensions of the matrices must be equal.
PRECONDITION
  - hMx1
      Purpose:       Check if it can be subtracted with matrix 2.
      Restrictions:  Handle to a valid matrix object.
  - hMx2
      Purpose:       Check if it can be subtracted with matrix 1.
      Restrictions:  Handle to a valid matrix object.
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       The correct value is returned accordingly.
  - Return value:  TRUE if the two matrices can be subtracted.
                   FALSE if otherwise.
  - hMx1:          The state of the matrix before the function call is preserved.
  - hMx2:          The state of the matrix before the function call is preserved.
Failure
  - N/A
*/
Boolean matrix_canBeSub(MATRIX hMx1, MATRIX hMx2);


/*
FUNCTION
  - Name:     matrix_copy
  - Purpose:  Copies the data from one matrix into another.
              C Opaque object design version of the copy assignment operator in C++.
PRECONDITION
  - phMxDest
      Purpose:       Matrix object to copy into.
      Restrictions:  Pointer to a handle to a valid matrix object or NULL handle.
  - hMxSrc
      Purpose:       Matrix object to copy from.
      Restrictions:  Handle to a valid matrix object.
POSTCONDITION
Success
  - Reason:        No memory allocation failure.
  - Summary:       Creates a copy of hMxSrc and stores it in the handle pointed to by phMxDest.
  - Return value:  SUCCESS
  - phMxDest:      The handle it points to stores a copy of hMxSrc.
                     - If it points to a valid matrix object, hMxSrc is copied into the existing matrix.
                     - If it points to a NULL handle, a new matrix is first created after which hMxSrc is copied into it.
  - hMxSrc:        The state of the matrix before the function call is preserved.
Failure
  - Reason:        Memory allocation failure.
  - Summary:       Doesn't create a copy of hMxSrc and nothing of significance happens.
  - Return value:  FAILURE
  - phMxDest:      If it is a pointer to a handle to valid matrix object, the state of the matrix before the function call is preserved.
                   If it is a pointer to a NULL handle, the handle remains NULL.
  - hMxSrc:        The state of the matrix before the function call is preserved.
*/
Status matrix_copy(MATRIX* phMxDest, MATRIX hMxSrc);


/*
FUNCTION
  - Name:     matrix_destroy
  - Purpose:  Destroys a matrix.
              C Opaque object design version of the destructor in C++.
PRECONDITION
  - phMx
      Purpose:       Matrix to destroy.
      Restrictions:  Pointer to a handle to a valid matrix object or NULL handle.
POSTCONDITION
Success
  - Reason:        The handle it points to stores a valid matrix object.
  - Summary:       Destroys the matrix.
  - Return value:  SUCCESS
  - phMx:          Frees all memory associated with the matrix and sets the handle to NULL.
Failure
  - Reason:        The handle it points to is NULL.
  - Summary:       No matrix is destroyed and nothing of significance happens.
  - Return value:  FAILURE
  - phMx:          The handle it points to remains NULL.
*/
Status matrix_destroy(MATRIX* phMx);


/*
FUNCTION
  - Name:     matrix_getEntry
  - Purpose:  Get the entry of a matrix at a row-column coordinate.
PRECONDITION
  - hMx
      Purpose:       Matrix to get the entry of.
      Restrictions:  Handle to a valid matrix object.
  - row
      Purpose:       Index of the row of the entry (i.e. row 1 = index 0).
      Restrictions:  Any integer >= 0.
  - col
      Purpose:       Index of the column of the entry (i.e. column 1 = index 0).
      Restrictions:  Any integer >= 0.
  - pEntry
      Purpose:       Store the entry.
      Restrictions:  Not NULL.
POSTCONDITION
Success
  - Reason:        The row-column coordinate is in bounds.
  - Summary:       Gets the entry of the matrix at the row-column coordinate.
  - Return value:  SUCCESS
  - hMx:           The state of the matrix before the function call is preserved.
  - pEntry:        The integer it points to is set to the entry at the row-column coordinate.
Failure
  - Reason:        The row-column coordinate is out of bounds.
  - Summary:       Doesn't get the entry of the matrix at the row-column coordinate and nothing of significance happens.
  - Return value:  FAILURE
  - hMx:           The state of the matrix before the function call is preserved.
  - pEntry:        The integer it points to is set to the special value of 0.
*/
Status matrix_getEntry(MATRIX hMx, int row, int col, double* pEntry);


/*
FUNCTION
  - Name:     matrix_initCopy
  - Purpose:  Initializes a new matrix that's a copy of another.
              C Opaque object design version of the copy constructor in C++.
PRECONDITION
  - hMxSrc
      Purpose:       Matrix object to copy from.
      Restrictions:  Handle to a valid matrix object.
POSTCONDITION
Success
  - Reason:        No memory allocation failure.
  - Summary:       Initializes and returns a new matrix that's a copy of hMxSrc.
  - Return value:  Handle to a valid matrix object that's a copy of hMxSrc.
  - hMxSrc:        The state of the matrix before the function call is preserved.
Failure
  - Reason:        Memory allocation failure.
  - Summary:       Doesn't initialize and return a new matrix and nothing of significance happens.
  - Return value:  NULL
  - hMxSrc:        The state of the matrix before the function call is preserved.
*/
MATRIX matrix_initCopy(MATRIX hMxSrc);


/*
FUNCTION
  - Name:     matrix_initMove
  - Purpose:  Initialize a new matrix handle by moving an existing matrix in a source handle into the new handle.
              It is a transfer of ownership of the matrix and not a copy.
              C Opaque object design version of the move constructor in C++.
PRECONDITION
  - phMxSrc
      Purpose:       Pointer to the matrix handle to move from.
      Restrictions:  Pointer to a handle to a valid matrix object or NULL handle.
POSTCONDITION
Success
  - Reason:        The handle pointed to by phMxSrc is a handle to a valid matrix object.
  - Summary:       Initializes a new matrix handle by moving an existing matrix in a source handle into the new handle.
                   The source handle loses ownership of the matrix object.
  - Return value:  Handle to a valid matrix object that is the same matrix of the handle pointed to by phMxSrc.
  - phMxSrc:       The handle it points to loses ownership of the matrix and is set to NULL.
Failure
  - Reason:        The handle pointed to by phMxSrc is a NULL handle.
  - Summary:       Doesn't initialize a new matrix handle and nothing of significance happens.
  - Return value:  NULL
  - phMxSrc:       The handle it points to remains NULL.
*/
MATRIX matrix_initMove(MATRIX* phMxSrc);


/*
FUNCTION
  - Name:     matrix_initDims
  - Purpose:  Initialize a new matrix with a given amount of rows and columns and the entries in a default state.
PRECONDITION
  - rows
      Purpose:       Rows of the new matrix.
      Restrictions:  Any positive integer.
  - columns
      Purpose:       Columns of the matrix.
      Restrictions:  Any positive integer.
POSTCONDITION
Success
  - Reason:        No memory allocation failure.
  - Summary:       Initializes and returns a new matrix with the given amount of rows and columns.
                   The value of every entry is 0 by default.
  - Return value:  Handle to a valid matrix object in the state as described above.
Failure
  - Reason:        Memory allocation failure.
  - Summary:       Doesn't initialize and return a new matrix and nothing of significance happens.
  - Return value:  NULL
*/
MATRIX matrix_initDims(int rows, int columns);


/*
FUNCTION
  - Name:     matrix_move
  - Purpose:  Move an existing matrix in a source handle into an existing destination handle.
              It is a transfer of ownership of the matrix and not a copy.
              C Opaque object design version of the move assignment operator in C++.
PRECONDITION
  - phMxDest
      Purpose:       Pointer to the matrix handle to move to.
      Restrictions:  Pointer to a handle to a valid matrix object or NULL handle.
  - phMxSrc
      Purpose:       Pointer to the matrix handle to move from.
      Restrictions:  Pointer to a handle to a valid matrix object or NULL handle.
POSTCONDITION
Success
  - Reason:        The handle pointed to by phMxSrc is a handle to a valid matrix object.
  - Summary:       Move the matrix in the source handle pointed to by phMxSrc into the destination handle pointed to by phMxDest.
                   The source handle pointed to by phMxSrc loses ownership of the matrix object.
  - Return value:  SUCCESS
  - phMxDest:      The handle it points to stores the matrix in the source handle pointed to by phMxSrc.
                   If prior to the move it is a pointer to a handle to a valid matrix object and not a NULL handle, the matrix object is destroyed.
  - phMxSrc:       Set to NULL.
Failure
  - Reason:        The handle pointed to by phMxSrc is a NULL handle.
  - Summary:       There is no move and nothing of significance happens.
  - Return value:  FAILURE
  - phMxDest:      If it is a pointer to a handle to a valid matrix object, the state of the matrix before the function call is preserved.
                   If it is a pointer to a NULL handle, the handle it points to remains NULL.
  - phMxSrc:       The handle it points to remains NULL.
*/
Status matrix_move(MATRIX* phMxDest, MATRIX* phMxSrc);


/*
FUNCTION
  - Name:     matrix_newMatrix
  - Purpose:  Create a new matrix using an existing matrix object with new entries, rows, and columns.
PRECONDITION
  - hMx
      Purpose:       Matrix to create the new matrix with.
      Restrictions:  hMx is a handle to a valid matrix object.
  - entries
      Purpose:       Entries of the new matrix.
      Restrictions:  Any array with size >= 1.
  - rows
      Purpose:       Rows of the new matrix.
      Restrictions:  Must equal the actual amount of rows of the matrix.
  - cols
      Purpose:       Columns of the new matrix.
      Restrictions:  Must equal the actual amount of columns of the matrix.
POSTCONDITION
Success
  - Reason:        No memory allocation failure.
  - Summary:       Creates a new matrix using an existing matrix object with new entries, rows, and columns.
  - Return value:  SUCCESS
Failure
  - Reason:        Memory allocation failure.
  - Summary:       Doesn't create a new matrix and nothing of significance happens.
  - Return value:  FAILURE
  - hMx:           The state of the matrix before the function call is be preserved.
*/
Status matrix_newMatrix(MATRIX hMx, const double* entries, int rows, int cols);


/*
FUNCTION
  - Name:     matrix_opAdd
  - Purpose:  Performs the matrix addition operation.
PRECONDITION
  - hMxs
      Purpose:       Matrices to be added.
      Restrictions:  Array of handles to valid matrix objects.
                     The dimensions of each matrix are the same.
  - hMxsSize
      Purpose:       The number of matrices being added (size of hMxs).
      Restrictions:  Must equal the actual size of hMxs.
  - phMxRes
      Purpose:       Store the matrix that is the result of the addition.
      Restrictions:  Pointer to a handle to a valid matrix object or NULL handle.
POSTCONDITION
Success
  - Reason:        No memory allocation failure.
  - Summary:       The matrices are added and the result is stored in the result matrix.
  - Return value:  SUCCESS
  - hMxs:          The state of the array and individual matrices before the function call is preserved.
  - phMxRes:       Stores the matrix that is the result of the addition.
                   If it was a pointer to a handle valid matrix object before the function call, the matrix's dimensions are adjusted if necessary.
                   If it was a pointer to a NULL handle before the function call, a new matrix gets created to store the result.
Failure
  - Reason:        Memory allocation failure.
  - Summary:       The matrices aren't added and nothing of significance happens.
  - Return value:  FAILURE
  - hMxs:          The state of the array and individual matrices before the function call is preserved.
  - phMxRes:       If it was a pointer to a handle to a valid matrix object before the function call, the state of the matrix before the function call is preserved.
                   If it was a pointer to a NULL handle before the function call, the handle remains NULL.
*/
Status matrix_opAdd(MATRIX* hMxs, int hMxsSize, MATRIX* phMxRes);


/*
FUNCTION
  - Name:     matrix_opDet
  - Purpose:  Perform the matrix determinant operation.
PRECONDITION
  - hMx
      Purpose:       Matrix to calculate the determinant of.
      Restrictions:  Handle to a valid matrix object.
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
  - Summary:       The determinant operation isn't performed and nothing of significance happens.
  - Return value:  0
  - hMx:           The state of the matrix before the function call is preserved.
  - pMem:          The Boolean it points to is set to FAILURE.
*/
double matrix_opDet(MATRIX hMx, Status* pMem);


/*
FUNCTION
  - Name:     matrix_opInv
  - Purpose:  Perform the matrix inverse operation.
PRECONDITION
  - hMx
      Purpose:       Matrix to calculate the inverse of.
      Restrictions:  Handle to a valid matrix object.
                     The rows equal the columns.
  - pMxIsInvertible
      Purpose:       Indicate if the matrix is invertible (determinant is 0).
                     If a matrix is invertible, its inverse can't be calculated.
      Restrictions:  Not NULL.
  - phMxRes
      Purpose:       Store the matrix that is the result of the addition.
      Restrictions:  Pointer to a handle to a valid matrix object or NULL handle.
POSTCONDITION
Success
  - Reason:           No memory allocation failure and matrix is vertible.
  - Summary:          Performs the matrix inverse operation and stores the inverted matrix in the result matrix.
  - Return value:     SUCCESS
  - hMx:              The state of the matrix before the function call is preserved.
  - pMxIsInvertible:  The Boolean it points to is set to FALSE.
  - phMxRes:          Stores the inverted matrix that is the result of the inverse operation.
                      If it was a pointer to a handle to a valid matrix object before the function call, the matrix's dimensions are adjusted if necessary.
                      If it was a pointer to a NULL handle before the function call, a new matrix gets created to store the result.
Failure
  - Reason:           Memory allocation failure or matrix is invertible.
  - Summary:          The inverse operation isn't performed and nothing of significance happens.
  - Return value:     FAILURE
  - hMx:              The state of the matrix before the function call is preserved.
  - pMxIsInvertible:  The Boolean it points to is set accordingly.
                        - FALSE is there is memory allocation failure.
                          This doesn't necessarily mean it's not invertible.
                          In this case, it can't be known if the matrix is invertible or not because the failure may have happened before this was determined.
                        - TRUE if otherwise.
  - phMxRes:          If it was a pointer to a handle to a valid matrix object before the function call, the state of the matrix before the function call is not guaranteed to be preserved.
                      If it was a pointer to a NULL handle before the function call, the handle is not guaranteed to still be NULL.
*/
Status matrix_opInv(MATRIX hMx, Boolean* pMxIsInvertible, MATRIX* phMxRes);


/*
FUNCTION
  - Name:     matrix_opMult
  - Purpose:  Performs the matrix multiplication operation.
PRECONDITION
  - hMx1
      Purpose:       Matrix to be multiplied with the other matrix.
      Restrictions:  Handle to a valid matrix object.
                     The columns of this matrix equal the rows of the other matrix.
  - hMx2
      Purpose:       Matrix to be multiplied with the other matrix.
      Restrictions:  Handle to a valid matrix object.
                     The rows of this matrix equal the columns of the other matrix.
  - phMxRes
      Purpose:       Store the matrix that is the result of the multiplication.
      Restrictions:  Pointer to a handle to a valid matrix object or NULL handle.
POSTCONDITION
Success
  - Reason:        No memory allocation failure.
  - Summary:       The matrices are multiplied and the result is stored in the result matrix.
  - Return value:  SUCCESS
  - hMx1:          The state of the matrix before the function call is preserved.
  - hMx2:          The state of the matrix before the function call is preserved.
  - phMxRes:       Stores the matrix that is the result of the multiplication.
                   If it was a pointer to a handle to a valid matrix object before the function call, the matrix's dimensions are adjusted if necessary.
                   If it was a pointer to a NULL handle before the function call, a new matrix gets created to store the result.
Failure
  - Reason:        Memory allocation failure.
  - Summary:       The matrices aren't multiplied and nothing of significance happens.
  - Return value:  FAILURE
  - hMx1:          The state of the matrix before the function call is preserved.
  - hMx2:          The state of the matrix before the function call is preserved.
  - phMxRes:       If it was a pointer to a handle to a valid matrix object before the function call, the state of the matrix before the function call is preserved.
                   If it was a pointer to a NULL handle before the function call, the handle remains NULL.
*/
Status matrix_opMult(MATRIX hMx1, MATRIX hMx2, MATRIX* phMxRes);


/*
FUNCTION
  - Name:     matrix_opPow
  - Purpose:  Performs the matrix power operation.
PRECONDITION
  - hMx
      Purpose:       Matrix to perform the power operation on.
      Restrictions:  Handle to a valid matrix object.
                     The rows equals the columns.
  - power
      Purpose:       The power to use in the power operation.
      Restrictions:  Any positive integer.
  - phMxRes
      Purpose:       Store the matrix that is the result of the power operation.
      Restrictions:  Pointer to a handle to a valid matrix object or NULL handle.
POSTCONDITION
Success
  - Reason:        No memory allocation failure.
  - Summary:       The power operation is performed the result is stored in the result matrix.
  - Return value:  SUCCESS
  - hMxs:          The state of the matrix before the function call is preserved.
  - phMxRes:       Stores the matrix that is the result of the power operation.
                   If it was a pointer to a handle to a valid matrix object before the function call, the matrix is destroyed before storing the result.
Failure
  - Reason:        Memory allocation failure.
  - Summary:       The power operation isn't performed and nothing of significance happens.
  - Return value:  FAILURE
  - hMxs:          The state of the matrix before the function call is preserved.
  - phMxRes:       If it was a pointer to a handle to a valid matrix object before the function call, the state of the matrix before the function call is preserved.
                   If it was a pointer to a NULL handle before the function call, the handle remains NULL.
*/
Status matrix_opPow(MATRIX hMx, int power, MATRIX* phMxRes);


/*
FUNCTION
  - Name:     matrix_opSub
  - Purpose:  Performs the matrix subtraction operation.
PRECONDITION
  - hMxs
      Purpose:       Matrices to be subtracted.
      Restrictions:  Array of handles to valid matrix objects.
  - hMxsSize
      Purpose:       The number of matrices being subtracted (size of hMxs).
      Restrictions:  Must equal the actual size of hMxs.
  - phMxRes
      Purpose:       Store the matrix that is the result of the subtraction.
      Restrictions:  Pointer to a handle to a valid matrix object or NULL handle.
POSTCONDITION
Success
  - Reason:        No memory allocation failure.
  - Summary:       The matrices are subtracted and the result is stored in the result matrix.
  - Return value:  SUCCESS
  - hMxs:          The state of the array and individual matrices before the function call is preserved.
  - phMxRes:       Stores the matrix that is the result of the subtraction.
                   If it was a pointer to a handle to a valid matrix object before the function call, the matrix's dimensions are adjusted if necessary.
                   If it was a pointer to a NULL handle before the function call, a new matrix gets created to store the result.
Failure
  - Reason:        Memory allocation failure.
  - Summary:       The matrices aren't subtracted and nothing of significance happens.
  - Return value:  FAILURE
  - hMxs:          The state of the array and individual matrices before the function call is preserved.
  - phMxRes:       If it was a pointer to a handle to a valid matrix object before the function call, the state of the matrix before the function call is preserved.
                   If it was a pointer to a NULL handle before the function call, the handle remains NULL.
*/
Status matrix_opSub(MATRIX* hMxs, int hMxsSize, MATRIX* phMxRes);


/*
FUNCTION
  - Name:     matrix_opTrans
  - Purpose:  Perform the matrix transpose operation.
PRECONDITION
  - hMx
      Purpose:       Matrix to be transposed.
      Restrictions:  Handle to a valid matrix object.
  - phMxRes
      Purpose:       Store the matrix that is the result of the transpose operation.
      Restrictions:  Pointer to a handle to a valid matrix object or NULL handle.
POSTCONDITION
Success
  - Reason:        No memory allocation failure.
  - Summary:       Performs the matrix transpose operation and stores the result in the result matrix
  - Return value:  SUCCESS
  - hMx:           The matrix before the function call is preserved.
  - phMxRes:       Stores the matrix that is the result of the transpose operation.
                   If it was a pointer to a handle to a valid matrix object before the function call, the matrix's dimensions are adjusted if necessary.
                   If it was a pointer to a NULL handle before the function call, a new matrix gets created to store the result.
Failure
  - Reason:        Memory allocation failure.
  - Summary:       The matrix isn't transposed and nothing of significance happens.
  - Return value:  FAILURE
  - hMx:           The state of the matrix before the function call is preserved.
  - phMxRes:       If it was a pointer to a handle to a valid matrix object before the function call, the state of the matrix before the function call is preserved.
                   If it was a pointer to a NULL handle before the function call, the handle remains NULL.
*/
Status matrix_opTrans(MATRIX hMx, MATRIX* phMxRes);


/*
FUNCTION
  - Name:     matrix_print
  - Purpose:  Print the entries of the matrix to stdout.
PRECONDITION
  - hMx
      Purpose:       Matrix to print.
      Restrictions:  Handle to a valid matrix object.
POSTCONDITION
Success
  - Reason:        All cases.
  - Summary:       Prints the entries of the matrix to stdout.
  - Return value:  N/A
  - hPoly:         The state of the matrix before the function call is preserved.
Failure
  - N/A
*/
void matrix_print(MATRIX hMx);


/*
FUNCTION
  - Name:     matrix_setEntry
  - Purpose:  Set the entry of a matrix at a row-column coordinate.
PRECONDITION
  - hMx
      Purpose:       Matrix to set the entry in.
      Restrictions:  Handle to a valid matrix object.
  - row
      Purpose:       Index of the row of the entry (i.e. row 1 = index 0).
      Restrictions:  Any integer >= 0.
  - col
      Purpose:       Index of the column of the entry (i.e. column 1 = index 0).
      Restrictions:  Any integer >= 0.
  - entry
      Purpose:       New entry.
      Restrictions:  Not NULL.
POSTCONDITION
Success
  - Reason:        The row-column coordinate is in bounds.
  - Summary:       Sets the entry of the matrix at the row-column coordinate.
  - Return value:  SUCCESS
  - hMx:           The state of the matrix before the function call is preserved.
Failure
  - Reason:        The row-column coordinate is out of bounds.
  - Summary:       Doesn't set the entry of the matrix at the row-column coordinate and nothing of significance happens.
  - Return value:  FAILURE
  - hMx:           The state of the matrix before the function call is preserved.
*/
Status matrix_setEntry(MATRIX hMx, int row, int col, double entry);


#endif