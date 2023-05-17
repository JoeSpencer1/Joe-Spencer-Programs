#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include "matrix.h"
using namespace std;

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

double Matrix::determinant(vector<int> rem)
{
    double det;
    double sign = 1;
    vector<int> nRem;
    if (rem.size() == 2)
    {
        det = matrix[height - 2][rem[0]] * matrix[height - 1][rem[1]];
        det -= matrix[height - 2][rem[1]] * matrix[height - 1][rem[0]];
    }
    else
    {
        for (int i = 0; i < rem.size(); i++)
        {
            nRem.clear();
            for (int j = 0; j < i; j++)
            {
                nRem.push_back(rem[j]);
            }
            for (int j = i + 1; j < rem.size(); j++)
            {
                nRem.push_back(rem[j]);
            }
            det += sign * matrix[height - rem.size()][rem[i]] * determinant(nRem);
            sign *= -1;
        }
    }
    return det;
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
    vector<vector<double> > poly;
    double real = 0;
    double imaginary = 0;
    QR(height - 1, A);
    for (int i = 0; i < height; i++)
    {
        if ((i < height - 1) && ((E[i + 1][i] > accuracy * height) || (E[i + 1][i] < 0 - accuracy * height)))
        {
            poly = polynomial(i);
            realEigen.push_back(poly[0][0]);
            imaginaryEigen.push_back(poly[1][0]);
            realEigen.push_back(poly[0][1]);
            imaginaryEigen.push_back(poly[1][1]);
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
            if ((length > accuracy) || (length < (0 - accuracy)))
            {
                tempQ[j][i] /= length;
            }
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
        if ((i < (height - 1)) && ((Eb.getMatrix()[i + 1][i] * Eb.getMatrix()[i + 1][i]) > (accuracy * height)))
        {
            double a = (Ea.getMatrix()[i][i] * Ea.getMatrix()[i + 1][i + 1]) - (Ea.getMatrix()[i + 1][i] * Ea.getMatrix()[i][i + 1]);
            double b = (Eb.getMatrix()[i][i] * Eb.getMatrix()[i + 1][i + 1]) - (Eb.getMatrix()[i + 1][i] * Eb.getMatrix()[i][i + 1]);
            temError = (a - b) * (a - b);
            i++;            
        }
        else
        {
            temError = (Eb.getMatrix()[i][i] - Ea.getMatrix()[i][i]) * (Eb.getMatrix()[i][i] - Ea.getMatrix()[i][i]);
        }
        error += temError;
        if (temError > accuracy)
        {
            error += temError / height;
        }
    }
    double complex = 0;
    for (int i = 1; i < height; i++)
    {
        complex += Eb.getMatrix()[i][i - 1] * Eb.getMatrix()[i][i - 1];
    }
    complex = sqrt(complex) / height;
    if (((complex < accuracy) && (error < accuracy * accuracy * accuracy * height)) || 
        ((complex >= accuracy * height) && (error < accuracy * accuracy * accuracy * height)))
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

    [A-aI  -(-bI)] [vr]  =  [0]
    [-bI     A-aI] [vi]     [0]   or:

    [bI      A-aI] [vi]  =  [0]
    [A-ai     -bI] [vr]     [0]
    */
    vector<vector<double> > BaaB;
    vector<double> rEigenv;
    vector<double> cEigenv;
    vector<double> tEigenv;
    vector<double> tempRow;
    vector<int> order;
    int row;
    int bottom;
    int skip;
    double factor1;
    double factor2;
    for (int i = 0; i < height; i++)
    {
        // Construct BaaB matrix
        for (int j = 0; j < (height * 2); j++)
        {
            tEigenv.push_back(0);
            order.push_back(j);
            for (int k = 0; k < (width * 2); k++)
            {
                if (((j < height) && (k < width)) || ((j >= height) && (k >= width)))
                {
                    if (k == j)
                    {
                        tempRow.push_back(imaginaryEigen[i]);
                        if (j >= height)
                        {                    
                            tempRow[k] *= -1;
                        }
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
                        tempRow.push_back(matrix[j - height][k]);
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
for(int i=0;i<BaaB.size();i++){for(int j=0;j<BaaB[0].size();j++){cout<<BaaB[i][j]<<" ";}cout<<endl;}
        BaaB = multiplicity(BaaB, i);
for(int i=0;i<BaaB.size();i++){for(int j=0;j<BaaB[0].size();j++){cout<<BaaB[i][j]<<" ";}cout<<endl;}
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
                            order[j + height] = order[k + height];
                            order[k] = row;
                            order[k + height] = row + height;
                        }
                    }
                }
            }
        }
        // Algebraically diagonalize BaaB
        for (int j = 0; j < (height * 2); j++)
        {
            factor1 = BaaB[j][j];
            if ((factor1 < (0 - tolerance)) || (factor1 > tolerance))
            {
                for (int k = (j + 1); k < (height * 2); k++)
                {
                    factor2 = BaaB[k][j] / factor1;
                    for (int l = 0; l < (width * 2); l++)
                    {
                        BaaB[k][l] -= BaaB[j][l] * factor2;
                    }
                }
            }
        }
        // Set bottom entry to 1 unless it is zero. If it is zero, cancel it and find others.
        bottom = height * 2 - 1;
        // Account for special case of matrices with an eigenvalue on the main diagonal.
        if (diagonal(BaaB) == true)
        {
            skip = numPrev(i);
cout<<"Skip: "<<i<<" "<<skip<<endl;
        }
        while (((BaaB[bottom][bottom] > tolerance) || (BaaB[bottom][bottom] < (0 - tolerance))) || (skip > 0))
        {
            if (((BaaB[bottom][bottom] <= tolerance) && (BaaB[bottom][bottom] >= (0 - tolerance))) && (skip > 0))
            {
                skip --;
            }
            bottom --;
            if (bottom < 0)
            {
                bottom = height * 2 - 1;
                break;
            }
        }
cout<<"Bottom="<<bottom<<"\n";
        for (int j = (height * 2 - 1); j > bottom; j--)
        {
            for (int k = 0; k < (height * 2); k++)
            {
                BaaB[k][j] = 0;
            }
        }
        // Put in value for starting eigenvectors
        rEigenv.clear();
        cEigenv.clear();
        tEigenv.clear();
        for (int j = 0; j < height; j++)
        {
            rEigenv.push_back(0);
            cEigenv.push_back(0);
            tEigenv.push_back(0);
            tEigenv.push_back(0);
        }
        tEigenv[bottom] = 1;
        for (int j = bottom - 1; j >= 0; j--)
        {
            factor2 = 0;
            for (int k = bottom; k > j; k--)
            {
                factor2 -= tEigenv[k] * BaaB[j][k];
            }
            if ((BaaB[j][j] > accuracy) || (BaaB[j][j] < (0 - accuracy)))
            {
                tEigenv[j] = factor2 / BaaB[j][j];    
            }
        }
        // Create real and complex eigenvectors
        for (int j = 0; j < height * 2; j++)
        {
            if (order[j] >= height)
            {
                rEigenv[order[j] % height] = tEigenv[j];
            }
            else
            {
                cEigenv[order[j]] = tEigenv[j];
            }
        }
        // Set bottom entry to one
        bottom = height - 1;
        while ((rEigenv[bottom] == 0) && (cEigenv[bottom] == 0))
        {
            bottom--;
        }
        vector<vector<double> > eigenV = normalizeBottom(rEigenv, cEigenv);
        realEigenVectors.push_back(eigenV[0]);
        imaginaryEigenVectors.push_back(eigenV[1]);
        tempRow.clear();
        order.clear();
        rEigenv.clear();
        cEigenv.clear();
        BaaB.clear();
    }
    return;
}

double Matrix::l2norm()
{
    if (norm2 != 0)
    {
        return norm2;
    }
    if (matrix[0].size() == 1)
    {
        for (int i = 0; i < matrix.size(); i++)
        {
            norm2 += matrix[i][0] * matrix[i][0];
        }
        norm2 = sqrt(norm2);
    }
    else
    {
        Matrix A = Matrix(height, width, matrix);
        Matrix A2 = A.cross(A.transpose(), false);
        vector<vector<double> > normV = A2.returnEigen();
        for (int i = 0; i < normV[0].size(); i++)
        {
            if ((normV[0][i] * normV[0][i]) + (normV[1][i] * normV[1][i]) > norm2)
            {
                norm2 = (normV[0][i] * normV[0][i]) + (normV[1][i] * normV[1][i]);
            }
            norm2 = sqrt(norm2);
        }
    }
    return norm2;
}