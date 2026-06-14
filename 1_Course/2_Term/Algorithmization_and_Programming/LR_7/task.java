import static java.lang.Math.*;
import java.util.Locale;
import java.util.Scanner;

class task {
    public static void main(String args[]) {
        Scanner input = new Scanner(System.in);
        input.useLocale(Locale.US);

        double z, a, b, x;
        
        while (true) {
            System.out.print("Input a -> ");
            a = input.nextDouble();
            System.out.print("Input b -> ");
            b = input.nextDouble();
            if (b > a) {
                break;
            } else {
                System.out.println("Параметр a должен быть больше чем параметр b! Повторите ввод...\n");
            }
        }
        
        System.out.print("Input x -> ");
        x = input.nextDouble();
        

        if (x <= a) {
            z = 1.7 * sin(x);
        } else if (a < x && x < b) {
            z = cos(x) + x*x;
        } else {
            z = pow(x, 5);
        }

        System.out.println("\nz = " + z);
        input.close();
    }
}