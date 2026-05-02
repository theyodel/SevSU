using Plots, Random

# ------------------- Параметры -------------------
m, n = 5, 1000
# Границы интервалов для каждой строки (вариант 4)
bounds = [
    (0.34, 0.40),   # строка 1
    (0.34, 0.40),   # строка 2
    (0.34, 0.40),   # строка 3
    (0.18, 0.25),   # строка 4
    (0.06, 0.96)    # строка 5
]

# ------------------- 1. Генерация матрицы A -------------------
Random.seed!(42)
A = rand(m, n)          # равномерное распределение [0, 1)

# ------------------- 2. Вывод первых 10 столбцов -------------------
println("Первые 10 столбцов матрицы A (5×10):")
display(A[:, 1:10])
println()

# ------------------- 3. Функция проверки условия -------------------
function logzn(amin, amax, x)
    return (amin <= x < amax) ? 1.0 : 0.0
end

# ------------------- 4. Формирование матрицы B -------------------
B = zeros(m, n)
for i in 1:m
    amin, amax = bounds[i]
    for j in 1:n
        B[i, j] = logzn(amin, amax, A[i, j])
    end
end

# ------------------- 5. Функция частоты -------------------
function fregp(v)
    return sum(v) / length(v)
end

# ------------------- 6. Частоты для полной выборки (N=1000) -------------------
freq_full = [fregp(B[i, :]) for i in 1:m]
println("Частоты событий (N=1000):")
for (i, f) in enumerate(freq_full)
    println("  Событие $i: $(round(f, digits=5))")
end

# ------------------- 7. Зависимость q_k(N) для N=1..1000 -------------------
cumsum = cumsum(B, dims=2)      # накопленные суммы по столбцам
N_vals = 1:n
q_vals = cumsum ./ N_vals'      # транспонирование для broadcasting

# Построение графиков
plot(xlabel="Число испытаний N", ylabel="Частота q(N)",
     title="Зависимость частоты событий от числа испытаний",
     legend=:right, grid=true, linewidth=2)

for i in 1:m
    plot!(N_vals, q_vals[i, :], label="Событие $i (теор. $(bounds[i][2]-bounds[i][1]))")
end

hline!([0.06], linestyle=:dash, linecolor=:gray, label="")
hline!([0.07], linestyle=:dash, linecolor=:gray, label="")
hline!([0.90], linestyle=:dash, linecolor=:gray, label="")

display(current())