del list.exe, facet.exe, point.exe, segment.exe

SET GFLAG=/D_CRT_SECURE_NO_DEPRECATE    

cl point.cpp /DPOINT_DEBUG %GFLAG%
cl list.cpp point.cpp /DLIST_DEBUG %GFLAG%
cl segment.cpp point.cpp /DSEGMENT_DEBUG %GFLAG%
cl facet.cpp segment.cpp point.cpp /DFACET_DEBUG %GFLAG%
rem cl convex.cpp facet.cpp segment.cpp point.cpp %GFLAG% /FeCONVHULL.exe

cl Convex1D.cpp InOuPList.cpp facet.cpp point.cpp segment.cpp /DCONVEX1D_DEBUG
cl InOuPList.cpp facet.cpp point.cpp segment.cpp /DINOUPLIST_DEBUG

cl test.cpp Convex3D.cpp Convex2D.cpp Convex1D.cpp InOuPList.cpp facet.cpp point.cpp segment.cpp /DCONVEX3D_DEBUG3 /FeConvex_Hull.exe
