#include <stdio.h>
#include <math.h>

#include "convex.h"


Convex::Convex()
{
};

Convex::~Convex()
{
};

void  Convex::InputPointList(PointList *Ps)
{
    int i, n;
    Point P;

    printf("Enter quantity of points \n");
    scanf("%d", &n);
    for (i=0; i<n; i++)
    {
        printf("Enter point #%d ", i+1);
        P.InputPoint();
        Ps->Add(P);
    }
};

// Output list of points
void Convex::OutputPointList (PointList *Ps)
{
    Point P;

    while (!Ps->IsEmpty())
    {
        Ps->Remove(&P);
        P.OutputPoint();
        printf("\n");
    }
};

// ��������� � ����i� ����i ������� ����i��i� �����
void Convex::CreateOutsideSet( FacetList* Polyhedron, PointList *freePointList )
{
    Point P;
    Facet *pF;

    while ( freePointList->Remove( &P ) )
    {
        Polyhedron->GetFirst(&pF);
        do {
            if ( pF->IsOnFacet( P ) > eps )
            {
                pF->listVertices.Add( P );   // ���������� � ������� ����i��i� �����
                break;                       // ����� ���������� ���� � ���� ������
            };
        } while ( Polyhedron->GetNext(&pF) );
    };

};

// �����1��� �� � ����1��� ������� ����� � ������ ������ FL,
// ���� ��� � ���1�� ����i � ��� ���� ����1��� ����� �� �����1� ������� �������� 0
int Convex::IsEmptyOutsideList(FacetList* Polyhedron)
{
    Facet *pF;

    Polyhedron->GetFirst(&pF);
    do {
        if ( !pF->listVertices.IsEmpty())
            return 0;
    } while ( Polyhedron->GetNext(&pF) );

    return 1;
};

// �������, ������ �������� ������ ����i��i� ����� ��� ����� ����i
int Convex::PrintOutsideList(FacetList* Polyhedron)
{
    Facet *pF;

    printf("-------------------\n");
    Polyhedron->GetFirst(&pF);
    do {
        pF->PrintCoordinates(1);
    } while ( Polyhedron->GetNext(&pF) );

    printf("===================\n");
    return 1;
};

// ������� ����� �� ������ ��������
void Convex::AddPointToHull(FacetList* Polyhedron, PointList *listUnclaimedVertices)
{
    Facet   *pF, *pF2;
    Point   P;
    Point   DistP, DistCur;
    int     re;
    Segment S;
    SegmentList Horizont;

    Polyhedron->GetFirst(&pF);
    do {
        if ( pF->listVertices.GetFirst(&DistP))
        {
            FacetList VisibleFaces;

            // ��������� ����i��� �i������� ����� DistP �i� ����i pF
            DistCur = DistP;
            do {
                if ((pF->DistanceToPoint(DistP))<(pF->DistanceToPoint(DistCur)))
                    DistP.Init(DistCur);
            } while (pF->listVertices.GetNext(&DistCur));

            #ifdef CONVEX_DEBUG
                printf( "Search the furthest point to facet ");
                pF->PrintCoordinates(0);
                printf( ".  DisP= ");
                DistP.OutputPoint();
                printf( "\n");
            #endif            

            // ����������� �� ��i� ������ i �����i � ����� DistP ������ �� ������ VisibleFaces,
            do {
                Polyhedron->GetFirst(&pF);
                re = 0;
                do {                                
                    if (pF->IsOnFacet(DistP)>eps)    
                    {
                        Facet *pF2 = new Facet;
                        *pF2 = (*pF);

                        #ifdef CONVEX_DEBUG
                            pF->PrintCoordinates(0);
                            printf( " added to list of visibles from DisP\n");
                        #endif            
                    
                        VisibleFaces.Add(pF2);
                        Polyhedron->FindAndRemoveAll( pF );
                        re = 1;
                    };                
                } while ( Polyhedron->GetNext(&pF) && !re );
            } while (re);
                    
            // �������� ��i �����i ����i i ��i ����i��i ����� � ��� �i������� �� ������ ��������������
            if ( VisibleFaces.GetFirst(&pF) )
            {
                do {                                            
                    while ( pF->listVertices.Remove(&P))            
                    {
                        listUnclaimedVertices->Add( P );
                        P.OutputPoint();
                        printf( " added to list of Unclaimed points\n");
                    };

                } while ( VisibleFaces.GetNext(&pF) );
            };

            // � ������ �������������� ��������� �������� ����� DistP
            listUnclaimedVertices->FindAndRemoveAll( DistP );

            // ���������� � ������ ������ ���i, ��� ���� ������ ������������ �i� �������� ����� �� �� ���������
            FacetToSegmentsGorizon(&VisibleFaces, &Horizont);
            Horizont.GetFirst(&S);
            do {
                pF2 = new Facet;
                pF2->CreateOrientatedFace( S.A, S.B, DistP, insidePoint );
                Polyhedron->Add( pF2 );
            } while ( Horizont.GetNext(&S) );
            
            return; // ���� ����� (DistP) � �������� ���������
        };
    } while ( Polyhedron->GetNext(&pF) );

};

// ������������ ������ ������ �� ������ �����, � ���������� ������i�
void Convex::FacetToPoints(FacetList *FL, PointList *PL)
{
    Facet       *pF;
    PointList   PT;
    Point       P;
    
    if ( FL->GetFirst(&pF) )
    {
        do {
            PT.Add(pF->A);
            PT.Add(pF->B);
            PT.Add(pF->C);
        } while (FL->GetNext(&pF));
    };

    while(!PT.IsEmpty())
    {
        PT.Remove(&P);
        PL->Add(P);
        PT.FindAndRemoveAll(P);
    };
};

// 
void Convex::FacetToSegmentsGorizon(FacetList *FL, SegmentList *SL)
{
    Facet       *pF;
    Segment     S;
    SegmentList SN;

    if ( FL->GetFirst(&pF) )
    {
        // � ����� ���� ��������� ��� �����
        do {
            S.Init(pF->A, pF->B);
            SN.Add(S);
            S.Init(pF->A, pF->C);
            SN.Add(S);
            S.Init(pF->C, pF->B);
            SN.Add(S);
        } while (FL->GetNext(&pF));
   
        // � ������ ����� ��������� �������� ���� ������ ����� ������ ������
        while(!SN.IsEmpty())
        {
            SN.Remove(&S);
            if (SN.FindAndRemoveAll(S)==0)
            {
                SL->Add(S);

                #ifdef CONVEX_DEBUG
                    printf( "Add to horizon : ");
                    S.PrintPoints();
                #endif            
            };
        };
    
    };
}

void Convex::DelVertexDoubles(PointList *PL)
{
    PointList PP;
    Point P, L;
    Segment S;
    SegmentList SN;

    if ( PL->GetFirst(&P) )
    {
        do {
            PP.Add(P);
        } while (PL->GetNext(&P));
    };
    
    if ( PL->GetFirst(&L) )
    {
        do {
            if ( PP.GetFirst(&P) )
            {
                do {
                    S.Init(P, L); 
                    SN.Add(S);
                } while (PP.GetNext(&P));
            };  
        } while (PL->GetNext(&L));
    }; 

    if ( PL->GetFirst(&P) )
    {
        do {
            if ( SN.GetFirst(&S) )
            {
                do {
                    if (S.Into(P))
                        PL->FindAndRemoveAll(P);
                } while (SN.GetNext(&S));
            };  
        } while (PL->GetNext(&P));
    };
}


int Convex::CreateSimplex(FacetList* HullFaces, PointList *AllPoints)
{
    Point   PCurr, PCurr2, PMinX, PMinY, PMinZ, PMaxX, PMaxY, PMaxZ;
    Segment S;
    Facet   F, *pF;
    int     go, go2;

    AllPoints->GetFirst(&PCurr);
    PMinX.Init(PCurr);    PMinY.Init(PCurr);    PMinZ.Init(PCurr);
    PMaxX.Init(PCurr);    PMaxY.Init(PCurr);    PMaxZ.Init(PCurr);

    while( AllPoints->GetNext(&PCurr) )            // 1) find the minimum and maximum point to each direction
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

    if ( PMinX.IsMatch(PMaxX) || PMinY.IsMatch(PMaxY) || PMinZ.IsMatch(PMaxZ) )
    {
        printf( "Points are coplanars !\n" );
        return 0;                                   // 2) all points are coplanars
    };

    S.Init(PMaxX, PMinX);                           // 3) check to colinear
    AllPoints->GetFirst(&PCurr);
    go = go2 = 1;
    while ( go & go2 )
    {
        if (S.Square(PCurr)>0)
            go = 0;
        else
            go2 = AllPoints->GetNext(&PCurr);
    }
    if (go) {
        printf( "Points are colinears !\n" );
        return 0;   // all points are colinears
    };
                                                    
    F.Init(PMaxX, PMinX, PCurr);                     // 4) check to coplanar
    AllPoints->GetFirst(&PCurr2);
    go = go2 = 1;
    while ( go && go2 )
    {
        if (F.DistanceToPoint(PCurr2)>0)
            go = 0;
        else
            go2 = AllPoints->GetNext(&PCurr2);
    }
    if (go)
    {
        printf( "Points are coplanars !!\n" );
        return 0;                                   // all points are coplanar
    };
/*
    AllPoints->GetFirst(&PCurr);
    AllPoints->GetNext(&PCurr2);
    AllPoints->GetNext(&PMaxX);
    AllPoints->GetNext(&PMinX);
*/
    // Now we have four points : PMaxX, PMinX, PCurr, PCurr2
    pF = new Facet; 
    pF->CreateOrientatedFace( PCurr, PCurr2, PMaxX, PMinX );
    HullFaces->Add( pF );
    pF = new Facet; 
    pF->CreateOrientatedFace( PMinX, PCurr, PCurr2, PMaxX );
    HullFaces->Add( pF );
    pF = new Facet; 
    pF->CreateOrientatedFace( PMaxX, PMinX, PCurr, PCurr2 );
    HullFaces->Add( pF );
    pF = new Facet; 
    pF->CreateOrientatedFace( PMaxX, PMinX, PCurr2, PCurr );
    HullFaces->Add( pF );

    AllPoints->FindAndRemoveAll( PMinX );
    AllPoints->FindAndRemoveAll( PMaxX );
    AllPoints->FindAndRemoveAll( PCurr2 );
    AllPoints->FindAndRemoveAll( PCurr );

    insidePoint.Init(   (PMinX.GetX()+PMaxX.GetX()+PCurr2.GetX()+PCurr.GetX())/4, 
                        (PMinX.GetY()+PMaxX.GetY()+PCurr2.GetY()+PCurr.GetY())/4, 
                        (PMinX.GetZ()+PMaxX.GetZ()+PCurr2.GetZ()+PCurr.GetZ())/4 );

    return 1;
};


int Convex::Calculation (FacetList* HullFacets, PointList *AllPoints)
{
    if ( !CreateSimplex(HullFacets, AllPoints) )
    {
        printf( "Points are coplanars\n" );
        return 0;
    };
    
    do {
        CreateOutsideSet( HullFacets, AllPoints ); 

#ifdef CONVEX_DEBUG
        PrintOutsideList( HullFacets );
#endif
        AddPointToHull( HullFacets, AllPoints);
    } while (!IsEmptyOutsideList(HullFacets) || !AllPoints->IsEmpty()) ;    // ���� ��� � ���� � ������ ������ FL ����1��� ������� �����

#ifdef CONVEX_DEBUG
    PrintOutsideList(HullFacets);
#endif

    return 1;
};



int main()
{
    Convex      CX;
    PointList   AllPoints, HullPoints;
    FacetList   HullFacets;

    CX.InputPointList(&AllPoints);
    
    CX.Calculation (&HullFacets, &AllPoints );
    CX.FacetToPoints(&HullFacets, &HullPoints);

    CX.OutputPointList (&HullPoints);

    return 0;

}
