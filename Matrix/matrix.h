#ifndef MATRIX_H
#define MATRIX_H

#include <string>
#include <vector>
#include <queue>
#include <cmath>
using namespace std;

class Matrix
{
public:
    // These are the different ways a matrix can be created, stored in their own file.
    Matrix(string filename);
    Matrix(int rows, int columns, vector<vector<double> > data);
    Matrix(int rows, int columns, vector<vector<double> > data, bool keep);
    Matrix();
    ~Matrix();
    // These functions are useful matrix operations.
    Matrix add(Matrix Other);
    Matrix cross(Matrix Other, bool keep);
    double characteristic();
    double decEntry();
    double determinant(vector<int> rem);
    void eigenValues();
    void eigenVecs();
    void eigen1x1();
    void eigen2x2();
    Matrix invert();
    bool invertible();
    void l2Norm();
    void QR(int n, Matrix Ea);
    Matrix scale(double constant);
    void solve(Matrix solution);
    Matrix subtract(Matrix Other);
    double trace();
    Matrix transpose();
    // The matrix menu is in its own file so it can be easily accessible.
    void menu();
    // These are more of overhead functions for the program.
    void addToList(Matrix A);
    bool checkCompatibility(Matrix Other);
    void checkValidity(ifstream & readMatrix);
    bool compareQR();
    bool diagonal(vector<vector<double> > BaaB);
    double determinant();
    string findName();
    string generateName();
    int getHeight();
    vector<vector<double> > getMatrix();
    string getName();
    int getWidth();
    Matrix householder();
    vector<vector<double> > identity(double factor);
    int intEntry();
    string longName(string shortName);
    vector<vector<double> > multiplicity(vector<vector<double> > BaaB, int n);
    vector<vector<double> > normalizeBottom(vector<double> real, vector<double> imag);
    int numPrev(int n);
    void printEigen();
    void printEigenVec();
    void printMatrix();
    void publishFile();
    void publishNew(int rows, int columns, vector<vector<double> > newMatrix);
    vector<vector<double> > polynomial(int row);
    vector<vector<double> > returnEigen();
    Matrix retreiveMatrix(string number);
    vector<vector<double> > rref();
    void setMatrix(vector<vector<double> > toSet);
    bool square();
    int strChoice();
    void switchRows(vector<vector<double> > toSwap, int row1, int row2);
    int vectorPosition(vector<Matrix> matrixMenu, vector<int> numMenu);
    vector<vector<double> > wilkinson(double a, double b, double c);
    bool ynResponse();
private:
    vector<vector<double> > matrix;
    string name;
    int height;
    int width;
    double norm1;
    double norm2;
    vector<vector<double> > Q;
    vector<vector<double> > R;
    vector<vector<double> > E;
    vector<double> realEigen;
    vector<double> imaginaryEigen;
    vector<vector<double> > realEigenVectors;
    vector<vector<double> > imaginaryEigenVectors;
    double tolerance = 0.01;
    double accuracy = 0.000001;
    double finest = 0.000000005;
    double change;
    int counter;
    vector<Matrix> matrixMenu;
    vector<int> numMenu;
    bool publish;
};

#endif