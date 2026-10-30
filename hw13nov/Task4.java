import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class Task4 {
    public static void main(String[] args) {
        File file = new File("duomenys.txt");

        try {
            Scanner scanner = new Scanner(file);

            if (scanner.hasNextInt()) {
                int n = scanner.nextInt();total number count 'n'
                int count = 0;

                if (n > 0 && scanner.hasNextInt()) {
                    int previous = scanner.nextInt();

                    for (int i = 1; i < n; i++) {
                        if (scanner.hasNextInt()) {
                            int current = scanner.nextInt();
                            if (current < previous) {
                                count++;
                            }
                            previous = current; 
                        }
                    }
                }

                System.out.println(count);
            }

            scanner.close();
        } catch (FileNotFoundException e) {
            System.out.println("File not found: " + e.getMessage());
        }
    }
}