#include <iostream>
#include <string>
using namespace std;

/* Example: Composition & Inheritance: Devices
    Composition: a Device HAS a Battery.
    Inheritance: a Phone IS a Device, and a Laptop IS a Device.
*/

// ---------------------------------------------------------------
// Battery: a simple class that will be used as a member of Device
// ---------------------------------------------------------------
class Battery {
    int capacityMah; // private: only Battery's own functions can touch it
public:
    Battery(int capacityMah) : capacityMah(capacityMah) { }
    int getCapacity() { return capacityMah; }
};

// ---------------------------------------------------------------
// Device: the base class
// ---------------------------------------------------------------
class Device {
    string name;
protected:
    Battery battery; // composition: a Battery object lives inside every Device
public:
    // The initialiser list builds the members.
    // battery(capacityMah) calls Battery's constructor.
    Device(string name, int capacityMah) : name(name), battery(capacityMah) { }

    virtual ~Device() { }                 // virtual destructor: good practice for base classes

    string getName() { return name; }

    // virtual = derived classes are allowed to replace this function
    virtual int runtimeHours() {
        return battery.getCapacity() / 100;
    }

    virtual string describe() {
        return name + ": " + to_string(battery.getCapacity()) + " mAh, runs for "
               + to_string(runtimeHours()) + " h";
    }
};

// ---------------------------------------------------------------
// Phone: inherits from Device, adds a screen size, changes nothing else about runtime
// ---------------------------------------------------------------
class Phone : public Device {
    int screenInches;
public:
    // Device(name, capacityMah) in the initialiser list builds the base part first
    Phone(string name, int capacityMah, int screenInches)
        : Device(name, capacityMah), screenInches(screenInches) { }

    // Reuses the base description and adds to it
    string describe() override {
        return Device::describe() + ", " + to_string(screenInches) + " inch screen";
    }
};

// ---------------------------------------------------------------
// Laptop: inherits from Device, overrides runtimeHours()
// ---------------------------------------------------------------
class Laptop : public Device {
    int gpuWatts;
public:
    Laptop(string name, int capacityMah, int gpuWatts)
        : Device(name, capacityMah), gpuWatts(gpuWatts) { }

    // A power-hungry GPU shortens the runtime. Device::runtimeHours() gives the base value.
    int runtimeHours() override {
        return battery.getCapacity() / (100 + gpuWatts);
    }

    // Device::describe() calls runtimeHours(). Because it is virtual, the Laptop's version is used.
    string describe() override {
        return Device::describe() + ", " + to_string(gpuWatts) + " W GPU";
    }
};

int main() {
    Device device("Radio", 2000);
    Phone phone("Pixel", 5000, 6);
    Laptop laptop("Zenbook", 12000, 100);

    cout << device.describe() << endl;
    cout << phone.describe() << endl;
    cout << laptop.describe() << endl;

    // Polymorphism: a Device pointer can point to a Phone or a Laptop,
    // and the correct describe() is still called.
    Device* devices[] = { new Phone("Galaxy", 4500, 7), new Laptop("ThinkPad", 8000, 40) };
    for (Device* d : devices) {
        cout << d->describe() << endl;
        delete d;
    }
    return 0;
}