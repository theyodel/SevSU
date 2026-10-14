import numpy as np
import matplotlib.pyplot as plt

def f(x):
    return x**4 + 8*x**3 - 6*x**2 - 72*x + 90

def viewPlot(flag=0, x=0, y=0):
    arr = np.linspace(-10, 6, 500)
    plt.plot(arr, f(arr), label="f(x)")
    plt.grid(True)
    if flag == 1:
        plt.plot(x, y, 'o', color="red", label="Найденная точка минимума")
    plt.legend()
    plt.show()

HEADER = (" № Итерации |    a    |    b    |   x_1   |  f(x_1) |"
          "   x_2   |  f(x_2) ")
ROW    = ("{:11d} | {:7.4f} | {:7.4f} | {:7.4f} | {:7.3f} |"
          " {:7.4f} | {:7.3f} ")

def print_result(name, x_min, f_min, a, b, iterations):
    print(f"\n\nРезультат вычисления по методу {name}:")
    print(f"    Точка минимума: x_min = {x_min:.6f}")
    print(f"    Значение в точке минимума: f(x_min) = {f_min:.6f}")
    print(f"    Интервал (границы): [{a:.6f}; {b:.6f}]")
    print(f"    Длина интервала: {np.abs(b - a):.6f}")
    print(f"    Заняло итераций: {iterations}")
    print("==========================================================\n\n")


def method_division(eps):
    a, b = -10, 6
    x_m = (a + b) / 2
    l = b - a
    iterations = 0
    print(HEADER)
    while np.abs(l) >= eps:
        iterations += 1
        x_1 = a + l / 4
        x_2 = b - l / 4
        fx_1 = f(x_1)
        fx_2 = f(x_2)
        fx_m = f(x_m)
        if fx_1 < fx_m:
            b = x_m
            x_m = x_1
            l = b - a
        elif fx_2 < fx_m:
            a = x_m
            x_m = x_2
            l = b - a
        else:
            a = x_1
            b = x_2
            l = b - a
        fx_m = f(x_m)
        print(ROW.format(iterations, a, b, x_1, fx_1, x_2, fx_2, x_m, fx_m))
    print_result("деления интервала", x_m, f(x_m), a, b, iterations)
    viewPlot(1, x_m, f(x_m))


def method_fibonacci(eps):
    a, b = -10, 6
    l = b - a
    iterations = 0
    ratio = np.ceil(l / eps)
    x_min, f_min = 10**6, 10**6

    # Генерация чисел Фибоначчи
    F = [1, 1]
    while F[-1] < ratio:
        F.append(F[-1] + F[-2])
    n = len(F) + 1
    while len(F) <= n + 2:
        F.append(F[-1] + F[-2])

    print(HEADER)
    while n > 1:
        iterations += 1
        x_1 = a + (F[n]     / F[n + 2]) * (b - a)
        x_2 = a + (F[n + 1] / F[n + 2]) * (b - a)
        fx_1 = f(x_1)
        fx_2 = f(x_2)
        print(ROW.format(iterations, a, b, x_1, fx_1, x_2, fx_2))

        if n == 2:
            if fx_1 <= fx_2:
                x_min, f_min = x_1, fx_1
            else:
                x_min, f_min = x_2, fx_2
            break
        else:
            if fx_1 <= fx_2:
                b = x_2
                x_2, fx_2 = x_1, fx_1
                n -= 1
                x_1 = a + (F[n] / F[n + 2]) * (b - a)
                fx_1 = f(x_1)
            else:
                a = x_1
                x_1, fx_1 = x_2, fx_2
                n -= 1
                x_2 = a + (F[n + 1] / F[n + 2]) * (b - a)
                fx_2 = f(x_2)

    print_result("Фибоначчи", x_min, f_min, a, b, iterations)
    viewPlot(1, x_min, f_min)


def method_golden(eps):
    tau = 0.38197
    a, b = -10, 6
    iterations = 0

    x_1 = a + tau * (b - a)
    x_2 = b - tau * (b - a)
    fx_1, fx_2 = f(x_1), f(x_2)

    print(HEADER)
    while np.abs(b - a) > eps:
        iterations += 1
        if fx_1 <= fx_2:
            b = x_2
            x_2, fx_2 = x_1, fx_1
            x_1 = a + tau * (b - a)
            fx_1 = f(x_1)
        else:
            a = x_1
            x_1, fx_1 = x_2, fx_2
            x_2 = b - tau * (b - a)
            fx_2 = f(x_2)

        x_m = (a + b) / 2
        print(ROW.format(iterations, a, b, x_1, fx_1, x_2, fx_2))

    x_min = (a + b) / 2
    f_min = f(x_min)
    print_result("золотого сечения", x_min, f_min, a, b, iterations)
    viewPlot(1, x_min, f_min)


def mainloop():
    eps = 0.05
    while True:
        print("======== МЕНЮ ========")
        print("1. Метод деления интервалов")
        print("2. Метод Фибоначчи")
        print("3. Метод золотого сечения")
        print("4. Показать график функции на отрезке [-10; 6]")
        print("5. Выход")
        c = int(input("-> "))

        if c == 5:
            return
        elif c == 4:
            viewPlot(0)
        elif c == 1:
            method_division(eps)
        elif c == 2:
            method_fibonacci(eps)
        elif c == 3:
            method_golden(eps)
        else:
            print("\n\nНеизвестная команда\n\n")

mainloop()