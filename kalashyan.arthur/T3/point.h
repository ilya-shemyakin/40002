#ifndef POINT_H
#define POINT_H

struct Point {
    int x, y;
    bool operator==(const Point &o) const {
        return x == o.x && y == o.y;
    }

    bool operator<(const Point &o) const {
        return x < o.x || (x == o.x && y < o.y);
    }
};

#endif // POINT_H
