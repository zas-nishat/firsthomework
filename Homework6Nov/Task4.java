public class Task4 {
    public static void main(String[] args) {
        // tens digit (t) and ones digit (o)
        for (int t = 1; t <= 9; t++) {
            for (int o = 0; o <= 9; o++) {
                // tens digit is half its ones digit -> o == 2 * t
                if (o == 2 * t) {
                    int originalNumber = 10 * t + o;
                    int reversedNumber = 10 * o + t;

                    if (reversedNumber - originalNumber == 36) {
                        System.out.println("The original number is: " + originalNumber);
                    }
                }
            }
        }
    }
}