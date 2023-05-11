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
    Matrix subtract(Matrix Other);
    Matrix scale(double constant);
    Matrix cross(Matrix Other, bool keep);
    double characteristic();
    Matrix invert();
    Matrix transpose();
    double determinant();
    double determinant(vector<vector<double> > toDetermine);
    void solve(Matrix solution);
    bool invertible();
    double trace();
    void eigenValues();
    void QR(int n, Matrix Ea);
    void eigen1x1();
    void eigen2x2();
    void eigenVecs();
    double l2norm();
    // These are more of overhead functions for the program.
    void menu();
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