import java.util.Scanner;

public class Task4 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        if (!scanner.hasNextInt()) return;
        int n = scanner.nextInt();

        int[][] matrix = new int[n][n];

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                matrix[i][j] = scanner.nextInt();
            }
        }

        // Diagonal elements transformation
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                // Main diagonal: i == j, Secondary diagonal: i + j == n - 1
                if (i == j || i + j == n - 1) {
                    matrix[i][j] = 0;
                }
            }
        }

        // Print modified matrix
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                System.out.print(matrix[i][j] + (j == n - 1 ? "" : " "));
            }
            System.out.println();
        }

        scanner.close();
    }
}