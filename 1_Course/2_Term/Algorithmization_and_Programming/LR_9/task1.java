
import java.util.Scanner;

public class task1 {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        
        System.out.print("Введите длину массива -> ");
        int n = in.nextInt();
        int[] arr = new int[n];

        for (int i = 0; i < n; i++) {
            System.out.printf("Введите %d элемент массива -> ", i);
            arr[i] = in.nextInt();
        }

    }
}
