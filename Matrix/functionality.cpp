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
        number = intEntry();
        for (int i = 0; i <= numMenu.size(); i ++)
        {   
            if (number > numMenu.size())
            {
                cout << "Current matrix menu length is " << numMenu.size() << endl;
            }
            else if (number == 0)
            {
                return -1;
            }
            else if (numMenu[i] == number)
            {
                position = i;
                found = true;
                cout << "Matrix " << numMenu[i] << " successfully found.\n";
            }
        }
        if (found == false)
        {
            cout << "Sorry, there is not any matrix at this index.\n";
            cout << "Press 0 to escape.\n";
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
    string response;
    while (1)
    {
        cout << "Please enter a response Y/N:\n";
        cin >> response;
        if ((response == "Y") || (response == "y"))
        {
            return true;
        }
        if ((response == "N") || (response == "n"))
        {
            return false;
        }
        cout << "Invalid response.\n";
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
        cout << "Enter 0 to escape.\n";
        cin >> name;
        name = longName(name);
        test.open(name);
        if (name == "Matrix0.txt")
        {
            return name;
        }
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
        cout << "Width != height\n";
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

double Matrix::determinant()
{
    vector<double> temp;
    vector<vector<double> > tempMatrix;
    vector<int> rem;
    if (height == width)
    {
        if (height == 1)
        {
            return matrix[0][0];
        }
        for (int i = 0; i < height; i++)
        {
            rem.push_back(i);
        }
        return determinant(rem);
    }
    else
    {
        cout << "This matrix is not square and does not have a determinant.\n";
        return 0;
    }
}

vector<vector<double> > Matrix::polynomial(int row)
{
    vector<vector<double> > polynomial;
    vector<double> temp;
    polynomial.push_back(temp);
    polynomial.push_back(temp);
    double real;
    double imaginary;
    double a = 1;
    double b = -1 * (E[row][row] + E[row + 1][row + 1]);
    double c = E[row][row] * E[row + 1][row + 1] - E[row + 1][row] * E[row][row + 1];
    if ((b * b - 4 * a * c) < 0)
    {
        polynomial[0].push_back(-1 * b / (2 * a));
        polynomial[1].push_back(sqrt(4 * a * c - b * b) / (2 * a));
        polynomial[0].push_back(-1 * b / (2 * a));
        polynomial[1].push_back(-1 * sqrt(4 * a * c - b * b) / (2 * a));
    }
    else
    {
        polynomial[0].push_back((-1 * b + sqrt(b * b - 4 * a * c)) / (2 * a));
        polynomial[1].push_back(0);
        polynomial[0].push_back((-1 * b - sqrt(b * b - 4 * a * c)) / (2 * a));
        polynomial[1].push_back(0);
    }
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

Matrix Matrix::retreiveMatrix(string number)
{
    cout << "Matrix " + number + ": Use matrix already entered?\n";
    bool existing = ynResponse();
    if (existing == true)
    {
        int position = vectorPosition(matrixMenu, numMenu);
        if (position < 0)
        {
            cout << "Exiting.\n";
            vector<vector<double> > blankMatrix;
            return Matrix(0, 0, blankMatrix);
        }
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
            cout << endl << realEigen[i] << " - " << imaginaryEigen[i] << "i";
            i++;
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

void Matrix::printEigenVec()
{
    for (int i = 0; i < height; i++)
    {
        cout << (i + 1) << ": \n";
        for (int j = 0; j < height; j++)
        {
            if ((realEigenVectors[i][j] > tolerance) || (realEigenVectors[i][j] < (0 - tolerance))
            || (imaginaryEigenVectors[i][j] == 0))
            {
                if ((realEigenVectors[i][j] < 0) && (realEigenVectors[i][j] > (0 - tolerance)))
                {
                    realEigenVectors[i][j] = 0 - realEigenVectors[i][j];
                }
                cout << realEigenVectors[i][j];
            }
            if ((imaginaryEigenVectors[i][j] > tolerance) || (imaginaryEigenVectors[i][j] < (0 - tolerance)))
            {
                if ((realEigenVectors[i][j] > tolerance) || (realEigenVectors[i][j] < (0 - tolerance)))
                {
                    if (imaginaryEigenVectors[i][j] > 0)
                    {
                        cout << " + " << imaginaryEigenVectors[i][j] << "i";
                    }
                    else
                    {
                        cout << " - " << (-1 * imaginaryEigenVectors[i][j]) << "i";
                    }
                }
                else
                {
                    cout << imaginaryEigenVectors[i][j] << "i";
                }
            }
            cout << endl;
        }
        cout << endl;
    }
}

int Matrix::strChoice()
{
    string sc;
    int choice;
    cin >> sc;
    if ((sc == "0") || (sc == "1") || (sc == "2") || (sc == "3") || (sc == "4")
        || (sc == "5") || (sc == "6") || (sc == "7") || (sc == "8") || (sc == "9")
        || (sc == "10") || (sc == "11") || (sc == "12") || (sc == "13") || (sc == "1"))
    {
        choice = stoi(sc);
    }
    else if ((sc == "X") || (sc == "x"))
    {
        choice = -1;
    }
    else
    {
        cout << "Please enter a valid option number 0-14, or the X key.\n";
        choice = strChoice();
    }
    return choice;
}

int Matrix::intEntry()
{
    string num;
    char n;
    bool isNum = true;
    int retNum;
    // Make sure number entry is actually an integer
    do
    {
        cin >> num;
        for (int i = 0; i < num.size(); i++)
        {
            n = num[i];
            if ((n != '0') && (n != '1') && (n != '2') && (n != '3') && (n != '4')
                && (n != '5') && (n != '6') && (n != '7') && (n != '8') && (n != '9'))
            {
                isNum = false;
            }
        }
        if (isNum == false)
        {
            cout << "Please enter a number\n";
            cin.clear();
            cin.ignore(INT_MAX);
        }
        cin.clear();
    }
    while (isNum == false);
    retNum = stoi(num);
    return retNum;
}

double Matrix::decEntry()
{
    string num;
    string n;
    int counter = 0;
    bool isNum = true;
    double retNum;
    // Make sure number entry is actually an integer
    do
    {
        cin >> num;
        for (int i = 0; i < n.length(); i++)
        {
            n = num[i];
            if ((n != "0") && (n != "1") && (n != "2") && (n != "3") && (n != "4")
                && (n != "5") && (n != "6") && (n != "7") && (n != "8") && (n != "9"))
            {
                if ((n == ".") && (counter < 1))
                {
                    counter ++;
                }
                else
                {
                    isNum = false;
                }
            }
        }
        if (isNum == false)
        {
            cout << "Please enter a number\n";
            cin.clear();
            cin.ignore(INT_MAX);
        }
    }
    while (isNum == false);
    retNum = stod(num);
    return retNum;
}

vector<vector<double> > Matrix::returnEigen()
{
    eigenValues();
    vector<vector<double> > eigenV;
    eigenV.push_back(realEigen);
    eigenV.push_back(imaginaryEigen);
    return(eigenV);
}

vector<vector<double> > Matrix::normalizeBottom(vector<double> real, vector<double> imag)
{
    int last = real.size();
    double rEnd = 0;
    double iEnd = 0;
    double rEnt;
    double iEnt;
    double factor;
    vector<vector<double> > newVecs;
    while ((rEnd == 0) && (iEnd == 0))
    {
        last --;
        rEnd = real[last];
        iEnd = imag[last] * -1;
    }
    if (iEnd != 0)
    {
        // Multiply each entry by the factor you found
        for (int i = 0; i <= last; i++)
        {
            rEnt = real[i] * rEnd - imag[i] * iEnd;
            iEnt = real[i] * iEnd + imag[i] * rEnd;
            real[i] = rEnt;
            imag[i] = iEnt;
        }
    }
    factor = real[last];
    for (int i = 0; i <= last; i++)
    {
        real[i] /= factor;
        imag[i] /= factor;
    }
    newVecs.push_back(real);
    newVecs.push_back(imag);
    return (newVecs);
}

vector<vector<double> > Matrix::multiplicity(vector<vector<double> > BaaB, int n)
{
    vector<vector<double> > BaaB2 = BaaB;
    vector<vector<double> > BaaB3 = BaaB;
    double entry;
    for (int i = 0; i < n; i++)
    {
        if ((realEigen[i] == realEigen[n]) && (imaginaryEigen[i] == imaginaryEigen[n]))
        {
            for (int j = 0; j < width; j++)
            {
                for (int k = 0; k < height; k++)
                {
                    entry = 0;
                    // Multiply real components
                    for (int l = height; l < height * 2; l++)
                    {
                        entry += BaaB2[l][j] * BaaB2[k][l];
                    }
                    // Multiply imaginary components
                    for (int l = 0; l < width; l++)
                    {
                        entry -= BaaB2[l][j] * BaaB2[k][l];
                    }
                    BaaB3[k][j + width] = entry;
                    BaaB3[k + height][j] = entry;
                    entry = 0;
                    // Multiply real and imaginary components
                    for (int l = 0; l < width; l++)
                    {
                        entry += BaaB2[l][j] * BaaB2[k][l + width];
                        entry += BaaB2[l + width][j] * BaaB2[k][l];
                    }
                    BaaB3[k][j] = entry;
                    BaaB3[k + height][j + width] = entry;
                }
            }
        }
        BaaB2 = BaaB3;
    }
    return BaaB2;
}

int Matrix::numPrev(int n)
{
    int num = 0;
    for (int i = 0; i < n; i++)
    {
        if ((realEigen[i] == realEigen[n]) && (imaginaryEigen[i] == imaginaryEigen[n]))
        {
            num ++;
        }
    }
    return num;
}

bool Matrix::diagonal(vector<vector<double> > BaaB)
{
    bool diag = false;
    for (int i = 0; i < width; i++)
    {
        if ((BaaB[i][i] == 0) && (BaaB[i + width][i] == 0))
        {
            diag = true;
        }
    }
    return diag;
}