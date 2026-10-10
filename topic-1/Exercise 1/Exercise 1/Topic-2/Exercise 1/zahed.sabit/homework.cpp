#include <iostream>
#include <string>
using namespace std;

/* Exercise 1: Class Creation

    Create a class called Employee.

    I have filled out the main() function to utilize the class you will be writing.
    Make sure the class name and function names are identical to the ones I used.

    1 Point:
        Create a class Employee containing publically accessible variables:
        - age        (in years) (integer)
        - experience (in years) (integer)
        - salary     (in Eur)   (integer)

    1 Point:
        Create a method (function) called calculateTax() that calculates how much tax the employee pays (25%).
        It must return an integer (rounded down).
        Example: if salary is 2000, return 500

    1 Point:
        Create a method called printInfo() that displays all information about the employee on the screen.

        It should use cout to print the result as such (replace <value> with actual values):
        Age: <age>
        Experience: <experience>
        Salary: <salary>
        Tax: <tax>

    1 Point:
        Create a constructor that assigns default values to the employee when an Employee object is created without any arguments.

        Use these default values:
        - age: 30
        - experience: 5
        - salary: 2000
*/

////////////////////////////////////////////////////
// Write your class here
class Employee {
public:
    int age;
    int experience;
    int salary;

    // Parameterized constructor with default values to handle both cases
    Employee(int a = 30, int e = 5, int s = 2000) {
        age = a;
        experience = e;
        salary = s;
    }

    int calculateTax() {
        return salary * 25 / 100;
    }

    void printInfo() {
        cout << "Age: " << age << "\n"
             << "Experience: " << experience << "\n"
             << "Salary: " << salary << "\n"
             << "Tax: " << calculateTax() << "\n";
    }
};// Do not touch anything below this line
////////////////////////////////////////////////////

int main(int argc, char* argv[]) {

    // Do not edit code below this line.
    // A separate script invokes this code, and reads cout to check if it matches expected values (and does not crash).

    int test = stoi(argv[1]);
    int age = stoi(argv[2]);
    int experience = stoi(argv[3]);
    int salary = stoi(argv[4]);

    Employee employee(age, experience, salary);

    switch (test)
    {
        case 1: // Object created correctly?
        {
            cout << employee.age << " "
                 << employee.experience << " "
                 << employee.salary;
            break;
        }

        case 2: // printInfo() test.
        {
            employee.printInfo();
            break;
        }

        case 3: // Test whether the constructor has default values.
        {
            Employee employee;

            cout << employee.age << " "
                 << employee.experience << " "
                 << employee.salary;
            break;
        }

        case 4: // Test tax calculation.
        {
            cout << employee.calculateTax();
            break;
        }
    }

    return 0;
}