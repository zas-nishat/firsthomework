import java.util.Scanner;

public class task1 {
    public static void main(String[] args) {
        try (Scanner input = new Scanner(System.in)) {
            int v = input.nextInt();
            int n = input.nextInt();

            int k = (v * 12) / n;

            System.out.println(k);
        }
    }
}
