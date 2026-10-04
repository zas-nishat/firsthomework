import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);

        int v = input.nextInt();
        int n = input.nextInt();

        int k = (v * 12) / n;

        System.out.println(k);
    }
}