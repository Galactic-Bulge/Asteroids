#ifndef AABB_H
#define AABB_H

// DirectX
#include <DirectXMath.h>

struct AABB
{
public:
    DirectX::XMFLOAT2 min, max;

    /*
     * Construct an instance of an Axis-Aligned Bounding Box.
     * @param   min     The upper-left point.
     * @param   max     The lower-right point.
     */
    AABB(DirectX::XMFLOAT2 min, DirectX::XMFLOAT2 max);

    /*
     * Construct an instance of an Axis-Aligned Bounding Box.
     * @param   center      The center point.
     * @param   halfWidth   Half the width of the AABB.
     * @param   halfHeight  Half the height of the AABB.
     */
    AABB(DirectX::XMFLOAT2 center, float halfWidth, float halfHeight);

    // Deleted and default constructors.
    AABB(void) = delete;
    AABB(const AABB& rhs) = default;
    AABB& operator=(const AABB& rhs) = default;
    AABB(AABB&& rhs) = default;
    AABB& operator=(AABB&& rhs) = default;

    /*
     * Performs a test to see if two AABB's are intersecting.
     * @param   rhs     The AABB to test against.
     * @returns A boolean indicating if the two AABB's intersect.
     */
    bool IntersectsWith(const AABB& rhs);

    /*
     * Performs a test to see if a point is contained within an AABB.
     * @param   point   The point to test against.
     * @returns A boolean indicating if the point is within the AABB.
     */
    bool ContainsPoint(const DirectX::XMFLOAT2& point);
};

#endif
