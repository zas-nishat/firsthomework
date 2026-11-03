import java.util.Scanner;

public class Task6 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter an integer N (N >= 2): ");
        int n = scanner.nextInt();

        System.out.println("Prime numbers up to " + n + ":");
        for (int number = 2; number <= n; number++) {
            boolean isPrime = true;

            for (int i = 2; i * i <= number; i++) {
                if (number % i == 0) {
                    isPrime = false;
                    break;
                }
            }

            if (isPrime) {
                System.out.print(number + " ");
            }
        }
        System.out.println();
        scanner.close();
    }
}