#include <stdio.h>
#include <math.h>

#include "convex3D.h"

//#define CONVEX3D_DEBUG

Convex3D::Convex3D()
{
};

Convex3D::~Convex3D()
{
};

// утворюємо у кожнiй гранi множину зовнiшнiх точок
void Convex3D::CreateOutsideSet( FacetList* Polyhedron, PointList *freePointList )
{
    Point P;
    Facet *pF;

    while ( freePointList->Remove( &P ) )
    {
        Polyhedron->GetFirst(&pF);
        do {
            if ( pF->IsOnFacet( P ) > eps )
            {
                pF->listVertices.Add( P );   // добавляємо в множину зовнiшнiх точок
                break;                       // точку добавляємо лише в один список
            };
        } while ( Polyhedron->GetNext(&pF) );
    };

};

// перев1ряє чи є зовнiшня множина точок у списку граней FL,
// якщо хоч у одн1єї гранi є хоч одна зовнiшня точка то функц1я повертає значення 0
int Convex3D::IsEmptyOutsideList(FacetList* Polyhedron)
{
    Facet *pF;

    Polyhedron->GetFirst(&pF);
    do {
        if ( !pF->listVertices.IsEmpty())
            return 0;
    } while ( Polyhedron->GetNext(&pF) );

    return 1;
};

// тестова, просто виводить списки зовнiшнiх точок для кожної гранi
int Convex3D::PrintOutsideList(FacetList* Polyhedron)
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

// добавка точки до опуклої оболонки
void Convex3D::AddPointToHull(FacetList* Polyhedron, PointList *listUnclaimedVertices)
{
    Facet   *pF;
    Point   P, A, B;
    Point   DistP, DistCur;
    int     re;
    Segment S;
    List<Segment> Horizont;

    Polyhedron->GetFirst(&pF);
    do {
        if ( pF->listVertices.GetFirst(&DistP))
        {
            FacetList VisibleFaces;

            // Знаходимо найбiльш вiддалену точку DistP вiд гранi pF
            DistCur = DistP;
            do {
                if ((pF->DistanceToPoint(DistP))<(pF->DistanceToPoint(DistCur)))
                    DistP.Init(DistCur);
            } while (pF->listVertices.GetNext(&DistCur));

            #ifdef CONVEX3D_DEBUG
                printf( "Search the furthest point to facet ");
                pF->PrintCoordinates(0);
                printf( "\tDisP= ");
                DistP.OutputPoint();
                printf( "\n");
            #endif

            // Проходимось по всiх гранях i видимi з точки DistP додаємо до списку VisibleFaces,
            do {
                Polyhedron->GetFirst(&pF);
                re = 0;
                do {
                    if (pF->IsOnFacet(DistP)>eps)
                    {
                        Facet *pF2 = new Facet;
                        *pF2 = (*pF);

                        #ifdef CONVEX3D_DEBUG
                            pF->PrintCoordinates(0);
                            printf( " added to list of visibles from DisP\n");
                        #endif

                        VisibleFaces.Add(pF2);
                        Polyhedron->FindAndRemoveAll( pF );
                        re = 1;
                    };
                } while ( Polyhedron->GetNext(&pF) && !re );
            } while (re);

            // обходимо всi видимi гранi i всi зовнiшнi точки з них вiдносимо до списку невикористаних
            if ( VisibleFaces.GetFirst(&pF) )
            {
                do {
                    while ( pF->listVertices.Remove(&P))
                    {
                        listUnclaimedVertices->Add( P );
                    };

                } while ( VisibleFaces.GetNext(&pF) );
            };

            // з списку невикористаних прибираємо знайдену точку DistP
            listUnclaimedVertices->FindAndRemoveAll( DistP );

            // добавляємо в список граней новi, бічні грані конуса побудованого вiд знайденої точки до її горизонту
            FacetToSegmentsGorizon(&VisibleFaces, &Horizont);
            Horizont.GetFirst(&S);
            do {
                S.GetA(&A);
                S.GetB(&B);
                Facet *pF2 = new Facet(A, B, DistP, insidePoint );
                Polyhedron->Add( pF2 );
            } while ( Horizont.GetNext(&S) );

            return; // одна точка (DistP) в оболонку добавлена
        };
    } while ( Polyhedron->GetNext(&pF) );

};

//
void Convex3D::FacetToSegmentsGorizon(FacetList *FL, List<Segment> *SL)
{
    Facet       *pF;
    Segment     S;
    List<Segment>  SN;
    Point A, B, C;

    if ( FL->GetFirst(&pF) )
    {
        // з кожної грані створюємо три ребра
        do {
            pF->GetA(&A);
            pF->GetB(&B);
            pF->GetC(&C);
            S.Init(A, B);
            SN.Add(S);
            S.Init(A, C);
            SN.Add(S);
            S.Init(C, B);
            SN.Add(S);
        } while (FL->GetNext(&pF));

        // В список ребер горизонту включаємо лише зовнішні ребра видимої області
        while(!SN.IsEmpty())
        {
            SN.Remove(&S);
            if (SN.FindAndRemoveAll(S)==0)
            {
                SL->Add(S);

                #ifdef CONVEX3D_DEBUG
                    printf( "Add to horizon : ");
                    S.PrintPoints();
                #endif
            };
        };

    };
}


int Convex3D::CreateSimplex(FacetList* HullFaces, PointList *AllPoints)
{
    Point   PCurr, PCurr2;
//    Segment S;
//    Facet   F;
    int     go, go2;

    AllPoints->GetFirst(&PCurr);
    Point PMinX(PCurr);    Point PMinY(PCurr);    Point PMinZ(PCurr);
    Point PMaxX(PCurr);    Point PMaxY(PCurr);    Point PMaxZ(PCurr);

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

//    if ( PMinX.IsMatch(PMaxX) || PMinY.IsMatch(PMaxY) || PMinZ.IsMatch(PMaxZ) )
//    {
//        printf( "Points are coplanars !\n" );
//        return 0;                                   // 2) all points are coplanars
//    };

    Segment S(PMaxX, PMinX);                           // 3) check to colinear
    AllPoints->GetFirst(&PCurr);
    go = go2 = 1;
    while ( go & go2 )
    {
        if (S.Square(PCurr)>0)
            go = 0;
        else
            go2 = AllPoints->GetNext(&PCurr);
    }
//    if (go) {
//        printf( "Points are colinears !\n" );
//        return 0;   // all points are colinears
//    };

    Facet F(PMaxX, PMinX, PCurr);                     // 4) check to coplanar
    AllPoints->GetFirst(&PCurr2);
    go = go2 = 1;
    while ( go && go2 )
    {
        if (F.DistanceToPoint(PCurr2)>0)
            go = 0;
        else
            go2 = AllPoints->GetNext(&PCurr2);
    }
//    if (go)
//    {
//        printf( "Points are coplanars !!\n" );
//        return 0;                                   // all points are coplanar
//    };

    // Now we have four points : PMaxX, PMinX, PCurr, PCurr2
    Facet* pF = new Facet( PCurr, PCurr2, PMaxX, PMinX );
    HullFaces->Add( pF );
    pF = new Facet( PMinX, PCurr, PCurr2, PMaxX );
    HullFaces->Add( pF );
    pF = new Facet( PMaxX, PMinX, PCurr, PCurr2 );
    HullFaces->Add( pF );
    pF = new Facet( PMaxX, PMinX, PCurr2, PCurr );
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


void Convex3D::Calculation (PointList *AllPoints)
{
    InOuPList Ptr;
    FacetList *HullFacets=new FacetList;


    CreateSimplex(HullFacets, AllPoints);
//    {
//        printf( "Points are coplanars\n" );
//        return 0;
//    };

    do {
        CreateOutsideSet( HullFacets, AllPoints );

#ifdef CONVEX3D_DEBUG
        PrintOutsideList( HullFacets );
#endif
        AddPointToHull( HullFacets, AllPoints);
    } while (!IsEmptyOutsideList(HullFacets) || !AllPoints->IsEmpty()) ;    // Доки хоч у одної з граней списку FL зовнiшня множина точок

#ifdef CONVEX3D_DEBUG
    PrintOutsideList(HullFacets);
#endif

    Ptr.FacetToPoints(HullFacets);
    AllPoints->Init(Ptr.PL);
};

/*
int main()
{
    Convex3D      CX;
    PointList   AllPoints, HullPoints;
    FacetList   HullFacets;

    CX.InputPointList(&AllPoints);

    CX.Calculation (&HullFacets, &AllPoints );
    CX.FacetToPoints(&HullFacets, &HullPoints);

    CX.OutputPointList (&HullPoints);

    return 0;

}
*/