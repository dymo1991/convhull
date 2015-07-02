#include <stdio.h>
#include <math.h>

#include "point.h"


Point::Point()
{
    X = Y = Z = 0;
};

Point::Point(double TX, double TY, double TZ)
{
    X=TX;
    Y=TY;
    Z=TZ;
};

Point::Point(Point &NewP)
{
    X=NewP.X;
    Y=NewP.Y;
    Z=NewP.Z;
}; 

void Point::Init(double NewX, double NewY, double NewZ)
{
    X=NewX;
    Y=NewY;
    Z=NewZ;
};

void Point::Init(Point NewP)
{
    X=NewP.X;
    Y=NewP.Y;
    Z=NewP.Z;
}; 

void Point::GetP(Point *NewP)
{
    NewP->X=X;
    NewP->Y=Y;
    NewP->Z=Z;
};

int Point::IsMatch( Point B)
{
    if ((fabs(X-B.X)<eps) && (fabs(Y-B.Y)<eps) && (fabs(Z-B.Z)<eps))
        return 1;
    else
        return 0;
};

void Point::InputPoint()
{
    fprintf(stderr, "Enter coordinates of point, X Y Z\n");
    scanf("%lf", &X);
    scanf("%lf", &Y);
    scanf("%lf", &Z);

};

void Point::OutputPoint()
{
    printf("(%1.0lf,%1.0lf,%1.0lf)", X, Y, Z );
};

double Point::DistanceTo( Point B )
{
    return (sqrt( (X-B.X)*(X-B.X)
                  +(Y-B.Y)*(Y-B.Y)
                  +(Z-B.Z)*(Z-B.Z)) );    
}

//----------------------------------------------------------------------------


#ifdef POINT_DEBUG

int main()
{
    Point Pt, Pt2;

    Pt.InputPoint();
    Pt2.InputPoint();

    printf("Pt : X=%lf, Y=%lf, Z=%lf\n", Pt.GetX(), Pt.GetY(), Pt.GetZ());
    printf("Pt2 : " );
    Pt2.OutputPoint();

    printf("\nDistance=%lf\n", Pt.DistanceTo(Pt2));
}

#endif // POINT_DEBUG