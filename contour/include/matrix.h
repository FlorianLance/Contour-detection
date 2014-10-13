#ifndef _MATRIX
#define _MATRIX

#include <iostream>

/**
* @class Classe de manipulation de matrices basique utilisée pour initialiser les masques.
*
*/
class Matrix{

  public:

    Matrix(const unsigned int, const unsigned int);
    Matrix(const unsigned int, const unsigned int, int *);
    Matrix(const unsigned int, const unsigned int, double *);
    ~Matrix();

    double getVal(const unsigned int) const;
    double getVal(const unsigned int, const unsigned int) const;
    void setMatrix(double *);
    void setVal(const unsigned int,const double);

  private:

    unsigned int colonnes;
    unsigned int lignes;
    double *mat;

};


#endif
