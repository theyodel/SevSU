import java.util.Scanner;

class array {
    private int n;
    private double []arr;

    void createArray(int len) {
        Scanner in = new Scanner(System.in);
        
        this.n = len;
        arr = new double[len];

        for (int i = 0; i < n; i++) {
            System.out.printf("Введите %d элемент массива -> ", i);
            arr[i] = in.nextDouble();
        }

        in.close();
    }

    void printArray() {
        System.out.print("{ ");
        for (int i = 0; i < this.n; i++) {
            System.out.printf("%.4f; ", arr[i]);
        }
        System.out.printf("}\n");
    }

    double sumBetweenNegatives() {
        double sum = 0;
        int fOtric = -1, lOtric = -1;

        for (int i = 0; i < this.n; i++) {
            if (arr[i] < 0 && fOtric == -1) fOtric = i;
            if (arr[i] < 0 && fOtric < i) lOtric = i;
        }

        if (lOtric == -1) return -1;

        for (int i = fOtric+1; i < lOtric; i++) {
                sum += arr[i];             
        }
        return sum;
    }

    double sumOfOdds() {
        double sum = 0;
        for (int i = 1; i < n; i += 2) {
            sum += arr[i];
        }
        return sum;
    }

    void replaceByModule() {
        for (int i = 0; i < this.n; i++) {
            if (Math.abs(arr[i]) < 1) arr[i] = 1;
        }
    }

    void bubbleSort() {
        for (int i = 0; i < n-1; i++) {
            for (int j = 0; j < n-1-i; j++) {
                if (arr[j] > arr[j+1]) {
                    double temp = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = temp;
                }
            }
        }
    }
}

public class task2 {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int n;
        
        array arr = new array();

        while (true) {
            System.out.print("Введите длину массива -> ");
            n = in.nextInt();

            if (n <= 0) System.out.println("    Длина массива не может быть отрицательной или равна нулю! Повторите ввод...");
            else break;
        }

        arr.createArray(n);
        
        System.out.println("\nПолученный массив:");
        arr.printArray();

        System.out.printf("\nСумма элементов массива с нечётными номерами: %.4f\n", arr.sumOfOdds());

        if (arr.sumBetweenNegatives() == -1) System.out.println("\nВ массиве <= 1 отрицательного элемента! Поиск суммы между двух отрицательных элементов невозможен!");
        else System.out.printf("\nСумма элементов массива между первым и последним отрицательным элементом: %.4f\n", arr.sumBetweenNegatives());

        arr.replaceByModule();
        arr.bubbleSort();
        System.out.println("Результат замены элементов, чей модуль < 1 на 1 и сортировки, методом 'пузырька':");
        arr.printArray();

        in.close();
    }
}