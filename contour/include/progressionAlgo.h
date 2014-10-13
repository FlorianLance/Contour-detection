#ifndef PROGRESSIONALGO_H
#define PROGRESSIONALGO_H

#include <QObject>

class Progression: public QObject

{
    Q_OBJECT


    public:
        Progression(const int);

        int v()const;
        int pourcentage()const;
        void setV(const int);
        void setMax(const int);

        void maj();

    signals:
        void modificationProgression();

    private:
        int vmax;
        int vactu;
};


#endif // PROGRESSIONALGO_H
