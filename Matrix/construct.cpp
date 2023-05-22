#include <iostream>
#include <fstream>
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
        if (fileName == "0")
        {
            break;
        }
        fileName = longName(fileName);
        fileName = fileName;
        readMatrix.open(fileName);
        if (!readMatrix.is_open())
        {
            cout << "Enter 0 to exit if necessary.\n";
        }
    }
    checkValidity(readMatrix);
    readMatrix >> height;
    checkValidity(readMatrix);
    readMatrix >> width;
    checkValidity(readMatrix);
    double input;
    norm1 = 0;
    norm2 = 0;
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
                    norm1 += input;
                }
                else
                {
                    norm1 -= input;
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
    norm1 = 0;
    norm2 = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (matrix[i][j] > 0)
            {
                norm1 += matrix[i][j];
            }
            else
            {
                norm1 -= matrix[i][j];
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
    norm1 = 0;
    norm2 = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (matrix[i][j] > 0)
            {
                norm1 += matrix[i][j];
            }
            else
            {
                norm1 -= matrix[i][j];
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
        norm1 = 0;
        norm2 = 0;
        valid = true;
        cout << "Please enter a height:\n";
        height = intEntry();
        cout << "Please enter a width:\n";
        width = intEntry();
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
                        norm1 += stod(strNum);
                    }
                    else
                    {
                        norm1 -= stod(strNum);
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
        name = generateName();
    }
    cout << "Matrix will be stored in file \"" << name << "\"\n\n";
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
    Q.clear();
    R.clear();
    E.clear();
    realEigen.clear();
    imaginaryEigen.clear();
    realEigenVectors.clear();
    matrixMenu.clear();
    numMenu.clear();
};