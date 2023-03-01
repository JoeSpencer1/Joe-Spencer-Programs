#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include "matrix.h"
using namespace std;

int Matrix::vectorPosition(vector<Matrix> matrixMenu, vector<int> numMenu)
{
    int number;
    int position;
    bool found = false;
    while (found == false)
    {
        cout << "Please enter the number position.\n";
        cin >> number;
        for (int i = 0; i < numMenu.size(); i ++)
        {   
            if (numMenu[i] == number)
            {
                position = i;
                found = true;
                cout << "Matrix " << numMenu[i] << " successfully found.\n";
            }
        }
        if (found == false)
        {
            cout << "Sorry, there is not any matrix at this index.\n";
        }
        cin.clear();
    }
    return position;
}

void Matrix::checkValidity(ifstream & readMatrix)
{
    if (readMatrix.eof())
    {
        publishFile();
        cout << "Invalid matrix file.\n";
        height = 0;
        width = 0;
    }    
}

bool Matrix::ynResponse()
{
    char response;
    while (1)
    {
        cout << "Please enter a response Y/N:\n";
        cin >> response;
        if ((response == 'Y') || (response == 'y'))
        {
            return true;
        }
        if ((response == 'N') || (response == 'n'))
        {
            return false;
        }
    }
}

void Matrix::publishFile()
{
    ofstream publish;
    publish.open(name);
    publish << height << endl;
    publish << width << endl;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            publish << matrix[i][j] << "\t";
        }
        publish << endl;
    }
    publish.close();
}

void Matrix::printMatrix()
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

void Matrix::publishNew(int rows, int columns, vector<vector<double> > newMatrix)
{
    if ((rows == 0) || (columns == 0))
    {
        return;
    }
    else
    {
        string stringNum = generateName();
        ofstream publish;
        publish.open(stringNum);
        publish << rows << endl;
        publish << columns << endl;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                publish << newMatrix[i][j] << "\t";
            }
        publish << endl;
        }
        publish.close();   
    }
}

string Matrix::generateName()
{
    int title = 1;
    string stringNum = "Matrices/Matrix" + to_string(title) + ".txt";
    ifstream test;
    test.open(stringNum);
    while(test.is_open())
    {
        test.close();
        title ++;
        stringNum = "Matrices/Matrix" + to_string(title) + ".txt";
        test.open(stringNum);
    }
    test.close();
    return stringNum;
}

string Matrix::findName()
{
    string name;
    cout << "Please enter the name of your matrix file.\n";
    cin >> name;
    name = longName(name);
    ifstream test;
    test.open(name);
    while(!test.is_open())
    {
        test.close();
        cout << "Please enter a valid name.\n";
        cin >> name;
        name = longName(name);
        test.open(name);
    }
    test.close();
    return name;
}

string Matrix::getName()
{
    return name;
}

int Matrix::getWidth()
{
    return width;
}

int Matrix::getHeight()
{
    return height;
}

vector<vector<double> > Matrix::getMatrix()
{
    return matrix;
}

bool Matrix::checkCompatibility(Matrix Other)
{
    if ((getWidth() == 1) && (Other.getWidth() == 1) && (getHeight() == Other.getHeight()))
    {
        return true;
    }
    if (getWidth() == Other.getHeight())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Matrix::square()
{
    if (height == width)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Matrix::switchRows(vector<vector<double> > toSwap, int row1, int row2)
{
    double temp;
    if ((row1 < toSwap.size()) && (row2 < toSwap.size()))
    {
        for (int counter = 0; counter < toSwap[row1].size(); counter++)
        {
            temp = toSwap[row1][counter];
            toSwap[row1][counter] = toSwap[row2][counter];
            toSwap[row2][counter] = temp;
        }
    }
    return;
}

bool Matrix::invertible()
{
    if (width != height)
    {
        cout << "Width != height.\n";
        return false;
    }
    double det = characteristic();
    if (det == 0)
    {
        cout << "Determinant = 0\n";
        return false;
    }
    else
    {
        return true;
    }
}

vector<vector<double> > Matrix::rref()
{
    double scale;
    vector<vector<double> > solve;
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[0].size(); j++)
        {
            solve[i][j] = matrix[i][j];
        }
    }
    if (solve.size() != solve[0].size())
    {
        cout << "Sorry, this matrix is not square.\n";
        solve.clear();
    }
    else
    {
        for (int i = 0; i < solve.size(); i++)
        {
            int j = i + 1;
            while (solve[i][i] == 0)
            {
                if (j == solve.size())
                {
                    cout << "This matrix is not invertible.\n";
                    solve.clear();
                    return solve;
                }
                switchRows(solve, i, j);
                j++;
            }
            for (int k = i + 1; k < solve.size(); k++)
            {
                if (solve[k][i] != 0)
                {
                    scale = solve[i][i] / solve[k][i];
                    for (int l = 0; l < solve.size(); l++)
                    {
                        solve[k][l] *= scale;
                        solve[k][l] -= solve[i][l];
                    }
                }
            }
        }
    }
    return solve;
}

void Matrix::menu()
{
    int choice;
    bool existing;
    int position;
    char response;
    double constant;
    double tr;
    cout << "Please select your desired matrix option:\n";
    cout << "Press 0 to view menu.\n";
    cin >> choice;
    if (choice == 0)
    {
        cout << "0: Exit, 1: Enter a New Matrix, 2: Read Matrix from File\n";
        cout << "3: Display, 4: Get Dimensions, 5: Scale by Constant\n";
        cout << "6: Find Determinant, 7: Find Eigenvalues and Eigenvectors\n";
        cout << "8: Invert, 9: Transpose, 10: Find Trace, 11: Solve";
        cout << "12: Add, 13: Subtract, 14: Cross-Multiply\n";
        cin >> choice;
    }
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
    if ((choice > 2) && (choice < 11))
    {
        Matrix A = getMatrix("1");
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
            cin >> constant;
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
            A.eigenValues();
            cout << "Eigenvalues:\n";
            A.printEigen();
            A.eigenVecs();
        }
        if (choice == 8)
        {
            Matrix C = A.invert();
            cout << endl;
            cout << "Inverse:\n";
            C.printMatrix();
            addToList(C);
        }
        if (choice == 9)
        {
            Matrix C = A.transpose();
            cout << endl;
            cout << "Transpose:\n";
            C.printMatrix();
            addToList(C);
        }
        if (choice == 10)
        {
            tr = A.trace();
            cout << "Trace = " << tr << endl;
        }
    }
    // 2 Matrices
    if ((choice > 9) && (choice < 15))
    {
        Matrix A = getMatrix("1");
        Matrix B = getMatrix("2");
        if (choice == 11)
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
        if (choice == 12)
        {
            Matrix C = A.add(B);
            C.printMatrix();
            addToList(C);
            cout << "Sum added to position " << matrixMenu.size() << endl;
        }
        if (choice == 13)
        {
            Matrix C = A.subtract(B);
            C.printMatrix();
            addToList(C);
            cout << "Difference added to position " << matrixMenu.size() << endl;
        }
        if (choice == 14)
        {
            Matrix C = A.cross(B, true);
            C.printMatrix();
            addToList(C);
            cout << "Product added to position " << matrixMenu.size() << endl;
        }
    }
    else
    {
        cout << "Please enter a valid option number 0-14.\n";
    }
    cout << endl;
    menu();
}

vector<double> Matrix::polynomial(int row)
{
    vector<double> polynomial;
    double real;
    double imaginary;
    double a = 1.0;
    double b = -1 * (E[row][row] + E[row + 1][row + 1]);
    double c = E[row][row] * E[row + 1][row + 1] - E[row + 1][row] * E[row][row + 1];
    polynomial.push_back(-1 * b / (2 * a));
    polynomial.push_back(sqrt(4 * a * c - b * b) / (2 * a));
    return polynomial;
}

void Matrix::addToList(Matrix A)
{
    matrixMenu.push_back(A);
    int nextNum;
    if (numMenu.size() == 0)
    {
        nextNum = 1;
    }
    else
    {
        nextNum = numMenu.size() + 1;
    }
    numMenu.push_back(nextNum);
}

Matrix Matrix::getMatrix(string number)
{
    cout << "Matrix " + number + ": Use matrix already entered?\n";
    bool existing = ynResponse();
    if (existing == true)
    {
        int position = vectorPosition(matrixMenu, numMenu);
        Matrix A = matrixMenu[position];
        return A;
    }
    else
    {
        cout << "Use matrix from file? Y/N:\n";
        existing = ynResponse();
        if (existing == true)
        {
            Matrix A = Matrix(" ");
            addToList(A);
            return A;
        }
        else
        {
            Matrix A = Matrix();
            addToList(A);
            return A;
        }
    }
}

string Matrix::longName(string shortName)
{
    string newName = "Matrices/" + shortName + ".txt";
    return newName;
}

vector<vector<double> > Matrix::wilkinson(double a, double b, double c)
{
    double delta = (a - c) / 2.0;
    double sign = 1.0;
    if (delta < 0)
    {
        sign = -1.0;
    }
    double mu = c - sign * b * b / (delta * sign + sqrt(delta * delta + b * b));
    return identity(mu);
}

vector<vector<double> > Matrix::identity(double factor)
{
    vector<double> tempRow;
    vector<vector<double> > tempMatrix;
    for (int i = 0; i < width; i++)
    {
        tempRow.push_back(0.0);
    }
    for (int i = 0; i < height; i++)
    {
        tempMatrix.push_back(tempRow);
    }
    for (int i = 0; i < height; i++)
    {
        tempMatrix[i][i] = factor;
    }
    return tempMatrix;
}

void Matrix::printEigen()
{
    for (int i = 0; i < height; i++)
    {
        cout << realEigen[i];
        if (imaginaryEigen[i] != 0)
        {
            cout << " + " << imaginaryEigen[i] << "i";
            i++;
            cout << endl << realEigen[i] << " - " << imaginaryEigen[i] << "i";
        }
        cout << endl;
    }
}
    
void Matrix::setMatrix(vector<vector<double> > toSet)
{
    vector<double> tempRow;
    vector<vector<double> > newMatrix;
    if (height != toSet.size())
    {
        height = toSet.size();
    }
    if ((height > 0) && (width != toSet[0].size()))
    {
        width = toSet[0].size();
    }
    matrix.clear();
    for (int i = 0; i < height; i++)
    {
        tempRow.clear();
        for (int j = 0; j < width; j++)
        {
            tempRow.push_back(toSet[i][j]);
        }
        matrix.push_back(tempRow);
    }
    return;
}

bool Matrix::compareQR()
{
    bool equal = true;
    for (int i = height - 1; i >= 0; i--)
    {
        for (int j = width - 1; j >= 0; j--)
        {
            if (matrix[i][j] != R[i][j])
            {
                equal = false;
                break;
            }
        }
    }
    return equal;
}