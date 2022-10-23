#ifndef MATRIX_H
#define MATRIX_H

#include <string>
#include <vector>
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
    // You still need to figure out the QR() algorithm.
    vector<Matrix> QR();
    // The trace of the matrix is the sum of its eigenvalues
    // The determinant of the matrix is the product of its eigenvalues.
/*void eigenVectors();*/
    // These are more of overhead functions for the program.
    void menu();
    void publishFile();
    vector<double> createPolynomial();
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
    string generateName();
    string findName();
    bool square();
    vector<vector<double> > rref();
    string getName();
    void addToList(Matrix A);
    Matrix getMatrix(string number);
    vector<double> complexRoot(double real, double imag, int pow);
    string longName(string shortName);
private:
    vector<vector<double> > matrix;
    string name;
    int height;
    int width;
    double size;
    vector<Matrix> Q;
    vector<Matrix> R;
    vector<Matrix> E;
    vector<double> realEigen;
    vector<double> imaginaryEigen;
    vector<vector<double> > eigenVectors;
    double tolerance = 0.01;
    double accuracy = 0.00001;
    vector<Matrix> matrixMenu;
    vector<int> numMenu;
    vector<double> polynomial;
    double pi = 3.141592653238;
    bool publish;
};

#endif