#include <iostream>
#include <string>
using namespace std;

/* Task 1: vowel / consonant extraction.
    Input: a single argument (string) that comes in through argv[1]
    Output (and this has to be exact):
        1) print all vowels from the string in order they appear
        2) print a space
        3) print all consonants from the string in order they appear
        E.g.: "Hello, world!" would output "eoo Hllwrld"

    Note: list of vowels: a, e, i, o, u, y. The rest of the alphabet are consonants.
*/

int main(int argc, char *argv[])
{
    // Ensure an argument was provided
    if (argc < 2)
    {
        return 0;
    }

    string input = argv[1];
    string vowels = "";
    string consonants = "";

    // Helper lambda or inline check for vowels (case-insensitive)
    for (char c : input)
    {
        char lower = tolower(c);
        if (lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u' || lower == 'y')
        {
            vowels += c;
        }
        else if ((lower >= 'a' && lower <= 'z'))
        {
            // It's an alphabet letter but not a vowel, so it's a consonant
            consonants += c;
        }
        // Note: Characters that are neither letters nor vowels (like spaces, punctuation)
        // are ignored in the separation, matching standard behavior.
    }

    // Print vowels, a space, and then consonants
    cout << vowels << " " << consonants;

    return 0;
}