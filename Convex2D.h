#include "Convex1D.h"

class Convex2D: public Convex1D
{
    public:
        Convex2D();
       ~Convex2D();

        virtual void Calculation(PointList *PL);

    protected:
        Point   insidePoint;    // �����i�� �窠, 業�� ���⪮���� ᨬ�����. ������⮢������ ��� ��i���i� �࠭��
    
    private:
        SegmentListEx *SL;
        void CreateBegPol(PointList *PL);
        int IsPointIntoTrian(Point T, SegmentEx S);
        int IsPointIntoPolyg(Point T);
        void CentrOfPolyg();
        void DelPointIntoPolyg(PointList *PL);
        int IsPointOverRip(Point T, SegmentEx *S);
        void AddPointListOverRip(PointList *PL);
        void CreateNewPol(PointList *PL);
   
};