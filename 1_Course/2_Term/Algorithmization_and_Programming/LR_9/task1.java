import java.util.Scanner;

public class task1 {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int n;

        while (true) {
            System.out.print("Введите длину массива -> ");
            n = in.nextInt();

            if (n <= 0) System.out.println("    Длина массива не может быть отрицательной или равна нулю! Повторите ввод...");
            else break;
        }
        
        double[] arr = new double[n];
        int fOtric = -1, lOtric = -1;

        for (int i = 0; i < n; i++) {
            System.out.printf("Введите %d элемент массива -> ", i);
            arr[i] = in.nextDouble();
            if (arr[i] < 0 && fOtric == -1) fOtric = i;
            if (arr[i] < 0 && fOtric < i) lOtric = i;
        }

        System.out.printf("Полученный массив: \n{ ");
        for (int i = 0; i < n; i++) {
            System.out.printf("%.4f; ", arr[i]);
        }
        System.out.printf("}\n\n");
        
        double sum = 0;
        for (int i = 1; i < n; i += 2) {
            sum += arr[i];
        }
        System.out.printf("Сумма элементов массива с нечётными номерами: %.4f\n\n", sum);

        sum = 0;
        if (lOtric == -1) System.out.println("В массиве <= 1 отрицательного элемента! Поиск суммы между двух отрицательных элементов невозможен!");
        else {
            for (int i = fOtric+1; i < lOtric; i++) {
                sum += arr[i];             
            }
            System.out.printf("Сумма элементов массива между первым и последним отрицательным элементом: %.4f\n\n", sum);
        }

        for (int i = 0; i < n; i++) {
            if (Math.abs(arr[i]) < 1) arr[i] = 1;
        }

        for (int i = 0; i < n-1; i++) {
            for (int j = 0; j < n-1-i; j++) {
                if (arr[j] > arr[j+1]) {
                    double temp = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = temp;
                }
            }
        }

        System.out.printf("Результат замены элементов, чей модуль < 1 на 1 и сортировки, методом 'пузырька': \n{ ");
        for (int i = 0; i < n; i++) {
            System.out.printf("%.4f; ", arr[i]);
        }
        System.out.printf("}\n\n");
    
        in.close();
    }
}