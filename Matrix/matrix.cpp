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
    vector<double> eigenVector;
    double temp1;
    double temp2;
    double D0;
    double D1;
    double C;
    bool valid = square();
    int numTerms = matrix.size();
    if ((valid == false) || (width > 2))
    {
        cout << "This program can only calculate eigenvalues\n";
        cout << "for square matrices 2×2 and smaller.";
        return;
    }
    else
    {
        vector<double> poly = createPolynomial();
        if (width == 1)
        {
            realEigen.push_back(matrix[0][0]);
            cout << "The eigenvalue of this matrix is " << realEigen[0] << endl;
            cout << "Its eigenvecror is [1].\n";
        }
        if (width == 2)
        {
            temp1 = poly[1] * poly[1] - 4 * poly[0] * poly[2];
            if (temp1 < 0)
            {
                temp1 *= -1;
                imaginaryEigen.push_back(sqrt(temp1) / (2 * poly[2]));
                imaginaryEigen.push_back(sqrt(temp1) / (-2 * poly[2]));
                temp1 = -1 * poly[1] / (2 * poly[2]);
                realEigen.push_back(temp1);
                realEigen.push_back(temp1);
                eigenVector.push_back((matrix[1][1] - realEigen[0]) / matrix[1][0]);
                eigenVector.push_back(imaginaryEigen[0] / matrix[1][0]);
                eigenVector.push_back(1);
                eigenVectors.push_back(eigenVector);
                eigenVector.clear();
                eigenVector.push_back(eigenVectors[0][0]);
                eigenVector.push_back(-1 * eigenVectors[0][1]);
                eigenVector.push_back(1);
                eigenVectors.push_back(eigenVector);
                eigenVector.clear();
                cout << "The eigenvalues of this matrix are" << endl << realEigen[0] << " + " << imaginaryEigen[0] << "i\n";
                cout << " and " << realEigen[0] << " - " << imaginaryEigen[0] << "i\n";
                cout << "Its eigenvectors are [" << eigenVectors[0][0] << " + " << abs(eigenVectors[0][1]) << "i, 1]\n";
                cout << "and [" << eigenVectors[1][0] << " - " << abs(eigenVectors[1][1]) << "i, 1]\n";
            }
            else
            {
                imaginaryEigen.push_back(0);
                imaginaryEigen.push_back(0);
                temp1 = sqrt(temp1) / (2 * poly[2]);
                temp2 = -1 * poly[1] / (2 * poly[2]);
                realEigen.push_back(temp2 + temp1);
                eigenVector.push_back((matrix[1][1] - realEigen[0]) / matrix[1][0]);
                eigenVector.push_back(1);
                eigenVectors.push_back(eigenVector);
                eigenVector.clear();
                realEigen.push_back(temp2 - temp1);
                eigenVector.push_back((matrix[1][1] - realEigen[1]) / matrix[1][0]);
                eigenVector.push_back(1);
                eigenVectors.push_back(eigenVector);
                eigenVector.clear();
                cout << "The eigenvalues of this matrix are " << realEigen[0] << " and " << realEigen[1] << endl;
                cout << "Its eigenvectors are [" << eigenVectors[0][0] << " , " << eigenVectors[0][1] << "]\n";
                cout << "and [" << eigenVectors[1][0] << " , " << eigenVectors[1][1] << "]\n";
            }
        }
    }
    /*
    The sum of the eigenvalues is the discriminant, the product is the determinant.
    Doing row
    2 3 -> 2-λ 3 -> 2-λ 3            -> 2-λ 3
    4 6    4 6-λ    2-λ (6-λ)(2-λ)/4    0 (6-λ)(2-λ)/4-3
    */
}

vector<Matrix> Matrix::QR()
{
    double length = 0;
    double parallel = 0;
    double temppar = 0;
    double dotProduct = 0;
    double dQ;
    double dR;
    double error = 0;
    vector<vector<double> > tempQ;
    vector<vector<double> > tempR;
    vector<vector<double> > tempE;
    vector<double> tempRow;
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
    // For each column, you need to find the perpendicular component.
    for (int i = 0; i < width; i++)
    {
        /*
        This loop needs to find the values of each entry in the Q and R matrices.
        The columns in the Q matrix are normalized, and the entries in the R matrix are
        the component magnitude in the direction of the Q matrix. The first row in the
        R matrix corresponds to the first column in the Q matrix. After the Q and R
        matrices are created, the next matrix is found by E=QR -> E_next=RQ.
        */
        length = 0;
        parallel = 0;
        for (int j = 0; j < height; j++)
        {
            temppar = 0;
            length += tempE[j][i] * tempE[j][i];
            for (int k = 0; k < i; k++)
            { 
                temppar += tempE[j][i] * tempR[j][k];
            }
            parallel += sqrt(temppar);
        }
        parallel = sqrt(parallel);
        for (int j = 0; j < height; j++)
        {
            tempQ[j][i] = tempE[j][i] * parallel / length;
        }
        for (int j = 0; j < i; j++)
        {
            temppar = 0;
            for (int k = 0; k < height; k++)
            {
                temppar += tempR[k][j] * tempE[k][j];
            }
            tempR[j][i] = temppar;
        }
        tempR[i][i] = parallel;
    }
    Q.push_back(Matrix(height, width, tempQ));
    R.push_back(Matrix(height, width, tempR));
    tempQ = Q[Q.size() - 1].getMatrix();
    tempR = R[R.size() - 1].getMatrix();
    Matrix newE = R[R.size() - 1].cross(Q[Q.size() - 1]);
    E.push_back(newE);
    if (Q.size() > 1)
    {
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < width; j++)
            {
                dQ = tempQ[i][j];
                dR = tempR[i][j];
                error += dQ * dQ;
                error += dR * dR;
            }            
        }
    }
    if (error <= size / 20)
    {
        QRf.push_back(Q[Q.size() - 1]);
        QRf.push_back(R[R.size() - 1]);
        return QRf;
    }
    else
    {
        QRf = QR();
        return QRf;
    }
}