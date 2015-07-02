#include "Convex0D.h"

class Convex1D: public Convex0D
{

    public:
        Convex1D();
        ~Convex1D();

        int Build1DHull (Segment *S, PointList *PL);
        virtual void Calculation(PointList *PL);
};