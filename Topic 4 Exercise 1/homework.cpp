#include <iostream>
#include <string>
#include <cstring>
using namespace std;

/* Exercise 1: Composition & Inheritance: Vehicles

    In this exercise you will write four classes: Engine, Vehicle, Car and Truck.
    Vehicle HAS an Engine (composition). Car and Truck ARE Vehicles (inheritance).

    Create a class called "Engine":
        - it must have a private variable of type int: horsepower
        - create a public constructor that initialises it:
              Engine(int horsepower)
        - create a public getter:
              int getHorsepower() const

    Create a class called "Vehicle":
        - it must have a private variable of type string: name
        - it must have a private variable of type Engine: engine
          (this is composition: a Vehicle is built out of an Engine object, not a pointer)
        - create a public constructor that initialises both, in this order:
              Vehicle(string name, int horsepower)
          Use the constructor's initialiser list to build the Engine:
              Vehicle(...) : name(name), engine(horsepower) { }
        - create a public getter:
              string getName() const
        - create a public virtual function that returns the top speed in km/h:
              virtual int topSpeed() const
          The top speed of a plain Vehicle is: 100 + horsepower / 2   (integer division)
        - create a public virtual function that returns a description:
              virtual string describe() const
          It must return a string in exactly this format:
              "<name>: <horsepower> hp, top speed <topSpeed> km/h"
          e.g. "Beetle: 50 hp, top speed 125 km/h"
          Tip: use to_string() to convert numbers to strings, and + to join strings.
          Tip: Vehicle cannot read engine's private variable directly. Use engine's getter.
        - give it a virtual destructor (good practice for any base class that has virtual functions)

    Create a class called "Car" (inherits from Vehicle):
        - it must have a private variable of type int: doors
        - create a public constructor that initialises these values, in this order:
              Car(string name, int horsepower, int doors)
          Pass name and horsepower on to the Vehicle constructor via the initialiser list:
              Car(...) : Vehicle(name, horsepower), doors(doors) { }
        - Car does NOT change its top speed (it uses the one inherited from Vehicle).
        - override describe() so that it returns the Vehicle's description followed by the doors:
              "<Vehicle description>, <doors> doors"
          e.g. "Civic: 200 hp, top speed 200 km/h, 4 doors"
          Tip: reuse the base class version instead of rebuilding the string:
              Vehicle::describe()

    Create a class called "Truck" (inherits from Vehicle):
        - it must have a private variable of type int: loadKg
        - create a public constructor that initialises these values, in this order:
              Truck(string name, int horsepower, int loadKg)
        - override topSpeed(): a loaded truck is slower. It returns the Vehicle's top speed
          minus loadKg / 100   (integer division)
          Tip: call Vehicle::topSpeed() to get the base value.
        - override describe() so that it returns the Vehicle's description followed by the load:
              "<Vehicle description>, load <loadKg> kg"
          e.g. "Volvo: 400 hp, top speed 250 km/h, load 5000 kg"
          Note: because Vehicle::describe() calls topSpeed(), and topSpeed() is virtual,
          the Truck's slower speed will automatically show up in the description.

    Hint: Declare the classes in this order:
    Engine, Vehicle, Car, Truck
    (a class must be fully defined before it is used as a member or as a base class).
*/

////////////////////////////////////////////////////
// Write your code here
class Engine {
private:
    int horsepower;

public:
    Engine(int horsepower) : horsepower(horsepower) {}

    int getHorsepower() const {
        return horsepower;
    }
};

class Vehicle {
private:
    string name;
    Engine engine;

public:
    Vehicle(string name, int horsepower)
        : name(name), engine(horsepower) {}

    virtual ~Vehicle() {}

    string getName() const {
        return name;
    }

    virtual int topSpeed() const {
        return 100 + engine.getHorsepower() / 2;
    }

    virtual string describe() const {
        return name + ": " +
               to_string(engine.getHorsepower()) +
               " hp, top speed " +
               to_string(topSpeed()) + " km/h";
    }
};

class Car : public Vehicle {
private:
    int doors;

public:
    Car(string name, int horsepower, int doors)
        : Vehicle(name, horsepower), doors(doors) {}

    string describe() const override {
        return Vehicle::describe() + ", " +
               to_string(doors) + " doors";
    }
};

class Truck : public Vehicle {
private:
    int loadKg;

public:
    Truck(string name, int horsepower, int loadKg)
        : Vehicle(name, horsepower), loadKg(loadKg) {}

    int topSpeed() const override {
        return Vehicle::topSpeed() - loadKg / 100;
    }

    string describe() const override {
        return Vehicle::describe() + ", load " +
               to_string(loadKg) + " kg";
    }
};



// Do not touch anything below this line
////////////////////////////////////////////////////

Vehicle* vehicleFromArgs(const char* type, const char* name, const char* argHorsepower, const char* argExtra)
{
    int horsepower = stoi(argHorsepower);
    int extra      = stoi(argExtra);

    if (strcmp(type, "Car") == 0)
        return new Car(name, horsepower, extra);
    if (strcmp(type, "Truck") == 0)
        return new Truck(name, horsepower, extra);
    if (strcmp(type, "Vehicle") == 0)
        return new Vehicle(name, horsepower);
    return nullptr;
}

int main(int argc, char* argv[]) {
    char* testArgs[] = {
        (char*)"command",
        (char*)"Truck", (char*)"Volvo", (char*)"400", (char*)"5000"
    };

    if (argc <= 1)
        argv = testArgs;

    Vehicle* vehicle = vehicleFromArgs(argv[1], argv[2], argv[3], argv[4]);
    cout << vehicle->describe() << endl;

    delete vehicle;
    return 0;
}
