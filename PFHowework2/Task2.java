import java.util.Locale;
import java.util.Scanner;

public class Task2 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        if (!scanner.hasNextInt()) return;
        int n = scanner.nextInt();

        int[] residents = new int[n];
        for (int i = 0; i < n; i++) {
            residents[i] = scanner.nextInt();
        }

        int leftTotal = 0;
        int leftHouseCount = 0;
        int rightTotal = 0;
        int rightHouseCount = 0;

        // House numbers 1-indexed (1 to n)
        for (int i = 0; i < n; i++) {
            int houseNumber = i + 1;
            if (houseNumber % 2 != 0) { // Odd number = Left side
                leftTotal += residents[i];
                leftHouseCount++;
            } else { // Even number = Right side
                rightTotal += residents[i];
                rightHouseCount++;
            }
        }

        int totalResidents = leftTotal + rightTotal;

        System.out.println("Total residents: " + totalResidents);
        System.out.println("Residents on the left: " + leftTotal);
        System.out.println("Residents on the right: " + rightTotal);

        // Average calculations
        if (leftHouseCount > 0) {
            double leftAvg = (double) leftTotal / leftHouseCount;
            System.out.println("Average per house on the left: " + formatAverage(leftAvg));
        } else {
            System.out.println("Average per house on the left: N/A");
        }

        if (rightHouseCount > 0) {
            double rightAvg = (double) rightTotal / rightHouseCount;
            System.out.println("Average per house on the right: " + formatAverage(rightAvg));
        } else {
            System.out.println("Average per house on the right: N/A");
        }

        scanner.close();
    }

    // Integers hole integer print korbe, decimal hole properly format korbe
    private static String formatAverage(double avg) {
        if (avg == (long) avg) {
            return String.format(Locale.US, "%d", (long) avg);
        } else {
            return String.format(Locale.US, "%.2f", avg);
        }
    }
}