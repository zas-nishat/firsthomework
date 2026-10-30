import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.Scanner;

public class Task3 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        if (!scanner.hasNextInt()) return;
        int numSubjects = scanner.nextInt();
        int numStudents = scanner.nextInt();

        int[][] grades = new int[numStudents][numSubjects];
        double[] studentAverages = new double[numStudents];
        double totalSumAll = 0;

        List<Integer> studentsWithFails = new ArrayList<>();
        double maxAverage = -1;

        for (int i = 0; i < numStudents; i++) {
            int studentSum = 0;
            boolean hasGradeBelow5 = false;

            for (int j = 0; j < numSubjects; j++) {
                grades[i][j] = scanner.nextInt();
                studentSum += grades[i][j];

                if (grades[i][j] < 5) {
                    hasGradeBelow5 = true;
                }
            }

            studentAverages[i] = (double) studentSum / numSubjects;
            totalSumAll += studentAverages[i];

            if (studentAverages[i] > maxAverage) {
                maxAverage = studentAverages[i];
            }

            if (hasGradeBelow5) {
                studentsWithFails.add(i + 1); // Student numbers are 1-indexed
            }
        }

        double groupAverage = totalSumAll / numStudents;

        // Output Formatting
        System.out.println("Student averages:");
        for (double avg : studentAverages) {
            System.out.println(formatValue(avg));
        }

        System.out.println("\nGroup average:");
        System.out.println(formatValue(groupAverage));

        System.out.println("\nHighest-performing student:");
        for (int i = 0; i < numStudents; i++) {
            if (Math.abs(studentAverages[i] - maxAverage) < 0.00001) {
                System.out.println(i + 1);
            }
        }

        System.out.println("\nStudents with at least one grade below 5:");
        if (studentsWithFails.isEmpty()) {
            System.out.println("None");
        } else {
            for (int studentNum : studentsWithFails) {
                System.out.println(studentNum);
            }
        }

        scanner.close();
    }

    private static String formatValue(double val) {
        if (val == (long) val) {
            return String.format(Locale.US, "%d", (long) val);
        } else {
            return String.format(Locale.US, "%.2f", val);
        }
    }
}