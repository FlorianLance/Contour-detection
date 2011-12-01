
#include "../include/progressionAlgo.h"

Progression::Progression(const int _max): vmax(_max), vactu(0){}

int Progression::v()const {return vactu;}

int Progression::pourcentage()const {return 100*vactu / vmax ;}

void Progression::setV(const int _v){ vactu = _v;}

void Progression::setMax(const int _vmax){vmax = _vmax;}

void Progression::maj(){emit modificationProgression();};
