#include "convex2d.h"

class Convex3D: public Convex2D
{
    public:
        Convex3D();
        ~Convex3D();

        virtual void Calculation(PointList *PL);

    private:

        int CreateSimplex(FacetList* HullFaces, PointList *AllPoints);
        void CreateOutsideSet(FacetList* Polyhedron, PointList *freePointList);
        int IsEmptyOutsideList (FacetList *Polyhedron);
        void FacetToSegmentsGorizon(FacetList *FL, List<Segment> *SL);
        void AddPointToHull(FacetList* Polyhedron, PointList *listUnclaimedVertices);
//        void MakeRightFacetList (FacetList *FL, SegmentList *SN, Point A, Point T);
/*
        FacetList FacetFromDistP (FacetList *FL);
*/
        int PrintOutsideList(FacetList* Polyhedron);

};
