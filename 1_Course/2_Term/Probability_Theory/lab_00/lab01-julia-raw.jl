using Plots
using Printf

xValues = Float64[]
functionValues = Float64[]
x = -1.0

println("\nТаблица значений x и f(x):")
println("+--------+----------+")
println("|   x    |   f(x)   |")
println("+--------+----------+")

for x in -1.0:0.1:0.0
    if 1 - 3*x != 0.0
        f = cos(pi * x^2) / sqrt(1 - 3*x)
        push!(xValues, x)
        push!(functionValues, f)
        @printf("| %-6.2f | %-7.4f  |\n", x, f)
    else
        println("|   NaN   |    NaN     |")
    end
end

println("+--------+----------+")

# Строим график
plot(xValues, functionValues,
     color=:purple,
     xlabel="x",
     ylabel="f(x)",
     title="График функции f(x) = cos(π x²)/√(1-3x)",
     legend=false,
     marker=:circle,
     linestyle=:solid)