#include "InOuPList.h"
#include <stdio.h>

InOuPList::InOuPList()
{
    PL=new PointList;
};

InOuPList::~InOuPList()
{
};

// фунцiя перетворення списку  граней у список точок, з яких цi гранi складаються
void InOuPList::FacetToPoints(FacetList *FL)
{
    Facet       *pF;
    PointList   PT;
    Point       T, A, B, C;

    pF=new Facet;
    if ( FL->GetFirst(&pF) )
    {
        do {
            pF->GetA(&A);
            pF->GetB(&B);
            pF->GetC(&C);
            PT.Add(A);
            PT.Add(B);
            PT.Add(C);
        } while (FL->GetNext(&pF));
    };

    while(!PT.IsEmpty())
    {
        PT.Remove(&T);
        PL->Add(T);
        PT.FindAndRemoveAll(T);
    };

};
// фунцiя перетворення списку ребер у список точок, з яких цi гранi складаються
void InOuPList::SegmToPoints(SegmentList *SL)
{
    Segment       *pS;
    PointList   PT;
    Point       T, A, B;

    if ( SL->GetFirst(&pS) )
    {
        do {
            pS->GetA(&A);
            pS->GetB(&B);
            PT.Add(A);
            PT.Add(B);
        } while (SL->GetNext(&pS));
    };

    while(!PT.IsEmpty())
    {
        PT.Remove(&T);
        PL->Add(T);
        PT.FindAndRemoveAll(T);
    };
};

// видалення зi списку точок точок, що лежать мiж двома iншими точками з цього  списку
void InOuPList::DelVertexDoubles()
{
    PointList PP;
    Point P, L, A, B;
    Segment *pR;
    SegmentList SR;

    PP.Init(PL);

    // створення всiх можливих вiдрiзкiв, що зєднують усi точки списку точок
    while ( !PP.IsEmpty())
    {
        PP.Remove(&P);
        if ( PL->GetFirst(&L) )
        {
            do {
                if (!P.IsMatch(L))
                {
                    Segment *pR = new Segment(P, L);
                    SR.Add(pR);
                }
            } while (PL->GetNext(&L));
        };
    };

    // видалення iз списку точок, тих точок, що лежать всерединi вiдрiзкiв списку вiдрiзкiв
    while ( !SR.IsEmpty())
    {
        SR.Remove(&pR);
        pR->GetA(&A);
        pR->GetB(&B);
        PL->GetFirst(&P);
        do {
            if ((pR->Into(P)) && (!P.IsMatch(A)) &&(!P.IsMatch(B)) )
            {
                PL->FindAndRemoveAll(P);
                PL->GetFirst(&P);
            }
        } while (PL->GetNext(&P));
    };
};

// введення списку точок
void  InOuPList::InputPointList()
{
    int i, n;
    Point P;

    fprintf(stderr, "Enter quantity of points \n");
    scanf("%d", &n);
    for (i=0; i<n; i++)
    {
        fprintf( stderr, "Enter point #%d ", i+1);
        P.InputPoint();
        PL->Add(P);
    }
};

// Output list of points
void InOuPList::OutputPointList ()
{
    Point P;

    while (!PL->IsEmpty())
    {
        PL->Remove(&P);
        P.OutputPoint();
        printf("\n");
    }
};

// визначення розмiрностi опуклої оболонки
int InOuPList::GetPointDimension ()
{
    Point   PCurr, PCurr2;
    Segment S;
//    Facet   F;
    int     go, go2;

    PL->GetFirst(&PCurr);
    Point PMinX(PCurr);    Point PMinY(PCurr);    Point PMinZ(PCurr);
    Point PMaxX(PCurr);    Point PMaxY(PCurr);    Point PMaxZ(PCurr);

    while( PL->GetNext(&PCurr) )            // 1) find the minimum and maximum point to each direction
    {
        if (PCurr.GetX()<PMinX.GetX())
            PMinX.Init(PCurr);
        if (PCurr.GetY()<PMinY.GetY())
            PMinY.Init(PCurr);
        if (PCurr.GetZ()<PMinZ.GetZ())
            PMinZ.Init(PCurr);
        if (PCurr.GetX()>PMaxX.GetX())
            PMaxX.Init(PCurr);
        if (PCurr.GetY()>PMaxY.GetY())
            PMaxY.Init(PCurr);
        if (PCurr.GetZ()>PMaxZ.GetZ())
            PMaxZ.Init(PCurr);
    };

    if ( PMinX.IsMatch(PMaxX) && PMinY.IsMatch(PMaxY) && PMinZ.IsMatch(PMaxZ) )
        return 0;                                   // 2) all points are coplanars
    else if (( PMinX.IsMatch(PMaxX) && (PMinY.IsMatch(PMaxY) || PMinZ.IsMatch(PMaxZ)) ) ||
        ( (PMinX.IsMatch(PMaxX) || PMinY.IsMatch(PMaxY)) && PMinZ.IsMatch(PMaxZ) )      ||
        ( PMinY.IsMatch(PMaxY) && (PMinX.IsMatch(PMaxX) || PMinZ.IsMatch(PMaxZ) )))
        return 1;
    
    if (PMaxX==PMinX)
		S.Init(PMaxY, PMinY);                           // 3) check to colinear   
    else
		S.Init(PMaxX, PMinX);                           // 3) check to colinear
	PL->GetFirst(&PCurr);
    go = go2 = 1;
    while ( go & go2 )
    {
        if (S.Square(PCurr)>0)
            go = 0;
        else
            go2 = PL->GetNext(&PCurr);
    }
    if (go)
        return 1;   // all points are colinears


    Facet F(PMaxX, PMinX, PCurr);                     // 4) check to coplanar
    PL->GetFirst(&PCurr2);
    go = go2 = 1;
    while ( go && go2 )
    {
        if (F.DistanceToPoint(PCurr2)>0)
            go = 0;
        else
            go2 = PL->GetNext(&PCurr2);
    }
    if (go)
        return 2;
                                           // all points are coplanar
    return 3;
};


//----------------------------------------------------------------
#ifdef  INOUPLIST_DEBUG
int main()
{
    InOuPList PtL;

    PtL.InputPointList();
    PtL.DelVertexDoubles();
    PtL.OutputPointList();

};
#endif // INOUPLIST_DEBUG