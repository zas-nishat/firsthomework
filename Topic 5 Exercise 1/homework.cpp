#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
using namespace std;

/* Exercise 1: Inheritance & Polymorphism: Shape Collision

    In this exercise you will write four classes: Shape, Square, Circle and Point.
    All shapes live in a 2D plane. Squares are axis-aligned (their edges are parallel
    to the X and Y axes, i.e. they are never rotated).

    Create an abstract class called "Shape":
        - it must have three pure virtual functions (declared with "= 0") that return a bool:
              virtual bool overlapsWith(Square* square) = 0;
              virtual bool overlapsWith(Circle* circle) = 0;
              virtual bool overlapsWith(Point* point) = 0;
        - all three are called "overlapsWith"; they are distinguished by their argument type
        - give it a virtual destructor (good practice for any base class that has virtual functions)

    Create a Square class (inherits from Shape):
        - declare Circle and Point as friend classes - this will allow them to access private variables
        - it must have private variables of type float: edgeLength, centerX, centerY
        - create a public constructor that initialises these values, in this order:
              Square(float edgeLength, float centerX, float centerY)
        - do not create overlap functions yet

    Create a Circle class (inherits from Shape):
        - declare Square and Point as friend classes - this will allow them to access private variables
        - it must have private variables of type float: radius, centerX, centerY
        - create a public constructor that initialises these values, in this order:
              Circle(float radius, float centerX, float centerY)
        - do not create overlap functions yet

    Create a Point class (inherits from Shape):
        - declare Circle and Square as friend classes - this will allow them to access private variables
        - it must have private variables of type float: x, y
        - create a public constructor that initialises these values, in this order:
              Point(float x, float y)
        - do not create overlap functions yet

    At this point we can implement collision detection.
    For each of Square, Circle and Point, implement 3 public overlapsWith() functions that override
    the ones in Shape - one for each shape (one of which is the class itself).
        - e.g. Square will have overlapsWith(Point* point), overlapsWith(Circle* circle) AND overlapsWith(Square* square)
        - Two shapes that merely touch (e.g. a point exactly on a circle's edge) DO count as overlapping.
          In other words, use <= rather than < in your comparisons.
        - To avoid implementing the same check twice (e.g. square.overlapsWith(point) and point.overlapsWith(square)),
          you may implement it in one direction, and in the other direction call the already implemented function like this:

              bool Square::overlapsWith(Point* point) { return point->overlapsWith(this); }

          (Note: "this" is already a pointer, which is why these functions take pointers.)
        - IMPORTANT: a function body can only use a class's members once that class is fully defined. Because
          of this, the overlapsWith() functions that call into another class cannot be written inside the class
          body (the other class would still be incomplete at that point). Instead:
              1. Declare all four classes first, with their overlapsWith() functions only declared (no body).
              2. After all classes are complete, define the functions outside the classes, e.g.
                     bool Square::overlapsWith(Circle* circle) { ... }
        - You should be able to figure out how to detect collision between Point <-> Circle, and Point <-> Square.
            Hints:
              - Point <-> Circle: the point is inside if its squared distance from the circle's center is <= radius².
              - Point <-> Square: the point is inside if |x - centerX| <= half AND |y - centerY| <= half,
                where half = edgeLength / 2.
              - Point <-> Point: they overlap if both x and y are equal.
              - Circle <-> Circle: they overlap if the squared distance between the centers
                is <= (radius1 + radius2)².
              - Square <-> Square: they overlap if, on BOTH axes, the distance between the centers
                is <= (edgeLength1 + edgeLength2) / 2.
            As for Square <-> Circle, here are steps you may use as a reference (note that variable names will need to be changed):

            Compute half the square's edge length: half = edgeLength / 2.
            Find the square's edges: left = squareX - half, right = squareX + half, bottom = squareY - half, top = squareY + half.
            Find the point on the square closest to the circle's center:
            - Take the circle's X and force it into the range [left, right]. If it's below left, use left; if it's above right, use right; otherwise keep it.
            - Do the same for the circle's Y with the range [bottom, top].
            Subtract that closest point from the circle's center to get the horizontal difference (dx) and vertical difference (dy).
            Compute dx² + dy² (the squared distance).
            Compare it to radius². If the squared distance is less than or equal to the squared radius, the shapes overlap. Otherwise, they don't.

        - Tip: comparing squared distances (dx*dx + dy*dy) avoids needing sqrt() and is more accurate.
*/ 

// this will inform Shape class that they exist before needing an implementation
class Square;
class Circle;
class Point;

////////////////////////////////////////////////////
// Write your code here

class Shape {
public:
    virtual bool overlapsWith(Square* square) = 0;
    virtual bool overlapsWith(Circle* circle) = 0;
    virtual bool overlapsWith(Point* point) = 0;
    virtual ~Shape() {}
};

class Square : public Shape {
    friend class Circle;
    friend class Point;
private:
    float edgeLength;
    float centerX;
    float centerY;
public:
    Square(float edgeLength, float centerX, float centerY)
        : edgeLength(edgeLength), centerX(centerX), centerY(centerY) {}

    bool overlapsWith(Square* square) override;
    bool overlapsWith(Circle* circle) override;
    bool overlapsWith(Point* point) override;
};

class Circle : public Shape {
    friend class Square;
    friend class Point;
private:
    float radius;
    float centerX;
    float centerY;
public:
    Circle(float radius, float centerX, float centerY)
        : radius(radius), centerX(centerX), centerY(centerY) {}

    bool overlapsWith(Square* square) override;
    bool overlapsWith(Circle* circle) override;
    bool overlapsWith(Point* point) override;
};

class Point : public Shape {
    friend class Circle;
    friend class Square;
private:
    float x;
    float y;
public:
    Point(float x, float y)
        : x(x), y(y) {}

    bool overlapsWith(Square* square) override;
    bool overlapsWith(Circle* circle) override;
    bool overlapsWith(Point* point) override;
};

// ---- Point overlap implementations ----

bool Point::overlapsWith(Point* point) {
    return (this->x == point->x && this->y == point->y);
}

bool Point::overlapsWith(Circle* circle) {
    float dx = this->x - circle->centerX;
    float dy = this->y - circle->centerY;
    return (dx * dx + dy * dy) <= (circle->radius * circle->radius);
}

bool Point::overlapsWith(Square* square) {
    float half = square->edgeLength / 2.0f;
    return (std::abs(this->x - square->centerX) <= half) &&
           (std::abs(this->y - square->centerY) <= half);
}

// ---- Circle overlap implementations ----

bool Circle::overlapsWith(Circle* circle) {
    float dx = this->centerX - circle->centerX;
    float dy = this->centerY - circle->centerY;
    float rSum = this->radius + circle->radius;
    return (dx * dx + dy * dy) <= (rSum * rSum);
}

bool Circle::overlapsWith(Point* point) {
    return point->overlapsWith(this);
}

bool Circle::overlapsWith(Square* square) {
    return square->overlapsWith(this);
}

// ---- Square overlap implementations ----

bool Square::overlapsWith(Square* square) {
    float maxDist = (this->edgeLength + square->edgeLength) / 2.0f;
    return (std::abs(this->centerX - square->centerX) <= maxDist) &&
           (std::abs(this->centerY - square->centerY) <= maxDist);
}

bool Square::overlapsWith(Point* point) {
    return point->overlapsWith(this);
}

bool Square::overlapsWith(Circle* circle) {
    float half = this->edgeLength / 2.0f;
    float left = this->centerX - half;
    float right = this->centerX + half;
    float bottom = this->centerY - half;
    float top = this->centerY + half;

    float closestX = circle->centerX;
    if (closestX < left) closestX = left;
    else if (closestX > right) closestX = right;

    float closestY = circle->centerY;
    if (closestY < bottom) closestY = bottom;
    else if (closestY > top) closestY = top;

    float dx = circle->centerX - closestX;
    float dy = circle->centerY - closestY;

    return (dx * dx + dy * dy) <= (circle->radius * circle->radius);
}

// Do not touch anything below this line
////////////////////////////////////////////////////

Shape* shapeFromArgs(const char* type, const char* argSize, const char* argX, const char* argY)
{
    float size = stof(argSize);
    float x    = stof(argX);
    float y    = stof(argY);

    if (strcmp(type, "Square") == 0)
        return new Square(size, x, y);
    if (strcmp(type, "Circle") == 0)
        return new Circle(size, x, y);
    if (strcmp(type, "Point") == 0)
        return new Point(x, y);
    return nullptr;
}

bool overlapsUsingCast(Shape* shape1, Shape* shape2, char* type2)
{
    if (strcmp(type2, "Square") == 0)
        return shape1->overlapsWith(static_cast<Square*>(shape2));

    if (strcmp(type2, "Circle") == 0)
        return shape1->overlapsWith(static_cast<Circle*>(shape2));

    if (strcmp(type2, "Point") == 0)
        return shape1->overlapsWith(static_cast<Point*>(shape2));

    return false;
}

int main(int argc, char* argv[]) {
    Shape* shape1;
    Shape* shape2;

    char* testArgs[] = {
        (char*)"command",
        (char*)"Point", (char*)"-1", (char*)"3", (char*)"8",
        (char*)"Circle", (char*)"3", (char*)"2", (char*)"4"
    };

    if (argc <= 1)
        argv = testArgs;

    shape1 = shapeFromArgs(argv[1], argv[2], argv[3], argv[4]);
    shape2 = shapeFromArgs(argv[5], argv[6], argv[7], argv[8]);
    cout << (overlapsUsingCast(shape1, shape2, argv[5]) ? "yes" : "no") << endl;

    delete shape1;
    delete shape2;
    return 0;
}