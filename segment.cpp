#include <math.h>
#include <stdio.h>

#include "segment.h"

Segment::Segment()
{
}

Segment::Segment(Point T1, Point T2):
    A(T1.GetX(), T1.GetY(), T1.GetZ()),
    B(T2.GetX(), T2.GetY(), T2.GetZ())
{
};


Segment::~Segment()
{
};

void Segment::Init(Point NewA, Point NewB)
{
    A.Init(NewA);
    B.Init(NewB);
};

void Segment::SetA(Point T)
{
    A.Init(T);
};

void Segment::SetB(Point T)
{
    B.Init(T);
};

void Segment::GetA(Point *T)
{
    T->Init(A);
};

void Segment::GetB(Point *T)
{
    T->Init(B); 
};

// Функцiя визначення довжини вiдрiзка
double Segment::Len()                                      
{
    return A.DistanceTo(B);
};

// Функцiя обчислення площi трикутника утвореного вiдрiзком i точкою T                                                           
double Segment::Square(Point T)
{
    double a, b, c, p;

    a= A.DistanceTo(T);
    b= B.DistanceTo(T);
    c= Len();
    p=(a+b+c)/2;
    if ((fabs(p-a)<eps) || (fabs(p-b)<eps) ||(fabs(p-c)<eps))
        return 0;
    else
        return sqrt(p*(p-a)*(p-b)*(p-c));
};

// Функцiя визначення чи лежить точка Т на прямiй проведенiй по вiдрiзку
int Segment::OnLine(Point T)                               
{
    if ( Square(T) <= delta)
        return 1;
    else
        return 0;
};

// Функцiя визначення чи лежить точка Т на вiдрiзку
int Segment::Into(Point T)                              
{
//    Segment s1,s2;
    double  l;

    Segment s1(A,T);
    Segment s2(B,T);
    l=Len();

    if ((OnLine(T)) && (l >= s1.Len()) && (l >= s2.Len()))
        return 1;
    else
        return 0;
};

int Segment::IsMatch(Segment S)
{
    if ( ((A==S.A) && (B==S.B)) || ((A==S.B) && (B==S.A)))
        return 1;
    else 
        return 0;
}

void Segment::PrintPoints()
{
    A.OutputPoint(); printf("-");
    B.OutputPoint(); printf("\n");
};

//----------------------------------------------------------------------------

#ifdef SEGMENT_DEBUG

int main()
{
    Point A, B, T;
    Segment S;
    double X,Y,Z;

    printf("Enter coordinate of point A ");
    A.InputPoint();

    printf("Enter coordinate of point B ");
    B.InputPoint();

    S.Init(A, B);
    printf("A: X=%lf, Y=%lf, Z=%lf\n", A.GetX(), A.GetY(), A.GetZ());
    printf("B: X=%lf, Y=%lf, Z=%lf\n", B.GetX(), B.GetY(), B.GetZ());
    printf("length(AB)=%lf\n", S.Len());

    printf("Enter coordinate of point T ");
    T.InputPoint();
    printf("T: X=%lf, Y=%lf, Z=%lf\n", T.GetX(), T.GetY(), T.GetZ());

    printf("square of ABT=%lf\n", S.Square(T));

    if (S.OnLine(T))
        printf("T OnLine AB\n");
    else
        printf("T NotOnLine AB\n");

    if (S.Into(T))
        printf("T Into [AB]\n");
    else
        printf("T NotInto [AB]\n");
};

#endif // SEGMENT_DEBUG
