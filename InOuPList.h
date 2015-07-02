#define FacetList List<Facet*>
#define SegmentList List<Segment*>
#define SegmentListEx List<SegmentEx*>

#include "facet.h"

class InOuPList
{
    public:
        InOuPList();
        ~InOuPList();

        PointList *PL;

        void FacetToPoints(FacetList *FL);
        void SegmToPoints(SegmentList *SL);
        void DelVertexDoubles();
        void InputPointList();
        void OutputPointList();
        int GetPointDimension();
};