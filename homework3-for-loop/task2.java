import java.util.Scanner;

public class task2 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter a positive integer N: ");
        int n = scanner.nextInt();

        int count = 0;
        System.out.println("Even numbers:");
        for (int i = 1; i <= n; i++) {
            if (i % 2 == 0) {
                System.out.print(i + " ");
                count++;
            }
        }
        System.out.println("\nCount: " + count);
        scanner.close();
    }
}