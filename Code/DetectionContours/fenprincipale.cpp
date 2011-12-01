#include "fenprincipale.h"
#include "ui_fenprincipale.h"

using namespace std;


FenPrincipale::FenPrincipale(QWidget *parent) : QMainWindow(parent),  ui(new Ui::FenPrincipale), imageChargee(false),
gradientInitialise(false), imageSeuillee(false), imageAff(false),ensembleConnexeCree(false), imageFermee(false),seuillagePrecedent(QString(".."))
{
    ui->setupUi(this);

    imageSource = NULL;
    imageModifiee = NULL;
    carteGradient = NULL;
    cartePentes = NULL;
    cloneImageSource = NULL;
    imageAffinee = NULL;
    imageContouree = NULL;
    imageModelisee = NULL;

    /// Initialisation fenêtre principale
        this->setWindowTitle(QString("Detection de contours - LANCE-POLI 2011-2012"));
        QMenu *menuFichier = menuBar()->addMenu("&Fichier");
        QAction *actionSauvegarder = new QAction("&Sauvergader image (droite)", this);
        menuFichier->addAction(actionSauvegarder);
        QAction *actionQuitter = new QAction("&Quitter", this);
        menuFichier->addAction(actionQuitter);

    /// Initialisation barre de progression
        prog = new Progression(0);
        QObject::connect(prog, SIGNAL(modificationProgression()),this,SLOT(modificationProgression()));

    /// Initialisation dimensions image affichee
        largeurImage = 500;
        hauteurImage = 450;

    /// Initialisation ComboBox listeImages
        QStringList listImages(QDir("../images/").entryList());
        ui->cb_choixImg->addItems(listImages);
        ui->cb_choixImg->removeItem(0);
        ui->cb_choixImg->removeItem(0);

    /// Initialisation ComboBox choixMasque
        QStringList listMasques;
        listMasques << "Kirsh" << "Sobel" << "Prewitt" << "Robinson";
        ui->cb_choixMasq->addItems(listMasques);

    /// Initialisation ComboBox choixMasque
        QStringList listSeuillage;
        listSeuillage << "Hysteresis" << "Global" << "Local";
        ui->cb_seuill->addItems(listSeuillage);

    /// Initialisation ComboBox choixAffinage
        QStringList listAffinage;
        listAffinage << "Amincissement" << "Extrema locaux";
        ui->cb_choixAffinage->addItems(listAffinage);

    /// Initialisation tab widgets
        ui->tw_visuels1->setTabText(0,QString("Source"));
        ui->tw_visuels2->setTabText(0,QString("Source"));
        ui->tw_visuels1->setTabText(1,QString("Seuillage"));
        ui->tw_visuels2->setTabText(1,QString("Seuillage"));
        ui->tw_visuels1->setTabText(2,QString("Gradients"));
        ui->tw_visuels2->setTabText(2,QString("Gradients"));
        ui->tw_visuels1->setTabText(3,QString("Pentes"));
        ui->tw_visuels2->setTabText(3,QString("Pentes"));
        ui->tw_visuels1->setTabText(4,QString("Affinage"));
        ui->tw_visuels2->setTabText(4,QString("Affinage"));
        ui->tw_visuels1->setTabText(5,QString("Fermeture"));
        ui->tw_visuels2->setTabText(5,QString("Fermeture"));
        ui->tw_visuels1->setTabText(6,QString("Mode. segm."));
        ui->tw_visuels2->setTabText(6,QString("Mode. segm."));
        ui->tw_visuels1->setCurrentIndex(0);
        ui->tw_visuels2->setCurrentIndex(0);

    /// Initialisation chemins
        cheminDossierImages = "../images/";

        /// Initialisation Slots
        QObject::connect(ui->pb_seuill, SIGNAL(clicked()),this,SLOT(seuillageImage()));
        QObject::connect(ui->pb_agrandir, SIGNAL(clicked()),this,SLOT(agrandirImage()));
        connect(actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
        connect(actionSauvegarder, SIGNAL(triggered()), this, SLOT(sauvegarder()));


}

FenPrincipale::~FenPrincipale()
{
    cvReleaseImage(&imageSource);
    cvReleaseImage(&imageModifiee);
    cvReleaseImage(&carteGradient);
    cvReleaseImage(&cartePentes);
    cvReleaseImage(&cloneImageSource);
    cvReleaseImage(&imageAffinee);
    cvReleaseImage(&imageContouree);
    cvReleaseImage(&imageModelisee);
    delete ui;
}


void FenPrincipale::agrandirImage()
{
    QDialog agrandissement;
    QGridLayout l;
    QLabel la;
    agrandissement.setLayout(&l);
    l.addWidget(&la);


    // Redimentionnement de l'image



QPixmap pixmap_img;

    int i = ui->tw_visuels2->currentIndex();

    if(i == 0)
    {
        if(imageSource){
        QImage *qimage = convIlpImageToQImage(imageSource);
        pixmap_img = QPixmap::fromImage(*qimage);
        la.setPixmap(pixmap_img);
     agrandissement.exec();}
    }
    else if(i == 1)
    {
        if(imageSeuillee){
        QImage *qimage = convIlpImageToQImage(imageModifiee);
        pixmap_img = QPixmap::fromImage(*qimage);
        la.setPixmap(pixmap_img);
     agrandissement.exec();}
    }
    else if(i == 2)
    {
        if(imageChargee){
        QImage *qimage = convIlpImageToQImage(carteGradient);
        pixmap_img = QPixmap::fromImage(*qimage);
        la.setPixmap(pixmap_img);
     agrandissement.exec();}
    }
    else if(i == 3)
    {
        if(imageChargee){
        QImage *qimage = convIlpImageToQImage(cartePentes);
        pixmap_img = QPixmap::fromImage(*qimage);
        la.setPixmap(pixmap_img);
     agrandissement.exec();}
    }
    else if(i == 4)
    {
        if(imageAffinee){
        QImage *qimage = convIlpImageToQImage(imageAffinee);
        pixmap_img = QPixmap::fromImage(*qimage);
        la.setPixmap(pixmap_img);
     agrandissement.exec();}
    }
    else if(i == 5)
    {
        if(imageContouree){
        QImage *qimage = convIlpImageToQImage(imageContouree);
        pixmap_img = QPixmap::fromImage(*qimage);
        la.setPixmap(pixmap_img);
     agrandissement.exec();}
    }
    else
    {
        if(imageModelisee){
        QImage *qimage = convIlpImageToQImage(imageModelisee);
        pixmap_img = QPixmap::fromImage(*qimage);
        la.setPixmap(pixmap_img);
     agrandissement.exec();}
    }

}




IplImage* FenPrincipale::convQImageToIplImage(const QImage& _q)
{

    IplImage* imageRetour = cvCreateImage(cvSize(_q.width(),_q.height()),IPL_DEPTH_8U,3);

    CvScalar valeur;
    valeur.val[0] = 0;

    for(int i = 0; i < _q.width(); i++)
    {
        for(int j = 0; j < _q.height(); j++)
        {
            QRgb p(_q.pixel(i,j));
            valeur.val[0] = qGray(p)-1;
            valeur.val[1] = qGray(p)-1;
            valeur.val[2] = qGray(p)-1;
            cvSet2D(imageRetour,j,i,valeur);           
        }
    }

    return imageRetour;
}

QImage* FenPrincipale::convIlpImageToQImage(const IplImage *_i)
{
    QImage *imageRetour = new QImage(_i->width, _i->height, QImage::Format_RGB32);

    CvScalar valeur;
    valeur.val[0] = 0;
    valeur.val[1] = 0;
    valeur.val[2] = 0;

    for(int j = 0; j < _i->width; j++)
    {
        for(int i = 0; i < _i->height; i++)
        {            
            valeur = cvGet2D(_i,i,j);
            imageRetour->setPixel(j,i, qRgb(valeur.val[0],valeur.val[1],valeur.val[2]));
        }
    }

    return imageRetour;
}


void FenPrincipale::sauvegarder()
{
    bool ok;
//    cout << "test";
//    QInputDialog getText(this);
////    getText.set;
    QString nomImg = cheminDossierImages + QInputDialog::getText(this, tr("Sauvegarde image(droite)"),
                                                          tr("Veuillez entrer le nom de la sauvegarde:"), QLineEdit::Normal,
                                                          QString("exempl.png"), &ok);

    if(nomImg.size()>3)
    {
        switch(ui->tw_visuels2->currentIndex())
        {
        case 0: if(imageChargee) {if(convIlpImageToQImage(imageSource)->save(QString(nomImg))) std::cout << "Image " << nomImg.toStdString() << " sauvée dans ./images. " << std::endl;}
                else std::cout << "Echec enregistrement " << nomImg.toStdString() << " (image source)" << std::endl;
            break;
        case 1: if(imageChargee) {if(convIlpImageToQImage(imageModifiee)->save(QString(nomImg))) std::cout << "Image " << nomImg.toStdString() << " sauvée dans ./images. " << std::endl;}
                else std::cout << "Echec enregistrement " << nomImg.toStdString() << " (image seuillee)" << std::endl;
            break;
        case 2: if(imageChargee) {if(convIlpImageToQImage(carteGradient)->save(QString(nomImg))) std::cout << "Image " << nomImg.toStdString() << " sauvée dans ./images. " << std::endl;}
                else std::cout << "Echec enregistrement " << nomImg.toStdString() << " (image gradient)" << std::endl;
            break;
        case 3: if(imageChargee) {if(convIlpImageToQImage(cartePentes)->save(QString(nomImg))) std::cout << "Image " << nomImg.toStdString() << " sauvée dans ./images. " << std::endl;}
                else std::cout << "Echec enregistrement " << nomImg.toStdString() << " (image pentes)" << std::endl;
            break;
        case 4: if(imageChargee) {if(convIlpImageToQImage(imageAffinee)->save(QString(nomImg))) std::cout << "Image " << nomImg.toStdString() << " sauvée dans ./images. " << std::endl;}
                else std::cout << "Echec enregistrement " << nomImg.toStdString() << " (image affinage)" << std::endl;
            break;
        case 5: if(imageChargee) {if(convIlpImageToQImage(imageContouree)->save(QString(nomImg))) std::cout << "Image " << nomImg.toStdString() << " sauvée dans ./images. " << std::endl;}
                else std::cout << "Echec enregistrement " << nomImg.toStdString() << " (image fermeture)" << std::endl;
            break;
        case 6: if(imageChargee) {if(convIlpImageToQImage(imageModelisee)->save(QString(nomImg))) std::cout << "Image " << nomImg.toStdString() << " sauvée dans ./images. " << std::endl;}
                else std::cout << "Echec enregistrement " << nomImg.toStdString() << " (image modelisee)" << std::endl;
            break;
        }
    }
            else std::cout  << "Nom invalide pour l'enregistrement. " << std::endl;


}


void FenPrincipale::chargerImage()
{
    std::cout << "Chargement de ";
    nomImage = QString(cheminDossierImages + ui->cb_choixImg->currentText());
    cout << nomImage.toStdString() << endl;

    QImage qimage_img(nomImage);
    if(imageSource != NULL) cvReleaseImage(&imageSource);
    imageSource = convQImageToIplImage(qimage_img);


    QPixmap pixmap_img = QPixmap::fromImage(qimage_img);

    // Redimentionnement de l'image
    pixmap_img = pixmap_img.scaled(largeurImage,hauteurImage,Qt::KeepAspectRatio);

    ui->la_source1->clear();
    ui->la_source1->setPixmap(pixmap_img);

    ui->la_source2->clear();
    ui->la_source2->setPixmap(pixmap_img);

    ui->tw_visuels1->setCurrentIndex(0);
    ui->tw_visuels2->setCurrentIndex(0);

    imageChargee = true;
    gradientsNonRecalcules = true;

}

void FenPrincipale::chargerMasque()
{
    QString qs_masque = ui->cb_choixMasq->currentText();

   if(imageChargee && qs_masque != QString(".."))
   {
       gradientsNonRecalcules = false;
      cout << "Initialisation du masque";

      Matrix *masqueX; // 0°
      Matrix *masqueY; // 90°
      Matrix *masqueK; // 45°
      Matrix *masqueL; // 135°

      if(qs_masque == QString("Sobel"))
      {
          int XSobel[] =          { 1,  0, -1, 2,  0, -2, 1,  0, -1 };
          int YSobel[] =          { 1,  2,  1, 0,  0,  0,-1, -2, -1 };
          masqueX = new Matrix(3,3, XSobel);   masqueY = new Matrix(3,3, YSobel);   cout << " de Sobel. "  << endl;  // initialisation masque Sobel
          nomMasque = "Sobel";
      }
      else if(qs_masque == QString("Prewitt"))
      {
          int XPrewitt[] =        {-1,  0,  1, -1,  0,  1, -1,  0,  1 };
          int YPrewitt[] =        {-1, -1, -1, 0,  0,  0, 1,  1,  1 };
          masqueX = new Matrix(3,3, XPrewitt); masqueY = new Matrix(3,3, YPrewitt); cout << " de Prewitt. "<< endl;  // initialisation masque Prewitt
          nomMasque = "Prewitt";
      }
      else if(qs_masque == QString("Kirsh"))// kirsh
      {
          int Kirsh1[] =          {-3, -3,  5, -3,  0,  5, -3, -3,  5 };
          int Kirsh2[] =          {-3, -3, -3,-3,  0, -3, 5,  5,  5 };
          int Kirsh3[] =          {-3, -3, -3,  5,  0, -3,  5,  5, -3 };
          int Kirsh4[] =          { 5,  5, -3,  5,  0, -3, -3, -3, -3 };
          masqueX = new Matrix(3,3, Kirsh1);   masqueY = new Matrix(3,3, Kirsh2);   cout << " de Kirsh. "  << endl;  // initialisation masque Kirsh
          masqueK = new Matrix(3,3, Kirsh3);   masqueL = new Matrix(3,3, Kirsh4);
          nomMasque = "Kirsh";
      }
      else
      {
          int Robinson1[] =  { -1,0,1, -1,0,1,-1,0,1};
          int Robinson2[] =  {  1,1,1,0,0,0,-1,-1,-1};
          int Robinson3[] =  { 0 ,1,1,-1,0,1,-1,-1,0};
          int Robinson4[] =  { 1,1,0,1,0,-1,0,-1,-1};
          masqueX = new Matrix(3,3, Robinson1);   masqueY = new Matrix(3,3, Robinson2);   cout << " de Robinson. "  << endl;  // initialisation masque Robinson
          masqueK = new Matrix(3,3, Robinson3);   masqueL = new Matrix(3,3, Robinson4);
          nomMasque = "Robinson";
      }


      if(qs_masque != QString("Kirsh") && qs_masque != QString("Robinson")) gradient = new Gradient(imageSource, *masqueX, *masqueY, prog);
      else                                                                  gradient = new Gradient(imageSource, *masqueX, *masqueY, *masqueK, *masqueL, prog);

      ui->pg_progressionAlgo->setValue(100);
      gradientInitialise = true;

      // affichage carte des gradients

          if(carteGradient != NULL) cvReleaseImage(&carteGradient);
          carteGradient = cvCloneImage(imageSource);
          gradient->affecterCarteGradient(carteGradient);
          QImage *qimage_carteGradient = convIlpImageToQImage(carteGradient);
          QPixmap pixmap_imgGradients = QPixmap::fromImage(*qimage_carteGradient);

          // Redimentionnement de l'image
          pixmap_imgGradients = pixmap_imgGradients.scaled(largeurImage,hauteurImage,Qt::KeepAspectRatio);

          ui->la_carteGradient1->clear();
          ui->la_carteGradient1->setPixmap(pixmap_imgGradients);

          ui->la_carteGradient2->clear();
          ui->la_carteGradient2->setPixmap(pixmap_imgGradients);
          ui->tw_visuels2->setCurrentIndex(2);

      // affichage carte des pentes

          if(cartePentes != NULL) cvReleaseImage(&cartePentes);
          cartePentes = cvCloneImage(imageSource);
          cartePentes = gradient->creerCartePentes(cartePentes);
          QImage *qimage_cartePentes = convIlpImageToQImage(cartePentes);
          QPixmap pixmap_imgPentes = QPixmap::fromImage(*qimage_cartePentes);

          // Redimentionnement de l'image
          pixmap_imgPentes = pixmap_imgPentes.scaled(largeurImage,hauteurImage,Qt::KeepAspectRatio);

          ui->la_pentes1->clear();
          ui->la_pentes1->setPixmap(pixmap_imgPentes);

          ui->la_pentes2->clear();
          ui->la_pentes2->setPixmap(pixmap_imgPentes);

   }
}

void FenPrincipale::seuillageImage()
{
    if(gradientsNonRecalcules)
    {
       cout << "Une nouvelle image a ete chargee et de nouveaux gradients ont ete calules, le masque va être réinitialisé avec les nouvelles valeurs." << endl;
       chargerMasque();
    }

    QString qs_seuillage = ui->cb_seuill->currentText();

    if(gradientInitialise && qs_seuillage != QString(".."))
    {

       if(cloneImageSource != NULL)  cvReleaseImage(&cloneImageSource);
       if(imageModifiee != NULL)  cvReleaseImage(&imageModifiee);
       cloneImageSource = cvCloneImage(imageSource);
        if(qs_seuillage == QString("Global"))
        {
            if(seuillagePrecedent != qs_seuillage)
            {
                ui->hs_seuill->setRange(0,300);
                ui->hs_seuill->setValue(100);
            }
            seuillagePrecedent = qs_seuillage;

            imageModifiee = seuillage.seuillageGlobal(cloneImageSource,*gradient, ui->hs_seuill->value(),prog);
            ui->pg_progressionAlgo->setValue(100);
            nomSeuillage = "Global";
        }
        else if(qs_seuillage == QString("Local"))
        {
            if(seuillagePrecedent != qs_seuillage)
            {
                ui->hs_seuill->setRange(0,10);
                ui->hs_seuill->setValue(2);
            }
            seuillagePrecedent = qs_seuillage;

            imageModifiee = seuillage.seuillageLocal(cloneImageSource,*gradient, ui->hs_seuill->value(),prog);
            ui->pg_progressionAlgo->setValue(100);
            nomSeuillage = "Local";
        }
        else
        {
            if(seuillagePrecedent != qs_seuillage)
            {

                ui->hs_seuill->setRange(0,650);
                if(nomMasque == QString("Robinson")) ui->hs_seuill->setValue(75);
                else ui->hs_seuill->setValue(350);

            }
             seuillagePrecedent = qs_seuillage;


            imageModifiee = seuillage.seuillageHysteresis(cloneImageSource,*gradient, ui->hs_seuill->value(),true,prog);
            ui->pg_progressionAlgo->setValue(100);
            nomSeuillage = "Hysteresis";
        }

        imageSeuillee = true;

        // affichage image seuillee

            QImage *qimage_imageModifiee = convIlpImageToQImage(imageModifiee);

            QPixmap pixmap_img = QPixmap::fromImage(*qimage_imageModifiee);

            // Redimentionnement de l'image
            pixmap_img = pixmap_img.scaled(largeurImage,hauteurImage,Qt::KeepAspectRatio);

            ui->la_seuillage1->clear();
            ui->la_seuillage1->setPixmap(pixmap_img);

            ui->la_seuillage2->clear();
            ui->la_seuillage2->setPixmap(pixmap_img);

            ui->tw_visuels2->setCurrentIndex(1);

    }



}

void FenPrincipale::affinageImage()
{
    QString qs_affinage = ui->cb_choixAffinage->currentText();

    if(imageSeuillee && qs_affinage != QString(".."))
    {
        Gradient *g = new Gradient(*gradient);

        if(imageAffinee != NULL)  cvReleaseImage(&imageAffinee);

        if(qs_affinage == QString("Amincissement"))
        {
            cout << "Affinage amincissment." << endl;
            imageAffinee = seuillage.seuillageAmincissementContours(imageModifiee,*g, prog);
            ui->pg_progressionAlgo->setValue(100);

        }
        else
        {
            cout << "Affinage extaction maximas locaux." << endl;
            imageAffinee = seuillage.seuillageExtractionMaximasLocaux(imageModifiee,*g, prog);
            ui->pg_progressionAlgo->setValue(100);
        }

        imageAff = true;

        // création ensemble connexes
            seuillage.creationEnsembleConnexe(imageAffinee,*g);
            ensembleConnexeCree = true;

        // affichage image affinee

            QImage *qimage_imageAffinee = convIlpImageToQImage(imageAffinee);

            QPixmap pixmap_img = QPixmap::fromImage(*qimage_imageAffinee);

            // Redimentionnement de l'image
            pixmap_img = pixmap_img.scaled(largeurImage,hauteurImage,Qt::KeepAspectRatio);

            ui->la_affinage1->clear();
            ui->la_affinage1->setPixmap(pixmap_img);

            ui->la_affinage2->clear();
            ui->la_affinage2->setPixmap(pixmap_img);

            ui->tw_visuels2->setCurrentIndex(4);



    }
}


void FenPrincipale::fermetureContoursImage()
{
    if(imageAff)
    {
        Gradient *g = new Gradient(*gradient);

        if(imageContouree != NULL) cvReleaseImage(&imageContouree);
        imageContouree = seuillage.fermetureContours(imageAffinee,*g, ui->hs_gradMinFermeture->value(), prog);
        ui->pg_progressionAlgo->setValue(100);

        imageFermee = true;

        QImage *qimage_imageContouree = convIlpImageToQImage(imageContouree);

        QPixmap pixmap_img = QPixmap::fromImage(*qimage_imageContouree);

        // Redimentionnement de l'image
        pixmap_img = pixmap_img.scaled(largeurImage,hauteurImage,Qt::KeepAspectRatio);

        ui->la_fermeture1->clear();
        ui->la_fermeture1->setPixmap(pixmap_img);

        ui->la_fermeture2->clear();
        ui->la_fermeture2->setPixmap(pixmap_img);

        ui->tw_visuels2->setCurrentIndex(5);
    }
}


void FenPrincipale::modelisationSegments()
{
    if(imageFermee)
    {

        if(imageModelisee != NULL) cvReleaseImage(&imageModelisee);
        imageModelisee = seuillage.modelisationSegments(imageContouree,ui->sp_tailleSegements->value(), ui->cb_colDiff->isChecked());

        QImage *qimage_imageModelisee = convIlpImageToQImage(imageModelisee);

        QPixmap pixmap_img = QPixmap::fromImage(*qimage_imageModelisee);

        // Redimentionnement de l'image
        pixmap_img = pixmap_img.scaled(largeurImage,hauteurImage,Qt::KeepAspectRatio);


        ui->la_modelisation1->clear();
        ui->la_modelisation1->setPixmap(pixmap_img);

        ui->la_modelisation2->clear();
        ui->la_modelisation2->setPixmap(pixmap_img);

        ui->tw_visuels2->setCurrentIndex(6);
    }
}

void FenPrincipale::modificationProgression() { ui->pg_progressionAlgo->setValue(prog->pourcentage()); }

void FenPrincipale::on_pb_chargerImg_clicked()                       {chargerImage();}

void FenPrincipale::on_pb_masque_clicked()                           {chargerMasque();}

void FenPrincipale::on_cb_choixMasq_currentIndexChanged(QString )    {chargerMasque();}

void FenPrincipale::on_cb_seuill_currentIndexChanged(QString )       {seuillageImage();}

void FenPrincipale::on_pb_Affiner_clicked()                          {affinageImage();}

void FenPrincipale::on_cb_choixAffinage_currentIndexChanged(QString ){affinageImage();}

void FenPrincipale::on_pb_fermeture_clicked()                        {fermetureContoursImage();}

void FenPrincipale::on_pb_modeSegments_clicked()                     {modelisationSegments();}
