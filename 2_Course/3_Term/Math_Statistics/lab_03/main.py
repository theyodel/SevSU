# =============================================================================
# ЛАБОРАТОРНАЯ РАБОТА №3. ПРОВЕРКА СТАТИСТИЧЕСКИХ ГИПОТЕЗ
# Вариант 5: Сравнение дисперсий (F-тест Фишера)
# =============================================================================

import sys
import io
import numpy as np
import pandas as pd
import matplotlib
import matplotlib.pyplot as plt
import seaborn as sns
import scipy
from scipy import stats
from scipy.stats import norm, f
from packaging.version import Version

# Настройка кодировки для вывода кириллицы в консоль (Windows / PyCharm)
if sys.stdout.encoding and sys.stdout.encoding.lower() != "utf-8":
    sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding="utf-8")

# ----------------------------- НАСТРОЙКИ ------------------------------------
ALPHA = 0.05                    # уровень значимости
RANDOM_STATE = 42               # для воспроизводимости
np.random.seed(RANDOM_STATE)

sns.set_style("whitegrid")
plt.rcParams["figure.figsize"] = (16, 10)
plt.rcParams["font.size"] = 11

def print_header(title: str) -> None:
    """Красивый заголовок в консоли."""
    print("\n" + "=" * 75)
    print(f"  {title}")
    print("=" * 75)

# =============================================================================
# 1. ГЕНЕРАЦИЯ ДАННЫХ
# =============================================================================
print_header("1. ГЕНЕРАЦИЯ ВЫБОРОК")

# По условию варианта 5:
#   X1: n1 = 30, дисперсия = 25  (σ1 = 5)
#   X2: n2 = 35, дисперсия = 10  (σ2 ≈ 3.162)
# Обе выборки из нормальных распределений.

n1, sigma1_sq = 30, 25.0       # Группа 1
n2, sigma2_sq = 35, 10.0       # Группа 2
sigma1 = np.sqrt(sigma1_sq)
sigma2 = np.sqrt(sigma2_sq)

mu1, mu2 = 50.0, 50.0

X1 = np.random.normal(loc=mu1, scale=sigma1, size=n1)
X2 = np.random.normal(loc=mu2, scale=sigma2, size=n2)

# Выборочные характеристики
s1_sq = X1.var(ddof=1)
s2_sq = X2.var(ddof=1)

print(f"\nВыборка X1: n = {len(X1)}, среднее = {X1.mean():.4f}, "
      f"дисперсия (s²) = {s1_sq:.4f}, std = {X1.std(ddof=1):.4f}")
print(f"Выборка X2: n = {len(X2)}, среднее = {X2.mean():.4f}, "
      f"дисперсия (s²) = {s2_sq:.4f}, std = {X2.std(ddof=1):.4f}")
print(f"\nТеоретические параметры:")
print(f"  σ1² = {sigma1_sq:.2f}  (σ1 = {sigma1:.4f})")
print(f"  σ2² = {sigma2_sq:.2f}  (σ2 = {sigma2:.4f})")

# =============================================================================
# 2. F-ТЕСТ ФИШЕРА ДЛЯ СРАВНЕНИЯ ДИСПЕРСИЙ
# =============================================================================
print_header("2. F-ТЕСТ ФИШЕРА (СРАВНЕНИЕ ДИСПЕРСИЙ)")

H0_VAR = "σ1² = σ2² (дисперсии равны)"
H1_VAR = "σ1² > σ2² (правосторонняя альтернатива)"

# -----------------------------------------------------------------------------
# 2.1. Ручной расчёт F-статистики
# -----------------------------------------------------------------------------
# F = s1² / s2²  (отношение большей дисперсии к меньшей, чтобы F ≥ 1)
F_stat = s1_sq / s2_sq

# Число степеней свободы
df1 = n1 - 1   # для числителя
df2 = n2 - 1   # для знаменателя

print("\n[2.1] Ручной расчёт F-статистики")
print(f"  H₀: {H0_VAR}")
print(f"  H₁: {H1_VAR}")
print(f"  Формула: F = s1² / s2²")
print(f"  F = {s1_sq:.4f} / {s2_sq:.4f} = {F_stat:.4f}")
print(f"  Степени свободы: df1 = n1 − 1 = {df1}, df2 = n2 − 1 = {df2}")

# Критическое значение для ПРАВОСТОРОННЕЙ области (одно число)
# F_кр = F_(1−α)(df1, df2) — квантиль уровня 1−α
F_crit = f.ppf(1 - ALPHA, df1, df2)

print(f"\n  Критическое значение (Kкрит):")
print(f"  Kкрит = F_(1−α)(df1, df2) = F_0.95({df1}, {df2}) = {F_crit:.4f}")

# -----------------------------------------------------------------------------
# 2.2. Решение по правилу: F > F_кр → отвергаем H₀
# -----------------------------------------------------------------------------
print("\n[2.2] Проверка гипотезы")
print(f"  Правило: если F-критерий > Kкрит → отвергаем H₀, иначе принимаем H₀")
print(f"  F-критерий = {F_stat:.4f}")
print(f"  Kкрит      = {F_crit:.4f}")

if F_stat > F_crit:
    reject = True
    print(f"  {F_stat:.4f} > {F_crit:.4f}  →  ОТВЕРГАЕМ H₀: «{H0_VAR}»")
else:
    reject = False
    print(f"  {F_stat:.4f} ≤ {F_crit:.4f}  →  ПРИНИМАЕМ H₀: «{H0_VAR}»")

# =============================================================================
# 3. ДОПОЛНИТЕЛЬНЫЕ ПРОВЕРКИ (для полноты картины)
# =============================================================================
print_header("3. ДОПОЛНИТЕЛЬНЫЕ ПРОВЕРКИ")

# --- 3.1. Проверка нормальности каждой выборки (предположение F-теста) ---
print("\n[3.1] Проверка нормальности (Шапиро–Уилк)")
for name, data in [("X1", X1), ("X2", X2)]:
    w, p = stats.shapiro(data)
    verdict = "ОТВЕРГАЕМ H₀ (не нормально)" if p < ALPHA else "не отвергаем H₀ (нормально)"
    print(f"  {name}: W = {w:.4f}, p = {p:.4f}  →  {verdict}")

# --- 3.2. Непараметрический аналог F-теста: критерий Левене ---
print("\n[3.2] Критерий Левене (устойчив к ненормальности)")
lev_stat, lev_p = stats.levene(X1, X2, center="median")
print(f"  H₀: дисперсии равны")
print(f"  Статистика Левене = {lev_stat:.4f}")
if lev_p < ALPHA:
    print(f"  p = {lev_p:.4f} < α = {ALPHA}  →  ОТВЕРГАЕМ H₀: «дисперсии равны»")
else:
    print(f"  p = {lev_p:.4f} ≥ α = {ALPHA}  →  ПРИНИМАЕМ H₀: «дисперсии равны»")

# --- 3.3. Критерий Бартлетта ---
print("\n[3.3] Критерий Бартлетта")
bart_stat, bart_p = stats.bartlett(X1, X2)
print(f"  H₀: дисперсии равны")
print(f"  Статистика Бартлетта = {bart_stat:.4f}")
if bart_p < ALPHA:
    print(f"  p = {bart_p:.4f} < α = {ALPHA}  →  ОТВЕРГАЕМ H₀: «дисперсии равны»")
else:
    print(f"  p = {bart_p:.4f} ≥ α = {ALPHA}  →  ПРИНИМАЕМ H₀: «дисперсии равны»")

# --- 3.4. Сравнение средних (для справки) ---
print("\n[3.4] Сравнение средних (t-критерий Уэлча, для справки)")
t_stat, t_p = stats.ttest_ind(X1, X2, equal_var=False)
print(f"  H₀: μ1 = μ2")
print(f"  t = {t_stat:.4f}")
if t_p < ALPHA:
    print(f"  p = {t_p:.4f} < α = {ALPHA}  →  ОТВЕРГАЕМ H₀: «μ1 = μ2»")
else:
    print(f"  p = {t_p:.4f} ≥ α = {ALPHA}  →  ПРИНИМАЕМ H₀: «μ1 = μ2»")

# =============================================================================
# 4. ВИЗУАЛИЗАЦИЯ
# =============================================================================
print_header("4. ПОСТРОЕНИЕ ГРАФИКОВ")

fig, axes = plt.subplots(2, 3, figsize=(16, 10))
fig.suptitle("Лабораторная работа №3 (Вариант 5). F-тест Фишера",
             fontsize=15, fontweight="bold")

# --- 4.1. Гистограмма X1 ---
ax = axes[0, 0]
ax.hist(X1, bins=10, density=True, alpha=0.6, color="steelblue",
        edgecolor="black", label=f"X1 (s²={s1_sq:.2f})")
x_range = np.linspace(X1.min() - 2, X1.max() + 2, 300)
ax.plot(x_range, norm.pdf(x_range, X1.mean(), X1.std(ddof=1)),
        "r-", lw=2, label="Нормальная кривая")
ax.set_title(f"Гистограмма X1 (n={n1}, σ²={sigma1_sq})")
ax.set_xlabel("Значение")
ax.set_ylabel("Плотность")
ax.legend()

# --- 4.2. Гистограмма X2 ---
ax = axes[0, 1]
ax.hist(X2, bins=10, density=True, alpha=0.6, color="seagreen",
        edgecolor="black", label=f"X2 (s²={s2_sq:.2f})")
x_range = np.linspace(X2.min() - 2, X2.max() + 2, 300)
ax.plot(x_range, norm.pdf(x_range, X2.mean(), X2.std(ddof=1)),
        "r-", lw=2, label="Нормальная кривая")
ax.set_title(f"Гистограмма X2 (n={n2}, σ²={sigma2_sq})")
ax.set_xlabel("Значение")
ax.set_ylabel("Плотность")
ax.legend()

# --- 4.3. Boxplot ---
ax = axes[0, 2]
if Version(matplotlib.__version__) >= Version("3.9"):
    bp = ax.boxplot([X1, X2], tick_labels=["X1", "X2"], patch_artist=True,
                    medianprops=dict(color="red", linewidth=2))
else:
    bp = ax.boxplot([X1, X2], labels=["X1", "X2"], patch_artist=True,
                    medianprops=dict(color="red", linewidth=2))
for patch, color in zip(bp["boxes"], ["steelblue", "seagreen"]):
    patch.set_facecolor(color)
    patch.set_alpha(0.6)
ax.set_title("Boxplot: X1 vs X2")
ax.set_ylabel("Значение")

# --- 4.4. QQ-plot X1 ---
ax = axes[1, 0]
stats.probplot(X1, dist="norm", plot=ax)
ax.set_title("QQ-plot X1")
ax.get_lines()[0].set_markerfacecolor("steelblue")
ax.get_lines()[0].set_markeredgecolor("steelblue")
ax.get_lines()[1].set_color("red")

# --- 4.5. QQ-plot X2 ---
ax = axes[1, 1]
stats.probplot(X2, dist="norm", plot=ax)
ax.set_title("QQ-plot X2")
ax.get_lines()[0].set_markerfacecolor("seagreen")
ax.get_lines()[0].set_markeredgecolor("seagreen")
ax.get_lines()[1].set_color("red")

# --- 4.6. Столбчатая диаграмма: выборочные vs теоретические дисперсии ---
ax = axes[1, 2]
categories = ["X1", "X2"]
sample_vars = [s1_sq, s2_sq]
theor_vars = [sigma1_sq, sigma2_sq]

x_pos = np.arange(len(categories))
width = 0.35

bars1 = ax.bar(x_pos - width/2, sample_vars, width,
               label="Выборочная s²", color="steelblue",
               edgecolor="black", alpha=0.8)
bars2 = ax.bar(x_pos + width/2, theor_vars, width,
               label="Теоретическая σ²", color="salmon",
               edgecolor="black", alpha=0.8)

# Подписи значений над столбцами
for bar in bars1:
    h = bar.get_height()
    ax.text(bar.get_x() + bar.get_width()/2, h + 0.3,
            f"{h:.2f}", ha="center", va="bottom", fontsize=10)
for bar in bars2:
    h = bar.get_height()
    ax.text(bar.get_x() + bar.get_width()/2, h + 0.3,
            f"{h:.2f}", ha="center", va="bottom", fontsize=10)

ax.set_xticks(x_pos)
ax.set_xticklabels(categories)
ax.set_ylabel("Дисперсия")
ax.set_title(f"Сравнение дисперсий: s² vs σ²\n"
             f"F_набл = {F_stat:.3f},  Kкрит = {F_crit:.3f}")
ax.legend()
ax.grid(True, alpha=0.3, axis="y")

plt.tight_layout()
plt.savefig("LR3_variant5_visualization.png", dpi=150, bbox_inches="tight")
print("\n✅ График сохранён: LR3_variant5_visualization.png")
plt.show()

# --- 4.7. Сравнение плотностей (KDE) ---
fig2, ax2 = plt.subplots(figsize=(10, 5))
sns.kdeplot(X1, ax=ax2, fill=True, color="steelblue", alpha=0.5,
            label=f"X1 (s²={s1_sq:.2f})")
sns.kdeplot(X2, ax=ax2, fill=True, color="seagreen", alpha=0.5,
            label=f"X2 (s²={s2_sq:.2f})")
ax2.axvline(X1.mean(), color="steelblue", linestyle="--", alpha=0.7)
ax2.axvline(X2.mean(), color="seagreen", linestyle="--", alpha=0.7)
ax2.set_title("Сравнение плотностей распределений X1 и X2 (KDE)")
ax2.set_xlabel("Значение")
ax2.set_ylabel("Плотность")
ax2.legend()
plt.tight_layout()
plt.savefig("LR3_variant5_density.png", dpi=150, bbox_inches="tight")
print("✅ График плотностей сохранён: LR3_variant5_density.png")
plt.show()

# =============================================================================
# 5. СВОДНАЯ ТАБЛИЦА РЕЗУЛЬТАТОВ
# =============================================================================
print_header("5. СВОДНАЯ ТАБЛИЦА РЕЗУЛЬТАТОВ")

results = pd.DataFrame({
    "Критерий": [
        "F-тест Фишера (σ1²=σ2²)",
        "Критерий Левене",
        "Критерий Бартлетта",
        "t-Уэлча (μ1=μ2, справка)",
    ],
    "Статистика": [
        f"F = {F_stat:.4f}",
        f"W = {lev_stat:.4f}",
        f"T = {bart_stat:.4f}",
        f"t = {t_stat:.4f}",
    ],
    "Критическое значение": [
        f"Kкрит = {F_crit:.4f}",
        "—",
        "—",
        "—",
    ],
    "Решение (α=0.05)": [
        "Отвергнуть H₀" if reject else "Принять H₀",
        "Отвергнуть H₀" if lev_p < ALPHA else "Принять H₀",
        "Отвергнуть H₀" if bart_p < ALPHA else "Принять H₀",
        "Отвергнуть H₀" if t_p < ALPHA else "Принять H₀",
    ],
})
print("\n" + results.to_string(index=False))

print("\nДетали F-теста:")
print(f"  s1² = {s1_sq:.4f} (df1 = {df1})")
print(f"  s2² = {s2_sq:.4f} (df2 = {df2})")
print(f"  F-критерий = {F_stat:.4f}")
print(f"  Критическое значение (Kкрит) = {F_crit:.4f}")
print(f"  Правило: F-критерий > Kкрит → отвергаем H₀")

# =============================================================================
# 6. ИТОГОВЫЕ ВЫВОДЫ
# =============================================================================
print_header("6. ИТОГОВЫЕ ВЫВОДЫ")

if reject:
    f_conclusion = (
        f"(F-критерий > Kкрит: {F_stat:.4f} > {F_crit:.4f}). "
        f"ОТВЕРГАЕМ H₀: дисперсии статистически значимо РАЗЛИЧАЮТСЯ."
    )
else:
    f_conclusion = (
        f"(F-критерий ≤ Kкрит: {F_stat:.4f} ≤ {F_crit:.4f}). "
        f"ПРИНИМАЕМ H₀: дисперсии можно считать равными."
    )

print(f"""
1. F-тест Фишера (σ1² = σ2², α = {ALPHA}):
   {f_conclusion}

2. Проверка предположений F-теста:
   - Нормальность X1 (Шапиро–Уилк): проверена
   - Нормальность X2 (Шапиро–Уилк): проверена
   (F-тест корректен только при нормальности обеих выборок)

3. Согласованность с другими тестами:
   - Левене:   статистика = {lev_stat:.4f}
   - Бартлетт: статистика = {bart_stat:.4f}
   - Если все три теста дают одинаковый вывод — результат надёжен.
   - При расхождении предпочтителен критерий Левене (устойчив к ненормальности).

4. Значимость в контексте ML:
   F-тест дисперсий применяется для:
   - Проверки гомоскедастичности перед ANOVA / линейной регрессией;
   - Сравнения стабильности двух моделей (меньшая дисперсия ошибки = лучше);
   - Отбора признаков (F-тест в SelectKBest).
""")

print("=" * 75)
print("  ЛАБОРАТОРНАЯ РАБОТА №3 (ВАРИАНТ 5) ВЫПОЛНЕНА")
print("=" * 75)