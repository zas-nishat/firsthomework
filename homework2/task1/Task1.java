package task1;

import java.util.Scanner;

public class Task1 {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        int M = input.nextInt();
        if (M >= 1896 && (M - 1896) % 4 == 0) {
            int edition = (M - 1896) / 4 + 1;
            System.out.println("Olympic year");
            System.out.println("Edition number: " + edition);
        } else {
            System.out.println("Not an Olympic year");
        }
        input.close();
    }
}