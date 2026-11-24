import plotly.graph_objects as go
import numpy as np
import rich.console as cls
import rich.table as tbl
import tempfile
import webbrowser
import os

np.random.seed(42) # для воспроизводимости

def f(x1, x2):
    return x1 * np.exp(x1) - (1 + np.exp(x1)) * np.cos(x2)

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
                else:
                    row.append(str(val))
            else:
                row.append("—")
        table.add_row(*row)

    console.print()
    console.print(table)

def viewPlot(x1: tuple = (-5, 2), x2: tuple = (-2.6, 1.8), pt: tuple | None = None, n: int = 150):
    X1, X2 = np.meshgrid(np.linspace(*x1, n), np.linspace(*x2, n))
    Z = f(X1, X2)

    fig = go.Figure()
    fig.add_surface(x=X1, y=X2, z=Z, colorscale="Viridis",
                    opacity=0.9, showscale=False)

    if pt:
        fig.add_scatter3d(x=[pt[0]], y=[pt[1]], z=[pt[2]],
                          mode="markers+text",
                          marker=dict(size=8, color="crimson",
                          line=dict(color="black", width=1)),
                          text=[f"min ({pt[0]:.2f}; {pt[1]:.2f}; {pt[2]:.3f})"],
                          textposition="top center",
                          textfont=dict(color="crimson", size=12))

    fig.update_layout(
        title=r"f(x1, x2) = x_1 * e^{x_1} - (1 + e^{x_1}) * cos(x_2)",
        scene=dict(
            xaxis_title="x1", yaxis_title="x2", zaxis_title="f",
            aspectmode="manual",
            aspectratio=dict(x=1, y=(x2[1]-x2[0])/(x1[1]-x1[0]), z=0.5),
        ),
        margin=dict(l=0, r=0, t=40, b=0),
    )
    fig.show()

    fd, path = tempfile.mkstemp(suffix=".html")
    os.close(fd)
    fig.write_html(path, include_plotlyjs="cdn", auto_open=False)
    webbrowser.open(f"file://{os.path.abspath(path)}")

def randomSearch(rx1: tuple = (-5, 2), rx2: tuple = (-2.6, 1.8), p: float = 0.95, dx1: float = 1, dx2: float = 1):
    v = (rx1[1] - rx1[0]) * (rx2[1] - rx2[0])
    dv = dx1 * dx2
    ro = dv / v
    n = np.floor(np.log(1-p)/np.log(1-ro)) + 1
    f_min = 10**10
    x_min = tuple()
    i = n + 1
    iters = 0
    max_iters = 10**5

    table = {'Итерация': [], 'S1': [], 'S2': [], 'x1': [], 'x2': [], 'f_i': [], 'f_min': []}

    while (i >= 0) and (max_iters >= iters):
        iters += 1
        sig1 = np.random.random()
        sig2 = np.random.random()
        x1 = rx1[0] + sig1 * (rx1[1] - rx1[0])
        x2 = rx2[0] + sig2 * (rx2[1] - rx2[0])
        f_i = f(x1, x2)
        if f_min > f_i:
            f_min = f_i
            x_min = (x1, x2)
        i -= 1

        table['Итерация'].append(iters)
        table['S1'].append(sig1)
        table['S2'].append(sig2)
        table['x1'].append(x1)
        table['x2'].append(x2)
        table['f_i'].append(f_i)
        table['f_min'].append(f_min)

    printTable('Случайный поиск', table)

    print(f"\n\nРезультат вычисления по методу СЛУЧАЙНОГО ПОИСКА")
    print(f"• Точка минимума: x_min = ({x_min[0]:.6f}; {x_min[1]:.6f})")
    print(f"• Значение в точке минимума: f(x_min) = {f_min:.6f}")
    print(f"• Заняло итераций: {iters}")
    print(f"• Вероятность определения экстремума P = {p:.2f}")
    print("="*58 + "\n\n")

    viewPlot(rx1, rx2, pt=(x_min[0], x_min[1], f_min))

    return (f_min, x_min)

def coordWeb(rx1: tuple = (-5, 2), rx2: tuple = (-2.6, 1.8),
             dx1: float = 1, dx2: float = 1,
             alpha: float = 10, eps: float = 0.005,
             verbose: bool = False):
    table = {'Итерация': [], 'x1': [], 'x2': [],
             'f(x1, x2)': [], 'f_min': [], 'x_min': [],
             '(a1; b1)': [], '(a2, b2)': []}

    a1, b1 = rx1
    a2, b2 = rx2
    f_star = f(a1, a2)
    f_min = np.inf
    x_min = (a1, a2)
    it = 0
    iters = 0

    while it < 100:
        it += 1
        xs = np.arange(a1, b1 + dx1/2, dx1)
        ys = np.arange(a2, b2 + dx2/2, dx2)

        # --- лог границ на КАЖДОЙ внешней итерации ---
        if verbose:
            print(f"[it {it:>2}] область: a1={a1:.4f}, b1={b1:.4f} | "
                  f"a2={a2:.4f}, b2={b2:.4f} | "
                  f"dx1={dx1:.4f}, dx2={dx2:.4f} | "
                  f"узлов={len(xs)}x{len(ys)}")

        prev_min = f_min               # для проверки улучшения

        for x2 in ys:
            for x1 in xs:
                val = f(x1, x2)
                if val < f_min:
                    f_min, x_min = val, (x1, x2)
                    iters += 1
                    # пишем в table ТОЛЬКО при реальном улучшении
                    table['Итерация'].append(iters)
                    table['x1'].append(x1)
                    table['x2'].append(x2)
                    table['f(x1, x2)'].append(val)
                    table['f_min'].append(f_min)
                    table['x_min'].append(f'({x_min[0]:.4f}, {x_min[1]:.4f})')
                    # --- лог границ области, в которой нашли улучшение ---
                    table['(a1; b1)'].append(f'({a1:.4f}; {b1:.4f})')
                    table['(a2, b2)'].append(f'({a2:.4f}; {b2:.4f})')

        if abs(f_star - f_min) < eps:
            break
        f_star = f_min

        a1, b1 = x_min[0] - dx1/2, x_min[0] + dx1/2
        a2, b2 = x_min[1] - dx2/2, x_min[1] + dx2/2
        dx1 /= alpha
        dx2 /= alpha

    printTable("Координатная сетка", table)

    print(f"\n\nРезультат вычисления по методу КООРДИНАТНОЙ СЕТКИ")
    print(f"• Точка минимума: x_min = ({x_min[0]:.6f}; {x_min[1]:.6f})")
    print(f"• Значение в точке минимума: f(x_min) = {f_min:.6f}")
    print(f"• Финальная область: "
          f"[{a1:+.6f}; {b1:+.6f}] × [{a2:+.6f}; {b2:+.6f}]")
    print(f"• Финальный шаг: dx1 = {dx1:.6f}, dx2 = {dx2:.6f}")
    print(f"• Заняло итераций: {iters} (внешних: {it})")
    print("="*58 + "\n\n")

    viewPlot(rx1, rx2, pt=(x_min[0], x_min[1], f_min))
    return f_min, x_min

def mainloop():
    while True:
        print("\n\n======== МЕНЮ ========")
        print("1. Метод координатной сетки с заданным шагом")
        print("2. Метод случайного поиска")
        print("3. Показать график функции")
        print("4. Выход")
        c = int(input("-> "))

        if c == 1:
            coordWeb()
    
        elif c == 2:
            randomSearch()
    
        elif c == 3:
            viewPlot()

        elif c == 4:
            return

        else:
            print("Неопознанная команда!\n\n")

if __name__ == "__main__":
    mainloop()