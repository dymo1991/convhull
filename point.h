#define eps 0.000000001

// class of point
class Point
{
    private:
        double X, Y, Z;                                     // coordinate
        int    mark;
    public:
        Point();
        Point (double TX, double TY, double TZ);            // Create a point
        Point(Point &NewP);
        ~Point()                                            // destructor
        {
        };

        void Init(double NewX, double NewY, double NewZ);
        void GetP(Point *NewP);
        double GetX()                                       // coordinate X
        {
            return X;
        };
        double GetY()                                       // coordinate Y
        {
            return Y;
        };
        double GetZ()                                       // coordinate Z
        {
            return Z;
        };
        int IsMatch(Point B);                               // is match two points
        void Init(Point NewP);
        void InputPoint();                                  // enter coordinats from keyboard
        void OutputPoint();                             	// print coordinats to screen
        double DistanceTo( Point B );
        int operator ==( Point &B )
        {
            return IsMatch(B);
        };
        int operator ==( Point *A )
        {
            return IsMatch(*A);
        };
};
