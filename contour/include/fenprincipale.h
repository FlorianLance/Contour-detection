#ifndef FENPRINCIPALE_H
#define FENPRINCIPALE_H

#include <QtGui>


#include "gradient.h"
#include "seuillage.h"
#include "progressionAlgo.h"
#include "fenprincipale_ui.h"

//#include <QString>
//#include <QDir>
//#include <QComboBox>
//#include <QPushButton>
//#include <QDialog>
//#include <QInputDialog>

#include <iostream>

namespace Ui {
    class FenPrincipale;
}

class FenPrincipale : public QMainWindow {
    Q_OBJECT

public:
    FenPrincipale(QWidget *parent = 0);
    ~FenPrincipale();


    /** @brief convertit une Qimage en iplimage */
    IplImage* convQImageToIplImage(const QImage& );

    /** @brief convertit une iplimage en Qimage */
    QImage* convIlpImageToQImage(const IplImage *);

private:
    Ui::FenPrincipale *ui;

    bool imageChargee;
    bool gradientInitialise;
    bool imageSeuillee;
    bool imageAff;
    bool gradientsNonRecalcules;
    bool ensembleConnexeCree;
    bool imageFermee;

    QString nomImage;
    QString cheminDossierImages;
    QString nomMasque;
    QString nomSeuillage;
    QString seuillagePrecedent;

    IplImage* imageSource;
    IplImage* imageModifiee;
    IplImage* carteGradient;
    IplImage* cartePentes;
    IplImage* cloneImageSource;
    IplImage* imageAffinee;
    IplImage* imageContouree;
    IplImage* imageModelisee;

    uint largeurImage;
    uint hauteurImage;

    Gradient *gradient;
    Seuillage seuillage;

    Progression *prog;

public slots:

    void sauvegarder();
    void chargerImage();
    void chargerMasque();
    void seuillageImage();
    void affinageImage();
    void fermetureContoursImage();
    void agrandirImage();
    void modificationProgression();
    void modelisationSegments();


private slots:
    void on_pb_modeSegments_clicked();
    void on_pb_masque_clicked();
    void on_pb_fermeture_clicked();
    void on_cb_choixAffinage_currentIndexChanged(QString );
    void on_pb_Affiner_clicked();
    void on_cb_seuill_currentIndexChanged(QString );
    void on_cb_choixMasq_currentIndexChanged(QString );
    void on_pb_chargerImg_clicked();
};

#endif // FENPRINCIPALE_H
