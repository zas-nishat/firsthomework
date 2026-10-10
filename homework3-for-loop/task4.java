public class task4 {
    public static void main(String[] args) {
        for (int num = 10; num <= 99; num++) {
            int tens = num / 10;
            int ones = num % 10;
            int reversed = ones * 10 + tens;

            if (ones == 2 * tens && (reversed - num) == 36) {
                System.out.println("The number the student thought of is: " + num);
            }
        }
    }
}