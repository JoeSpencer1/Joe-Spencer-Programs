#include <iostream>
#include "matrix.h"
using namespace std;

void Matrix::menu()
{
    int choice;
    bool existing;
    int position;
    char response;
    double constant;
    double tr;
    bool esc = false;
    cout << "Please select your desired matrix option,\n";
    cout << "or press 0 to view menu:\n";
    choice = strChoice();
    if (choice == 0)
    {
        cout << "0: Exit, 1: Enter a New Matrix, 2: Read Matrix from File\n";
        cout << "3: Display, 4: Get Dimensions, 5: Scale by Constant\n";
        cout << "6: Find Determinant, 7: Find Eigenvalues and Eigenvectors\n";
        cout << "8: Invert, 9: Transpose, 10: Find Trace, 11: Find L1 and L2 norms\n";
        cout << "12: Solve, 13: Add, 14: Subtract, 15: Cross-Multiply\n";
        cout << "Press the X key to go back.\n";
        choice = strChoice();
    }
    cout << endl;
    vector<vector<double> > tempData;
    vector<double> tempVector;
    tempData.push_back(tempVector);
    if (choice == 0)
    {
        cout << "Goodbye.\n";
        return;
    }
    // Options not requiring a matrix
    if (choice == 1)
    {
        Matrix A = Matrix();
        addToList(A);
        cout << "New matrix stored at position " << numMenu[matrixMenu.size() - 1] << ".\n";
    }
    if (choice == 2)
    {
        string name = findName();
        Matrix A = Matrix(name);
        addToList(A);
        cout << "New matrix stored at position " << numMenu[matrixMenu.size() - 1] << ".\n";
    }
    // One Matrix
    if ((choice > 2) && (choice < 12))
    {
        Matrix A = retreiveMatrix("1");
        if (choice == 3)
        {
            A.printMatrix();
        }
        if (choice == 4)
        {
            cout << "Width = " << A.getWidth() << endl;
            cout << "Height = " << A.getHeight() << endl;        
        }
        if (choice == 5)
        {
            cout << "Please enter the constant.\n";
            constant = decEntry();
            Matrix C = A.scale(constant);
            addToList(C);
        }
        if (choice == 6)
        {
            if (square() == true)
            {
                cout << "Determinant = " << A.determinant() << endl;
            }
            else
            {
                cout << "This matrix is not square.\n";
            }
        }
        if (choice == 7)
        {
            if ((A.getHeight() == 0) || (A.getHeight() != A.getWidth()) || (A.determinant() == 0))
            {
                cout << "Must be a square invertible matrix.\n";
            }
            else
            {
                A.eigenValues();
                cout << "\nEigenvalues:\n";
                A.printEigen();
                A.eigenVecs();
                cout << "\nEigenvectors:\n";
                A.printEigenVec();
            }
        }
        if (choice == 8)
        {
            Matrix C = A.invert();
            if (C.getHeight() > 0)
            {
                cout << "Inverse:\n";
                C.printMatrix();
                addToList(C);
            }
        }
        if (choice == 9)
        {
            Matrix C = A.transpose();
            cout << "Transpose:\n";
            C.printMatrix();
            addToList(C);
        }
        if (choice == 10)
        {
            tr = A.trace();
            cout << "Trace = " << tr << endl;
        }
        if (choice == 11)
        {
            A.l2Norm();
            cout << "L1 norm = " << norm1 << endl;
            cout << "L2 norm = " << norm2 << endl;
        }
    }
    // 2 Matrices
    if ((choice > 11) && (choice < 16))
    {
        Matrix A = retreiveMatrix("1");
        Matrix B = retreiveMatrix("2");
        if (choice == 12)
        {
            if ((A.getHeight() != A.getWidth()) || (A.getHeight() != B.getHeight()) || (B.getWidth() != 1))
            {
                cout << "The matrix equation Ax = b cannot be solved.\n";
            }
            else
            {
                Matrix C = A.invert();
                if (C.getMatrix().size() == 0)
                {
                    cout << "Matrix A is not invertible.\n";
                }
                else
                {
                    Matrix D = C.cross(B, true);
                    cout << "Ax = b: x =\n";
                    D.printMatrix();
                }
            }
        }
        if (choice == 13)
        {
            Matrix C = A.add(B);
            C.printMatrix();
            addToList(C);
            cout << "Sum added to position " << matrixMenu.size() << endl;
        }
        if (choice == 14)
        {
            Matrix C = A.subtract(B);
            C.printMatrix();
            addToList(C);
            cout << "Difference added to position " << matrixMenu.size() << endl;
        }
        if (choice == 15)
        {
            Matrix C = A.cross(B, true);
            C.printMatrix();
            addToList(C);
            cout << "Product added to position " << matrixMenu.size() << endl;
        }
    }
    cout << endl;
    menu();
}