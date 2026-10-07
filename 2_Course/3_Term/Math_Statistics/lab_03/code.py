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
from scipy.stats import norm, f as f_dist
from packaging.version import Version

# Настройка кодировки для вывода кириллицы в консоль (Windows / PyCharm)
if sys.stdout.encoding and sys.stdout.encoding.lower() != "utf-8":
    sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding="utf-8")

# ----------------------------- НАСТРОЙКИ ------------------------------------
ALPHA = 0.05                    # уровень значимости
RANDOM_STATE = 42               # для воспроизводимости
np.random.seed(RANDOM_STATE)

# Параметры выборок из условия варианта 5
N1, VAR1 = 30, 25.0             # X1: n = 30, дисперсия = 25
N2, VAR2 = 35, 10.0             # X2: n = 35, дисперсия = 10
SD1, SD2 = np.sqrt(VAR1), np.sqrt(VAR2)

sns.set_style("whitegrid")
plt.rcParams["figure.figsize"] = (16, 10)
plt.rcParams["font.size"] = 11

# --------------------------- ПРОВЕРКА ВЕРСИЙ --------------------------------
def print_header(title: str) -> None:
    """Красивый заголовок в консоли."""
    print("\n" + "=" * 75)
    print(f"  {title}")
    print("=" * 75)


def interpret_pvalue(p_value: float, alpha: float, h0_text: str) -> str:
    """
    Возвращает текстовую интерпретацию p-value.

    Параметры
    ---------
    p_value : float — вычисленное p-value
    alpha   : float — уровень значимости
    h0_text : str   — формулировка НУЛЕВОЙ гипотезы (утверждение, которое проверяем)
    """
    if p_value < alpha:
        return (f"p-value = {p_value:.4f} < α = {alpha}  →  "
                f"ОТВЕРГАЕМ H₀: «{h0_text}»")
    else:
        return (f"p-value = {p_value:.4f} ≥ α = {alpha}  →  "
                f"НЕТ оснований отвергнуть H₀: «{h0_text}»")


# =============================================================================
# 1. ГЕНЕРАЦИЯ ДАННЫХ
# =============================================================================
print_header("1. ГЕНЕРАЦИЯ ВЫБОРОК")

# Генерируем нормальные выборки с заданными дисперсиями и корректируем
# их выборочную дисперсию к точным значениям из условия задачи.

def generate_normal_with_variance(n: int, target_var: float,
                                   seed_offset: int) -> np.ndarray:
    """Генерирует выборку из N(0,1) и линейно масштабирует её так,
    чтобы выборочная дисперсия (ddof=1) точно равнялась target_var."""
    rng = np.random.default_rng(RANDOM_STATE + seed_offset)
    raw = rng.standard_normal(n)
    m = raw.mean()
    cur_var = raw.var(ddof=1)
    return m + (raw - m) * np.sqrt(target_var / cur_var)


X1 = generate_normal_with_variance(N1, VAR1, seed_offset=1)
X2 = generate_normal_with_variance(N2, VAR2, seed_offset=2)

print(f"\nВыборка X1: n₁ = {len(X1)}, среднее = {X1.mean():.4f}, "
      f"дисперсия = {X1.var(ddof=1):.4f}, std = {X1.std(ddof=1):.4f}")
print(f"Выборка X2: n₂ = {len(X2)}, среднее = {X2.mean():.4f}, "
      f"дисперсия = {X2.var(ddof=1):.4f}, std = {X2.std(ddof=1):.4f}")
print("\nОбе выборки извлечены из НОРМАЛЬНЫХ распределений (по условию).")
print(f"Ожидаемые дисперсии: σ₁² = {VAR1}, σ₂² = {VAR2}")


# =============================================================================
# 2. ПРОВЕРКА НОРМАЛЬНОСТИ (обязательное допущение F-теста)
# =============================================================================
print_header("2. ПРОВЕРКА НОРМАЛЬНОСТИ (ДОПУЩЕНИЕ F-ТЕСТА)")

H0_NORMAL = "распределение является нормальным"

# --- 2.1. Критерий Шапиро–Уилка для X1 ---
sw1_stat, sw1_p = stats.shapiro(X1)
print("\n[2.1] Шапиро–Уилк для X1")
print(f"  H₀: {H0_NORMAL} (для X1)")
print(f"  Статистика W = {sw1_stat:.4f}")
print(f"  {interpret_pvalue(sw1_p, ALPHA, H0_NORMAL)}")

# --- 2.2. Критерий Шапиро–Уилка для X2 ---
sw2_stat, sw2_p = stats.shapiro(X2)
print("\n[2.2] Шапиро–Уилк для X2")
print(f"  H₀: {H0_NORMAL} (для X2)")
print(f"  Статистика W = {sw2_stat:.4f}")
print(f"  {interpret_pvalue(sw2_p, ALPHA, H0_NORMAL)}")

# --- 2.3. Критерий Д'Агостино для обеих выборок ---
dag1_stat, dag1_p = stats.normaltest(X1)
dag2_stat, dag2_p = stats.normaltest(X2)
print("\n[2.3] Критерий Д'Агостино")
print(f"  X1: χ² = {dag1_stat:.4f}, p = {dag1_p:.4f} → "
      f"{'ОТВЕРГАЕМ H₀' if dag1_p < ALPHA else 'не отвергаем H₀'}")
print(f"  X2: χ² = {dag2_stat:.4f}, p = {dag2_p:.4f} → "
      f"{'ОТВЕРГАЕМ H₀' if dag2_p < ALPHA else 'не отвергаем H₀'}")

# --- 2.4. Сводка ---
normality_tests = {
    "Шапиро–Уилк (X1)": sw1_p,
    "Шапиро–Уилк (X2)": sw2_p,
    "Д'Агостино (X1)":  dag1_p,
    "Д'Агостино (X2)":  dag2_p,
}
print("\n[2.4] Сводка по тестам нормальности")
for name, p in normality_tests.items():
    verdict = "ОТВЕРГАЕМ H₀" if p < ALPHA else "не отвергаем H₀"
    print(f"  {name:22s}: p = {p:.4f}  →  {verdict}")


# =============================================================================
# 3. F-ТЕСТ ФИШЕРА ДЛЯ СРАВНЕНИЯ ДИСПЕРСИЙ
# =============================================================================
print_header("3. СРАВНЕНИЕ ДИСПЕРСИЙ (F-ТЕСТ ФИШЕРА)")

# --- 3.1. Основной F-тест (двусторонний) ---
# Для двусторонней альтернативы F-статистика берётся как отношение
# БОЛЬШЕЙ дисперсии к МЕНЬШЕЙ, а критическое значение — на уровне α/2.

s1_sq = X1.var(ddof=1)          # выборочная дисперсия X1
s2_sq = X2.var(ddof=1)          # выборочная дисперсия X2
df1 = len(X1) - 1                # степени свободы числителя
df2 = len(X2) - 1                # степени свободы знаменателя

# Определяем, какая дисперсия больше, чтобы корректно построить F
if s1_sq >= s2_sq:
    F_stat = s1_sq / s2_sq
    num_df, den_df = df1, df2
    num_name, den_name = "X1", "X2"
else:
    F_stat = s2_sq / s1_sq
    num_df, den_df = df2, df1
    num_name, den_name = "X2", "X1"

# Двусторонний p-value
p_value_two_sided = 2 * min(
    f_dist.cdf(F_stat, num_df, den_df),
    1 - f_dist.cdf(F_stat, num_df, den_df)
)
p_value_two_sided = min(p_value_two_sided, 1.0)

# Критическое значение на уровне α/2 (для двустороннего теста)
F_crit = f_dist.ppf(1 - ALPHA / 2, num_df, den_df)

H0_VAR = "σ₁² = σ₂² (дисперсии равны)"

print("\n[3.1] F-тест Фишера (двусторонний)")
print(f"  H₀: {H0_VAR}")
print(f"  H₁: σ₁² ≠ σ₂²")
print(f"  s₁² = {s1_sq:.4f},  s₂² = {s2_sq:.4f}")
print(f"  Большая дисперсия — у выборки {num_name}")
print(f"  Степени свободы: df₁ = {num_df}, df₂ = {den_df}")
print(f"  F-статистика   = {F_stat:.4f}")
print(f"  Fкрит (α/2={ALPHA/2}, df₁={num_df}, df₂={den_df}) = {F_crit:.4f}")
print(f"  {interpret_pvalue(p_value_two_sided, ALPHA, H0_VAR)}")

# --- 3.2. Односторонний F-тест (правый хвост) ---
p_value_one_sided = 1 - f_dist.cdf(F_stat, num_df, den_df)
F_crit_onesided = f_dist.ppf(1 - ALPHA, num_df, den_df)
print("\n[3.2] Односторонний F-тест (H₁: σ²_большая > σ²_меньшая)")
print(f"  F-статистика = {F_stat:.4f}")
print(f"  Fкрит (α={ALPHA}, df₁={num_df}, df₂={den_df}) = {F_crit_onesided:.4f}")
print(f"  p-value (one-sided) = {p_value_one_sided:.4f}")

# --- 3.3. Прямое сравнение с критическим значением из условия ---
print("\n[3.3] Сравнение с табличным критическим значением")
print(f"  Из условия: Fкрит (df₁=29, df₂=34, α=0.05) ≈ 1.812")
print(f"  F-статистика = {F_stat:.4f}")
if F_stat > 1.812:
    print("  F > Fкрит  →  H₀ ОТВЕРГАЕТСЯ (дисперсии различаются)")
else:
    print("  F ≤ Fкрит  →  H₀ не отвергается")


# =============================================================================
# 4. АЛЬТЕРНАТИВНЫЕ (РОБАСТНЫЕ) ТЕСТЫ НА РАВЕНСТВО ДИСПЕРСИЙ
# =============================================================================
print_header("4. АЛЬТЕРНАТИВНЫЕ ТЕСТЫ (устойчивые к ненормальности)")

H0_VAR_ALT = "дисперсии выборок равны"

# --- 4.1. Тест Левена ---
lev_stat, lev_p = stats.levene(X1, X2, center="median")
print("\n[4.1] Тест Левена (center='median')")
print(f"  H₀: {H0_VAR_ALT}")
print(f"  W-статистика = {lev_stat:.4f}")
print(f"  {interpret_pvalue(lev_p, ALPHA, H0_VAR_ALT)}")

# --- 4.2. Тест Бартлетта ---
bar_stat, bar_p = stats.bartlett(X1, X2)
print("\n[4.2] Тест Бартлетта (чувствителен к ненормальности)")
print(f"  H₀: {H0_VAR_ALT}")
print(f"  T-статистика = {bar_stat:.4f}")
print(f"  {interpret_pvalue(bar_p, ALPHA, H0_VAR_ALT)}")

# --- 4.3. Тест Флигнера–Киллина (непараметрический) ---
fk_stat, fk_p = stats.fligner(X1, X2)
print("\n[4.3] Тест Флигнера–Киллина (непараметрический)")
print(f"  H₀: {H0_VAR_ALT}")
print(f"  Статистика = {fk_stat:.4f}")
print(f"  {interpret_pvalue(fk_p, ALPHA, H0_VAR_ALT)}")

# --- 4.4. Проверка согласованности ---
print("\n[4.4] Проверка согласованности результатов")
test_results = {
    "F-тест (двусторонний)": p_value_two_sided,
    "Левен":                 lev_p,
    "Бартлетт":              bar_p,
    "Флигнер–Киллин":        fk_p,
}
rejected = sum(1 for p in test_results.values() if p < ALPHA)
print(f"  Из {len(test_results)} тестов отвергли H₀: {rejected}")
for name, p in test_results.items():
    verdict = "ОТВЕРГАЕМ" if p < ALPHA else "не отвергаем"
    print(f"    {name:24s}: p = {p:.4f}  →  {verdict}")


# =============================================================================
# 5. ВИЗУАЛИЗАЦИЯ
# =============================================================================
print_header("5. ПОСТРОЕНИЕ ГРАФИКОВ")

fig, axes = plt.subplots(2, 3, figsize=(16, 10))
fig.suptitle("Лабораторная работа №3. Сравнение дисперсий (F-тест Фишера)",
             fontsize=15, fontweight="bold")

# --- 5.1. Гистограмма X1 с нормальной кривой ---
ax = axes[0, 0]
ax.hist(X1, bins=10, density=True, alpha=0.6, color="steelblue",
        edgecolor="black", label="Выборка X1")
xr1 = np.linspace(X1.min() - 1, X1.max() + 1, 300)
ax.plot(xr1, norm.pdf(xr1, X1.mean(), X1.std(ddof=1)),
        "r-", lw=2, label="Нормальная кривая")
ax.set_title(f"X1: n₁={N1}, s₁²={s1_sq:.2f}")
ax.set_xlabel("Значение")
ax.set_ylabel("Плотность")
ax.legend()

# --- 5.2. Гистограмма X2 с нормальной кривой ---
ax = axes[0, 1]
ax.hist(X2, bins=10, density=True, alpha=0.6, color="seagreen",
        edgecolor="black", label="Выборка X2")
xr2 = np.linspace(X2.min() - 1, X2.max() + 1, 300)
ax.plot(xr2, norm.pdf(xr2, X2.mean(), X2.std(ddof=1)),
        "r-", lw=2, label="Нормальная кривая")
ax.set_title(f"X2: n₂={N2}, s₂²={s2_sq:.2f}")
ax.set_xlabel("Значение")
ax.set_ylabel("Плотность")
ax.legend()

# --- 5.3. Boxplot сравнения X1 и X2 ---
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

# --- 5.4. QQ-plot для X1 ---
ax = axes[1, 0]
stats.probplot(X1, dist="norm", plot=ax)
ax.set_title(f"QQ-plot X1 (SW p={sw1_p:.3f})")
ax.get_lines()[0].set_markerfacecolor("steelblue")
ax.get_lines()[0].set_markeredgecolor("steelblue")
ax.get_lines()[0].set_markersize(6)
ax.get_lines()[1].set_color("red")
ax.get_lines()[1].set_linewidth(2)

# --- 5.5. QQ-plot для X2 ---
ax = axes[1, 1]
stats.probplot(X2, dist="norm", plot=ax)
ax.set_title(f"QQ-plot X2 (SW p={sw2_p:.3f})")
ax.get_lines()[0].set_markerfacecolor("seagreen")
ax.get_lines()[0].set_markeredgecolor("seagreen")
ax.get_lines()[0].set_markersize(6)
ax.get_lines()[1].set_color("red")
ax.get_lines()[1].set_linewidth(2)

# --- 5.6. Распределение F-статистики с критической областью ---
ax = axes[1, 2]
x_f = np.linspace(0.01, max(4.0, F_stat + 1), 500)
ax.plot(x_f, f_dist.pdf(x_f, num_df, den_df), "k-", lw=2,
        label=f"F({num_df}, {den_df})")
# Правая критическая область (α/2)
x_crit_right = np.linspace(F_crit, x_f.max(), 200)
ax.fill_between(x_crit_right,
                f_dist.pdf(x_crit_right, num_df, den_df),
                color="red", alpha=0.35,
                label=f"Крит. область (α/2={ALPHA/2})")
# Левая критическая область (α/2)
F_crit_left = f_dist.ppf(ALPHA / 2, num_df, den_df)
x_crit_left = np.linspace(0.01, F_crit_left, 100)
ax.fill_between(x_crit_left,
                f_dist.pdf(x_crit_left, num_df, den_df),
                color="red", alpha=0.35)
ax.axvline(F_stat, color="blue", linestyle="--", lw=2,
           label=f"F = {F_stat:.3f}")
ax.axvline(F_crit, color="darkred", linestyle=":", lw=2,
           label=f"Fкрит = {F_crit:.3f}")
ax.set_title("Распределение F-статистики")
ax.set_xlabel("F")
ax.set_ylabel("Плотность")
ax.legend(fontsize=9)

plt.tight_layout()
plt.savefig("LR3_variant5_visualization.png", dpi=150, bbox_inches="tight")
print("\n✅ График сохранён: LR3_variant5_visualization.png")
plt.show()


# --- 5.7. Отдельный график KDE (сравнение плотностей) ---
fig2, ax2 = plt.subplots(figsize=(10, 5))
sns.kdeplot(X1, ax=ax2, fill=True, color="steelblue", alpha=0.5,
            label=f"X1 (s₁²={s1_sq:.2f})")
sns.kdeplot(X2, ax=ax2, fill=True, color="seagreen", alpha=0.5,
            label=f"X2 (s₂²={s2_sq:.2f})")
ax2.axvline(X1.mean(), color="steelblue", linestyle="--", alpha=0.7,
            label=f"Среднее X1 = {X1.mean():.2f}")
ax2.axvline(X2.mean(), color="seagreen", linestyle="--", alpha=0.7,
            label=f"Среднее X2 = {X2.mean():.2f}")
ax2.set_title("Сравнение плотностей распределений X1 и X2 (KDE)")
ax2.set_xlabel("Значение")
ax2.set_ylabel("Плотность")
ax2.legend()
plt.tight_layout()
plt.savefig("LR3_variant5_density.png", dpi=150, bbox_inches="tight")
print("✅ График плотностей сохранён: LR3_variant5_density.png")
plt.show()


# =============================================================================
# 6. СВОДНАЯ ТАБЛИЦА РЕЗУЛЬТАТОВ
# =============================================================================
print_header("6. СВОДНАЯ ТАБЛИЦА РЕЗУЛЬТАТОВ")

results = pd.DataFrame({
    "Критерий": [
        "Шапиро–Уилк (X1)",
        "Шапиро–Уилк (X2)",
        "F-тест Фишера",
        "Левен",
        "Бартлетт",
        "Флигнер–Киллин",
    ],
    "Статистика": [
        f"W = {sw1_stat:.4f}",
        f"W = {sw2_stat:.4f}",
        f"F = {F_stat:.4f}",
        f"W = {lev_stat:.4f}",
        f"T = {bar_stat:.4f}",
        f"χ² = {fk_stat:.4f}",
    ],
    "p-value": [
        sw1_p, sw2_p, p_value_two_sided, lev_p, bar_p, fk_p,
    ],
    "Решение (α=0.05)": [
        "Отвергнуть H₀" if sw1_p < ALPHA else "Не отвергать H₀",
        "Отвергнуть H₀" if sw2_p < ALPHA else "Не отвергать H₀",
        "Отвергнуть H₀" if p_value_two_sided < ALPHA else "Не отвергать H₀",
        "Отвергнуть H₀" if lev_p < ALPHA else "Не отвергать H₀",
        "Отвергнуть H₀" if bar_p < ALPHA else "Не отвергать H₀",
        "Отвергнуть H₀" if fk_p < ALPHA else "Не отвергать H₀",
    ],
})
print("\n" + results.to_string(index=False))


# =============================================================================
# 7. ИТОГОВЫЕ ВЫВОДЫ
# =============================================================================
print_header("7. ИТОГОВЫЕ ВЫВОДЫ")

# Проверка нормальности
both_normal = (sw1_p >= ALPHA) and (sw2_p >= ALPHA)
if both_normal:
    normality_conclusion = ("Обе выборки можно считать нормальными — "
                            "допущение F-теста выполнено.")
else:
    normality_conclusion = ("Одна или обе выборки отклоняются от нормального "
                            "распределения. F-тест может быть ненадёжен — "
                            "рекомендуется опираться на тест Левена.")

# Решение по F-тесту
if p_value_two_sided < ALPHA:
    f_conclusion = (f"F = {F_stat:.4f} > Fкрит = {F_crit:.4f} "
                    f"→ H₀ ОТВЕРГАЕТСЯ. "
                    f"Дисперсии σ₁² и σ₂² статистически значимо различаются.")
else:
    f_conclusion = (f"F = {F_stat:.4f} ≤ Fкрит = {F_crit:.4f} "
                    f"→ H₀ НЕ отвергается. "
                    f"Нет оснований считать дисперсии различными.")

print(f"""
1. Проверка нормальности (α = {ALPHA}):
   - Шапиро–Уилк для X1: W = {sw1_stat:.4f}, p = {sw1_p:.4f}
   - Шапиро–Уилк для X2: W = {sw2_stat:.4f}, p = {sw2_p:.4f}
   {normality_conclusion}

2. Основной F-тест Фишера (α = {ALPHA}):
   - Дисперсия X1: s₁² = {s1_sq:.4f}   (df₁ = {df1})
   - Дисперсия X2: s₂² = {s2_sq:.4f}   (df₂ = {df2})
   - F-статистика: F = s₁²/s₂² = {F_stat:.4f}
   - Критическое значение: Fкрит = {F_crit:.4f}
   - p-value (двусторонний) = {p_value_two_sided:.4f}
   {f_conclusion}

3. Альтернативные тесты (всего 4 теста, включая F-тест):
   - F-тест (двусторонний): p = {p_value_two_sided:.4f}
   - Левен:                 p = {lev_p:.4f}
   - Бартлетт:              p = {bar_p:.4f}
   - Флигнер–Киллин:        p = {fk_p:.4f}
   Из 4 тестов гипотезу о равенстве дисперсий отвергли: {rejected}.

4. Практический вывод:
   Дисперсия выборки X1 (s₁² ≈ {s1_sq:.1f}) примерно в {F_stat:.2f} раза превышает
   дисперсию выборки X2 (s₂² ≈ {s2_sq:.1f}). При уровне значимости α = {ALPHA}
   нулевая гипотеза о равенстве дисперсий ОТВЕРГАЕТСЯ.

5. Значимость в контексте ML:
   Проверка равенства дисперсий важна при выборе между t-критерием
   Стьюдента (равные дисперсии) и t-критерием Уэлча (неравные дисперсии),
   а также при анализе стабильности моделей (например, разброса метрик
   на кросс-валидации). Большая дисперсия указывает на менее стабильное
   поведение алгоритма.
""")

print("=" * 75)
print("  ЛАБОРАТОРНАЯ РАБОТА №3 ВЫПОЛНЕНА")
print("=" * 75)