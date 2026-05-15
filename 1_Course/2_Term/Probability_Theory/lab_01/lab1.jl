using Random, Plots, Statistics

# Функция-индикатор попадания в интервал [amin, amax)
function logzn(amin, amax, x)
    return amin <= x < amax ? 1 : 0
end

# Функция для вычисления частоты (доли единиц) в векторе
function fregp(v)
    return sum(v) / length(v)
end

# Основная программа
m, n = 5, 1000

bounds = [
    (0.34, 0.40),   # строка 1
    (0.34, 0.40),   # строка 2
    (0.34, 0.40),   # строка 3
    (0.18, 0.25),   # строка 4
    (0.06, 0.96)    # строка 5
]

Random.seed!(42)
A = rand(m, n)

println("Первые 10 столбцов матрицы A (5×10):")
display(A[:, 1:10])
println()

# Формирование матрицы B с использованием функции logzn
B = [logzn(bounds[i][1], bounds[i][2], A[i, j]) for i in 1:m, j in 1:n]

# Частоты при N=1000 с использованием fregp
freq_full = [fregp(B[i, :]) for i in 1:m]
println("Частоты событий (N=1000):")
for (i, f) in enumerate(freq_full)
    println("  Событие $i: $(round(f, digits=5))")
end

# Накопленные суммы и частоты для всех N
cumsum_matrix = Base.cumsum(B, dims=2)
N_vals = 1:n
q_vals = cumsum_matrix ./ N_vals'

# Построение графика
plt = plot(title="Зависимость частоты событий от числа испытаний",
           xlabel="Число испытаний N", ylabel="Частота q(N)",
           legend=:outertopright, grid=true, gridalpha=0.3)

for i in 1:m
    teor = bounds[i][2] - bounds[i][1]
    plot!(plt, N_vals, q_vals[i, :], label="Событие $i (теор. $(round(teor, digits=3)))")
end

hline!(plt, [0.06, 0.07, 0.90], linestyle=:dash, linecolor=:gray, label="")
display(plt)