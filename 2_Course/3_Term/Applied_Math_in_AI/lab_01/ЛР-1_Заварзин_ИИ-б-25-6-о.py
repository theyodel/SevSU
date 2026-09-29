import numpy as np
import matplotlib.pyplot as plt

def f(x):
    return x**4 + 8*x**3 - 6*x**2 - 72*x + 90

def viewPlot(flag = 0, x = 0, y = 0):
    arr = np.linspace(-10, 6, 500)
    plt.plot(arr, f(arr), label="f(x)")
    plt.grid(True)
    if flag == 1:
        plt.plot(x, y, 'o', color="red", label="Найденная точка минимума")
    plt.legend()
    plt.show()

def mainloop():
    eps = 0.05  # погрешность
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
            a = -10
            b = 6
            x_m = (a + b) / 2
            l = b - a
            iterations = 0

            while np.abs(l) >= eps:
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
                iterations += 1

            print("\n\nРезультат вычисления по методу деления интервала:")
            print(f"    Точка минимума: x_min = {x_m:.6f}")
            print(f"    Значение в точке минимума: f(x_min) = {fx_m:.6f}")
            print(f"    Заняло итераций: {iterations}")
            print("==========================================================\n\n")
            viewPlot(1, x_m, fx_m)

        elif c == 2:
            a = -10
            b = 6
            l = b - a
            iterations = 0
            ratio = l / eps
            x_min = 10**6
            f_min = x_min

            # Генерация чисел Фибоначчи
            F = [1, 1]
            while F[-1] < ratio:
                F.append(F[-1] + F[-2])
            n = len(F)
            while len(F) <= n + 2:
                F.append(F[-1] + F[-2])

            while n > 1:
                iterations += 1
                x_1 = a + (F[n] / F[n + 2]) * (b - a)
                x_2 = a + (F[n + 1] / F[n + 2]) * (b - a)
                fx_1 = f(x_1)
                fx_2 = f(x_2)
                if n == 2:
                    if fx_1 <= fx_2:
                        x_min = x_1
                        f_min = fx_1
                    else:
                        x_min = x_2
                        f_min = fx_2
                    break
                else:
                    if fx_1 <= fx_2:
                        b = x_2
                        x_2 = x_1
                        n -= 1
                        x_1 = a + (F[n] / F[n + 2]) * (b - a)
                        fx_1 = f(x_1)
                    else:
                        a = x_1
                        x_1 = x_2
                        n -= 1
                        x_2 = a + (F[n + 1] / F[n + 2]) * (b - a)
                        fx_2 = f(x_2)

            print("\n\nРезультат вычисления по методу Фибоначчи:")
            print(f"    Точка минимума: x_min = {x_min:.6f}")
            print(f"    Значение в точке минимума: f(x_min) = {f_min:.6f}")
            print(f"    Заняло итераций: {iterations}")
            print("==========================================================\n\n")
            viewPlot(1, x_min, f_min)

        elif c == 3:
            tau = 0.38197  # константа
            a = -10
            b = 6
            y = a + tau * (b - a)
            z = a + b - y
            fy = f(y)
            fz = f(z)

            if fy <= fz:
                b = z
            else:
                a = y

            k = 1
            x_prev = 10**6
            iterations = 0
            max_iter = 1000  # защита от зацикливания

            while iterations < max_iter:
                iterations += 1
                if fy <= fz:
                    y_next = a + b - y
                    fy_next = f(y_next)
                    z_next = y
                    fz_next = fy
                else:
                    y_next = z
                    fy_next = fz
                    z_next = a + b - z
                    fz_next = f(z_next)

                if fy_next <= fz_next:
                    a_next = a
                    b_next = z_next
                else:
                    a_next = y_next
                    b_next = b

                x_k = (a_next + b_next) / 2
                if k > 1:
                    if np.abs(x_prev - x_k) <= eps:
                        break

                a = a_next
                b = b_next
                y = y_next
                z = z_next
                fy = fy_next
                fz = fz_next
                x_prev = x_k
                k += 1

            x_min = x_k
            f_min = f(x_min)

            print("\n\nРезультат вычисления по методу золотого сечения:")
            print(f"    Точка минимума: x_min = {x_min:.6f}")
            print(f"    Значение в точке минимума: f(x_min) = {f_min:.6f}")
            print(f"    Заняло итераций: {iterations}")
            print("==========================================================\n\n")
            viewPlot(1, x_min, f_min)

        else:
            print("\n\nНеизвестная команда\n\n")

mainloop()