#include <math.h>
#include <stdio.h>

#include "facet.h"

Facet::Facet()
{
};


Facet::Facet(Point T1, Point T2, Point T3):
    A(T1.GetX(), T1.GetY(), T1.GetZ()),
    B(T2.GetX(), T2.GetY(), T2.GetZ()),
    C(T3.GetX(), T3.GetY(), T3.GetZ())
{
};

Facet::Facet( Point P1, Point P2, Point P3, Point T )
{
    Init(P1, P2, P3);
    if (IsOnFacet(T)<0)
        return;
    Init(P1, P3, P2);
};

Facet::~Facet()
{
};

void Facet::Init(Point NewA, Point NewB, Point NewC)
{
    A=NewA;
    B=NewB;
    C=NewC;
};

void Facet::GetA(Point *T)
{
    T->Init(A);
};

void Facet::GetB(Point *T)
{
    T->Init(B);
};

void Facet::GetC(Point *T)
{
    T->Init(C);
};

void Facet::PrintCoordinates( int IsPrintList )
{
    Point G, *pG=&G;

    A.OutputPoint(); printf("-");
    B.OutputPoint(); printf("-");
    C.OutputPoint();
    printf("\n");

    if ( IsPrintList && listVertices.GetFirst(pG) )
    {
        do {
            printf("\t ---> ");
            pG->OutputPoint();
            printf("\n");
        } while (listVertices.GetNext(pG));
    };
}

/*
void Facet::Create(Segment R, Point T)
{
    Point A1, B1;

    R.GetA(&A1);
    R.GetB(&B1);
    A.Init(A1);
    B.Init(B1);
    C.Init(T);
};
*/
// Перевiрка чи лежить точка Т на площинi
// якщо результат функцiї>0 над площиною, якщо менше пiд площиною, на площинi=0
double Facet::IsOnFacet(Point T)
{
    return ((T.GetX()-A.GetX())*(B.GetY()-A.GetY())*(C.GetZ()-A.GetZ())+
        (T.GetY()-A.GetY())*(B.GetZ()-A.GetZ())*(C.GetX()-A.GetX())+
        (T.GetZ()-A.GetZ())*(C.GetY()-A.GetY())*(B.GetX()-A.GetX())-
        (T.GetZ()-A.GetZ())*(B.GetY()-A.GetY())*(C.GetX()-A.GetX())-
        (T.GetX()-A.GetX())*(B.GetZ()-A.GetZ())*(C.GetY()-A.GetY())-
        (T.GetY()-A.GetY())*(C.GetZ()-A.GetZ())*(B.GetX()-A.GetX()));
};

// Обчислення вiдстаннi вiд точки Т до площини
double Facet::DistanceToPoint(Point T)
{
    return ((fabs(IsOnFacet(T)))/sqrt(((B.GetY()-A.GetY())*(C.GetZ()-A.GetZ())-
        (C.GetY()-A.GetY())*(B.GetZ()-A.GetZ()))*
        ((B.GetY()-A.GetY())*(C.GetZ()-A.GetZ())-
        (C.GetY()-A.GetY())*(B.GetZ()-A.GetZ()))+
        ((C.GetX()-A.GetX())*(B.GetZ()-A.GetZ())-
        (B.GetX()-A.GetX())*(C.GetZ()-A.GetX()))*
        ((C.GetX()-A.GetX())*(B.GetZ()-A.GetZ())-
        (B.GetX()-A.GetX())*(C.GetZ()-A.GetX()))+
        ((B.GetX()-A.GetX())*(C.GetY()-A.GetY())-
        (C.GetX()-A.GetX())*(B.GetY()-A.GetY()))*
        ((B.GetX()-A.GetX())*(C.GetY()-A.GetY())-
        (C.GetX()-A.GetX())*(B.GetY()-A.GetY()))));
};

// Перевiрка чи є поточна грань сусiдньою до гранi F
int Facet::NeighborFacet(Facet F)
{
    if (((A.IsMatch(F.A)) && ((B.IsMatch(F.B)) || (C.IsMatch(F.C)) || (B.IsMatch(F.C)) || C.IsMatch(F.B)))  ||
        ((A.IsMatch(F.B)) && ((B.IsMatch(F.A)) || (C.IsMatch(F.C)) || (B.IsMatch(F.C)) || C.IsMatch(F.A)))   ||
        ((A.IsMatch(F.C)) && ((B.IsMatch(F.B)) || (C.IsMatch(F.A)) || (B.IsMatch(F.A)) || C.IsMatch(F.B))))
       return 1;
    else
       return 0;
};

// Перевiрка чи гранi спiвпадають
int Facet::IsMatch (Facet F)
{
    if (((A.IsMatch(F.A)) && ((B.IsMatch(F.B)) && (C.IsMatch(F.C))) || ((B.IsMatch(F.C)) && C.IsMatch(F.B)))  ||
        ((A.IsMatch(F.B)) && ((B.IsMatch(F.A)) && (C.IsMatch(F.C))) || ((B.IsMatch(F.C)) && C.IsMatch(F.A)))  ||
        ((A.IsMatch(F.C)) && ((B.IsMatch(F.B)) && (C.IsMatch(F.A))) || ((B.IsMatch(F.A)) && C.IsMatch(F.B))))
       return 1;
    else
       return 0;
};

// Функцiя, що повертає спiльне ребро для сусiднiх граней
Segment Facet::NeighborSegm(Facet F)
{
    Segment S;
    if (A.IsMatch(F.A))
    {
        if ((B.IsMatch(F.B)) || (B.IsMatch(F.C)))
            S.Init(A, B);
        else if ((C.IsMatch(F.C)) ||  (C.IsMatch(F.B)))
            S.Init(A, C);
    }
    else if (A.IsMatch(F.B))
    {
        if ((B.IsMatch(F.A)) || (B.IsMatch(F.C)))
            S.Init(A, B);
        else if ((C.IsMatch(F.C)) || (C.IsMatch(F.A)))
            S.Init(A, C);
    }
    else if (A.IsMatch(F.C))
    {
        if ((B.IsMatch(F.B)) || (B.IsMatch(F.A)))
            S.Init(A, B);
        else if ((C.IsMatch(F.B)) || (C.IsMatch(F.A)))
            S.Init(A, C);
    }
    return S;
};

// Create facet from points A,B,C with positive IsOnFacet from its to T
/*
void Facet::CreateOrientatedFace( Point P1, Point P2, Point P3, Point T )
{
    Init(P1, P2, P3);
    if (IsOnFacet(T)<0)
        return;
    Init(P1, P3, P2);
};
*/
//----------------------------------------------------------------------------


#ifdef  FACET_DEBUG

#define N 2
int main()
{
    Point A, B, C, T, D, E, G;
    Point A1, B1;
    Segment S, S1;
    Facet F[N], FF;
    PointList Lt;
    int i;
    Point Pt, Pt1;

    printf("Enter coordinate of point T\n");
    T.InputPoint();

    for ( i = 0; i < N; i++ )
    {
        printf("Enter coordinate of point F%d.A\t", i+1);
        A.InputPoint();
        printf("Enter coordinate of point F%d.B\t", i+1);
        B.InputPoint();
        printf("Enter coordinate of point F%d.C\t", i+1);
        C.InputPoint();
        F[i].Init(A, B, C);

        printf("Facet #%d : ", i+1);
        if (F[i].IsOnFacet(T)>0)
            printf("OverFacet\t");
        else if (F[i].IsOnFacet(T)==0)
            printf("IntoFacet\t");
        else if (F[i].IsOnFacet(T)<0)
            printf("UnderFacet\t");
        printf("DistanceToPoint=%lf\n", F[i].DistanceToPoint(T));
        F[i].PrintCoordinates(0);
    };

    S.Init( A, B );
    FF.Create(S, T);
    FF.PrintCoordinates(0);

    for ( i = 1; i < N; i++ )
    {
        if ( F[0].NeighborFacet(F[i]) )
        {
            printf("Facet %d is neighborhood to 1\n", i+1);
            S=F[0].NeighborSegm(F[i]);
            S.GetA(&A1);
            S.GetA(&B1);
            printf("X=%lf, Y=%lf, Z=%lf\n", A1.GetX(), A1.GetY(), A1.GetZ());
            printf("X=%lf, Y=%lf, Z=%lf\n", B1.GetX(), B1.GetY(), B1.GetZ());
        }
        else
            printf("Facet %d is not neighborhood to 1\n", i+1);

        if (F[0].IsMatch(F[i]))
            printf("facet %d is match to 1", i+1);
        else
            printf("facet %d is not match to 1", i+1);
    };
};

#endif // FACET_DEBUG
