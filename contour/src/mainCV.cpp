/**
* TP1 : Analyse Première partie
* LANCE Florian : n°10604720
* POLI Armand   : n°10503753
*
*/

//#include <opencv/cv.h>
#include "opencv2/highgui/highgui.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <istream>

#include <dirent.h>
#include <ctype.h>

#include "matrix.h"
#include "gradient.h"
#include "masques.h"
#include "seuillage.h"

using namespace cv;
using namespace std;


    vector<string> listeImage;       // vector contenant les images du dossier ./images
    string nomChemin = "./images/";  // chemin du dossier contenant les images

    bool continuer         = true; // booleen servant a boucler sur la boucle principale du programme
    bool modificationImage = true; // booleen servant à boucler sur la partie modification de l'image
    bool parametreModifie  = true; // indicateur de modification d'une valeur sur les trackbar


    IplImage* img;    // images
    IplImage* carteGradient;
    IplImage* cartePentes;
    IplImage* cloneImg;
    IplImage* imgModif;
    IplImage* imgAffinee1;
    IplImage* imgAffinee2;

    int valSeuillageGlobal    = 150; // valeurs manipulées par les trackbars
    int valSeuillageLocal     = 1;
    int valSeuillagHysteresis = 300;

    void modifValSG(int val) {valSeuillageGlobal    = val; parametreModifie = true;} // procédures appelées par les trackbars
    void modifValSL(int val) {valSeuillageLocal     = val; parametreModifie = true;}
    void modifValSH(int val) {valSeuillagHysteresis = val; parametreModifie = true;}



int main()
{
    char key = ' ';
    int choixImage = 0, choixMasque = 0, choixSeuillage = 0;
    string nomMasque, titreImage, enregistrement;

    /// Récupération des images présentes dans le dossier ./images

    DIR * rep = opendir("../data/images");
    if (rep != NULL)
    {
        struct dirent * ent;

        while ((ent = readdir(rep)) != NULL)
        {
            string temp = ent->d_name;
            if(temp != ".." && temp != "." && temp != ".svn")  listeImage.push_back(ent->d_name );
        }
        closedir(rep);
    }

    while(continuer)
    {

        /// CHOIX DE L'IMAGE
            cout << endl <<  "Veuillez choisir une image: (entrez le numero correspondant dans la console)" << endl << endl;
            for (unsigned int i = 0; i < listeImage.size() ; i++) cout << i  << " --> "<<listeImage[i] << endl;
            cout << endl;

            cin >> choixImage;
            if (choixImage == 99) cin >> choixImage;
            choixImage = choixImage%listeImage.size();

            cout << "Chargement de l'image : " << nomChemin + listeImage[choixImage]  << endl << endl;

            img = cvLoadImage( (nomChemin + listeImage[choixImage]).c_str(),0);

        /// CHOIX DU MASQUE
            cout << "Choix du masque (console):" << endl;
            cout << "1 --> Sobel   (bi) "    << endl;
            cout << "2 --> Prewitt (bi) "    << endl;
            cout << "3 --> Kirsch  (multi) " << endl;
            cout << "4 --> Custom  (bi) "    << endl;
            cout << "5 --> Custom  (multi) " << endl;

            cin >> choixMasque;
            if (choixMasque== 99) cin >> choixMasque;

            Matrix *masqueX; // horizontal
            Matrix *masqueY; // vertical
            Matrix *masqueK; // 45°
            Matrix *masqueL; // 135°

             cout << "Initialisation du masque";

            double temp = 0; // valeur de remplissage des matrices custom
            switch(choixMasque)
            {
                case 1 : masqueX = new Matrix(3,3, XSobel);   masqueY = new Matrix(3,3, YSobel);   cout << " de Sobel. "  << endl;  // initialisation masque Sobel
                         nomMasque = "Sobel";
                break;
                case 2 : masqueX = new Matrix(3,3, XPrewitt); masqueY = new Matrix(3,3, YPrewitt); cout << " de Prewitt. "<< endl;  // initialisation masque Prewitt
                         nomMasque = "Prewitt";
                break;
                case 3 : masqueX = new Matrix(3,3, Kirsh1);   masqueY = new Matrix(3,3, Kirsh2);   cout << " de Kirsh. "  << endl;  // initialisation masque Kirsh
                         masqueK = new Matrix(3,3, Kirsh3);   masqueL = new Matrix(3,3, Kirsh4);
                         nomMasque = "Kirsh";
                break;
                case 4:

                        cout << "Remplissage de la matrice horizontale: (entrez les valeurs)" << endl;
                            masqueX = new Matrix(3,3);
                            for(int i = 0; i < 9; i++) {cout << "M " << i/3 << "." <<i%3 << endl; cin >> temp;  masqueX->setVal(i,temp); }

                        cout << "Remplissage de la matrice verticale: (entrez les valeurs)"   << endl;
                            masqueY = new Matrix(3,3);
                            for(int i = 0; i < 9; i++) {cout << "M " << i/3 << "." << i%3 << endl; cin >> temp; masqueY->setVal(i,temp); }
                            nomMasque = "custom bi";
                break;
                case 5:
                        cout << "Remplissage de la matrice horizontale: (entrez les valeurs)" << endl;
                            masqueX = new Matrix(3,3);
                            for(int i = 0; i < 9; i++) {cout << "M " << i/3 << "." <<i%3 << endl; cin >> temp;  masqueX->setVal(i,temp); }

                        cout << "Remplissage de la matrice verticale: (entrez les valeurs)"   << endl;
                            masqueY = new Matrix(3,3);
                            for(int i = 0; i < 9; i++) {cout << "M " << i/3 << "." << i%3 << endl; cin >> temp; masqueY->setVal(i,temp); }

                        cout << "Remplissage de la matrice 45°: (entrez les valeurs)" << endl;
                            masqueK = new Matrix(3,3);
                            for(int i = 0; i < 9; i++) {cout << "M " << i/3 << "." <<i%3 << endl; cin >> temp;  masqueK->setVal(i,temp); }

                        cout << "Remplissage de la matrice 120°: (entrez les valeurs)"   << endl;
                            masqueL = new Matrix(3,3);
                            for(int i = 0; i < 9; i++) {cout << "M " << i/3 << "." << i%3 << endl; cin >> temp; masqueL->setVal(i,temp); }
                        nomMasque = "custom multi";

                default: masqueX = new Matrix(3,3, XSobel);   masqueY = new Matrix(3,3, YSobel);   cout << " de Sobel. " << endl;  // initialisation de sobel par défaut
                         nomMasque = "Sobel";
                break;
            }

        /// CHOIX DU SEUILLAGE
            Gradient *gradient;
            Seuillage seuillage;

            if(choixMasque < 3 || choixMasque == 4) gradient = new Gradient(img, *masqueX,*masqueY);                   // initialisation gradient bi-directionnel
            else                                    gradient = new Gradient(img, *masqueX,*masqueY,*masqueK,*masqueL); // initialisation gradient multi-directionnel

            cout << endl;
            cout << "Choix de la methode de seuillage (console):" << endl;
            cout << "1 --> Seuillage global          "   << endl;
            cout << "2 --> Seuillage local           "   << endl;
            cout << "3 --> Seuillage par hysteresis  "   << endl;

            cin >> choixSeuillage;
            if (choixSeuillage== 99) cin >> choixSeuillage;

        /// AFFICHAGE DE L'IMAGE DE BASE
            cloneImg = cvCloneImage(img);
            cvShowImage (listeImage[choixImage].c_str(), cloneImg );
            cvMoveWindow(listeImage[choixImage].c_str(),0,0);

        /// AFFICHAGE DE LA CARTE DES GRADIENTS
            carteGradient = cvCloneImage(img);
            gradient->affecterCarteGradient(carteGradient);
            cvShowImage ("Carte des gradients", carteGradient);
            cvMoveWindow("Carte des gradients",img->width * 2,0);

        /// AFFICHAGE DE LA CARTE DES PENTES
            cartePentes = gradient->creerCartePentes(img);
            cvShowImage ("Carte des pentes (r->h, b->v)", cartePentes);
            cvMoveWindow("Carte des pentes (r->h, b->v)",img->width * 3,0);

        /// AFFICHAGE DE LA TRACKBAR CONTENANT LES PARAMETRES
            cvNamedWindow("Parametrages", 0);
            cvMoveWindow("Parametrages",0, img->height);
            cvResizeWindow("Parametrages", 1000, 100);

            cvCreateTrackbar("val seuil (global)                                          ", "Parametrages", &valSeuillageGlobal, 500, modifValSG);
            cvCreateTrackbar("taille voisinage(local) (1 - 10)                      ", "Parametrages", &valSeuillageLocal, 10, modifValSL);
            cvCreateTrackbar("val seuilH (hysteresis) sBas = 1/4 * sHaut ", "Parametrages", &valSeuillagHysteresis, 1000, modifValSH);

        /// DEBUT DE LA BOUCLE DE MODIFICATION

            cout << "Appuyer sur 'q' pour sortir (sur une fenetre). "<< endl;
            modificationImage = true;

            while (modificationImage)
            {
                if(parametreModifie)
                {

                    switch(choixSeuillage)
                    {
                        case 1:
                                imgModif   =  seuillage.seuillageGlobal    (cloneImg, *gradient, valSeuillageGlobal);
                                titreImage = "s-global " + nomMasque;
                        break;
                        case 2: imgModif   =  seuillage.seuillageLocal     (cloneImg, *gradient, valSeuillageLocal);
                                titreImage = "s-local " + nomMasque;
                        break;
                        case 3:
                                imgModif   =  seuillage.seuillageHysteresis(cloneImg, *gradient, valSeuillagHysteresis, true); // TODO choix connexite
                                titreImage = "s-hysteresis " + nomMasque;
                        break;
                        default:
                                imgModif   =  seuillage.seuillageGlobal    (cloneImg, *gradient, valSeuillageGlobal);
                                titreImage = "s-global " + nomMasque;
                        break;
                    }

                    parametreModifie = false;
                }

                cvShowImage (titreImage.c_str(), imgModif ); // affichage image modifiée
                cvMoveWindow(titreImage.c_str(), cloneImg->width,0);

                 key = cvWaitKey(1000);
                 if(key == 'q') modificationImage = false; // condition de sortie de la boucle

            }

        /// FIN DE LA BOUCLE DE MODIFICATION

        cvDestroyAllWindows();
        key = ' ';
        modificationImage = true;
        parametreModifie = true;

        /// AFFICHAGE DE L'IMAGE AFFINEE
            Seuillage affinage1,affinage2;

            imgAffinee1 = affinage1.seuillageAmincissementContours(imgModif, *gradient); // on lance les procédures d'affinage
            imgAffinee2 = affinage2.seuillageExtractionMaximasLocaux(imgModif, *gradient);

            cvShowImage ("Amincissement Contours", imgAffinee1 ); // affichage image affinée
            cvMoveWindow("Amincissement Contours", 0,0);
//
            cvShowImage ("Extraction Maximas Locaux", imgAffinee2 ); // affichage image affinée
            cvMoveWindow("Extraction Maximas Locaux", cloneImg->width,0);



        /// GESTION DE L'ENREGISTREMENT
            cout << "Enregistrer l'image ? (o: oui, n: non sur la console)" << endl;
            cin >> enregistrement;
            if(enregistrement == "o")
            {
                cout <<  "Entrer le nom de la sauvegarde sans l'extension (sur la console) " << endl;
                cin >> enregistrement;
                if(cvSaveImage((nomChemin + enregistrement + ".jpg").c_str(), imgAffinee1)) cout << "Image "<< enregistrement + ".jpg" << " sauvegardee. " << endl;
                else cout << "Erreur durant la sauvegarde. " << endl;
            }

        cout << endl << "Voulez-vous effectuer une autre operation ? (o: oui, n: non -> sur une fenetre) " <<endl;
        while((key != 'n') && (key != 'o'))
        {
             key = cvWaitKey(1000);
        }

        if(key == 'n') {continuer = false; cout << "Fermeture du programme. " << endl;}
        else cout << "-> nouvelle operation. " << endl;
        key = ' ';

        /// LIBERATION
            cvReleaseImage(&imgAffinee1);
            cvReleaseImage(&imgAffinee2);
            cvReleaseImage(&imgModif);
            cvReleaseImage(&cloneImg);
            cvReleaseImage(&carteGradient);
            cvReleaseImage(&img);
            cvDestroyAllWindows();

            // protection pour éviter les choix automatiques dans la prochaine boucle ( quand on se trompe entre la console et la fenêtre)
            choixMasque = 99;
            choixImage = 99;
            choixSeuillage = 99;

    }

	return 0;
}
