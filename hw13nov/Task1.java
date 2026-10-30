import java.util.Scanner;

public class Task1 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.print("Enter laptop price (p): ");
        double p = scanner.nextDouble();

        System.out.print("Enter current savings (s): ");
        double s = scanner.nextDouble();

        System.out.print("Enter weekly savings (m): ");
        double m = scanner.nextDouble();

        int k;
        if (s >= p) {
            k = 0;
        } else {
            k = (int) Math.ceil((p - s) / m);
        }

        System.out.println("k = " + k);
        scanner.close();
    }   
}