#ifndef COMMON_H
#define COMMON_H

struct Point
{
public:
    Point() = default;
    Point(int x, int y) : x(x), y(y){}
    int x;
    int y;
};

struct PositionAndSize
{
    PositionAndSize() = default;
    PositionAndSize(int x, int y, int width, int height) : x(x), y(y), width(width), height(height){}
    int x;
    int y;
    int width;
    int height;
};

#endif // COMMON_H
