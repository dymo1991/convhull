#include "segment.h"

// facet
class Facet
{
    private:
        Point       A, B, C;                            // vertexes of the facet

    public:
        PointList   listVertices;                       // list of points outside ot this face

        Facet();
        Facet(Point T1, Point T2, Point T3);            // create facet
        Facet( Point P1, Point P2, Point P3, Point T );
        ~Facet();

        void Init (Point NewA, Point NewB, Point NewC);

        void GetA(Point *T);                             // point A
        void GetB(Point *T);                             // point B
        void GetC(Point *T);                             // point C

        //void Create(Segment R, Point T);                // create facet with point T1 and segmnet R

        double IsOnFacet(Point T);                      // is point T on same plane with facet
        double DistanceToPoint(Point T);                // distance from facet to point from plural F
        int NeighborFacet(Facet F);                     // is two facet neighboring&
        int IsMatch(Facet F);                           // is two facet match
        Segment NeighborSegm(Facet F);

        void PrintCoordinates( int IsPrintList );
       // void CreateOrientatedFace( Point P1, Point P2, Point P3, Point T );
};
