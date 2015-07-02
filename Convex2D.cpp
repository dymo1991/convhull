#include "Convex2D.h"
#include <math.h>

Convex2D::Convex2D()
{
    SL=new SegmentListEx;
};

Convex2D::~Convex2D()
{
    delete SL;
};

// створення початкового многокутника (трикутника)
void Convex2D::CreateBegPol(PointList *PL)
{
    Point p1;
        //pmin, pmax;
    double min, max;
   
    PL->GetFirst(&p1);
    min=max=p1.GetX();
    Point pmin(p1);
    Point pmax(p1);
    while(PL->GetNext(&p1))
    {
        if (p1.GetX()>max)
        {
            max=p1.GetX();
            pmax.Init(p1);
        };
        if (p1.GetX()<min)
        {
            min=p1.GetX();
            pmin.Init(p1);
        };
    };

    if (min==max)
    {
        PL->GetFirst(&p1);
        min=max=p1.GetY();
        pmin.Init(p1);
        pmax.Init(p1);
        while(PL->GetNext(&p1))
        {
            if (p1.GetY()>max)
            {
                max=p1.GetY();
                pmax.Init(p1);
            };
            if (p1.GetY()<min)
            {
                min=p1.GetY();
                pmin.Init(p1);
            };
        };
    };

    SegmentEx *pS= new SegmentEx(pmin, pmax);
    PL->FindAndRemoveAll(pmin);
    PL->FindAndRemoveAll(pmax);

    PL->GetFirst(&p1);
    max=pS->Square(p1);
    pmax.Init(p1);
    while(PL->GetNext(&p1))
    {
        if (pS->Square(p1)>max)
        {
            max=pS->Square(p1);
            pmax.Init(p1);
        };
    };
    PL->FindAndRemoveAll(pmax);
    SL->Add(pS);
    pS->GetA(&p1);
    SegmentEx *pS1= new SegmentEx(p1, pmax);
    SL->Add(pS1);
    pS->GetB(&p1);
    SegmentEx *pS2= new SegmentEx(p1, pmax);
    SL->Add(pS2);

};

// визначення чи лежить точка в серединi трикутника утвореного ребром многокутника i центром многокутника
int Convex2D::IsPointIntoTrian(Point T, SegmentEx S)
{
    double sq, sq1, sq2, sq3;
    Point T1;

    if (S.Into(T))
        return 1;
    sq=S.Square(insidePoint);
    sq1=S.Square(T);
    S.GetA(&T1);
    Segment S1(T1, insidePoint);
    sq2=S1.Square(T);
    S.GetB(&T1);
    S1.Init(T1, insidePoint);
    sq3=S1.Square(T);
    if (fabs(sq-sq1-sq2-sq3)<eps)
        return 1;
    else
        return 0;
};

// визначення чи лежить точка всерединi многокутника
int Convex2D::IsPointIntoPolyg(Point T)
{
    SegmentEx *pS;

    CentrOfPolyg();
    SL->GetFirst(&pS);
    do
    {
        if(IsPointIntoTrian(T, *pS))
            return 1;
    }
    while(SL->GetNext(&pS));
    return 0;
};

// визначення центру многокутника точки insidePoint
void Convex2D::CentrOfPolyg()
{
    InOuPList Ptr;
    Point P;
    double X, Y, Z;
    int n;

    n=0;
    X=Y=Z=0;
    Ptr.SegmToPoints((SegmentList *)SL);
    Ptr.PL->GetFirst(&P);
    do
    {
        X=X+P.GetX();
        Y=Y+P.GetY();
        Z=Z+P.GetZ();
        n++;
    }
    while (Ptr.PL->GetNext(&P));
    X=X/n;
    Y=Y/n;
    Z=Z/n;
    insidePoint.Init(X, Y, Z);
};

// видалення точок, що лежать всерединi многокутника
void Convex2D::DelPointIntoPolyg(PointList *PL)
{
    Point T;

    PL->GetFirst(&T);
    do
    {
        if (IsPointIntoPolyg(T))
        {
            PL->FindAndRemoveAll(T);
            PL->GetFirst(&T);
        }
    }
    while (PL->GetNext(&T));


};

// чи лежить точка T над ребром многокутника
int Convex2D::IsPointOverRip(Point T, SegmentEx *S)
{
    Point A, B;
    double x1, x2, x3, y1, y2, y3, z1, z2, z3, x4, y4, z4;
    double det1, det2;

    S->GetA(&A);
    S->GetB(&B);

    x1=insidePoint.GetX()-A.GetX();
    y1=insidePoint.GetY()-A.GetY();
    z1=insidePoint.GetZ()-A.GetZ();

    x2=B.GetX()-A.GetX();
    y2=B.GetY()-A.GetY();
    z2=B.GetZ()-A.GetZ();

    x3=y1*z2-y2*z1;
    y3=x2*z1-x1*z2;
    z3=x1*y2-x2*y1;

    x4=T.GetX()-A.GetX();
    y4=T.GetY()-A.GetY();
    z4=T.GetZ()-A.GetZ();

    det1=x1*y2*z3+x2*y3*z1+x3*y1*z2-x3*y2*z1-x1*y3*z2-x2*y1*z3;
    det2=x4*y2*z3+x2*y3*z4+x3*y4*z2-x3*y2*z4-x4*y3*z2-x2*y4*z3;

    if (((det1>0) && (det2>0)) || ((det1<0) && (det2<0)))
        return 0;
    else
        return 1;
};

// додавання списку точок, що лежать над ребром, для кожного ребра з SL
void Convex2D::AddPointListOverRip(PointList *PL)
{
    PointList PR;
    Point P;
    SegmentEx *pS;

    PR.Init(PL);

    SL->GetFirst(&pS);
    do
    {
        PR.GetFirst(&P);
        do
        {
            if (IsPointOverRip(P, pS))
            {
                pS->listVertex.Add(P);
                PR.FindAndRemoveAll(P);
                PR.GetFirst(&P);
            }
        }
        while (PR.GetNext(&P));
        if (PR.IsEmpty())
            break;
    }
    while (SL->GetNext(&pS));
};

// створення нового многокутника
void Convex2D::CreateNewPol(PointList *PL)
{
    Point P, PDist;
    double Dist;
    SegmentEx *pS;
    SegmentListEx SS;

    pS=new SegmentEx;

    AddPointListOverRip(PL);

    SL->GetFirst(&pS);
    while (!SL->IsEmpty())
    {
        if(!pS->listVertex.IsEmpty())
        {
            pS->listVertex.GetFirst(&P);
            Dist=pS->Square(P);
            PDist.Init(P);
            do
            {
                if (pS->Square(P)>Dist)
                {
                    Dist=pS->Square(P);
                    PDist.Init(P);
                };
            }
            while (pS->listVertex.GetNext(&P));

            PL->FindAndRemoveAll(PDist);
            pS->GetA(&P);
            SegmentEx *pS1 = new SegmentEx(P, PDist);
            SS.Add(pS1);
            pS->GetB(&P);
            SegmentEx *pS2 = new SegmentEx(P, PDist);
            SS.Add(pS2);
            SL->FindAndRemoveAll(pS);
        }
        else
        {
            SS.Add(pS);
            SL->FindAndRemoveAll(pS);
        };
        SL->GetFirst(&pS);
    }

    while (!SS.IsEmpty())
    {
        SS.Remove(&pS);
        SL->Add(pS);
    };
};

// визначення списку точок з яких складається многокутник
void Convex2D::Calculation(PointList *PL)
{
    InOuPList Ptr;
    Point P;

    CreateBegPol(PL);
    DelPointIntoPolyg(PL);
    while(!PL->IsEmpty())
    {
        CreateNewPol(PL);
        if (PL->IsEmpty())
            break;
        DelPointIntoPolyg(PL);
    };
    Ptr.SegmToPoints((SegmentList *)SL);

    PL->Init(Ptr.PL);

};

//---------------------------------------------------------------------------------------
#ifdef  CONVEX2D_DEBUG

#include <stdio.h>

int main()
{
    InOuPList PtL;
    Convex1D  C;
    Convex2D  CX;

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
        if (PtL.GetPointDimension()==2)
        {
            CX.Calculation (PtL.PL);
            PtL.DelVertexDoubles();
            PtL.OutputPointList();
        }
    };
};
#endif // CONVEX2D_DEBUG