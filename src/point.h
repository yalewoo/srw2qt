#ifndef POINT_H
#define POINT_H


struct Point
{
public:
    Point() = default;
    Point(int x, int y);
    int x;
    int y;
};

#endif // POINT_H
