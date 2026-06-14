import static java.lang.Math.*;
import java.util.Locale;
import java.util.Scanner;

public class taskFor {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        input.useLocale(Locale.US);

        double z, a, b, deltaX, xStart, xEnd;
        
        while (true) {
            System.out.print("Input a -> ");
            a = input.nextDouble();
            System.out.print("Input b -> ");
            b = input.nextDouble();
            if (b > a) break;
            else System.out.println("Параметр b должен быть больше чем параметр a! Повторите ввод...\n");
            
        }

        while (true) {
            System.out.print("Input x_start -> ");
            xStart = input.nextDouble();
            System.out.print("Input x_end -> ");
            xEnd = input.nextDouble();
            if (xEnd > xStart) break;
            else System.out.println("Параметр x_end должен быть больше чем параметр x_start! Повторите ввод...\n");
        }
        
        while (true) {
            System.out.print("Input delta_x -> ");
            deltaX = input.nextDouble();
            if (deltaX > 0) break;
            else System.out.println("Параметр delta_x должен быть больше чем параметр x_start! Повторите ввод...\n");
        }
        System.out.println("\n              z = f(x)             ");
        System.out.println("+----------------+----------------+");
        System.out.println("|        x       |      f(x)      |");
        System.out.println("+----------------+----------------+");

        for (double now = xStart; now <= xEnd; now += deltaX) {
            if (now <= a) {
                z = 1.7 * sin(now);
            } else if (a < now && now < b) {
                z = cos(now) + pow(now, 2);
            } else {
                z = pow(now, 5);
            }
            System.out.printf("| %-14.4f | %-14.4f |\n", now, z);
            System.out.println("+----------------+----------------+");
        }

        input.close();
    }
}