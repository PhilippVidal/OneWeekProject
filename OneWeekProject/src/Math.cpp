#include "Math.h"
#include "Vec2.h"

bool Math::EvaluateCollisionAABBs(const Vec2f& positionA, const Vec2f& halfSizeA, const Vec2f& positionB, const Vec2f& halfSizeB)
{
    int xMaxA = positionA.x + halfSizeA.x;
    int xMinA = positionA.x - halfSizeA.x;
    int yMaxA = positionA.y + halfSizeA.y;
    int yMinA = positionA.y - halfSizeA.y;

    int xMaxB = positionB.x + halfSizeB.x;
    int xMinB = positionB.x - halfSizeB.x;
    int yMaxB = positionB.y + halfSizeB.y;
    int yMinB = positionB.y - halfSizeB.y;

    bool axisX = (xMaxA >= xMinB) && (xMinA <= xMaxB);
    bool axisY = (yMaxA >= yMinB) && (yMinA <= yMaxB);

    return axisX && axisY;
}

bool Math::EvaluateCollisionCircles(const Vec2f& positionA, float radiusA, const Vec2f& positionB, float radiusB)
{
    float radiiSqr = (radiusA + radiusB);
    radiiSqr *= radiiSqr;

    return (positionA - positionB).MagnitudeSqr() <= radiiSqr;
}

bool Math::EvaluateCollisionCircleAABB(const Vec2f& positionA, float radiusA, const Vec2f& positionB, const Vec2f& halfSizeB)
{
    //Distance between the circle and axis-aligned rectangle
    float distanceX = abs(positionA.x - positionB.x);
    float distanceY = abs(positionA.y - positionB.y);

    //Check distance on x and y axis, if too far away -> no collision
    if ( (distanceX > (halfSizeB.x + radiusA)) || (distanceY > (halfSizeB.y + radiusA)) )
    {
        return false;
    }

    //check if inside/in rectangle-range
    if ((distanceX <= halfSizeB.x) || (distanceY <= halfSizeB.y))
    {
        return true;
    }

    //Check rounded corners of rectangle
    //First get distance from circle to corner 
    float cornerDistanceSqr = Vec2f(distanceX - halfSizeB.x, distanceX - halfSizeB.x).MagnitudeSqr();

    //Then check if that distance is smaller than the radius of the circle
    return (cornerDistanceSqr * cornerDistanceSqr) <= (radiusA * radiusA);
}

float Math::GetRandomFloat(float min, float max)
{
    return min + (rand() / static_cast<float>(RAND_MAX)) * (max - min);
}




