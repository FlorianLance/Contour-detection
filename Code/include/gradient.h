#ifndef _GRADIENT
#define _GRADIENT

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "matrix.h"
#include "progressionAlgo.h"

#include <math.h>
#include <vector>
#include <string>

#include <QImage>


/**
* @class Classe gradient qui initialise les tableaux de gradients et de pentes à par d'une IplImage
*        et crée les cartes de gradients et de pentes.
*/
class Gradient
{
    public:

        /** @brief Constructeur par copie de gradient.*/
        Gradient(const Gradient&);

        /** @brief Constructeur de gradient avec une matrice horizontale et verticale.*/
        Gradient(const IplImage*, const Matrix&, const Matrix&, Progression*);

        /** @brief Constructeur de gradient avec une matrice horizontale, verticale, 45° et 135°.*/
        Gradient(const IplImage*, const Matrix&, const Matrix&, const Matrix& , const Matrix&, Progression*);

        /** @brief Destructeur de gradient.*/
        ~Gradient();

        /** @brief Retourne la somme totale des gradients.*/
        double getSommeTotale() const;

        /** @brief Retourne le gradient moyen.*/
        double getGradientMoyen() const;

        /** @brief Retourne la largeur de l'image. */
        int    getLargeur() const;

        /** @brief Retourne la hauteur de l'image. */
        int    getHauteur() const;

        /** @brief Retourne la valeur à l'indice i du tableau stockant la norme des gradients de chaque pixel de l'image. */
        int    getTab(const unsigned int) const;

        /** @brief Retourne la valeur à l'indice i du tableau des gradient verticaux. */
        int    getTabGV(const unsigned int) const;

        /** @brief Retourne la valeur à l'indice i du tableau des gradient horizontaux. */
        int    getTabGH(const unsigned int) const;

        /** @brief Retourne la valeur à l'indice i du tableau des directions. */
        double getDirection(const unsigned int) const;

        /** @brief Retourne la valeur à l'indice i du tableau des directions (0,45,90,135). */
        int    getPenteBi(const unsigned _i) const;

        /** @brief Retourne le signe du gradient à l'indice i. */
        int    getSigneGradient(const unsigned _i) const;

        /** @brief Modifie la valeur à l'indice i du tableau des gradients par val. */
        void   setTab(const unsigned int, const unsigned int);

        /** @brief Modifie la valeur à l'indice i et j du tableau des gradients par val. */
        void   setTab(const unsigned int, const unsigned int, const unsigned int);

        /** @brief Affecte à chaque pixel de l'image une valeur correspondant au gradient normalisé. */
        void affecterCarteGradient(IplImage*);

        /** @brief Retourne une image initialisé à partir de celles en paramètre ou lui
        sont affectés une valeur de pixel correspondant à la direction du gradient. */
        IplImage* creerCartePentes(const IplImage *);




    private:

         double        sommeTotale;  /**< Flottant contenant la somme de tous les gradients calculés de l'image. */
         double        gradientMoyen;/**< Flottant contenant le gradient moyen de l'image. */
         double        penteMoyenne; /**< Flottant contenant la valeur moyenne de la pente de l'image. */
         unsigned int taille;        /**< Entier contenant la taille de l'image en pixels. */
         unsigned int largeur;       /**< Entier contenant la largeur de l'image en pixels. */
         unsigned int hauteur;       /**< Entier contenant la hauteur de l'image en pixels. */

         int         *tab;           /**< Tableau d'entier stockant la norme des gradients. */
         double      *dG;            /**< Tableau de flottant stockant les directions. */
         int         *tabGHorizontal;/**< Tableau d'entier stockant les gradients horizontaux. */
         int         *tabGVertical;  /**< Tableau d'entier stockant les gradients verticeaux. */
         int         *penteBi;       /**< Tableau d'entiers stockant les directions (0,45,90,135). */
         int         *signeGradient; /**< Tableau d'entiers stockant le signe du gradient. . */

         Progression* prog;          /**< Progression de l'algorithme en cours. */






};


#endif
