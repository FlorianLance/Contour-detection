#include "../include/seuillage.h"

using namespace std;

static bool initialisationParametres = false;

///############################################################# CONSTRUCTEURS / DESTRUCTEURS

Seuillage::Seuillage(): nbPixelM(0), numEnsembleConvexe(0){}

Seuillage::~Seuillage(){}


IplImage *Seuillage::seuillageGlobal(const IplImage* _img, const Gradient& _g, const unsigned int _valS, Progression* _prog) const
{
    IplImage* retour = cvCloneImage(_img);
    CvScalar s1;
    int valS;
    int valTotalGrad = 0, valMoyenneGrad = 0;

    _prog->setMax(_img->height* _img->width);

    /// par défaut on initialise la valeur de seuillage par la moyenne des valeurs du tableau de gradients
    if(!initialisationParametres)
    {
        for (int i = 0; i < _img->height* _img->width; i++) valTotalGrad += _g.getTab(i);
        valMoyenneGrad = valTotalGrad / (_img->height* _img->width);
        valS = valMoyenneGrad;
        initialisationParametres = true;
    }
    else valS = _valS;

    for(int i=0; i < _img->height; i++)
    {
        for(int j=0; j < _img->width; j++)
        {
            if(_g.getTab((i)*(_img->width-1) + (j)) >  valS)
            {
                s1.val[0] = 0; s1.val[1] = 0; s1.val[2] = 0;
                cvSet2D(retour,i,j,s1);
            }
            else
            {
                s1.val[0] = 255; s1.val[1] = 255; s1.val[2] = 255;
                cvSet2D(retour,i,j,s1);
            }
            // actualisation barre de progression
            _prog->setV(i*(_img->width-1) + j);
            if(rand()%10 == 1)  emit  _prog->maj();
        }

    }
    return retour;
}

///############################################################# FONCTIONS

bool Seuillage::connexite4(const QPoint &p1, const QPoint &p2) const
{
    int val = sqrt(pow(p1.x() - p2.x(),2) + pow(p1.y() - p2.y(),2));
    if(val > 1) return false;
    return true;
}


IplImage *Seuillage::seuillageLocal     (const IplImage* _img, const Gradient& _g, const unsigned int _voisinage, Progression* _prog) const
{
    IplImage* retour = cvCloneImage(_img);
    CvScalar s1; s1.val[0] = 255; s1.val[1] = 255; s1.val[2] = 255;
    CvScalar s2; s2.val[0] = 0; s2.val[1] = 0; s2.val[2] = 0;

    for(int i = 0; i < _img->width* _img->height; i++) cvSet1D(retour,i,s1);
    int moyLocale = 0;
    int voisinage = 0;

    _prog->setMax(_g.getHauteur()*_g.getLargeur());

    // Défintion du voisinage sur lequel on va effectuer le voisinage local

    voisinage = 2 * _voisinage;
    if(_voisinage < 1) voisinage = 2;

    for (int i = 0; i < _img->height; i++)
    {
        for (int j = 0; j < _img->width; j++)
        {
            if((i >= voisinage) && (i < _img->height -voisinage) && (j >= voisinage) && (j < _img->width -voisinage))
            {
                moyLocale = 0;

                for(int k = i-(voisinage/2); k <= i+(voisinage/2); k++) // on parcours la matrice voisinagexvoisinage autour du point en cours //
                {
                    for(int p = j-(voisinage/2); p <= j+(voisinage/2); p++)
                    {
                        moyLocale += cvGet2D(_img,k,p).val[0]; /// on utilise la moyenne des valeurs des pixels qui donne un meilleur résultat que pour le gradient
//                        moyLocale += _g.getTab(k * (_img->width-1) + p);
                    }
                }

                // actualisation barre de progression
                _prog->setV(i*(_img->width-1) + j);
                if(rand()%10 == 1)  emit  _prog->maj();

                moyLocale = moyLocale / (voisinage*voisinage);

                if(_g.getTab((i)*(_img->width-1) + (j)) >  moyLocale) cvSet2D(retour,i,j,s2);
                else                                                  cvSet2D(retour,i,j,s1);
            }
        }
    }


    return retour;
}

IplImage *Seuillage::seuillageHysteresis(const IplImage* _img, Gradient& g, const unsigned int _seuilHaut, const bool _connexite, Progression* _prog)
{
    IplImage* retour = cvCloneImage(_img);

    CvScalar s1; s1.val[0] = 255; s1.val[1] = 255; s1.val[2] = 255;
    CvScalar s2; s2.val[0] = 0; s2.val[1] = 0; s2.val[2] = 0;

    unsigned int seuilHaut =  _seuilHaut;

    unsigned int seuilBas = 0.4 * seuilHaut;
    unsigned int tempGrad = 0;
    bool gradientFort = false;
    int nbPixelModifie = 0;

    _prog->setMax(g.getLargeur()*g.getHauteur());
//    Gradient newG(g);

    for (int i = 0; i < _img->height; i++)
    {
        for (int j = 0; j < _img->width; j++)
        {
            tempGrad = g.getTab((i * (_img->width-1) + j)); //cout << tempGrad << " " << seuilBas << " " << seuilHaut << endl;
            if     (tempGrad  <  seuilBas )  cvSet2D(retour,i,j, s1);
            else if(tempGrad  > seuilHaut) {cvSet2D(retour,i,j, s2); nbPixelModifie++;}
            else
            {
                if((i > 0) && (i < _img->height -1) && (j > 0) && (j < _img->width -1))
                {
                    gradientFort = false;

                    if(_connexite) /// connnexite de 8
                    {
                        for(int k = i-1; k <= i+1; k++) // on parcours la matrice 3x3 autour du point en cours //
                        {
                            for(int p = j-1; p <= j+1; p++)
                            {
                                tempGrad = g.getTab(k * (_img->width-1) + p);
                                if(tempGrad > seuilHaut) gradientFort = true;
                            }
                        }
                    }
                    else            /// connnexite de 4
                    {
                        tempGrad = g.getTab((i-1) * (_img->width-1) + j);     if(tempGrad > seuilHaut) gradientFort = true;
                        tempGrad = g.getTab( i    * (_img->width-1) + (j-1)); if(tempGrad > seuilHaut) gradientFort = true;
                        tempGrad = g.getTab( i    * (_img->width-1) + (j+1)); if(tempGrad > seuilHaut) gradientFort = true;
                        tempGrad = g.getTab((i+1) * (_img->width-1) + j);     if(tempGrad > seuilHaut) gradientFort = true;
                    }

                    if(gradientFort) {cvSet2D(retour,i,j, s2); g.setTab(j,i,seuilHaut+1);}
                    else             cvSet2D(retour,i,j, s1);
                }
                else
                {

                    cvSet2D(retour,i,j, s1);
                }
            }
            // actualisation barre de progression
            _prog->setV(i*(_img->width-1) + j);
            if(rand()%30 == 1)  emit  _prog->maj();
        }
    }

        if(nbPixelM < nbPixelModifie)
            {
                nbPixelM = nbPixelModifie;
                return seuillageHysteresis(retour,g,seuilHaut,_connexite, _prog);
            }
        else  {nbPixelM = 0; return retour; }
}



IplImage *Seuillage::seuillageExtractionMaximasLocaux(const IplImage* _img, Gradient& _g, Progression* _prog) const
{
    double GM1, GM2;
    double gH,gV;
//    double hauteurU = _img->height-1;
    CvScalar s1; s1.val[0] = 255; s1.val[1] = 255; s1.val[2] = 255;
    CvScalar s2; s2.val[0] = 0; s2.val[1] = 0; s2.val[2] = 0;

    IplImage* retour = cvCloneImage(_img);

     _prog->setMax(_g.getLargeur()*_g.getHauteur());

    for (int i = 0; i < _img->height; i++)
    {
        for (int j = 0; j < _img->width; j++)
        {
            if((i > 0) && (i < _img->height -1) && (j > 0) && (j < _img->width -1) && (cvGet2D(_img,i,j).val[0] == 0)) // pour éviter le bord de l'image
            {
                gH = _g.getTabGH(i * (_img->width-1) + j);
                gV = _g.getTabGV(i * (_img->width-1) + j);

                if((sqrt(pow(gV,2))) >= sqrt(pow(gH,2)) && gV != 0)
                {
//                     cout << "GM1 " << gV << " " <<gH << " " << GM1 << " "  << _g.getTab((i-1) * (_img->height-1) + j+1) << " "  <<   _g.getTab((i-1) * (_img->height-1) + j) << " -----";
                    if(gV ==0 ) {gV = 1; cout << "div 0  ";}// cas divison par 0

                    GM1 = (gH / gV) * _g.getTab((i-1) * (_img->width-1) + j) +
                         ((gV - gH) / gV) * _g.getTab((i-1) * _img->width + j+1);

                    GM2 = (gH / gV) * _g.getTab((i+1) * (_img->width-1) + j-1) +
                         ((gV - gH) / gV) * _g.getTab((i+1) * (_img->width-1) + j);

                }
                else if((sqrt(pow(gV,2))) < sqrt(pow(gH,2)) && gH != 0)
                {

                    if(gH ==0 ) {gH = 1; cout << "div 0  "; }// cas divison par 0

                    GM1 = (gV / gH) * _g.getTab((i) * (_img->width-1) + j + 1) +
                         ((gH - gV) / gH) * _g.getTab((i-1) * (_img->width-1) + j+1);

                    GM2 = (gV / gH) * _g.getTab((i) * (_img->width-1) + j-1) +
                         ((gH - gV) / gH) * _g.getTab((i+1) * (_img->width-1) + j-1);
                }
                else GM1 = 1000000;
//                cout << (_g.getTab(i* hauteurU + j) > GM1) << " " << (_g.getTab(i* hauteurU + j) > GM2) << " " << (cvGet2D(_img,i,j).val[0] == 0) << endl;
                if(!((_g.getTab(i* (_img->width-1) + j) > GM1) && (_g.getTab(i* (_img->width-1) + j) > GM2))) cvSet2D(retour,i,j, s1);

            }
            // actualisation barre de progression
            _prog->setV(i*(_img->width-1) + j);
            if(rand()%30 == 1)  emit  _prog->maj();
        }
    }
    return retour;
}


IplImage* Seuillage::seuillageAmincissementContours(const IplImage* _img, Gradient& _g, Progression* _prog) const
{
    IplImage* retour = cvCloneImage(_img);
    CvScalar s1; s1.val[0] = 255; s1.val[1] = 255; s1.val[2] = 255;
    CvScalar s2; s2.val[0] = 0; s2.val[1] = 0; s2.val[2] = 0;

     _prog->setMax(_g.getLargeur()*_g.getHauteur());


    for(int i = 0; i < _img->width* _img->height; i++) cvSet1D(retour,i,s1);


        for(int i=1; i<_img->height-1; i++)
        {
            for(int j=1; j<_img->width-1; j++)
            {
              if(cvGet2D(_img,i,j).val[0] == 0)
                {



                    if(_g.getPenteBi(j + (_img->width-1)*i ) == 0)
                    {
                        if((_g.getTab((i)*(_img->width-1) + j-1) <= _g.getTab(i*(_img->width-1)   + j))
                         && (_g.getTab(i*(_img->width-1) + j)     >  _g.getTab((i)*(_img->width-1) + j+1)))
                           {
                                cvSet2D(retour,i,j, s2);
                           }
                    }

                    else if(_g.getPenteBi(j + (_img->width-1)*i ) == 90)
                    {
                        if((_g.getTab((i-1)*(_img->width-1) + j) <= _g.getTab(i*(_img->width-1)   + j))
                         &&(_g.getTab(i*(_img->width-1) + j)     >  _g.getTab((i+1)*(_img->width-1) + j)) )
                           {
                                cvSet2D(retour,i,j, s2);
                           }
                    }

                    else if( _g.getPenteBi(j + (_img->width-1)*i ) == 45)
                    {
                            if((_g.getTab((i-1)*(_img->width-1) + j-1) <= _g.getTab(i*(_img->width-1)   + j))
                             &&(_g.getTab((i)*(_img->width-1) + (j))   >  _g.getTab((i+1)*(_img->width-1) + j+1)) )
                               {
                                    cvSet2D(retour,i,j, s2);
                               }
                    }
                    else if(_g.getPenteBi(j + _img->width*i ) ==135)
                    {
                            if((_g.getTab((i-1)*(_img->width-1) + j+1) <= _g.getTab(i*(_img->width-1)   + j))
                             &&(_g.getTab((i)*(_img->width-1) + (j))   >  _g.getTab((i+1)*(_img->width-1) + j-1)) )
                               {
                                    cvSet2D(retour,i,j, s2);
                               }
                    }
                }
              // actualisation barre de progression
              _prog->setV(i*(_img->width-1) + j);
              if(rand()%30 == 1)  emit  _prog->maj();
            }
        }

        return retour;
    }

IplImage* Seuillage::fermetureContours(const IplImage* _img,  Gradient& _g, const int _gradMin, Progression* _prog) const
{

    cout << "Fermeture des contours. " << endl;

    IplImage* retour = cvCloneImage(_img);
    vector<CvScalar> coordVoisins;
     _prog->setMax(_g.getLargeur()*_g.getHauteur());
    CvScalar s2; s2.val[0] = 255; s2.val[1] = 0; s2.val[2] = 0;

        for(int i=1; i<_img->height-1; i++)
        {
            // balayage vertical de haut en bas
            for(int j=1; j<_img->width-1; j++)
            {
                // balayage horizontal de gauche à droite

                // detection d'une extrémité
                if(cvGet2D(retour,i,j).val[0] == 0)
                {
                    coordVoisins.clear();
                    for(int k = i-1; k <= i+1; k++) // on parcours la matrice 3x3 autour du point en cours //
                    {

                        for(int p = j-1; p <= j+1; p++)
                        {
                            if(k != i || p != j)
                            {
                                CvScalar c; c.val[0] = k; c.val[1] = p;
                                if(cvGet2D(retour,k,p).val[0] == 0) coordVoisins.push_back(c);
                            }
                        }
                    }

                     QPoint coordP(i,j);
                    if(coordVoisins.size() == 2)
                    {
                        int diff = (coordVoisins[0].val[0] - coordVoisins[1].val[0]) + (coordVoisins[0].val[1] - coordVoisins[1].val[1]);
                        if( diff == 1 || diff == -1)
                        {
                            fermetureElementaire(retour, coordP, _g,0,_gradMin);
                        }
                    }

                    if(coordVoisins.size() == 1) {
                        fermetureElementaire(retour, coordP, _g,0,_gradMin);
                    }



                }

                // actualisation barre de progression
                _prog->setV(i*(_img->width-1) + j);
                if(rand()%30 == 1)  emit  _prog->maj();
            }
        }

        return retour;
}

void Seuillage::fermetureElementaire(IplImage* _retour, const QPoint& _c,  Gradient& _g, const int _numeroPointFermeture, const int _gradMin) const
{
    int gradientMax = 0;
    QPoint indiceGradientMax;
    CvScalar s2; s2.val[0] = 0; s2.val[1] = 0; s2.val[2] = 0;

    bool fin = false;
    QPoint pointPrecedent;
    QPoint pointParcours;

    if(_c.x() > 1 && _c.x() < _g.getHauteur()-1 && _c.y() >1 && _c.y() < _g.getLargeur()-1)
    {

        for(int k = _c.x()-1; k <= _c.x()+1; k++) // on parcours la matrice 3x3 autour du point en cours //
        {
            for(int p = _c.y()-1; p <= _c.y()+1; p++)
            {
                if( k != _c.x() || p != _c.y())
                {
                    if(cvGet2D(_retour,k,p).val[0] == 0) pointPrecedent = QPoint(k,p);
                }

            }
        }


        for(int k = _c.x()-1; k <= _c.x()+1; k++) // on parcours la matrice 3x3 autour du point en cours //
        {
            for(int p = _c.y()-1; p <= _c.y()+1; p++)
            {
                pointParcours = QPoint(k,p);

                if( (k != _c.x() || p != _c.y())   && !connexite4(pointPrecedent,pointParcours))
                {
                    if (cvGet2D(_retour,k,p).val[0] == 255)
                    {
                        int gradient = _g.getTab(k* (_g.getLargeur()-1) + p );

                        if(gradient > _gradMin)
                        {
                            if(gradientMax < gradient )
                            {
                                gradientMax = gradient;
                                indiceGradientMax.setX(k);
                                indiceGradientMax.setY(p);

                            }
                        }

                    }
                    else if( _numeroPointFermeture > 1) fin = true;
                }

            }
        }

        if(!fin && gradientMax != 0)
        {
            cvSet2D(_retour,indiceGradientMax.x(),indiceGradientMax.y(), s2);

           if(!fin)fermetureElementaire(_retour, indiceGradientMax, _g, _numeroPointFermeture+1, _gradMin);
        }
    }
}


void Seuillage::creationEnsembleConnexe(const IplImage* _img, const Gradient& _g)
{
        cout << "Création ensemble connexe. " << endl;
    std::vector<bool> tabPointsEffectues;
    tabConnexite.clear();
    tabPointsEffectues.assign(_g.getLargeur()*_g.getHauteur(), false);
    numEnsembleConvexe = 0;

    for(int i=0; i<_img->height; i++)
    {
        for(int j=0; j<_img->width; j++)
        {
            if(cvGet2D(_img,i,j).val[0] == 0 && tabPointsEffectues[i*(_img->width-1) + j] == false)
            {
                QPoint p(i,j);
                vector<QPoint> v;
                tabConnexite.push_back(v);

                ajoutEnsembleConnexe(_img, p, tabPointsEffectues);
                numEnsembleConvexe++;


            }
        }
    }

    cout << "L'image affinée comporte "<< tabConnexite.size() << " ensembles connexes. " << endl;
}

void Seuillage::ajoutEnsembleConnexe(const IplImage* _img, const QPoint& _p,std::vector<bool>& _tabPointsEffectues)
{
    //ajout du point dans l'ensemble connexe
    tabConnexite[numEnsembleConvexe].push_back(_p);
    _tabPointsEffectues[_p.x()* (_img->width-1) + _p.y()] = true;

    if(_p.x() > 0 && _p.x() < _img->height-1 && _p.y() > 0 && _p.y() < _img->width-1)
    {

        for(int k = _p.x()-1; k <= _p.x()+1; k++) // on parcours la matrice 3x3 autour du point en cours //
        {
            for(int p = _p.y()-1; p <= _p.y()+1; p++)
            {
                if(k != _p.x() || p != _p.y())
                {
                    if(cvGet2D(_img,k,p).val[0] == 0 && _tabPointsEffectues[k*(_img->width-1) + p]== false)
                    {
                        QPoint q(k,p);
//                            cout << q.x() << " " << q.y() <<  " " << tabConnexite[numEnsembleConvexe].size() <<  " " << tabConnexite.size()<<  " | ";
                           ajoutEnsembleConnexe(_img,q,_tabPointsEffectues);
                    }
                }
            }
        }
    } 
}


IplImage* Seuillage::modelisationSegments(const IplImage* _img, const uint _tailleSegments, const bool _col)
{
    IplImage *retour = cvCloneImage(_img);
    cout << "Modélisation des segments. " << endl;

    CvScalar s1,s2,s3,s4;
    s1.val[0] = 0; s1.val[1] = 0 ; s1.val[2] = 0;
    s2.val[0] = 255; s2.val[1] = 255; s2.val[2] = 255;
    s3.val[0] = 0 ; s3.val[1] = 0 ; s3.val[2] = 255;
    s4.val[0] = 255; s4.val[1] = 0; s4.val[2] = 0;


    CvScalar *sa, *sb;
    if(_col) {sa = &s3; sb = &s4;}
    else     {sa = &s1; sb = &s2;}

    uint tailleS = _tailleSegments;

    int pix = 0;


    for(uint i = 0; i < tabConnexite.size(); i++)
    {
        tailleS = _tailleSegments;
        if(tabConnexite[i].size() < tailleS && tabConnexite[i].size() >= 3) tailleS = tabConnexite[i].size();

        if(tabConnexite[i].size() > tailleS)
        {
            for(uint j = 0; j < tabConnexite[i].size(); j += tailleS)
            {
                // vérification taille ensemble connexe
                if(j + tailleS < tabConnexite[i].size())
                {
                    // vérification distance
                    
                    if(distance(tabConnexite.at(i)[j],tabConnexite.at(i)[j+tailleS] ) <= tailleS)
                    {

                        for(uint k = 0; k <= tailleS; k++) {
                            cvSet2D(retour,tabConnexite.at(i)[j+k].x(), tabConnexite.at(i)[j+k].y(),*sa);

                         pix++;
                        }

                        int diffX = sqrt(pow(tabConnexite.at(i)[j].x() - tabConnexite.at(i)[j+tailleS].x() ,2));
                        int diffY = sqrt(pow(tabConnexite.at(i)[j].y() - tabConnexite.at(i)[j+tailleS].y() ,2));

                        int x1 = tabConnexite.at(i)[j].x(), x2 = tabConnexite.at(i)[j+tailleS].x();
                        int y1 = tabConnexite.at(i)[j].y(), y2 = tabConnexite.at(i)[j+tailleS].y();

                        /// METHODE 2
                        tracerSegment(QPoint(x1,y1),QPoint(x2,y2),diffX,diffY,retour,*sb);

                        /// METHODE 1
//                        while(diffX != 0 && diffY != 0)
//                        {
//                            if(diffX > diffY)
//                            {
//                                diffX--;
//                            }
//                            else if (diffX > diffY)
//                            {
//                                diffY--;
//
//                            }
//                            else
//                            {
//                                diffX--;
//                                diffY--;
//
//                            }
//
//                            if     (x1 <= x2 && y1 <= y2) {cvSet2D(retour, x1 + diffX, y1 + diffY, *sb);}// cout << x1 + diffX << " " << y1 + diffY << endl;}
//                            else if(x1 <= x2 && y1 >  y2) {cvSet2D(retour, x1 + diffX, y1 - diffY, *sb);}// cout << x1 + diffX << " " << y1 - diffY << endl;}
//                            else if(x1 >  x2 && y1 <= y2) {cvSet2D(retour, x1 - diffX, y1 + diffY, *sb);}// cout << x1 - diffX << " " << y1 + diffY << endl;}
//                            else if(x1 >  x2 && y1 >  y2) {cvSet2D(retour, x1 - diffX, y1 - diffY, *sb);}// cout << x1 - diffX << " " << y1 - diffY << endl;}
//
//                        }
                    }

                }
            }

        }

    }

    return retour;
}

uint Seuillage::distance(const QPoint& _p1, const QPoint& _p2)
{
    return sqrt(pow(_p1.x() - _p2.x(),2)) + sqrt(pow(_p1.y() - _p2.y(),2));
}

void Seuillage::tracerSegment(const QPoint& _p1, const QPoint& _p2, const uint _diffX, const uint _diffY, IplImage* _img, const CvScalar _c)
{
    int d = distance(_p1,_p2);
    bool ecriture = false;
    CvScalar b; b.val[0] = 0; b.val[1] = 0;b.val[2] = 0;

//    cout << (d <= 2) << " "<< (_diffX == 1 && _diffY == 1) << " " << ( d == 1) << " " << (d == 0)<< " | ";
    if(d <= 2)
    {
        if( (_diffX == 1 && _diffY == 1) ||( d == 1) || (d == 0))
        {
            if(_c.val[2] != 255) cvSet2D(_img, _p1.x(), _p1.y(), _c);
            else  cvSet2D(_img, _p1.x(), _p1.y(), b);
            if(_c.val[2] != 255)cvSet2D(_img, _p2.x(), _p2.y(), _c);
            else cvSet2D(_img, _p2.x(), _p2.y(), b);
            ecriture = true;
        }
    }

    if(!ecriture)
    {
        QPoint pM((_p1 +_p2)/2);
        int diffX1 = sqrt(pow(_p1.x() - pM.x() ,2));
        int diffY1 = sqrt(pow(_p1.y() - pM.y() ,2));
        int diffX2 = sqrt(pow(_p2.x() - pM.x() ,2));
        int diffY2 = sqrt(pow(_p2.y() - pM.y() ,2));


        tracerSegment(_p1,pM,diffX1,diffY1,_img,_c);
        tracerSegment(pM,_p2,diffX2,diffY2,_img,_c);
    }

}
