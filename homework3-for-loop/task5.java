import java.util.Scanner;

public class task5 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter height (>=2): ");
        int height = scanner.nextInt();
        System.out.print("Enter width (>=2): ");
        int width = scanner.nextInt();

        for (int i = 1; i <= height; i++) {
            for (int j = 1; j <= width; j++) {
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