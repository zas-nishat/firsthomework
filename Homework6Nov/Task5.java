import java.util.Scanner;

public class Task5 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter height: ");
        int height = scanner.nextInt();
        System.out.print("Enter width: ");
        int width = scanner.nextInt();

        for (int i = 1; i <= height; i++) {
            for (int j = 1; j <= width; j++) {
                // Print '*' for borders, space for inside
                if (i == 1 || i == height || j == 1 || j == width) {
                    System.out.print("*");
                } else {
                    System.out.print(" ");
                }
            }
            System.out.println();
        }
        scanner.close();
    }
}