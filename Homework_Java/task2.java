import java.util.Scanner;

public class task2 {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        int length, width;
        double price;

        System.out.print("Enter length: ");
        length = sc.nextInt();

        System.out.print("Enter width: ");
        width = sc.nextInt();

        System.out.print("Enter tile price per square unit: ");
        price = sc.nextDouble();

        double area = length * width;
        double totalArea = area * 1.05; // 5% extra
        double totalCost = totalArea * price;

        System.out.println("Total cost = " + totalCost);
    }
}