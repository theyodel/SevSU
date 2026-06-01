import static java.lang.Math.*;
import java.util.Locale;
import java.util.Scanner;

public class taskWhile {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        in.useLocale(Locale.US);

        System.out.print("Input a -> ");
        double a = in.nextDouble();
        System.out.print("Input b -> ");
        double b = in.nextDouble();
        System.out.print("Input x_start -> ");
        double xStart = in.nextDouble();
        System.out.print("Input end -> ");
        double xEnd = in.nextDouble();
        System.out.print("Input delta_x -> ");
        double deltaX = in.nextDouble();
        double z;

        if (b < a) {
            System.err.println("Параметр a должен быть больше чем параметр b!");
            in.close();
            return;
        }

        System.out.println("+----------------+----------------+");
        System.out.println("|        x       |      f(x)      |");
        System.out.println("+----------------+----------------+");

        double now = xStart;

        while (now <= xEnd) {
            if (now <= a) {
                z = 1.7 * sin(now);
            } else if (a < now && now < b) {
                z = cos(now) + pow(now, 2);
            } else {
                z = pow(now, 5);
            }
            System.out.printf("| %-14.4f | %-14.4f |\n", now, z);
            System.out.println("+----------------+----------------+");
            now += deltaX;
        }
    }
}