#include "convex3D.h"
#include <stdio.h>

int main()
{
    InOuPList PtL;

    PtL.InputPointList();
    if (PtL.PL->IsEmpty())
        printf("List is Empty\n");
    else
    {
            Convex0D  *conv;
            int dimention = PtL.GetPointDimension();
          
            if (dimention==0)
                conv = new Convex0D;
            if (dimention==1)
                conv = new Convex1D;
            else if (dimention==2)
                conv = new Convex2D;
            else if (dimention==3)
                conv = new Convex3D;
            
            conv->Calculation (PtL.PL);
            PtL.DelVertexDoubles();
            PtL.OutputPointList();
        
            delete conv;
    };

    return 1;
};
