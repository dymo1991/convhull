#include "Convex1D.h"

//#define  CONVEX1D_DEBUG 1

Convex1D::Convex1D()
{
};

Convex1D::~Convex1D()
{
};

int Convex1D::Build1DHull (Segment *S, PointList *PL)
{
    Point P1, P2, P3;

    PL->GetFirst(&P1);
    PL->GetNext(&P2);
    S->Init(P1, P2);
    while (PL->GetNext(&P3))
    {
        if ((S->OnLine(P3)) && (!S->Into(P3)))
        {
            Point A, B;
			S->GetA(&A);
			S->GetB(&B);
			
			if (P3.DistanceTo(A)>P3.DistanceTo(B))
                S->Init(A, P3);
            else
                S->Init(B, P3);
        }
    }
    S->GetA(&P1);
    S->GetB(&P2);
    
    if (P1 == P2)
        return 0;
    else
        return 1;
};

// визначення списку точок
void Convex1D::Calculation(PointList *PL)
{
    Segment S;
    Point A, B;

    Build1DHull (&S, PL);
    S.GetA(&A);
    S.GetB(&B);

    PL->Clear();
    PL->Add(A);
    PL->Add(B);
};


//---------------------------------------------------------------------------------------
#ifdef  CONVEX1D_DEBUG

#include <stdio.h>

int main()
{
    InOuPList PtL;
    Convex1D  C;
    Segment S;
    Point A, B;

    PtL.InputPointList();
    if (PtL.PL->IsEmpty)
        printf("List is Empty\n");
    else
    {
        if (PtL.GetPointDimension()==0)
        {
            PtL.PL->GetFirst(&A);
            printf("X=%lf, Y=%lf, Z=%lf\n", A.GetX(), A.GetY(), A.GetZ());
        };
        if (PtL.GetPointDimension()==1)
        {
            C.Build1DHull (&S, PtL.PL);
            S.GetA(&A);
            S.GetB(&B);
            printf("X=%lf, Y=%lf, Z=%lf\n", A.GetX(), A.GetY(), A.GetZ());
            printf("X=%lf, Y=%lf, Z=%lf\n", B.GetX(), B.GetY(), B.GetZ());
        }
    };
};
#endif // CONVEX1D_DEBUG