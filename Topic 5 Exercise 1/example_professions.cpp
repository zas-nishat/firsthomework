#include <iostream> // Provides cout and endl for printing text.
#include <string>   // Provides the string type for storing text.

// "using namespace std;" lets us write string instead of std::string,
// and cout instead of std::cout.
//
// This is fine for short teaching programs, but it is usually avoided
// in larger programs, and should never be used in header files.
// Names from the standard library can clash with names you define
// yourself, and writing std:: makes it clear where a name comes from.
using namespace std;

// ============================================================
// BASE CLASS: Employee
// ============================================================

// A class is a blueprint for creating objects.
//
// An Employee object stores information about an employee and
// provides functions that employees can use.
//
// We call Employee a "base class" because other classes will
// inherit from it. This lets us define common functionality once
// instead of repeating the same code in every employee type.
class Employee
{
protected:
    // These variables store information about each employee.
    //
    // Each Employee object has its own name and salary.
    //
    // "protected" means:
    // - Functions inside Employee can access these variables.
    // - Functions inside classes derived from Employee can access them.
    // - Ordinary code outside these classes cannot access them directly.
    //
    // We use protected because Teacher and Doctor need access
    // to name in their own performDuty() functions.
    //
    // Compare this with "private", which would prevent Teacher
    // and Doctor from accessing these variables directly.
    //
    // Important: member variables are initialised in the order they
    // are DECLARED here (name first, then salary), not in the order
    // they appear in a constructor's initialiser list. If you ever
    // write the list in a different order, the compiler may warn you,
    // and you can get bugs if one member's value depends on another.

    string name;
    double salary;

public:
    // A constructor is a special function that runs automatically
    // when an object is created.
    //
    // Its purpose is to initialise the object's variables.
    //
    // This constructor takes two parameters:
    // - name: the employee's name.
    // - salary: the employee's monthly salary.
    //
    // The parameters have the same names as the member variables.
    // This is allowed because they have different scopes.
    Employee(string name, double salary)
        // This is a member initialiser list.
        //
        // It initialises the object's member variables before
        // the constructor body runs.
        //
        // The first "name" is the member variable.
        // The second "name" is the constructor parameter.
        //
        // The same applies to salary.
        //
        // Initialising members this way is generally preferable
        // to assigning their values inside the constructor body.
        // In some situations it is actually required:
        // - Calling a base-class constructor (see Teacher below).
        // - Initialising "const" members.
        // - Initialising reference members.
        // These cannot be assigned to later, so they must be
        // initialised in the list.
        : name(name), salary(salary)
    {
        // Nothing else needs to happen here.
        // The member variables have already been initialised.
    }

    // This function describes what an employee does.
    //
    // "virtual" is important for polymorphism.
    //
    // It allows a derived class to provide its own version
    // of this function, and for that version to be selected
    // when the function is called through a base-class pointer
    // or reference.
    //
    // For example:
    // - An Employee might perform general office work.
    // - A Teacher teaches a class.
    // - A Doctor treats a patient.
    //
    // The base-class version provides default behaviour.
    // Derived classes can replace that behaviour.
    //
    // The "const" after the parentheses is a promise that this
    // function does not modify the object. Printing a message does
    // not change the employee, so the promise is safe to make.
    // Marking functions const lets them be called on const objects
    // and through const references (see describe() below).
    // Any function that overrides this one must also be const.
    //
    // TRY THIS: remove the word "virtual" from this line, recompile,
    // and predict what the program prints. Both performDuty() calls
    // in main() will print "is doing general office work", because
    // without virtual, C++ chooses the function based on the pointer's
    // declared type (Employee*) rather than the actual object.
    virtual void performDuty() const
    {
        cout << name << " is doing general office work." << endl;
    }

    // This function prints the employee's details.
    //
    // It is not virtual because we do not need derived classes
    // to replace its behaviour in this example.
    //
    // It is const because it only reads name and salary. It does
    // not change them.
    //
    // Teacher and Doctor inherit this function automatically.
    //
    // Notice that it can access name and salary because they
    // are members of Employee.
    //
    // A note on output: cout uses default formatting for doubles,
    // so 2800 prints as 2800. Larger values such as 1234567 would
    // print in scientific notation (1.23457e+06). If you experiment
    // with bigger salaries, look up "fixed" and "setprecision"
    // from <iomanip> to control how numbers are displayed.
    void displayDetails() const
    {
        cout << name << " earns "
             << salary
             << " euros per month."
             << endl;
    }

    // A destructor runs when an object is destroyed.
    //
    // The tilde (~) before the class name identifies a destructor.
    //
    // Why is the destructor virtual?
    //
    // We might eventually use a pointer to Employee to refer
    // to a Teacher or Doctor object.
    //
    // If we delete an object through a base-class pointer,
    // a virtual destructor ensures that the derived class's
    // destructor runs as part of the destruction process.
    //
    // Without "virtual", deleting a derived object through a
    // base-class pointer is undefined behaviour. That means the
    // program might crash, leak resources, or appear to work
    // and then fail later. This is a real and common bug.
    //
    // This example does not allocate any objects dynamically,
    // so we do not actually need to delete anything.
    // Nevertheless, a virtual destructor is good practice
    // for a base class intended to be used polymorphically.
    virtual ~Employee() {}
};


// ============================================================
// DERIVED CLASS: Teacher
// ============================================================

// Teacher inherits from Employee.
//
// The syntax ": public Employee" means that Teacher is a
// publicly derived class of Employee.
//
// As a result, Teacher objects have:
// - A name.
// - A salary.
// - The displayDetails() function.
// - The performDuty() function.
//
// Teacher can also introduce its own functions or replace
// inherited virtual functions.
//
// This avoids duplicating the common employee functionality.
class Teacher : public Employee
{
public:
    // This is the constructor for Teacher.
    //
    // It accepts the teacher's name and salary.
    //
    // A Teacher is also an Employee, so the Employee part
    // of the object must be initialised first.
    Teacher(string name, double salary)

        // This calls the Employee constructor.
        //
        // It passes the parameters to Employee so that
        // Employee can initialise its name and salary.
        //
        // Without this initialiser, C++ would try to call
        // Employee's default constructor, which does not exist.
        : Employee(name, salary)
    {
        // The Employee constructor has already initialised
        // the inherited member variables.
        //
        // No additional initialisation is needed here.
    }

    // This function replaces Employee's performDuty() function
    // with behaviour specific to teachers.
    //
    // The "override" keyword tells the compiler that this
    // function is intended to override a virtual function
    // inherited from a base class.
    //
    // If we accidentally changed the function's name or
    // parameter list so that it no longer matched the base
    // function, the compiler would report an error.
    // This includes forgetting "const": a non-const performDuty()
    // would NOT match the const version in Employee, and the
    // compiler would reject the "override".
    //
    // The keyword is not strictly required, but it is useful
    // because it catches mistakes.
    void performDuty() const override
    {
        cout << name << " is teaching a class." << endl;
    }
};


// ============================================================
// DERIVED CLASS: Doctor
// ============================================================

// Doctor also inherits from Employee.
//
// Both Teacher and Doctor share the same basic employee data
// and functions, but they perform different duties.
class Doctor : public Employee
{
public:
    // The Doctor constructor works in the same way as
    // the Teacher constructor.
    //
    // It passes the name and salary to Employee's constructor
    // to initialise the inherited member variables.
    Doctor(string name, double salary)
        : Employee(name, salary)
    {
        // No additional initialisation is necessary.
    }

    // This overrides Employee::performDuty().
    //
    // The function has the same name and parameter list
    // (and the same const qualifier), but provides behaviour
    // appropriate for a doctor.
    void performDuty() const override
    {
        cout << name << " is treating a patient." << endl;
    }
};


// ============================================================
// FUNCTION OVERLOADING
// ============================================================

// Function overloading means defining multiple functions
// with the same name but different parameter lists.
//
// When a function is called, the compiler uses the arguments
// to decide which overload should be used.
//
// These functions are not distinguished by their return types.
// Their parameter types or parameter counts must differ.


// This overload accepts a reference to a const Employee.
//
// The '&' means staffMember is a reference to an existing object.
// It does not create a new Employee object or copy the object.
//
// A reference gives the function another way to access the
// original object.
//
// "const" means this function promises not to modify the object.
// That is why displayDetails() and performDuty() had to be
// declared const: a function that takes a const reference can
// only call const member functions.
//
// Because Teacher and Doctor inherit from Employee, either
// type of object can be passed to this function.
void describe(const Employee& staffMember)
{
    cout << "Employee details:" << endl;

    // Calls displayDetails() on the original object.
    //
    // This works for both Teacher and Doctor because
    // displayDetails() is inherited from Employee.
    staffMember.displayDetails();

    // Polymorphism also works through references, not just
    // pointers.
    //
    // staffMember is declared as a reference to Employee, but
    // performDuty() is virtual, so C++ calls the version belonging
    // to the actual object. For a Teacher this prints the teaching
    // message; for a Doctor it would print the treating message.
    staffMember.performDuty();
}


// This is a different overload of describe().
//
// It accepts a string instead of an Employee reference.
//
// The compiler can distinguish this function from the previous
// overload because their parameter types are different.
//
// Calling describe(teacher) selects the Employee overload.
// Calling describe("Central Hospital") selects this overload.
//
// A string literal such as "Central Hospital" can be converted
// to a string when passed to this function.
//
// We take the string by const reference rather than by value.
// This avoids making a copy of the string, and const promises
// that the function will not change it. For small programs the
// difference is tiny, but this is the usual habit for strings
// and other larger objects.
void describe(const string& workplace)
{
    cout << "Workplace: " << workplace << endl;
}


// ============================================================
// MAIN FUNCTION
// ============================================================

// Execution of this program begins in main().
//
// The int return type means main returns an integer to the
// operating system. Returning 0 conventionally indicates
// that the program completed successfully.
int main()
{
    // Create a Teacher object.
    //
    // This calls the Teacher constructor with:
    // - name = "Sarah"
    // - salary = 2800
    //
    // The Teacher constructor then calls the Employee
    // constructor to initialise the inherited members.
    //
    // The object is created as a local variable. It will be
    // destroyed automatically when main() finishes.
    Teacher teacher("Sarah", 2800);

    // Create a Doctor object in the same way.
    Doctor doctor("James", 4200);


    // --------------------------------------------------------
    // INHERITANCE AND POLYMORPHISM
    // --------------------------------------------------------

    // A pointer stores the memory address of an object.
    //
    // The '*' in "Employee*" indicates that staffMember is a
    // pointer to an Employee.
    //
    // The '&' in "&teacher" means "take the address of teacher".
    // It is different from the '&' in the describe() functions,
    // where '&' declares a reference.
    //
    // Although staffMember is an Employee pointer, it can point
    // to a Teacher object because Teacher inherits from Employee.
    //
    // No new object is created here. The pointer simply refers
    // to the Teacher object that already exists.
    Employee* staffMember = &teacher;

    // The arrow operator "->" is used to access a member
    // through a pointer.
    //
    // This:
    //     staffMember->performDuty();
    //
    // is equivalent to:
    //     (*staffMember).performDuty();
    //
    // The '*' dereferences the pointer, giving access to the
    // object it points to. The parentheses are necessary
    // because the dot operator has higher precedence.
    //
    // Because performDuty() is virtual, C++ calls the version
    // belonging to the actual object, which is a Teacher.
    //
    // Output:
    // Sarah is teaching a class.
    staffMember->performDuty();


    // A pointer can be changed to point to another compatible
    // object.
    //
    // Here, staffMember stops pointing to teacher and instead
    // points to doctor.
    //
    // This does not change or copy either object. It only
    // changes the address stored in the pointer.
    staffMember = &doctor;

    // The pointer's declared type is still Employee*.
    //
    // However, the object it currently points to is a Doctor.
    //
    // Because performDuty() is virtual, the Doctor version
    // is called.
    //
    // Output:
    // James is treating a patient.
    staffMember->performDuty();


    // --------------------------------------------------------
    // INHERITED FUNCTIONS
    // --------------------------------------------------------

    // Teacher and Doctor inherit displayDetails() from Employee.
    //
    // We can call the inherited function directly on each object.
    // We do not need to write another implementation in either
    // derived class.
    teacher.displayDetails();
    doctor.displayDetails();


    // --------------------------------------------------------
    // FUNCTION OVERLOADING
    // --------------------------------------------------------

    // The argument is a Teacher object.
    //
    // Teacher inherits from Employee, so it can be passed
    // to describe(const Employee&).
    //
    // The compiler selects the overload accepting an Employee
    // reference. Inside that function, displayDetails() prints
    // Sarah's employee information, and performDuty() prints
    // what she does (the Teacher version, because it is virtual).
    describe(teacher);

    // This argument is a string literal.
    //
    // The compiler selects describe(const string&) instead.
    // That overload prints a workplace description.
    describe("Central Hospital");


    // Returning 0 indicates successful program completion.
    //
    // The local objects doctor and teacher are automatically
    // destroyed when main() ends. We do not need to use delete
    // because we did not create them with new.
    return 0;
}


// ============================================================
// EXPECTED OUTPUT
// ============================================================
//
// If your program is working correctly, you should see:
//
// Sarah is teaching a class.
// James is treating a patient.
// Sarah earns 2800 euros per month.
// James earns 4200 euros per month.
// Employee details:
// Sarah earns 2800 euros per month.
// Sarah is teaching a class.
// Workplace: Central Hospital