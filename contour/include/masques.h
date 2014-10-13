#ifndef _MASQUES_
#define _MASQUES_

/// Fichier contenant les masques pouvant être utilisés dans le programme


        int XPrewitt[] =          {-1,  0,  1,
                                 -1,  0,  1,
                                 -1,  0,  1 };


        int YPrewitt[] =        {-1, -1, -1,
                                  0,  0,  0,
                                  1,  1,  1 };

        int XSobel[] =          { 1,  0, -1,
                                  2,  0, -2,
                                  1,  0, -1 };

        int YSobel[] =          { 1,  2,  1,
                                  0,  0,  0,
                                 -1, -2, -1 };


        int Kirsh1[] =          {-3, -3,  5,
                                 -3,  0,  5,
                                 -3, -3,  5 };

        int Kirsh2[] =          {-3, -3, -3,
                                 -3,  0, -3,
                                  5,  5,  5 };

        int Kirsh3[] =          {-3, -3, -3,
                                  5,  0, -3,
                                  5,  5, -3 };

        int Kirsh4[] =          { 5,  5, -3,
                                  5,  0, -3,
                                 -3, -3, -3 };





#endif
