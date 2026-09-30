import java.util.Scanner;

public class task3 {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);

        int x1 = input.nextInt();
        int y1 = input.nextInt();
        int x2 = input.nextInt();
        int y2 = input.nextInt();

        int length = Math.abs(x2 - x1);
        int width = Math.abs(y1 - y2);

        int s = length * width;
        int p = 2 * (length + width);

        System.out.println("Area = " + s);
        System.out.println("Perimeter = " + p);
    }
}