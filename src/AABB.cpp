#include "AABB.h"

using namespace DirectX;

/*
 * Construct an instance of an Axis-Aligned Bounding Box.
 * @param   min     The upper-left point.
 * @param   max     The lower-right point.
 */
AABB::AABB(DirectX::XMFLOAT2 min, DirectX::XMFLOAT2 max)
    : min(min), max(max)
{
    /* Nothing to do. */
}

/*
 * Construct an instance of an Axis-Aligned Bounding Box.
 * @param   center      The center point.
 * @param   halfWidth   Half the width of the AABB.
 * @param   halfHeight  Half the height of the AABB.
 */
AABB::AABB(DirectX::XMFLOAT2 center, float halfWidth, float halfHeight)
{
    min = XMFLOAT2{center.x - halfWidth, center.y - halfHeight};
    max = XMFLOAT2{center.x + halfWidth, center.y + halfHeight};
}

/*
 * Performs a test to see if two AABB's are intersecting.
 * @param   rhs     The AABB to test against.
 * @returns A boolean indicating if the two AABB's intersect.
 */
bool AABB::IntersectsWith(const AABB& rhs)
{
    if(max.x < rhs.min.x || min.x > rhs.max.x || max.y < rhs.min.y || min.y > rhs.max.y)
    {
        return false;
    }

    return true;
}

/*
 * Performs a test to see if a point is contained within an AABB.
 * @param   point   The point to test against.
 * @returns A boolean indicating if the point is within the AABB.
 */
bool AABB::ContainsPoint(const DirectX::XMFLOAT2& point)
{
    if(point.x < min.x || point.x > max.x || point.y < min.y || point.y > max.y)
    {
        return false;
    }

    return true;
}
