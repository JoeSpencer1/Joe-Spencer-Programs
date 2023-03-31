#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include "matrix.h"
using namespace std;

Matrix::Matrix(string fileName)
{
    // This segment reads in the matrix from a file. The matrix is written with its height first, then width, then the data.
    if ((fileName[0] != 'M') || (fileName[1] != 'a') || (fileName[2] != 't') || (fileName[3] != 'r') || (fileName[4] != 'i')
        || (fileName[5] != 'c') || (fileName[6] != 'e') || (fileName[7] != 's') || (fileName[8] != '/'))
    {
        fileName = longName(fileName);
    }
    if ((fileName[fileName.length() - 1] != 't') || (fileName[fileName.length() - 2] != 'x') 
        || (fileName[fileName.length() - 3] != 't') || (fileName[fileName.length() - 4] != '.'))
    {
        fileName = longName(fileName);
    }
    ifstream readMatrix(fileName);
    while (!readMatrix.is_open())
    {   
        cout << "Please enter the name of a matrix file contained in this folder.\n";
        cin >> fileName;
        fileName = longName(fileName);
        fileName = fileName;
        readMatrix.open(fileName);
    }
    checkValidity(readMatrix);
    readMatrix >> height;
    checkValidity(readMatrix);
    readMatrix >> width;
    checkValidity(readMatrix);
    double input;
    norm = 0;
    vector<double> row;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            checkValidity(readMatrix);
            if (!readMatrix.eof())
            {
                readMatrix >> input;
                if (input > 0)
                {
                    norm += input;
                }
                else
                {
                    norm -= input;
                }
                row.push_back(input);
            }
        }
        matrix.push_back(row);
        row.clear();
    }
    Q = matrix;
    R = matrix;
    E = matrix;
    readMatrix.close();
    name = fileName;
    publish = true;
}

Matrix::Matrix(int rows, int columns, vector<vector<double> > data)
{
    height = rows;
    width = columns;
    matrix = data;
    norm = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (matrix[i][j] > 0)
            {
                norm += matrix[i][j];
            }
            else
            {
                norm -= matrix[i][j];
            }
        }
    }
    if ((rows > 0) && (columns > 0))
    {
        name = longName(generateName());
    }
    Q = matrix;
    R = matrix;
    E = matrix;
    publish = true;
}

Matrix::Matrix(int rows, int columns, vector<vector<double> > data, bool keep)
{
    height = rows;
    width = columns;
    matrix = data;
    norm = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (matrix[i][j] > 0)
            {
                norm += matrix[i][j];
            }
            else
            {
                norm -= matrix[i][j];
            }
        }
    }
    Q = matrix;
    R = matrix;
    E = matrix;
    publish = false;
}

Matrix::Matrix()
{
    bool valid;
    string line;
    string strNum = "";
    vector<double> tempRow;
    do
    {
        norm = 0;
        valid = true;
        cout << "Please enter a height:\n";
        cin >> height;
        cout << "Please enter a width:\n";
        cin >> width;
        cout << "Please enter a valid " << height << "×" << width << " matrix,\n";
        cout << "separated by single spaces and enters:\n";
        getline(cin, line);
        line = "";
        for (int i = 0; i < height; i++)
        {
            cin.clear();
            getline(cin, line);
            for (auto j : line)
            {
                if (j == ' ')
                {
                    tempRow.push_back(stod(strNum));
                    if (stod(strNum) > 0)
                    {
                        norm += stod(strNum);
                    }
                    else
                    {
                        norm -= stod(strNum);
                    }
                    strNum = "";
                }
                else
                {
                    strNum.push_back(j);
                }
            }
            tempRow.push_back(stod(strNum));
            matrix.push_back(tempRow);
            if (tempRow.size() != width)
            {
                valid = false;
            }
            tempRow.clear();
            strNum = "";
        }
        if (valid == false)
        {
            cout << "This is not a valid matrix.\n";
        }
    }
    while (valid == false);
    if ((height > 0) && (width > 0))
    {
        name = longName(generateName());
    }
    Q = matrix;
    R = matrix;
    E = matrix;
    publish = true;
}

Matrix::~Matrix()
{
    // This segment publishes the matrix and then deletes it
    if (publish == true)
    {
        publishFile();
    }
    matrix.clear();
/*    Q.clear();
    R.clear();
    E.clear();*/
    realEigen.clear();
    imaginaryEigen.clear();
    realEigenVectors.clear();
    matrixMenu.clear();
};

Matrix Matrix::add(Matrix Other)
{
    if ((Other.getWidth() != width) || (Other.getHeight() != height))
    {
        cout << "These matrices have different dimensions and cannot be added.\n";
        vector<vector<double> > blankMatrix;
        return Matrix(0, 0, blankMatrix);
    }
    else
    {
        vector<double> tempRow;
        vector<vector<double> > tempMatrix;
        vector<vector<double> > toAdd = Other.getMatrix();
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j ++)
            {
                tempRow.push_back(matrix[i][j] + toAdd[i][j]);
            }
            tempMatrix.push_back(tempRow);
            tempRow.clear();
        }
        return Matrix(getHeight(), getWidth(), tempMatrix);
    }
}

Matrix Matrix::subtract(Matrix Other)
{
    if ((Other.getWidth() != width) || (Other.getHeight() != height))
    {
        cout << "These matrices have different dimensions and cannot be added.\n";
        vector<vector<double> > blankMatrix;
        return Matrix(0, 0, blankMatrix);
    }
    else
    {
        vector<double> tempRow;
        vector<vector<double> > tempMatrix;
        vector<vector<double> > toAdd = Other.getMatrix();
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j ++)
            {
                tempRow.push_back(matrix[i][j] - toAdd[i][j]);
            }
            tempMatrix.push_back(tempRow);
            tempRow.clear();
        }
        return Matrix(getHeight(), getWidth(), tempMatrix);
    }
}

Matrix Matrix::scale(double constant)
{
    vector<double> tempRow;
    vector<vector<double> > tempMatrix;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tempRow.push_back(matrix[i][j] * constant);
        }
        tempMatrix.push_back(tempRow);
        tempRow.clear();
    }
    return Matrix(getHeight(), getWidth(), tempMatrix);
}

Matrix Matrix::cross(Matrix Other, bool keep)
{
    bool compatiable = checkCompatibility(Other);
    double tempNum = 0;
    vector<vector<double> > toCross = Other.getMatrix();
    vector<vector<double> > newMatrix;
    vector<double> tempRow;
    if (compatiable == true)
    {
        if (getWidth() == 1)
        {
            if (height == 1)
            {
                tempNum = matrix[0][0] * toCross[0][0];
                if ((tempNum > accuracy) || (tempNum < 0 - accuracy))
                { 
                    tempRow.push_back(tempNum);
                }
                else
                {
                    tempRow.push_back(0);
                }
                newMatrix.push_back(tempRow);
                tempRow.clear();
            }
            if (height == 2)
            {
                tempNum = matrix[0][0] * toCross[1][0];
                if ((tempNum > accuracy) || (tempNum < 0 - accuracy))
                { 
                    tempRow.push_back(tempNum);
                }
                else
                {
                    tempRow.push_back(0);
                }
                newMatrix.push_back(tempRow);
                tempRow.clear();
                tempRow.push_back(matrix[1][0] * toCross[0][0]);
                newMatrix.push_back(tempRow);
            }
            else
            {
                tempRow.push_back(matrix[getHeight() - 2][0] * toCross[getHeight() - 1][0]);
                newMatrix.push_back(tempRow);
                tempRow.clear();
                tempRow.push_back(matrix[getHeight() - 1][0] * toCross[0][0]);
                newMatrix.push_back(tempRow);
                tempRow.clear();
                for (int i = 0; i < getHeight() - 2; i++)
                {
                    tempRow.push_back(matrix[i + 1][0] * toCross[i + 2][0]);
                    newMatrix.push_back(tempRow);
                    tempRow.clear();
                }
                for (int i = 0; i < getHeight() - 2; i++)
                {
                    newMatrix[i][0] -= matrix[i + 2][0] * toCross[i + 1][0];
                }
                newMatrix[height - 2][0] -= matrix[0][0] * toCross[height - 1][0];
                newMatrix[height - 1][0] -= matrix[1][0] * toCross[0][0];
            }
        }
        else
        {
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < Other.getWidth(); j++)
                {
                    tempNum = 0;
                    for (int k = 0; k < width; k++)
                    {
                        tempNum += (matrix[i][k] * toCross[k][j]);
                    }
                    tempRow.push_back(tempNum);
                }
                newMatrix.push_back(tempRow);
                tempRow.clear();
            }
        }
        if (keep == true)
        {
            publishNew(getHeight(), Other.getWidth(), newMatrix);
        }
        return Matrix(newMatrix.size(), newMatrix[0].size(), newMatrix);
    }
    else 
    {
        cout << "These matrices are not compatible.\n";
        return Matrix(0, 0, newMatrix);
    }
}

double Matrix::characteristic()
{
    if (height == width)
    {
        double characteristic = 1.0;
        double subtract = 1.0;
        for (int i = 0; i < height; i++)
        {
            characteristic *= matrix[i][i];
            subtract *= matrix[i][height - i - 1];
        }
        characteristic = characteristic - subtract;
        return characteristic;
    }
    else
    {
        cout << "This is not a square matrix.\n";
        return 0;
    }
}

Matrix Matrix::householder()
{
    vector<double> v;
    vector<vector<double> > A;
    vector<vector<double> > Q;
    vector<vector<double> > QA;
    double alph;
    double r;
    double entry;
    A = matrix;
    Q = matrix;
    QA = matrix;
    for (int i = 0; i < height - 2; i++)
    {
        alph = 0;
        v.clear();
        for (int j = i + 1; j < height; j++)
        {
            alph += A[j][i] * A[j][i];
        }
        alph = sqrt(alph);
        if (A[i + 1][i] > 0)
        {
            alph *= -1.0;
        }
        r = sqrt(0.5 * (alph * alph - alph * A[i + 1][i]));
        for (int j = 0; j < height; j++)
        {
            v.push_back(0);
        }
        v[i + 1] = (A[i + 1][i] - alph) / (2 * r);
        for (int j = i + 2; j < height; j++)
        {
            v[j] = A[j][i] / (2.0 * r);
        }
        for (int j = 0; j < height; j++)
        {
            for (int k = 0; k <= j; k++)
            {
                Q[j][k] = v[j] * v[k] * -2.0;
                if (j == k)
                {
                    Q[j][k] += 1;
                }
                else
                {
                    Q[k][j] = Q[j][k];
                }
            }
        }
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < height; k++)
            {
                entry = 0;
                for (int l = 0; l < width; l++)
                {
                    entry += Q[j][l] * A[l][k];
                }
                QA[j][k] = entry;
            }
        }
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < height; k++)
            {
                entry = 0;
                for (int l = 0; l < width; l++)
                {
                    entry += QA[j][l] * Q[l][k];
                }
                A[j][k] = entry;
            }
        }
    }
    return Matrix(height, width, A, false);
}

Matrix Matrix::invert()
{
    double scale;
    vector<vector<double> > inverse;
    vector<vector<double> > solve;
    vector<double> tempVector;
    vector<double> row;
    Matrix publish = Matrix(0, 0, inverse);
    if (invertible() == false)
    {
        return publish;
    }
    else
    {
        for (int i = 0; i < matrix.size(); i++)
        {
            for (int j = 0; j < matrix[0].size(); j++)
            {
                tempVector.push_back(matrix[i][j]);
                if (j == i)
                {
                    row.push_back(1);
                }
                else
                {
                    row.push_back(0);
                }
            }
            solve.push_back(tempVector);
            tempVector.clear();
            inverse.push_back(row);
            row.clear();
        }
        for (int i = 0; i < solve.size(); i++)
        {
            int j = i + 1;
            while (solve[i][i] == 0)
            {
                if (j == solve.size())
                {
                    cout << "This matrix is not invertible.\n";
                    inverse.clear();
                    return Matrix(0, 0, inverse);
                }
                switchRows(solve, i, j);
                switchRows(inverse, i, j);
                j++;
            }
            scale = solve[i][i];
            for (int j = 0; j < solve.size(); j++)
            {
                solve[i][j] /= scale;
                inverse[i][j] /= scale;
            }
            for (int j = i + 1; j < solve.size(); j++)
            {
                scale = solve[j][i];
                for (int k = 0; k < solve.size(); k++)
                {
                    solve[j][k] -= solve[i][k] * scale;
                    inverse[j][k] -= inverse[i][k] * scale;
                }
            }
        }
        for (int i = solve.size() - 1; i >= 0; i--)
        {
            for (int j = 0; j < i; j++)
            {
                scale = solve[j][i];
                solve[j][i] -= solve[i][i] * scale;
                for (int k = 0; k < solve.size(); k++)
                {
                    inverse[j][k] -= inverse[i][k] * scale;
                }
            }
        }
        publish = Matrix(inverse.size(), inverse[0].size(), inverse);
    }
    solve.clear();
    row.clear();
    inverse.clear();
    return publish;
}

Matrix Matrix::transpose()
{
    vector<vector<double> > transpose;
    vector<double> tempRow;
    for (int i = 0; i < height; i++)
    {
        transpose.push_back(tempRow);
        for (int j = 0; j < width; j++)
        {
            transpose[i].push_back(matrix[j][i]);
        }
    }
    Matrix publish = Matrix(transpose.size(), transpose[0].size(), transpose);
    return publish;
}

double Matrix::determinant()
{
    vector<double> temp;
    vector<vector<double> > tempMatrix;
    if (height == width)
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                temp.push_back(matrix[i][j]);
            }
            tempMatrix.push_back(temp);
            temp.clear();
        }
        return determinant(tempMatrix);
    }
    else
    {
        cout << "This matrix is not square and does not have a determinant.\n";
        return 0;
    }
}

double Matrix::determinant(vector<vector<double> > toDetermine)
{
    vector<vector<double> > nextDetermine;
    double temp;
    double factor;
    double determinant = 1;
    int counter;
    if (toDetermine[0].size() == 1)
    {
        determinant = toDetermine[0][0];
    }
    else if (toDetermine[0].size() == 2)
    {
        determinant = toDetermine[0][0] * toDetermine[1][1] - toDetermine[0][1] * toDetermine[1][0];
    }
    else
    {
        for (int i = 0; i < toDetermine.size(); i++)
        {
            if (toDetermine[i][i] == 0)
            {
                counter = i + 1;
                while ((toDetermine[i][counter] == 0) && (counter < width - 1))
                {
                    counter ++;
                }
                switchRows(toDetermine, i, counter);
            }
            else
            {
                for (int j = i + 1; j < height; j++)
                {
                    factor = toDetermine[j][i] / toDetermine[i][i];
                    for (int k = i; k < width; k++)
                    {
                        toDetermine[j][k] -= factor * toDetermine[i][k];
                    }
                }
            }
        }
        for (int i = 0; i < height; i++)
        {
            determinant *= toDetermine[i][i];
        }
    }
    return determinant;
}

void Matrix::solve(Matrix solution)
{
    if (getHeight() != solution.getHeight())
    {
        cout << "This matrix is a different height from the solution vector.\n";
        return;
    }
    else
    {
        Matrix inverse = invert();
        if (inverse.getHeight() == 0)
        {
            cout << "This equation cannot be solved.\n";
        }
        else
        {
            Matrix answer = inverse.cross(solution, true);
            answer.printMatrix();
        }
    }
}

double Matrix::trace()
{
    double trace = 0;
    if (width == height)
    {
        for (int i = 0; i < height; i++)
        {
            trace += matrix[i][i];
        }
    }
    else
    {
        cout << "This matrix is not square.\n";
    }
    return trace;
}

void Matrix::eigenValues()
{
    Matrix A = Matrix(height, width, matrix);
    vector<double> poly;
    double real = 0;
    double imaginary = 0;
    QR(height - 1, A);
    for (int i = 0; i < height; i++)
    {
        if ((i < height - 1) && ((E[i + 1][i] > accuracy) || (E[i + 1][i] < 0 - accuracy)))
        {
            poly = polynomial(i);
            real = poly[0];
            imaginary = poly[1];
            realEigen.push_back(real);
            imaginaryEigen.push_back(imaginary);
            realEigen.push_back(real);
            imaginaryEigen.push_back(imaginary);
            i++;
        }
        else
        {
            real = E[i][i];
            imaginary = 0;
            realEigen.push_back(real);
            imaginaryEigen.push_back(imaginary);
        }
    }
    return;
}

void Matrix::QR(int n, Matrix Ea)
{
    if (compareQR() == false)
    {
        return;
    }
    if (height == 1)
    {
        eigen1x1();
        return;
    }
    if (height == 2)
    {
        eigen2x2();
        return;
    }
    double length = 0;
    double temDep = 0;
    double error = 0;
    double temError = 0;
    vector<vector<double> > tempQ;
    vector<vector<double> > mu;
    tempQ = Ea.getMatrix();
    if ((n > 0) && (((tempQ[n - 1][n] > accuracy) || (tempQ[n - 1][n] < (0 - accuracy))) && (((tempQ[n - 1][n] - tempQ[n][n - 1]) < accuracy) && ((tempQ[n - 1][n] - tempQ[n][n - 1]) > 0 - accuracy))))
    {
        mu = wilkinson(tempQ[n - 1][n - 1], tempQ[n - 1][n], tempQ[n][n]);
    }
    else
    {
        mu = identity(tempQ[n][n]);
    }
    Matrix muvec = Matrix(height, width, mu);
    mu.clear();
    tempQ.clear();
    Matrix tQ = Ea.subtract(muvec);
    tempQ = tQ.getMatrix();
    // For each column, you need to find the perpendicular component.
    for (int i = 0; i < width; i++)
    {
    /*
    Steps: 1: Dot the column with previous columns and subtract them to 
    isolate linearly independent columns. 2: Subtract dependant columns to form orthogonal
    basis for Q. 3: Find the new magnitude of Q and normalize it. 4: Determine value of R
    by R=Q'A. 5: Find next value of A by A'=RQ
    */
        for (int j = 0; j < i; j++) // Cylces through previous columns
        {
            temDep = 0;
            for (int k = 0; k < height; k++) // Find dot product with previous column
            {
                temDep += tempQ[k][j] * tempQ[k][i];
            }
            // Step 2: Subtract dependant columns
            for (int k = 0; k < height; k++)
            {
                tempQ[k][i] -= temDep * tempQ[k][j];
            }
        }
        // Step 3: Find new magnitude of Q and normalize
        length = 0;
        for (int j = 0; j < height; j++)
        {
            length += tempQ[j][i] * tempQ[j][i];
        }
        length = sqrt(length);
        for (int j = 0; j < height; j++)
        {
            tempQ[j][i] /= length;
        }
    }
    // Step 4: Find R by E=QR->R=Q'E
    Matrix Qa = Matrix(height, width, tempQ);
    Matrix Qt = Qa.transpose();
    Matrix Et = Ea.subtract(muvec);
    Matrix Ra = Qt.cross(Et, false);
    Matrix Eb = Ra.cross(Qa , false).add(muvec);
    // This section checks if the matrix has been solved within the tolerance.
    for (int i = 0; i < height; i++)
    {
        tempQ[i].clear();
    }
    tempQ.clear();
    for (int i = 0; i < height; i++)
    {
        if ((i < (height - 1)) && ((Eb.getMatrix()[i + 1][i] * Eb.getMatrix()[i + 1][i]) > (accuracy * accuracy)))
        {
            double a = Ea.getMatrix()[i][i + 1] * Ea.getMatrix()[i + 1][i];
            double b = Eb.getMatrix()[i][i + 1] * Eb.getMatrix()[i + 1][i];
            temError = (a - b) * (a - b);
            temError = temError * temError;
            i++;            
        }
        else
        {
            temError = (Eb.getMatrix()[i][i] - Ea.getMatrix()[i][i]) * (Eb.getMatrix()[i][i] - Ea.getMatrix()[i][i]);
        }
        error += temError;
        if (temError > accuracy)
        {
            error += temError * height;
        }
    }
    if (error < accuracy * accuracy * accuracy * height)
    {
        n--;
        error = 0;
        for (int i = 0; i < height - 1; i++)
        {
            error += Eb.getMatrix()[i + 1][i] * Eb.getMatrix()[i + 1][i];
        }
        if (error < accuracy)
        {
            n = 0;
        }
        if (n < 1)
        {
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    Q[i][j] = Qa.getMatrix()[i][j];
                }
            }
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    R[i][j] = Ra.getMatrix()[i][j];
                }
            }
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    E[i][j] = Ea.getMatrix()[i][j];
                }
            }
            return;
        }
    }
    QR(n, Eb);
}

void Matrix::eigen1x1()
{
    realEigen.push_back(matrix[0][0]);
    imaginaryEigen.push_back(0);
    vector<double> eigenVec;
    eigenVec.push_back(1.0);
    realEigenVectors.push_back(eigenVec);
}

void Matrix::eigen2x2()
{
    double real;
    double imaginary;
    double a = 1.0;
    double b = -1 * (E[0][0] + E[1][1]);
    double c = (E[0][0] * E[1][1]) - (E[0][1] * E[1][0]);
    vector<double> eigenVec;
    real = -1 * b / (2 * a);
    imaginary = (b * b - 4 * a * c) / ((2 * a) * (2 * a));
    if (imaginary < 0)
    {
        imaginary = -1 * sqrt(-1 * imaginary);
    }
    else
    {
        imaginary = sqrt(imaginary);
    }
    if (imaginary > 0)
    {
        realEigen.push_back(real + imaginary);
        realEigen.push_back(real - imaginary);
        imaginaryEigen.push_back(0);
        imaginaryEigen.push_back(0);
    }
    else
    {
        imaginary *= -1;
        realEigen.push_back(real);
        realEigen.push_back(real);
        imaginaryEigen.push_back(imaginary);
        imaginaryEigen.push_back(0 - imaginary);
    }
}

void Matrix::eigenVecs()
{
    /*
    You can set up 2 equations, with a as the real portion and b as the
    imaginary portion, so λ=a+bi. They are:
    (A-aI)yr-bIyi=0, (A-aI)yi+bIyr=0
    If you make a 2n*2n matrix you can solve this. It looks like this:

    [A-aI     -bi] [vr ] =  [0]
    [b    (A-aI)i] [vii]    [0]   This means that

    [A-aI     b ] [vr]  =  [0] 
    [b      A-ai] [vi]     [0]    or:

    [b      A-ai] [vi]  =  [0]
    [A-ai      b] [vr]     [0]
    */
    vector<vector<double> > BaaB;
    vector<double> rEigenv;
    vector<double> cEigenv;
    vector<double> tempRow;
    vector<int> order;
    int row;
    int bottom;
    double factor1;
    double factor2;
    for (int i = 0; i < height; i++)
    {
        // Construct BaaB matrix
        for (int j = 0; j < (height * 2); j++)
        {
            order.push_back(j);
            for (int k = 0; k < (width * 2); k++)
            {
                order.push_back(k);
                if (((j < height) && (k < height)) || ((j >= height) && (k >= height)))
                {
                    if ((k == j) || (k == j + width))
                    {
                        tempRow.push_back(imaginaryEigen[i]);
                    }
                    else
                    {
                        tempRow.push_back(0);
                    }
                }
                else
                {
                    if (k < width)
                    {
                        tempRow.push_back(matrix[j - width][k]);
                    }
                    else
                    {
                        tempRow.push_back(matrix[j][k - width]);
                    }
                    if ((k == (j - width)) || (k == (j + width)))
                    {
                        tempRow[k] -= realEigen[i];
                    }
                }
            }
            BaaB.push_back(tempRow);
            tempRow.clear();
        }
//cout<<"BaaB:\n"; for (int i = 0; i < BaaB.size(); i++){for (int j = 0; j < BaaB.size(); j++){cout<<int(BaaB[i][j])<<" ";}cout<<"\n";}cout<<"\n";
        // Rearrange BaaB so it does not have any zero entries down its main diagonal.
        if (imaginaryEigen[i] == 0)
        {
            for (int j = 0; j < height; j++)
            {
                tempRow = BaaB[j];
                for (int k = 0; k < width * 2; k++)
                {
                    BaaB[j][k] = BaaB[j + height][k];
                }
                BaaB[j + height] = tempRow;
                order[j] = j + height;
                order[j + height] = j;
            }
            for (int j = 0; j < height - 1; j++)
            {
                if (BaaB[j][j] == 0)
                {
                    for (int k = j + 1; k < height; k++)
                    {
                        if (BaaB[k][j] != 0)
                        {
                            tempRow = BaaB[j];
                            BaaB[j] = BaaB[k];
                            BaaB[j + height] = BaaB[k];
                            BaaB[k] = tempRow;
                            BaaB[k + height] = tempRow;
                            row = order[j];
                            order[j] = order[k];
                            order[j + height] = order[k];
                            order[k] = row;
                            order[k + height] = row;
                        }
                    }
                }
            }
        }
//cout<<"BaaB2:\n"; for (int i = 0; i < BaaB.size(); i++){for (int j = 0; j < BaaB.size(); j++){cout<<int(BaaB[i][j])<<" ";}cout<<"\n";}
        // Algebraically solve BaaB
        for (int j = 0; j < (height * 2); j++)
        {
            factor1 = BaaB[j][j];
//cout<<"1:" <<factor1 << endl;
            if ((factor1 < (0 - tolerance)) || (factor1 > tolerance))
            {
                for (int k = (j + 1); k < (height * 2); k++)
                {
                    factor2 = BaaB[k][j] / factor1;
//cout<<"2: "<< factor2 << endl;
                    for (int l = 0; l < (width * 2); l++)
                    {
                        BaaB[k][l] -= BaaB[j][l] * factor2;
                    }
                }
            }
        }
cout<<"BaaB3:\n"; for (int i = 0; i < BaaB.size(); i++){for (int j = 0; j < BaaB.size(); j++){cout<<int(BaaB[i][j])<<" ";}cout<<"\n";}
        // Set bottom entry to 1 unless it is zero. If it is zero, cancel it and find others.
        bottom = height * 2 - 1;
        while ((BaaB[bottom][bottom] > tolerance) || (BaaB[bottom][bottom] < (0 - tolerance)))
        {
            for (int j = 0; j < (height * 2); j++)
            {
                BaaB[j][bottom] = 0;
            }
            bottom --;
        }
        // Put in value for starting eigenvectors
        rEigenv.clear();
        cEigenv.clear();
        for (int j = 0; j < height; j++)
        {
            rEigenv.push_back(0);
            cEigenv.push_back(0);
        }
        // Set bottom entry of eigenvector to one
/*        if (bottom < height)
        {
            cEigenv[bottom] = 1.0;
        }
        else
        {
            rEigenv[bottom - height] = 1;
        }
        for (int j = bottom - 1; j >= 0; j--)
        {
            if (j >= height)
            {
                rEigenv[j % height] = -1 * BaaB[j][bottom];
            }
            else
            {
                cEigenv[j] = -1 * BaaB[j][bottom];
            }
*/        }
        // Algebraically solve for other entries of eigenvector.
        for (int j = bottom - 1; j >= 0; j--)
        {
            factor1 = BaaB[j][j];
            for (int k = j - 1; k >= 0; k--)
            {
                if (k >= height)
                {
                    rEigenv[k % height] = -1 * BaaB[j][k];
                }
                else
                {
                    cEigenv[k] = -1 * BaaB[j][k];
                }
            }
        }
        realEigenVectors.push_back(rEigenv);
        imaginaryEigenVectors.push_back(cEigenv);
        tempRow.clear();
        order.clear();
        rEigenv.clear();
        cEigenv.clear();
        BaaB.clear();
    }
    return;
}