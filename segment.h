#define delta 0.000000001

#include "point.h"
#include "list.h"

#define PointList List<Point>

// segment on plane
class Segment
{
    private:
        Point A, B ;                            // ends of segment

    public:
        Segment();
        Segment (Point T1, Point T2);           // create segment
        ~Segment();                             // destructor

        void Init (Point NewA, Point NewB);
        void SetA(Point T);                     // set point A
        void SetB(Point T);                     // set point B
        void GetA(Point *T);
        void GetB(Point *T);
        double Len();                           // length of segment
        double Square(Point T);                 // square  triangle TAB with sign
        int OnLine(Point T);                    // is point T on the same line with the segment?
        int Into(Point T);                      // is point T into the segment?
        int IsMatch( Segment S);
        int operator ==( Segment &S )
        {
            return IsMatch(S);
        };
        void PrintPoints();
};

// сегмент зi списком точок
class SegmentEx : public Segment
{
    public:
        SegmentEx()
        {
        };
        SegmentEx (Point T1, Point T2) 
            : Segment (T1, T2)
        {
        };           // create segment
        PointList listVertex;                   // list of points outside ot this segment
};