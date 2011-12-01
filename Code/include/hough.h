#ifndef HOUGH_H
#define HOUGH_H

#include <vector>

class Hough
{
    public:
        Hough(const int largeur, const int hauteur);
        ~Hough();
        void vote(int x,int y);
        std::vector<double> gagnant();
        std::vector<double> rhoThetaToAb(const double rho, const double theta);


    private:
        int largeur,hauteur;
        int maxIndexTheta, maxIndexRho;
        double maxRho;

        std::vector<int> accumulateur;


};

#endif // HOUGH_H
