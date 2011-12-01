#ifndef _SEUILLAGE_
#define _SEUILLAGE_

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "gradient.h"
#include "progressionAlgo.h"

/**
* @class Classe permettant d'appliquer des seuillages sur une IplImage à partir d'un Gradient
*        contient les 3 types de seuillages  (local,global, hysteresis) ainsi qu'une fonction d'affinage par extraction des maximas locaux.
*
*/
class Seuillage{

    public:

        /** @brief Constructeur par défaut de Seuillage. */
        Seuillage();

        /** @brief Destructeur par défaut de Seuillage. */
        ~Seuillage();

        /** @brief Retourne un seuillage global de l'image en paramètre à partir de son gradient.
        *          la valeur de seuil peut⁻être paramétrée mais est initialisée par défaut par la moyenne des gradients.
        */
        IplImage *seuillageGlobal    (const IplImage*, const Gradient&, const unsigned int, Progression*) const;

        /** @brief Retourne un seuillage local de l'image en paramètre à partir de son gradient.
        *          la taille du voisinage est paramètrable.
        */
        IplImage *seuillageLocal     (const IplImage*, const Gradient&, const unsigned int, Progression*) const;

        /** @brief Retourne un seuillage par hystérésis de l'image en paramètre à partir de son gradient.
        *          la valeur de seuil est paramétrable ainsi que la connexité utilisée (8 si à true)
        *          cette fonction s'appelle récursivement jusqu'à ce qu'elle n'ai plus rien à modifier.
        */
        IplImage *seuillageHysteresis(const IplImage*,  Gradient&, const unsigned int, const bool, Progression*);

        /** @brief Retourne un affinage par extraction des maximas locaux de l'image en paramètre à partir de son gradient. */
        IplImage *seuillageExtractionMaximasLocaux(const IplImage*,Gradient&, Progression*) const;

        /** @brief Retourne un affinage par amincissement des contours de l'image en paramètre à partir de son gradient. */
        IplImage *seuillageAmincissementContours(const IplImage*, Gradient&, Progression*) const;

        /** @brief Retourne une image dont les contours ont été refermés. */
        IplImage* fermetureContours(const IplImage*,  Gradient&, const int, Progression*) const;

        /** @brief Procédure récursive appelée par fermetureContours*/
        void fermetureElementaire(IplImage*, const QPoint&,  Gradient&, const int, const int) const ;

        /** @brief Renvoie true si les 2 points sont 4 connexes*/
        bool connexite4(const QPoint &, const QPoint &) const;

        /** @brief Crée l'ensemble connexe*/
        void creationEnsembleConnexe(const IplImage*, const Gradient&);

        /** @brief Procédure récursive appelée par creationEnsembleConnexe*/
        void ajoutEnsembleConnexe(const IplImage*,const QPoint&, std::vector<bool>&);

        /** @brief Modélise les segments à partir de l'ensemble connexe */
        IplImage* modelisationSegments(const IplImage*, const uint, const bool);

        /** @brief Retourne la distance entre 2 points */
        uint distance(const QPoint&, const QPoint&);

        /** @brief Trace un segment sur l'image en paramètre entre les 2 points*/
        void tracerSegment(const QPoint&, const QPoint&, const uint, const uint, IplImage*, const CvScalar);

    private:

        int nbPixelM;                                     /**< Entier contenant le nombre de pixels modifiés lors d'une passe avec le seuillage par hystérésis. */
        std::vector< std::vector <QPoint> > tabConnexite; /**< Tableau contenant les ensembles de points connexes d'une image contourée */
        int numEnsembleConvexe;                           /**< Numéro de l'ensemble connexe actuel */


};


#endif
