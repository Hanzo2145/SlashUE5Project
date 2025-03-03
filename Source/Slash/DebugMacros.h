#pragma once
#include "DrawDebugHelpers.h"

#define DRAW_Sphere(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 24.f, 12, FColor::Red, true);
#define Draw_Sphere_Color(Location, Color) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 8.f, 12, Color, false, 5.f); 
#define DRAW_LINE(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, Endlocation, FColor::Red, true, -1.f, 0, 1); 
#define DRAW_POINT(location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, true);
#define DRAW_VECTOR(StartLocation, EndLocation) if(GetWorld()) \
{ \
     DrawDebugLine(GetWorld(), StartLocation, Endlocation, FColor::Red, true, -1.f, 0, 1); \
     DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Red, true); \
}


//TickFunctions Macros:

#define DRAW_SphereTick(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 24, 12, FColor::Red, false, -1.f);
#define DRAW_LINETick(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, Endlocation, FColor::Red, false, -1.f, 0, 1);
#define DRAW_POINTTick(location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, flase, -1.f);
#define DRAW_VECTORTick(StartLocation, EndLocation) if(GetWorld()) \
{ \
     DrawDebugLine(GetWorld(), StartLocation, Endlocation, FColor::Red, false, -1.f, 0, 1); \
     DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Red, flase, -1.f); \
}

