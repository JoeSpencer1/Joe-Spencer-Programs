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
    fileName = longName(fileName);
    ifstream readMatrix(fileName);
    while (!readMatrix.is_open())
    {
        cout << "Please enter the name of a matrix file contained in this folder.\n";
        cin >> fileName;
        fileName = longName(fileName);
        readMatrix.open(fileName);
    }
    checkValidity(readMatrix);
    readMatrix >> height;
    checkValidity(readMatrix);
    readMatrix >> width;
    checkValidity(readMatrix);
    double input;
    size = 0;
    vector<double> row;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            checkValidity(readMatrix);
            if (!readMatrix.eof())
            {
                readMatrix >> input;
                size += input * input;
                row.push_back(input);
            }
        }
        matrix.push_back(row);
        row.clear();
    }
    size = sqrt(size);
    readMatrix.close();
    name = fileName;
}

Matrix::Matrix(int rows, int columns, vector<vector<double> > data)
{
    height = rows;
    width = columns;
    matrix = data;
    size = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            size += matrix[i][j];
        }
    }
    size = sqrt(size);
    if ((rows > 0) && (columns > 0))
    {
        name = longName(generateName());
    }
}

Matrix::Matrix()
{
    bool valid;
    string line;
    string strNum = "";
    vector<double> tempRow;
    do
    {
        size = 0;
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
                    size += stod(strNum) * stod(strNum);
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
    size = sqrt(size);
    if ((height > 0) && (width > 0))
    {
        name = longName(generateName());
    }
}

Matrix::~Matrix()
{
    // This segment publishes the matrix and then deletes it
    publishFile();
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

Matrix Matrix::cross(Matrix Other)
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
        publishNew(getHeight(), Other.getWidth(), newMatrix);
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
        cout << "Inverse published as " << publish.getName();
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
    cout << "Transpose published as " << publish.getName();
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
            Matrix answer = inverse.cross(solution);
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
    vector<Matrix> QRf = QR();
    Matrix Q = QRf[0];
    Matrix R = QRf[1];
    cout << "R:\n";
    R.printMatrix();
    cout << "\nQ:\n";
    Q.printMatrix();
    return;
}

vector<Matrix> Matrix::QR()
{
cout << "1\n";
    double length = 0;
    double inDep = 0;
    double temDep = 0;
    double dotProduct = 0;
    double error = 0;
    vector<vector<double> > tempQ;
    vector<vector<double> > tempR;
    vector<vector<double> > tempE;
    vector<vector<double> > oldE;
    vector<double> tempRow;
    vector<double> tempCol;
    vector<Matrix> QRf;
    // Get temporary Q and R matrices and E matrix.
    if (Q.size() == 0)
    {
        E.push_back(Matrix(height, width, matrix));
    }
    else
    {
        E.push_back(R[R.size() - 1].cross(Q[Q.size() - 1]));
    }
    tempE = E[E.size() - 1].getMatrix();
    for (int i = 0; i < width; i++)
    {
        tempRow.push_back(0);
    }
    for (int i = 0; i < width; i++)
    {
        tempQ.push_back(tempRow);
        tempR.push_back(tempRow);
    }
cout << "2\n";
    // For each column, you need to find the perpendicular component.
    for (int i = 0; i < width; i++)
    {
        /*
        This loop needs to find the values of each entry in the Q and R matrices.
        The columns in the Q matrix are normalized, and the entries in the R matrix are
        the component magnitude in the direction of the Q matrix. The first row in the
        R matrix corresponds to the first column in the Q matrix. After the Q and R
        matrices are created, the next matrix is found by E=QR -> E_next=RQ.
        The steps for this are: 1: Find the column magnitude. 2: Dot the column with
        previous columns to find the magnitude in their directions. 3: Add these 
        magnitudes to the R matrix and subtract from the column. 4: Find magnitude of
        what remainins of the column, normalize column, and add final entry to R matrix.
        5: Cross-multiply R*Q to obtain next E.
        */
        length = 0;
        inDep = 0;
        // Step 1: This loop finds the length of the column.
        for (int j = 0; j < height; j++)
        {
            length += tempE[j][i] * tempE[j][i];
        }
        length = sqrt(length);
        inDep = length;
        size += length;
        // Step 2: Find magnitude in directions of previous columns
cout << "3\n";
        for (int j = 0; j < i; j++)
        {
            temDep = 0;
            for (int k = 0; k < height; k++)
            {
                temDep += tempQ[k][j] * tempE[k][i];
                tempCol[k] = tempE[k][i];
            }
            // Setp 3: Add these magnitudes to the R matrix
            temDep = sqrt(temDep);
            inDep -= temDep;
            tempR[j][i] = temDep;
        }
        // Step 4: Place remaining magnitude on diagonal of R matrix
        tempR[i][i] = inDep;
    }
    Q.push_back(Matrix(height, width, tempQ));
    R.push_back(Matrix(height, width, tempR));
    // Step 5: Cross-multiply R*Q to obtain next matrix.
    Matrix newE = R[R.size() - 1].cross(Q[Q.size() - 1]);
    E.push_back(newE);
    // This section checks if the matrix has been solved within the tolerance.
    if (E.size() > 1)
    {
cout << "4\n";
        oldE = E[E.size() - 2].getMatrix();
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < width; j++)
            {
                error += (newE.getMatrix()[i][j] - oldE[i][j]) * (newE.getMatrix()[i][j] - oldE[i][j]);
            }            
        }
        if (error <= size / error)
        {
cout << "5\n";
            QRf.clear();
            QRf.push_back(Q[Q.size() - 1]);
            QRf.push_back(R[R.size() - 1]);
            return QRf;
        }
    }
cout << "6\n";
    QRf = QR();
    return QRf;
}