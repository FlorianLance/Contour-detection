#include "../include/matrix.h"

using namespace std;

///############################################################# CONSTRUCTEURS / DESTRUCTEURS

Matrix::Matrix(const unsigned int _lignes, const unsigned int _colonnes) : colonnes(_colonnes), lignes(_lignes)
{
    mat = new double[colonnes*lignes];
}

Matrix::Matrix(const unsigned int _lignes, const unsigned int _colonnes, int *tab) : colonnes(_colonnes), lignes(_lignes)
{
    mat = new double[colonnes*lignes];
    for(unsigned int i = 0; i < colonnes*lignes; i++)
    {
        mat[i] = tab[i];
    }
}

Matrix::Matrix(const unsigned int _lignes, const unsigned int _colonnes, double *tab) : colonnes(_colonnes), lignes(_lignes)
{
    mat = new double[colonnes*lignes];
    for(unsigned int i = 0; i < colonnes*lignes; i++)
    {
        mat[i] = tab[i];
    }
}

Matrix::~Matrix()
{
    delete[] mat;
}

///############################################################# ACCESSEURS

double Matrix::getVal(const unsigned int _i)const{if( _i < colonnes* lignes) return mat[_i];
return 0;}


double Matrix::getVal(const unsigned int _i, const unsigned int _j)const { if(( _i < lignes) && ( _j < colonnes)) return mat[_i*lignes + _j];
return 0;}

///############################################################# MUTATEURS

void Matrix::setVal(const unsigned int _i, const double _val) {if( _i < colonnes* lignes) mat[_i] = _val;}

void Matrix::setMatrix(double *tab) { for(unsigned int i = 0; i < colonnes*lignes; i++)  mat[i] = tab[i];}




