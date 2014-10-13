#include "gradient.h"

#define M_PI 3.14159265

using namespace std;

///############################################################# CONSTRUCTEURS / DESTRUCTEURS

#undef sqrt
#include <math.h>

Gradient::Gradient(const Gradient& _g) :sommeTotale(_g.sommeTotale), gradientMoyen(_g.gradientMoyen), penteMoyenne(_g.penteMoyenne),
    taille(_g.taille), largeur(_g.largeur), hauteur(_g.hauteur)
{
    tab = new int[taille];
    dG  = new double[taille];

    tabGHorizontal = new int[taille];
    tabGVertical   = new int[taille];
    penteBi        = new int[taille];
    signeGradient  = new int[taille];

    prog = _g.prog;

    for(unsigned int i = 0; i < taille; i++)
    {
        tab[i] = _g.tab[i];
        dG[i]  = _g.dG[i];
        tabGHorizontal[i] = _g.tabGHorizontal[i];
        tabGVertical[i]   = _g.tabGVertical[i];
        penteBi[i] = _g.penteBi[i];
        signeGradient[i] = _g.signeGradient[i];

    }


}

Gradient::Gradient(const IplImage* _img, const Matrix& _matX, const Matrix& _matY, Progression* _prog) :
        sommeTotale(0), gradientMoyen(0), taille(_img->height*_img->width), largeur(_img->width), hauteur(_img->height)
{
    // initialisation du tableau contenant les gradients
    tab = new int[taille];
    dG  = new double[taille];

    tabGHorizontal = new int[taille];
    tabGVertical   = new int[taille];
    penteBi        = new int[taille];
    signeGradient  = new int[taille];

    int norme = 0, S1 = 0, S2 = 0;
    double pente = 0, penteTotale = 0;

    prog = _prog;

    if(prog)
        prog->setMax(taille);

    for(int i=1; i < _img->height-1; i++)
	{
		for(int j=1; j < _img->width-1; j++)
		{
		    S1 = 0; S2 = 0; pente = 0;

		    for(int k = i-1; k <= i+1; k++)
		    {
		        for(int p = j-1; p <= j+1; p++)
		        {
		            S1 += cvGet2D(_img,k,p).val[0] * _matX.getVal(k-i+1, p-j+1);
		            S2 += cvGet2D(_img,k,p).val[0] * _matY.getVal(k-i+1, p-j+1);
                        }
		    }

            int indice = i*(_img->width-1) + j;

            pente = atan((double)S1/(double)S2);
            if(S2 == 0) pente = M_PI/2;

            dG[i*(_img->width-1) + j] = pente;

            // remplissage des tableaux de gradients verticaux et horizontaux pour l'affinage par extractions des maximas locaux
            tabGHorizontal[indice] = S1;
            tabGVertical  [indice] = S2;

            // récupération du signe des gradients
            if(S1+S2 < 0) signeGradient[indice] = -1;
            else          signeGradient[indice] =  1;

            // calcul de la norme
            norme = (int)sqrt((float)(S1*S1 + S2*S2));
            tab[i*(_img->width-1) + j] = norme;

            sommeTotale += norme;
            penteTotale += pente;

            // actualisation barre de progression
            if(prog)
            {
                prog->setV(i*(_img->width-1) + j);
                if(rand()%10 == 1)  emit  prog->maj();
            }


            if     (tabGHorizontal[indice] >=  tabGVertical  [indice]) penteBi[indice] = 0;
            else                                                       penteBi[indice] = 90;
		}
	}

    gradientMoyen = sommeTotale/((_img->height-2)* (_img->width-2) ) ;

    // Remplissage du tour du tableau avec le gradient moyen
    for(int i = 0; i < _img->width ; i++){setTab(i,0, gradientMoyen); setTab(i,_img->height -1 , gradientMoyen);}
    for(int i = 0; i < _img->height; i++){setTab(0,i, gradientMoyen); setTab(  _img->width-1 ,i, gradientMoyen);}


    penteMoyenne = penteTotale/((_img->height-2)* (_img->width-2) ) ;
//    cout << penteMoyenne << " " << penteTotale<< endl;
}

Gradient::Gradient(const IplImage* _img, const Matrix& _mat1, const Matrix& _mat2,const Matrix& _mat3, const Matrix& _mat4, Progression* _prog) :
        sommeTotale(0), gradientMoyen(0), taille(_img->height * _img->width), largeur(_img->width), hauteur(_img->height)
{
    tab = new int[taille];
    dG  = new double[taille];
    double norme;

    vector<int>    S(4,0);
    vector<double> N(4,0);

    tabGHorizontal = new int[taille];
    tabGVertical   = new int[taille];
    penteBi        = new int[taille];
    signeGradient  = new int[taille];


    prog = _prog;
    if(prog)
        prog->setMax(taille);

    double pente, penteTotale = 0;

    for(int i=1; i < _img->height-1; i++) // on ne calcule pas le gradient sur les bords, mais on lui appliquera le gradient moyen a la fin
    {
        for(int j=1; j < _img->width-1; j++)
        {
            for(unsigned int l = 0; l <4; l++) S[l] = 0; // remise à 0 de tous les éléments du tableau

            for(int k = i-1; k <= i+1; k++) // on parcours la matrice 3x3 autour du point en cours //
            {
                for(int p = j-1; p <= j+1; p++)
                {
		            S[0] += cvGet2D(_img,k,p).val[0] * _mat1.getVal(k-i+1, p-j+1);
		            S[1] += cvGet2D(_img,k,p).val[0] * _mat2.getVal(k-i+1, p-j+1);
		            S[2] += cvGet2D(_img,k,p).val[0] * _mat3.getVal(k-i+1, p-j+1);
		            S[3] += cvGet2D(_img,k,p).val[0] * _mat4.getVal(k-i+1, p-j+1);
                }
            }

            // récupération du signe des gradients
            if(S[0]+S[1]+S[2]+S[3] < 0) signeGradient[i*(_img->width-1) + j] = -1;
            else                        signeGradient[i*(_img->width-1) + j] =  1;

            for(int l = 0; l < 4; l++) N[l] = sqrt(pow((double)S[l],2));

            int indice = i*(_img->width-1) + j;
            /// remplissage des tableaux de gradients verticaux et horizontaux pour l'affinage par extractions des maximas locaux
            tabGHorizontal[indice] = S[0];
            tabGVertical  [indice] = S[1];

            norme = 0.25 * (N[0]+N[1]+N[2]+N[3]);


            double PenteMax = max(max(max(N[0],N[1]),N[2]),N[3]);

            pente = atan (sqrt(pow(PenteMax,2)));

            if     (PenteMax == N[0]) penteBi[indice] = 0;
            else if(PenteMax == N[1]) penteBi[indice] = 90;
            else if(PenteMax == N[2]) penteBi[indice] = 45;
            else if(PenteMax == N[3]) penteBi[indice] = 135;

            tab[i*(_img->width-1) + j] = norme;
            sommeTotale += norme;
            penteTotale += pente;

            // actualisation barre de progression
            if(prog)
            {
                prog->setV(i*(_img->width-1) + j);
                if(rand()%10 == 1) emit prog->maj();
            }
        }
    }

     gradientMoyen = sommeTotale/((_img->height-2)* (_img->width-2) ) ;
     penteMoyenne = penteTotale/((_img->height-2)* (_img->width-2) ) ;

//     Remplissage du tour du tableau avec le gradient moyen
    for(int i = 0; i < _img->width ; i++){setTab(i,0, gradientMoyen); setTab(i,_img->height -1 , gradientMoyen);}
    for(int i = 0; i < _img->height; i++){setTab(0,i, gradientMoyen); setTab(_img->width-1 ,i, gradientMoyen);}
}


Gradient::~Gradient()
{
    delete[] tab;
    delete[] dG;
    delete[] tabGHorizontal;
    delete[] tabGVertical;
    delete[] penteBi;
    delete[] signeGradient;
}

///############################################################# ACCESSEURS
int    Gradient::getLargeur() const {return largeur;}

int    Gradient::getHauteur() const {return hauteur;}

double Gradient::getSommeTotale() const   {return sommeTotale;}

double Gradient::getGradientMoyen() const {return gradientMoyen;}

int   Gradient::getTab(const unsigned int _i)const
{
    if(_i < taille ) return tab[_i];
    else {std::cout << "Erreur d'acces au tableau. (getTab gradient) " << _i << std::endl; return 0;}
}

double Gradient::getDirection(const unsigned int _i) const
{
    if(_i < taille ) return dG[_i];
    else {std::cout << "Erreur d'acces au tableau. (getDirection gradient) " << _i << std::endl; return 0;}
}

int Gradient::getTabGH(const unsigned _i) const
{
    if(_i < taille) return tabGHorizontal[_i];
    else {std::cout << "Erreur d'acces au tableau. (getTabGH gradient) " << _i << std::endl; return 0;}
}

int Gradient::getTabGV(const unsigned _i) const
{
    if(_i < taille ) return tabGVertical[_i];
    else {std::cout << "Erreur d'acces au tableau. (getTabGV gradient) " << _i << std::endl; return 0;}
}

int Gradient::getPenteBi(const unsigned _i) const
{
    if(_i < taille ) return penteBi[_i];
    else {std::cout << "Erreur d'acces au tableau. (getPenteBi gradient) " << _i << std::endl; return 0;}
    return 0;
}

int Gradient::getSigneGradient(const unsigned _i) const
{
    if(_i < taille ) return signeGradient[_i];
    else {std::cout << "Erreur d'acces au tableau. (signeGradient gradient) " << _i << std::endl; return 0;}
    return 0;
}
///############################################################# MUTATEURS

void Gradient::setTab(const unsigned int _i,const unsigned int _val)
{
    if(_i < taille) tab[_i]=_val;
    else {std::cout << "Erreur d'acces au tableau. (setTab gradient) " <<  _i << std::endl;}
}

void Gradient::setTab(const unsigned int _i, const unsigned int _j, const unsigned int _val)
{
    if((_i < largeur ) && (_j < hauteur))  tab[_j * (largeur-1) + _i]=_val;
    else {std::cout << "Erreur d'acces au tableau. (setTab gradient) " <<  _i << " " << _j << std::endl;}
}


///############################################################# FONCTIONS

void Gradient::affecterCarteGradient(IplImage* _img)
{
    CvScalar s1;

    int gradientMax = 0;
    for(int i=0; i < _img->height; i++)
    {
        for(int j=0; j < _img->width; j++)
        {
            if(tab[i* (largeur-1) + j] > gradientMax) gradientMax = tab[i* (largeur-1) + j];
//            cvSet2D(_img,i,j,s1);
        }
    }

    for(int i=0; i < _img->height; i++)
    {
        for(int j=0; j < _img->width; j++)
        {
            int valeur = (tab[i* (largeur-1) +j] * 255) / gradientMax;
            s1.val[0] = valeur;
            s1.val[1] = valeur;
            s1.val[2] = valeur;
            cvSet2D(_img,i,j,s1);

        }
    }
}

IplImage* Gradient::creerCartePentes(const IplImage *_img)
{
    IplImage* retour = cvCreateImage(cvSize(_img->width, _img->height), IPL_DEPTH_8U ,3);
//    int valGH =0, valGV = 0;

    CvScalar s1; s1.val[0] = 255; s1.val[1] = 0;   s1.val[2] = 0;
    CvScalar s2; s2.val[0] = 0;   s2.val[1] = 255; s2.val[2] = 0;
    CvScalar s3; s3.val[0] = 0;   s3.val[1] = 0;   s3.val[2] = 255;
    CvScalar s4; s4.val[0] = 255; s4.val[1] = 0;   s4.val[2] = 255;

    for(int i=0; i < _img->height; i++)
    {
        for(int j=0; j < _img->width; j++)
        {
//            valGH = sqrt(pow(tabGHorizontal[i* (largeur-1) +j], 2));
//            valGV = sqrt(pow(tabGVertical  [i* (largeur-1) +j] ,2));

//            if      ( valGH > valGV) cvSet2D(retour,i,j,s1);
//            else if ( valGV < valGH) cvSet2D(retour,i,j,s2);
//            else       cvSet2D(retour,i,j,s3);
            switch(penteBi[i* (_img->width-1) +j])
            {
            case 0:     cvSet2D(retour,i,j,s1);
                break;
            case 45:    cvSet2D(retour,i,j,s2);
                break;
            case 90:    cvSet2D(retour,i,j,s3);
                break;
            case 135:   cvSet2D(retour,i,j,s4);
                break;
            }

        }
    }
    return retour;
}


