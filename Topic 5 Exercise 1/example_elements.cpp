#include <iostream>
#include <string>
#include <cstring>
using namespace std;

/* Elements: applying one element to another

    Three elements (Earth, Fire, Water) can be applied to each other, and what happens
    depends on BOTH of them. "a.applyTo(&b)" means: apply a to b.

    Most results are NOT symmetric (water on fire is not the same as fire on water),
    so most functions are written on their own. The one exception is Earth + Water,
    which gives the same result in both directions, so we only write it once and let
    the other direction call it.
*/

// Forward declarations: these tell the compiler "these classes exist" so that
// Element can mention Earth*, Fire* and Water* before the classes are defined.
// (Pointers to an incomplete class are allowed; using its members is not.)
class Earth;
class Fire;
class Water;

// Abstract base class: it has pure virtual functions (= 0), so you cannot create
// an Element object directly. It only defines what every element must be able to do.
class Element {
public:
    // Three overloads with the same name, one per possible target.
    // The compiler picks the right one based on the type of the argument.
    virtual string applyTo(Earth* target) = 0;
    virtual string applyTo(Fire* target) = 0;
    virtual string applyTo(Water* target) = 0;

    // Virtual destructor: makes sure deleting through an Element* cleans up properly.
    virtual ~Element() {}
};

// Each subclass declares (but does not yet define) all three overloads.
// "override" asks the compiler to check that we really match a function in Element.
class Earth : public Element {
public:
    string applyTo(Earth* target) override;
    string applyTo(Fire* target) override;
    string applyTo(Water* target) override;
};

class Fire : public Element {
public:
    string applyTo(Earth* target) override;
    string applyTo(Fire* target) override;
    string applyTo(Water* target) override;
};

class Water : public Element {
public:
    string applyTo(Earth* target) override;
    string applyTo(Fire* target) override;
    string applyTo(Water* target) override;
};

// The function bodies are written AFTER all classes, so that every class is
// complete by the time one function refers to another class's members.
// The parameters are unnamed where we don't use them (avoids "unused" warnings).

// ---- Earth applied to ... ----
string Earth::applyTo(Earth*) { return "makes a bigger pile"; }
string Earth::applyTo(Fire*)  { return "smothers the fire"; }
// string Earth::applyTo(Water*) { return "muddies the water"; }

// ---- Fire applied to ... ----
string Fire::applyTo(Earth*)  { return "bakes the earth into clay"; }
string Fire::applyTo(Fire*)   { return "burns brighter"; }
string Fire::applyTo(Water*)  { return "evaporates the water"; }

// ---- Water applied to ... ----
// string Water::applyTo(Earth*) { return "turns the earth into mud"; }
string Water::applyTo(Fire*)  { return "extinguishes the fire"; }
string Water::applyTo(Water*) { return "makes a bigger puddle"; }

// use function from the opposing side if it's symmetrical - avoid reimplementation
//
// Earth + Water has the same result in both directions, so only one side gets the
// real answer:
string Earth::applyTo(Water*) { return "water and mud mixes"; }

// The other side hands the work over to it. Here "this" is a Water*, so
// target->applyTo(this) calls Earth::applyTo(Water*) (the line above), and the
// result is passed straight back.
// Careful: this only works because Earth::applyTo(Water*) does NOT call back.
// If both sides delegated to each other, the program would recurse forever.
string Water::applyTo(Earth* target) { return target->applyTo(this); }

// ---- apply(): choosing the right overload at RUNTIME ----
//
// The problem: inside a loop we only have Element* pointers. The compiler only
// knows "this is some kind of Element" and has no idea whether it is really an
// Earth, a Fire or a Water. But applyTo() has three overloads, and the compiler
// must pick one at compile time based on the STATIC type of the argument.
// An Element* matches none of them, so we can't just write source->applyTo(target).
//
// The fix: ask the target what it really is, then pass it along with its
// real type. That "asking" is done with dynamic_cast.
//
// dynamic_cast<Earth*>(target) checks, while the program runs, whether the object
// that target points to is really an Earth:
//   - if yes, it returns a valid Earth* pointing to that same object
//   - if no, it returns nullptr (which counts as "false" in an if)
// This only works because Element has virtual functions (it is "polymorphic").
//
// Two things are happening at once in each if:
//   1. "Earth* earth = dynamic_cast<Earth*>(target)" declares a new variable AND
//      assigns the result of the cast to it.
//   2. The if then tests that variable: non-null means true, nullptr means false.
// The variable only exists inside that if / else-if chain.
string apply(Element* source, Element* target) {
    if      (Earth* earth = dynamic_cast<Earth*>(target))   return source->applyTo(earth);
    else if (Fire* fire = dynamic_cast<Fire*>(target))      return source->applyTo(fire);
    else if (Water* water = dynamic_cast<Water*>(target))   return source->applyTo(water);
    else return "Unknown element";
}
// Notice that we never cast "source". We don't need to: applyTo is a
// VIRTUAL function, so C++ automatically calls the version belonging to the
// source's real type (Earth::applyTo, Fire::applyTo or Water::applyTo).
//
// So two different mechanisms combine to find the right function:
//   - the SOURCE is resolved by virtual functions (automatic)
//   - the TARGET is resolved by our dynamic_cast checks (manual)
// This pattern is sometimes called "double dispatch".

int main(int argc, char* argv[]) {
    // Plain objects on the stack - no new/delete needed here.
    Earth earth = Earth();
    Fire fire = Fire();
    Water water = Water();

    // We pass the ADDRESS of the target (&), because the functions take pointers.
    // The static types are known here (Earth, Fire, Water), so the compiler can pick
    // the correct overload directly. That's why no casting is needed here.

    // Earth applied to everything
    cout << earth.applyTo(&earth) << endl;
    cout << earth.applyTo(&water) << endl;
    cout << earth.applyTo(&fire) << endl;

    // Fire applied to everything
    cout << fire.applyTo(&earth) << endl;
    cout << fire.applyTo(&water) << endl;
    cout << fire.applyTo(&fire) << endl;

    // Water applied to everything
    // (water on earth is the delegated case: it ends up in Earth::applyTo(Water*))
    cout << water.applyTo(&earth) << endl;
    cout << water.applyTo(&water) << endl;
    cout << water.applyTo(&fire) << endl;

    // ---- Looping over every combination using the base class ----
    //
    // Above, we called each function by hand and the compiler knew the exact
    // type every time. Now we want to try ALL 9 combinations without writing 9 lines.
    //
    // An array of Element* pointers can hold addresses of Earth, Fire AND Water
    // objects together, because each of them "is an" Element (inheritance).
    // We store the address (&) of each of our three objects from above.
    // Note: the array doesn't copy the objects, it only points to them,
    // so earth, fire and water must stay alive while we use the array.
    Element* elements[3] = { &earth, &fire, &water };

    // Outer loop: each element takes a turn as the SOURCE (the one being applied).
    // This is a "range-based for loop": element_1 becomes each pointer in the
    // array in turn, first &earth, then &fire, then &water.
    for (Element* element_1 : elements)

        // Inner loop: for EACH source, every element takes a turn as the TARGET
        // (the one being applied to).
        for (Element* element_2 : elements)

            // Together the loops produce all 3 x 3 = 9 pairs:
            //   earth->earth, earth->fire, earth->water,
            //   fire->earth,  fire->fire,  fire->water,
            //   water->earth, water->fire, water->water
            //
            // We call apply() instead of applyTo() directly, because here both
            // variables are plain Element* pointers. apply() figures out the real
            // type of the target (using dynamic_cast, see above) and calls the
            // correct overload. The result string is then printed.
            cout << apply(element_1, element_2) << endl;

    return 0;
}