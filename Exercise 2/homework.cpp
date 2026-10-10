#include <iostream>
#include <string>
#include <iomanip> // used for me to print the output to 2 decimal points

using namespace std;

/* Exercise 2: Class Usage

    A line is described by two endpoints.
    Create two classes:
        "Point": describes the coordinates (x and y) of a point (both should be of type "float")
        "Line": is defined by two "Point" objects.
    Create class methods in the "Line" class that calculate the length of the line.
    Hint: you will need to use the Pythagorean theorem for the calculation.

    You may need to include a new library for this task.
*/

////////////////////////////////////////////////////
// Write your code here

#include <cmath> // Required for sqrt()

class Point
{
public:
    float x;
    float y;

    // Constructor for Point
    Point(float px, float py)
    {
        x = px;
        y = py;
    }
};

class Line
{
public:
    Point p1;
    Point p2;

    // Constructor for Line using a member initializer list
    Line(Point point1, Point point2) : p1(point1), p2(point2) {}

    // Method to calculate the length using the Pythagorean theorem
    float getLength()
    {
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        return sqrt(dx * dx + dy * dy);
    }
};

// Do not touch anything below this line
////////////////////////////////////////////////////

int main(int argc, char *argv[])
{

    float x1 = stof(argv[1]);
    float y1 = stof(argv[2]);
    float x2 = stof(argv[3]);
    float y2 = stof(argv[4]);

    Point p1 = Point(x1, y1);
    Point p2 = Point(x2, y2);
    Line line = Line(p1, p2);

    // make sure it outputs the number to 2 decimal points for evaluation
    cout << fixed;
    cout << setprecision(2);
    cout << line.getLength();

    return 0;
}
