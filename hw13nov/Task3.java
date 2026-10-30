import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class Task3 {
    public static void main(String[] args) {
        File file = new File("duomenys.txt");

        try {
            Scanner scanner = new Scanner(file);

            if (scanner.hasNextInt()) {
                int n = scanner.nextInt(); 
                int negativeCount = 0;

                for (int i = 0; i < n; i++) {
                    if (scanner.hasNextInt()) {
                        int temp = scanner.nextInt();
                        if (temp < 0) {
                            negativeCount++;
                        }
                    }
                }

                if (negativeCount > 0) {
                    System.out.println("Number of negative temperature days: " + negativeCount);
                } else {
                    System.out.println("No negative temperatures recorded");
                }
            }

            scanner.close();
        } catch (FileNotFoundException e) {
            System.out.println("File not found: " + e.getMessage());
        }
    }
}