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
    Matrix(string filename);
    Matrix(int rows, int columns, vector<vector<double> > data);
    Matrix(int rows, int columns, vector<vector<double> > data, bool keep);
    Matrix();
    ~Matrix();
    // These functions are all useful matrix operations.
    Matrix add(Matrix Other);
    double characteristic();
    double determinant(vector<vector<double> > toDetermine, int column, int row);
    void eigenValues();
    void eigenVecs();
    void eigen1x1();
    void eigen2x2();
    Matrix invert();
    bool invertible();
    Matrix cross(Matrix Other, bool keep);
    double l2norm();
    void QR(int n, Matrix Ea);
    Matrix scale(double constant);
    void solve(Matrix solution);
    Matrix subtract(Matrix Other);
    double trace();
    Matrix transpose();
    // These are more of overhead functions for the program.
    void menu();
    double determinant();
    void publishFile();
    vector<double> polynomial(int row);
    bool checkCompatibility(Matrix Other);
    int vectorPosition(vector<Matrix> matrixMenu, vector<int> numMenu);
    void checkValidity(ifstream & readMatrix);
    bool ynResponse();
    int getWidth();
    int getHeight();
    void switchRows(vector<vector<double> > toSwap, int row1, int row2);
    vector<vector<double> > getMatrix();
    void printMatrix();
    void publishNew(int rows, int columns, vector<vector<double> > newMatrix);
    void printEigen();
    string generateName();
    string findName();
    bool square();
    vector<vector<double> > rref();
    string getName();
    void addToList(Matrix A);
    Matrix getMatrix(string number);
    string longName(string shortName);
    Matrix householder();
    void setMatrix(vector<vector<double> > toSet);
    vector<vector<double> > wilkinson(double a, double b, double c);
    vector<vector<double> > identity(double factor);
    bool compareQR();
    void printEigenVec();
    int strChoice();
    int intEntry();
    double decEntry();
    vector<vector<double> > returnEigen();
    vector<vector<double> > normalizeBottom(vector<double> real, vector<double> imag);
    vector<vector<double> > multiplicity(vector<vector<double> > BaaB, int n);
private:
    vector<vector<double> > matrix;
    string name;
    int height;
    int width;
    double norm;
    vector<vector<double> > Q;
    vector<vector<double> > R;
    vector<vector<double> > E;
    vector<double> realEigen;
    vector<double> imaginaryEigen;
    vector<vector<double> > realEigenVectors;
    vector<vector<double> > imaginaryEigenVectors;
    double tolerance = 0.01;
    double accuracy = 0.00001;
    vector<Matrix> matrixMenu;
    vector<int> numMenu;
    bool publish;
};

#endif