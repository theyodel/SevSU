import numpy as np
import matplotlib.pyplot as plt
import rich.console as cls
import rich.table as tbl

def f(x):
    """
    ## Заданная функция по варианту 4.
    """
    return x**4 + 8*x**3 - 6*x**2 - 72*x + 90

def df(x):
    """
    ## Производная функции по варианту 4.
    """
    return 4*x**3 + 24*x**2 - 12*x - 72

def printTable(name: str, _table: dict):
    """
    ## Вывод таблицы итераций в консоль через rich.
    """
    console = cls.Console()
    table = tbl.Table(title=name, show_lines=True, header_style='bold')

    for col in _table.keys():
        table.add_column(header=col, justify="center")

    n_rows = max((len(v) for v in _table.values()), default=0)

    for i in range(n_rows):
        row = []
        for values in _table.values():
            if i < len(values):
                val = values[i]
                if isinstance(val, float):
                    row.append(f"{val:.6f}")
                elif isinstance(val, (bool, np.bool_)):
                    row.append("✅" if val else "❌")
                else:
                    row.append(str(val))
            else:
                row.append("—")
        table.add_row(*row)

    console.print()
    console.print(table)

def viewPlot(flag = False, start = 0, end = 0, x_min = 0, f_min = 0, title: str = ''):
    x = np.linspace(start, end, 500)
    plt.plot(x, f(x), label=r"$ f(x) = x^4 + 8x^3 - 6x^2 - 72x + 90 $")
    plt.grid(True)
    if flag:
        plt.plot(x_min, f_min, 'o', color="red", label="Найденная точка минимума")
    plt.legend()
    plt.title(label=title)
    plt.show()

def quadraticApproximation(_x1: float, eps1: float = 0.0003, eps2: float = 0.003, dx: int = 1):
    """
    ## Функция поиска минимума по методу квадратичной аппроксимации.
    """
    cond1 = cond2 = skip = False
    table = {
        "Итерации": [], "x1": [], "x2": [], "x3": [], 
        "f(x1)": [], "f(x2)": [], "f(x3)": [], 'x_min': [], 'f_min': [],
        "x_hat": [], "f(x_hat)": [], 
        "a1": [], "a2": [], 
        "Условие 1": [], "Условие 2": []
    }
    x1 = _x1
    iterations = 0
    while not (cond1 and cond2):
        iterations += 1
        if not skip:
            x2 = x1 + dx     # 2
            f_x1 = f(x1)     # 3
            f_x2 = f(x2)     # 3
            if f_x1 > f_x2:  # 4a
                x3 = x1 + 2 * dx
            else:            # 4b
                x3 = x1 - dx
            f_x3 = f(x3)     # 5

            res = sorted([(f_x1, x1), (f_x2, x2), (f_x3, x3)])

        table['Итерации'].append(iterations)
        table['x1'].append(x1)
        table['x2'].append(x2)
        table['x3'].append(x3)
        table['f(x1)'].append(f_x1)
        table['f(x2)'].append(f_x2)
        table['f(x3)'].append(f_x3)

        f_min = res[0][0]  # 6
        x_min = res[0][1]  # 6
        table['x_min'].append(x_min)
        table['f_min'].append(f_min)

        a1 = (f_x2 - f_x1) / (x2 - x1)
        a2 = (1 / (x3 - x2)) * (((f_x3 - f_x1) / (x3 - x1)) - ((f_x2 - f_x1) / (x2 - x1)))
        table['a1'].append(a1)
        table['a2'].append(a2)

        _x = 0.5 * (x2 + x1) - (a1 / (2*a2))
        f_hat = f(_x)
        table['x_hat'].append(_x)
        table['f(x_hat)'].append(f_hat)

        cond1 = np.fabs((f_min - f_hat) / f_hat) < eps1
        cond2 = np.fabs((x_min - _x) / _x) < eps2
        table['Условие 1'].append(cond1)
        table['Условие 2'].append(cond2)

        if not (cond1 and cond2):
            if min(x1, x3) <= _x <= max(x1, x3):
                pts = [(x1, f_x1), (x2, f_x2), (x3, f_x3), (_x, f_hat)]
                pts.sort(key=lambda p: p[0])
                best_idx = min(range(4), key=lambda i: pts[i][1])

                if best_idx == 0:
                    triple = pts[:3]
                elif best_idx == 3:
                    triple = pts[-3:]
                else:
                    triple = pts[best_idx - 1: best_idx + 2]

                (x1, f_x1), (x2, f_x2), (x3, f_x3) = triple
                res = sorted([(f_x1, x1), (f_x2, x2), (f_x3, x3)])

                skip = True
            else:
                x1 = _x
                skip = False
        else:
            break

    printTable("Квадратичная аппроксимация", table)

    print(f"\n\nРезультат вычисления по методу КВАДРАТИЧНОЙ АППРОКСИМАЦИИ:")
    print(f"• Точка минимума: x_min = {_x:.6f} ± {eps1}")
    print(f"• Значение в точке минимума: f(x_min) = {f_hat:.6f} ± {eps2}")
    print(f"• Заняло итераций: {iterations}")
    print("="*58 + "\n\n")

    if GRAPHICS_ON:
        viewPlot(flag=True, x_min=_x, f_min=f_hat, start=A, end=B, title='Квадратичная аппроксимация')

    return (_x, f_hat)

def cubicApproximation(_x0: float, eps1: float = 0.0003, eps2: float = 0.003, dx: int = 1):
    """
    ## Функция поиска минимума по методу кубической аппроксимации.
    ## Алгоритм полностью соответствует методичке (шаги 1-7).
    """
    table = {
        "Итерации": [],
        "x1": [], "x2": [],
        "f(x1)": [], "f(x2)": [],
        "df(x1)": [], "df(x2)": [],
        "z": [], "w": [], "mu": [],
        "x_hat": [], "f(x_hat)": [], "df(x_hat)": [],
        "Условие 1": [], "Условие 2": []
    }

    # Шаги 1–4: поиск интервала [x1, x2], где f'(x) меняет знак
    x0 = _x0
    df_x0 = df(x0)
    direction = 1 if df_x0 < 0 else -1

    bracket = [(x0, df_x0)]
    x_prev = x0
    k = 0
    x_curr = x_prev + (2 ** k) * dx * direction
    while df(x_prev) * df(x_curr) > 0 and k < 50:
        bracket.append((x_curr, df(x_curr)))
        k += 1
        x_prev = x_curr
        x_curr = x_prev + (2 ** k) * dx * direction
    bracket.append((x_curr, df(x_curr)))

    x1, x2 = x_prev, x_curr
    if x1 > x2:
        x1, x2 = x2, x1

    f_x1, f_x2 = f(x1), f(x2)
    df_x1, df_x2 = df(x1), df(x2)

    # Шаги 5–7: итерации метода кубической аппроксимации
    _x, f_hat, df_hat = x1, f_x1, df_x1
    iterations = 0
    cond1 = cond2 = False
    MAX_ITER = 200

    while not (cond1 and cond2) and iterations < MAX_ITER:
        iterations += 1

        denom_x = x2 - x1
        if abs(denom_x) < 1e-15:
            # Вырожденный интервал — дальше идти некуда
            break

        # Шаг 5: точка минимума кубического полинома
        z = 3 * (f_x1 - f_x2) / denom_x + df_x1 + df_x2
        disc = z * z - df_x1 * df_x2
        if disc < 0:
            disc = 0.0
        w = np.sqrt(disc) if denom_x > 0 else -np.sqrt(disc)

        denom = df_x2 - df_x1 + 2 * w
        mu = 0.5 if abs(denom) < 1e-15 else (df_x2 + w - z) / denom

        if mu < 0:
            _x = x2
        elif mu > 1:
            _x = x1
        else:
            _x = x2 - mu * denom_x

        f_hat = f(_x)
        df_hat = df(_x)

        # Шаг 6: условие убывания f(x̄) < f(x1)
        while f_hat >= f_x1 and abs(_x - x1) > 1e-14:
            _x = _x - 0.5 * (_x - x1)
            f_hat = f(_x)
            df_hat = df(_x)

        # Шаг 7: условия окончания
        cond1 = abs(df_hat) <= eps1
        cond2 = abs((_x - x1) / _x) <= eps2 if _x != 0 else True

        table['Итерации'].append(iterations)
        table['x1'].append(x1)
        table['x2'].append(x2)
        table['f(x1)'].append(f_x1)
        table['f(x2)'].append(f_x2)
        table['df(x1)'].append(df_x1)
        table['df(x2)'].append(df_x2)
        table['z'].append(z)
        table['w'].append(w)
        table['mu'].append(mu)
        table['x_hat'].append(_x)
        table['f(x_hat)'].append(f_hat)
        table['df(x_hat)'].append(df_hat)
        table['Условие 1'].append(cond1)
        table['Условие 2'].append(cond2)

        # Шаг 7б: обновление интервала по знаку f'(x̄)
        if not (cond1 and cond2):
            if df_hat < 0:
                x1, f_x1, df_x1 = _x, f_hat, df_hat
            elif df_hat > 0:
                x2, f_x2, df_x2 = _x, f_hat, df_hat
            else:
                break

    printTable("Кубическая аппроксимация", table)

    print(f"\n\nРезультат вычисления по методу КУБИЧЕСКОЙ АППРОКСИМАЦИИ:")
    print(f"• Точка минимума: x_min = {_x:.6f} ± {eps1}")
    print(f"• Значение в точке минимума: f(x_min) = {f_hat:.6f} ± {eps2}")
    print(f"• Заняло итераций: {iterations}")
    print("=" * 58 + "\n\n")

    if GRAPHICS_ON:
        viewPlot(flag=True, x_min=_x, f_min=f_hat, start=A, end=B, title='Кубическая аппроксимация')

    return (_x, f_hat)


GRAPHICS_ON = True
EPS1, EPS2  = 0.0003, 0.003
DX          = 1
A, B        = -10, 6


def mainloop():
    global A, B, DX, EPS1, EPS2, GRAPHICS_ON
    while True:
        print("\n\n======== МЕНЮ ========")
        print("1. Метод квадратичной аппроксимации")
        print("2. Метод кубической аппроксимации")
        print(f"3. Показать график функции на отрезке [{A}; {B}]")
        print("4. Изменить настройки программы")
        print("5. Выход")
        c = int(input("-> "))

        if c == 1:
            quadraticApproximation(A, EPS1, EPS2, DX)

        elif c == 2:
            cubicApproximation(A, EPS1, EPS2, DX)

        elif c == 3:
            viewPlot(flag=False, start=A, end=B)

        elif c == 4:
            print("\n\n======== НАСТРОЙКИ ========")
            print(f"1. Изменить границы интервала (сейчас: [{A}, {B}])")
            print(f"2. Изменить погрешности (сейчас eps1={EPS1}, eps2={EPS2})")
            print(f"3. Изменить шаг (сейчас dx={DX})")
            print(f"4. {"Выключить" if GRAPHICS_ON else "Включить"} отображение графиков")
            print("5. В меню")
            s = int(input("-> "))
            if s == 1:
                A, B = map(float, input("Введите значения A и B через пробел -> ").split())
            elif s == 2:
                EPS1, EPS2 = map(float, input("Введите значения eps1 и eps2 через пробел -> ").split())
            elif s == 3:
                DX = float(input("Введите новый шаг dx -> "))
            elif s == 4:
                GRAPHICS_ON = not GRAPHICS_ON
            else:
                break

        elif c == 5:
            return
        
        else:
            print("\n\nНеизвестная команда\n\n")

if __name__ == "__main__":
    mainloop()