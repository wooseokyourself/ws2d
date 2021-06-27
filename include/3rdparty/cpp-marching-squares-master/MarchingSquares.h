#ifndef __MARCHINGSQUARES__
#define __MARCHINGSQUARES__
/**
 * A simple implementation of the marching squares algorithm that can identify
 * perimeters in an supplied byte array. The array of data over which this
 * instances of this class operate is not cloned by this class's constructor
 * (for obvious efficiency reasons) and should therefore not be modified while
 * the object is in use. It is expected that the data elements supplied to the
 * algorithm have already been thresholded. The algorithm only distinguishes
 * between zero and non-zero values.
 * 
 * @author Tom Gibara
 * Ported to C++ by Juha Reunanen
 *
 */

#include <vector>
#include <sstream>
#include <utility>
#include <box2d/b2_math.h>

namespace MarchingSquares {

    struct Direction {
        Direction() : x(0), y(0) {}
        Direction(int x, int y) : x(x), y(y) {}
		Direction(b2Vec2 vec) : x(vec.x), y(vec.y) {}
        int x;
        int y;
    };

    struct Result {
        int initialX = -1;
        int initialY = -1;
        std::vector<Direction> directions;
    };

    inline bool operator== (const Direction& a, const Direction& b) {
        return a.x == b.x && a.y == b.y;
    }

    inline Direction operator* (const Direction& direction, int multiplier) {
        return Direction(direction.x * multiplier, direction.y * multiplier);
    }

    inline Direction operator+ (const Direction& a, const Direction& b) {
        return Direction(a.x + b.x, a.y + b.y);
    }

    inline Direction& operator+= (Direction& a, const Direction& b) {
        a.x += b.x;
        a.y += b.y;
        return a;
    }

    inline Direction MakeDirection(int x, int y) { return Direction(x, y); }

    inline Direction East()      { return MakeDirection( 1,  0); }
    inline Direction Northeast() { return MakeDirection( 1,  1); }
    inline Direction North()     { return MakeDirection( 0,  1); }
    inline Direction Northwest() { return MakeDirection(-1,  1); }
    inline Direction West()      { return MakeDirection(-1,  0); }
    inline Direction Southwest() { return MakeDirection(-1, -1); }
    inline Direction South()     { return MakeDirection( 0, -1); }
    inline Direction Southeast() { return MakeDirection( 1, -1); }

    inline bool isSet(int x, int y, int width, int height, unsigned char* data) {
        return x <= 0 || x > width || y <= 0 || y > height
            ? false
            : data[(y - 1) * width + (x - 1)] != 0;
    }
	
    inline int value(int x, int y, int width, int height, unsigned char* data) {
        int sum = 0;
        if (isSet(x, y, width, height, data)) sum |= 1;
        if (isSet(x + 1, y, width, height, data)) sum |= 2;
        if (isSet(x, y + 1, width, height, data)) sum |= 4;
        if (isSet(x + 1, y + 1, width, height, data)) sum |= 8;
        return sum;
    }

    Result FindPerimeter(int initialX, int initialY, int width, int height, unsigned char* data);
    Result FindPerimeter(int width, int height, unsigned char* data);
	Result FindPerimeter(int width, int height, unsigned char* data, int lookX, int lookY);
	Direction FindEdge(int width, int height, unsigned char* data, int lookX, int lookY);
}

#endif