import static java.lang.Math.*;
import java.util.Locale;
import java.util.Scanner;

class task {
    public static void main(String args[]) {
        Scanner in = new Scanner(System.in);
        in.useLocale(Locale.US);

        System.out.print("Input a -> ");
        double a = in.nextDouble();
        System.out.print("Input b -> ");
        double b = in.nextDouble();
        System.out.print("Input x -> ");
        double x = in.nextDouble();
        double z;

        if (b < a) {
            System.err.println("Параметр a должен быть больше чем параметр b!");
            in.close();
            return;
        }

        if (x <= a) {
            z = 1.7 * sin(x);
        } else if (a < x && x < b) {
            z = cos(x) + pow(x, 2);
        } else {
            z = pow(x, 5);
        }

        System.out.println("z = " + z);
        in.close();
    }
}