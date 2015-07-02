#include "convex0D.h"

Convex0D::Convex0D()
{
};

Convex0D::~Convex0D()
{
};

void Convex0D::Calculation(PointList *PL)
{
    Point A;
    
    PL->GetFirst(&A);
         
    PL->Clear();
    PL->Add(A);
};


