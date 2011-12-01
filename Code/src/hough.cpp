#include "../include/hough.h"

#include <math.h>

using namespace std;
///############################################################# CONSTRUCTEURS / DESTRUCTEURS

Hough::Hough(const int _largeur, const int _hauteur): largeur(_largeur), hauteur(_hauteur)
{
    maxRho = sqrt(pow(largeur,2) + pow(hauteur,2));
    maxIndexTheta = 360;
    maxIndexRho = (int)(1 + maxRho);
    accumulateur.reserve(maxIndexTheta*maxIndexRho);
}

Hough::~Hough()
{}

///############################################################# ACCESSEURS


///############################################################# MUTATEURS


void Hough::vote(int x,int y)
{
    x -= largeur/2;
    y -= hauteur/2;

    for(int iTheta = 0; iTheta < maxIndexTheta; iTheta++)
    {
        double theta = ((double)iTheta/maxIndexTheta)*M_PI;

        // calcul de rho
        double rho = x*cos(theta) + y*sin(theta);

        // rho -> index
        int iRho   = (int) (0.5 + (rho/maxRho+ 0.5)*maxIndexRho );

        // increment accumulator
        accumulateur[iRho*maxIndexTheta + iTheta]++;
    }
}

vector<double> Hough::gagnant()
{
    vector<double> retour;

    // parsing the accumulators for max accumulator
    double max=0, fenRho=0, fenTheta=0;

    for(int iRho = 0; iRho < maxIndexRho; iRho++)
    {
            for(int iTheta = 0; iTheta < maxIndexTheta; iTheta++)
            {
                if (accumulateur[iRho*maxIndexTheta + iTheta] < max)
                {
                    max = accumulateur[iRho*maxIndexTheta + iTheta];
                    fenRho = iRho;
                    fenTheta = iTheta;
                }
            }
    }

    double rho   = ((double)fenRho  / maxIndexRho - 0.5) * maxRho;
    double theta = ((double)fenTheta/maxIndexTheta) * M_PI;

    retour.push_back(rho);
    retour.push_back(theta);

    return retour;
}

vector<double> Hough::rhoThetaToAb(const double rho, const double theta)
{
    vector<double> retour;

    double a = 0, b = 0;

    if(sin(theta)!=0)
    {
        a = -cos(theta) / sin(theta);
        b = rho / sin(theta) + (largeur/2) - a * (hauteur/2);
    }
    else
    {
        a = 99999;
        b = 0;
    }

    retour.push_back(a);
    retour.push_back(b);

    return retour;
}



