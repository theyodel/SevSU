# =============================================================
# ЛАБОРАТОРНАЯ РАБОТА №4. ОДНОФАКТОРНЫЙ ДИСПЕРСИОННЫЙ АНАЛИЗ
# =============================================================
# Полная версия: предпосылки → ANOVA → LSD vs Тьюки → CV → графики
# =============================================================


# -------------------------------------------------------------
# БЛОК 0. ИМПОРТЫ
# -------------------------------------------------------------
import numpy as np
import pandas as pd
from scipy import stats
import matplotlib.pyplot as plt
import seaborn as sns

from sklearn.pipeline import Pipeline
from sklearn.preprocessing import OneHotEncoder, FunctionTransformer
from sklearn.compose import ColumnTransformer
from sklearn.linear_model import LinearRegression
from sklearn.dummy import DummyRegressor
from sklearn.model_selection import cross_val_score, RepeatedKFold

sns.set_theme(style="whitegrid", palette="deep")
plt.rcParams["figure.dpi"] = 110
plt.rcParams["axes.titlesize"] = 11
plt.rcParams["axes.labelsize"] = 10


# =============================================================
# БЛОК 1. ⚙️ КОНФИГУРАЦИЯ ВАРИАНТА  (МЕНЯЙ ТОЛЬКО ЗДЕСЬ)
# =============================================================
VARIANT = 5           # номер варианта (1..10)
ALPHA = 0.05          # уровень значимости
N_OBS = 15            # число наблюдений в каждой группе
RANDOM_SEED = VARIANT # для воспроизводимости

# --- CV-параметры (для устойчивой оценки) ---
CV_N_SPLITS = 5
CV_N_REPEATS = 10    # повторы кросс-валидации

VARIANTS = {
    1:  {"name": "Удобрения (урожайность, ц/га)",
         "groups": ["A", "B", "C", "D"],
         "means":  [10.0, 12.5, 11.0, 15.0], "sigma": 2.0},
    2:  {"name": "Режимы термообработки (прочность, МПа)",
         "groups": ["T1", "T2", "T3", "T4"],
         "means":  [410, 425, 418, 455], "sigma": 15.0},
    3:  {"name": "Методики обучения (средний балл)",
         "groups": ["M1", "M2", "M3", "M4"],
         "means":  [3.8, 4.1, 3.9, 4.6], "sigma": 1.2},
    4:  {"name": "Дозы препарата (снижение АД, мм рт. ст.)",
         "groups": ["D1", "D2", "D3", "D4"],
         "means":  [12, 15, 13, 20], "sigma": 4.0},
    5:  {"name": "Сорта картофеля (крахмал, %)",
         "groups": ["S1", "S2", "S3", "S4"],
         "means":  [14.0, 15.2, 14.5, 17.0], "sigma": 1.5},
    6:  {"name": "Алгоритмы сортировки (время, мс)",
         "groups": ["A1", "A2", "A3", "A4"],
         "means":  [120, 135, 128, 160], "sigma": 8.0},
    7:  {"name": "Виды корма (привес, кг)",
         "groups": ["K1", "K2", "K3", "K4"],
         "means":  [18, 21, 19.5, 25], "sigma": 2.5},
    8:  {"name": "Поставщики сырья (примеси, ppm)",
         "groups": ["P1", "P2", "P3", "P4"],
         "means":  [42, 38, 45, 30], "sigma": 6.0},
    9:  {"name": "Температурные режимы (выход реакции, %)",
         "groups": ["R1", "R2", "R3", "R4"],
         "means":  [78, 82, 80, 88], "sigma": 2.0},
    10: {"name": "Филиалы (средний чек, тыс. руб.)",
         "groups": ["F1", "F2", "F3", "F4"],
         "means":  [15, 17, 16.5, 21], "sigma": 3.5},
}

if VARIANT not in VARIANTS:
    raise ValueError(f"Вариант {VARIANT} не найден. Допустимо 1..10.")

cfg = VARIANTS[VARIANT]
STUDY_NAME = cfg["name"]
GROUPS     = cfg["groups"]
MEANS      = cfg["means"]
SIGMA      = cfg["sigma"]

rng = np.random.default_rng(RANDOM_SEED)

print("=" * 70)
print(f"ВАРИАНТ {VARIANT}: {STUDY_NAME}")
print("=" * 70)
print(f"Группы:  {GROUPS}")
print(f"Средние: {MEANS}")
print(f"σ = {SIGMA},  n = {N_OBS} в каждой группе,  α = {ALPHA}")
print("=" * 70)


# =============================================================
# БЛОК 2. ГЕНЕРАЦИЯ ДАННЫХ
# =============================================================
records = []
for g, mu in zip(GROUPS, MEANS):
    for v in rng.normal(loc=mu, scale=SIGMA, size=N_OBS):
        records.append((g, v))

df = pd.DataFrame(records, columns=["group", "value"])
df["group"] = pd.Categorical(df["group"], categories=GROUPS, ordered=True)

k = df["group"].nunique()
N = len(df)
print(f"\nСгенерировано наблюдений: N = {N}, групп: k = {k}")


# =============================================================
# БЛОК 3. ОПИСАТЕЛЬНЫЕ СТАТИСТИКИ
# =============================================================
desc = df.groupby("group", observed=True)["value"].agg(
    count="count", mean="mean", std="std", var="var",
    median="median", min="min", max="max"
).round(4)

print("\n" + "=" * 70)
print("БЛОК 3. ОПИСАТЕЛЬНЫЕ СТАТИСТИКИ ПО ГРУППАМ")
print("=" * 70)
print(desc.to_string())


# =============================================================
# БЛОК 4. ПРОВЕРКА ПРЕДПОСЫЛОК ANOVA
# =============================================================
print("\n" + "=" * 70)
print("БЛОК 4.1. ПРОВЕРКА НОРМАЛЬНОСТИ (Shapiro–Wilk)")
print("=" * 70)
shapiro_results = {}
for g in GROUPS:
    x = df.loc[df["group"] == g, "value"].to_numpy()
    W, p = stats.shapiro(x)
    shapiro_results[g] = (W, p)
    verdict = "не отвергается" if p > ALPHA else "ОТВЕРГАЕТСЯ"
    print(f"  Группа {g}: W = {W:.4f}, p = {p:.4f}  →  H0 {verdict}")

print("\n" + "=" * 70)
print("БЛОК 4.2. ПРОВЕРКА ГОМОГЕННОСТИ ДИСПЕРСИЙ")
print("=" * 70)
samples = [df.loc[df["group"] == g, "value"].to_numpy() for g in GROUPS]
lev_stat, lev_p = stats.levene(*samples, center="median")
bar_stat, bar_p = stats.bartlett(*samples)

print(f"  Levene   : stat = {lev_stat:.4f}, p = {lev_p:.4f}  → "
      f"H0 {'не отвергается' if lev_p > ALPHA else 'ОТВЕРГАЕТСЯ'}")
print(f"  Bartlett : stat = {bar_stat:.4f}, p = {bar_p:.4f}  → "
      f"H0 {'не отвергается' if bar_p > ALPHA else 'ОТВЕРГАЕТСЯ'}")


# =============================================================
# БЛОК 5. ОДНОФАКТОРНЫЙ ANOVA
# =============================================================
grand_mean = df["value"].mean()
ss_total   = ((df["value"] - grand_mean) ** 2).sum()
ss_between = sum(len(sub) * (sub["value"].mean() - grand_mean) ** 2
                 for _, sub in df.groupby("group", observed=True))
ss_within  = sum(((sub["value"] - sub["value"].mean()) ** 2).sum()
                 for _, sub in df.groupby("group", observed=True))

df_b, df_w = k - 1, N - k
ms_b, ms_w = ss_between / df_b, ss_within / df_w
F_manual   = ms_b / ms_w
p_manual   = 1 - stats.f.cdf(F_manual, df_b, df_w)
F_crit     = stats.f.ppf(1 - ALPHA, df_b, df_w)

anova_table = pd.DataFrame({
    "Источник":  ["Между группами", "Внутри групп", "Общая"],
    "SS":        [ss_between, ss_within, ss_total],
    "df":        [df_b, df_w, N - 1],
    "MS":        [ms_b, ms_w, np.nan],
    "F":         [F_manual, np.nan, np.nan],
    "p":         [p_manual, np.nan, np.nan],
}).round(4)

print("\n" + "=" * 70)
print("БЛОК 5. ТАБЛИЦА ОДНОФАКТОРНОГО ANOVA (ручной расчёт)")
print("=" * 70)
print(anova_table.to_string(index=False))
print(f"\n  F_крит({ALPHA}; {df_b}, {df_w}) = {F_crit:.4f}")
print(f"  Вывод: F_набл = {F_manual:.4f} "
      f"{'>' if F_manual > F_crit else '<'} F_крит  →  "
      f"H0 {'ОТВЕРГАЕТСЯ' if p_manual < ALPHA else 'не отвергается'}")

F_scipy, p_scipy = stats.f_oneway(*samples)
print(f"\n  Сверка со scipy.f_oneway: F = {F_scipy:.4f}, p = {p_scipy:.4f}")
assert np.isclose(F_manual, F_scipy), "Расхождение ручного и scipy F!"
print("  ✔ Ручной расчёт совпадает со scipy.")


# =============================================================
# БЛОК 6. АПОСТЕРИОРНЫЙ АНАЛИЗ
# =============================================================
# 6.1 НСР (LSD)
t_crit = stats.t.ppf(1 - ALPHA / 2, df_w)
LSD    = t_crit * np.sqrt(2 * ms_w / N_OBS)

print("\n" + "=" * 70)
print("БЛОК 6.1. НАИМЕНЬШАЯ СУЩЕСТВЕННАЯ РАЗНИЦА (НСР / LSD)")
print("=" * 70)
print(f"  t_крит({1-ALPHA/2}; {df_w}) = {t_crit:.4f}")
print(f"  MS_within = {ms_w:.4f}")
print(f"  НСР = {LSD:.4f}")

# 6.2 Попарные сравнения по LSD
group_means = df.groupby("group", observed=True)["value"].mean()
pairs = []
for i, gi in enumerate(GROUPS):
    for gj in GROUPS[i + 1:]:
        diff = abs(group_means[gi] - group_means[gj])
        pairs.append({
            "Пара":    f"{gi} – {gj}",
            "|Δmean|": round(diff, 4),
            "НСР":     round(LSD, 4),
            "Значимо": "да" if diff > LSD else "нет",
        })
pair_df = pd.DataFrame(pairs)

print("\n" + "=" * 70)
print("БЛОК 6.2. ПОПАРНЫЕ СРАВНЕНИЯ ПО НСР")
print("=" * 70)
print(pair_df.to_string(index=False))

# 6.3 Тьюки + автоматическое сравнение с LSD
print("\n" + "=" * 70)
print("БЛОК 6.3. КРИТЕРИЙ ТЬЮКИ (HSD) И СРАВНЕНИЕ С LSD")
print("=" * 70)

try:
    from statsmodels.stats.multicomp import pairwise_tukeyhsd
    tukey = pairwise_tukeyhsd(df["value"], df["group"], alpha=ALPHA)
    print(tukey)

    # --- Автоматическое сравнение LSD и Тьюки ---
    tukey_summary = pd.DataFrame(
        data=tukey._results_table.data[1:],
        columns=tukey._results_table.data[0]
    )
    tukey_summary["reject"] = tukey_summary["reject"].astype(bool)

    # Множества значимых пар
    lsd_sig = set()
    for _, row in pair_df.iterrows():
        if row["Значимо"] == "да":
            a, b = [s.strip() for s in row["Пара"].split("–")]
            lsd_sig.add(tuple(sorted((a, b))))

    tukey_sig = set()
    for _, row in tukey_summary.iterrows():
        if row["reject"]:
            tukey_sig.add(tuple(sorted((row["group1"], row["group2"]))))

    only_lsd  = lsd_sig - tukey_sig
    only_tukey = tukey_sig - lsd_sig

    print("\n  Сводка по значимым парам:")
    print(f"    По LSD   : {sorted(['–'.join(p) for p in lsd_sig])}")
    print(f"    По Тьюки : {sorted(['–'.join(p) for p in tukey_sig])}")

    if only_lsd:
        print(f"\n  ⚠ Пары, значимые по LSD, но НЕ по Тьюки:")
        for a, b in sorted(only_lsd):
            print(f"      {a}–{b}  (риск FWER у LSD)")
    else:
        print("\n  ✔ Расхождений между LSD и Тьюки нет.")

    if only_tukey:
        print(f"\n  ⚠ Пары, значимые по Тьюки, но НЕ по LSD:")
        for a, b in sorted(only_tukey):
            print(f"      {a}–{b}")

    # Итоговое решение — на основе Тьюки (более надёжного)
    significant_pairs_final = sorted(["–".join(p) for p in tukey_sig])

except ImportError:
    print("  statsmodels не установлен. Установите: pip install statsmodels")
    significant_pairs_final = sorted(["–".join(p) for p in lsd_sig])


# =============================================================
# БЛОК 7. КРОСС-ВАЛИДАЦИЯ (устойчивая оценка)
# =============================================================
X = df[["group"]]
y = df["value"].to_numpy()

# Модель с фактором
pipe_factor = Pipeline([
    ("ohe", ColumnTransformer([
        ("cat", OneHotEncoder(drop="first"), ["group"])
    ])),
    ("lr", LinearRegression()),
])

# Нулевая модель: только среднее, без one-hot
pipe_null = Pipeline([
    ("identity", FunctionTransformer()),
    ("dummy", DummyRegressor(strategy="mean")),
])

# RepeatedKFold даёт более устойчивую оценку среднего R²
cv = RepeatedKFold(n_splits=CV_N_SPLITS,
                   n_repeats=CV_N_REPEATS,
                   random_state=RANDOM_SEED)

scores_r2_factor  = cross_val_score(pipe_factor, X, y, cv=cv, scoring="r2")
scores_r2_null    = cross_val_score(pipe_null,   X, y, cv=cv, scoring="r2")
scores_mae_factor = -cross_val_score(pipe_factor, X, y, cv=cv,
                                     scoring="neg_mean_absolute_error")
scores_mae_null   = -cross_val_score(pipe_null,   X, y, cv=cv,
                                     scoring="neg_mean_absolute_error")

def ci95(a):
    a = np.asarray(a, dtype=float)
    return stats.t.interval(0.95, len(a) - 1,
                            loc=a.mean(), scale=stats.sem(a))

ci_r2_factor = ci95(scores_r2_factor)
ci_r2_null   = ci95(scores_r2_null)
delta_r2 = scores_r2_factor.mean() - scores_r2_null.mean()

print("\n" + "=" * 70)
print(f"БЛОК 7. КРОСС-ВАЛИДАЦИЯ (RepeatedKFold: "
      f"{CV_N_SPLITS}×{CV_N_REPEATS} = {CV_N_SPLITS * CV_N_REPEATS} оценок)")
print("=" * 70)
print(f"  R² с фактором : mean = {scores_r2_factor.mean():.4f}, "
      f"std = {scores_r2_factor.std():.4f}, "
      f"95% ДИ = [{ci_r2_factor[0]:.4f}; {ci_r2_factor[1]:.4f}]")
print(f"  R² без фактора: mean = {scores_r2_null.mean():.4f}, "
      f"std = {scores_r2_null.std():.4f}, "
      f"95% ДИ = [{ci_r2_null[0]:.4f}; {ci_r2_null[1]:.4f}]")
print(f"  MAE с фактором : mean = {scores_mae_factor.mean():.4f}, "
      f"std = {scores_mae_factor.std():.4f}")
print(f"  MAE без фактора: mean = {scores_mae_null.mean():.4f}, "
      f"std = {scores_mae_null.std():.4f}")
print(f"\n  ΔR² = {delta_r2:.4f}")

# --- Честная интерпретация ДИ ---
ci_includes_zero = (ci_r2_factor[0] <= 0 <= ci_r2_factor[1])
print("\n  Интерпретация:")
if delta_r2 > 0 and not ci_includes_zero:
    print("    ✔ Фактор устойчиво улучшает модель: 95% ДИ не включает 0.")
    cv_conclusion = "фактор устойчиво улучшает модель"
elif delta_r2 > 0 and ci_includes_zero:
    print("    ⚠ Фактор улучшает модель в среднем, но 95% ДИ для R²"
          " включает 0 — оценка неустойчива (малый объём выборки).")
    print("      Основной вывод о значимости фактора опирается на ANOVA,"
          " а не на кросс-валидацию.")
    cv_conclusion = "фактор улучшает модель в среднем, но оценка R² неустойчива"
else:
    print("    ⚠ Фактор не даёт устойчивого улучшения модели.")
    cv_conclusion = "фактор не улучшает модель"

if scores_r2_factor.std() > 0.1:
    print("    ⚠ Разброс R² по фолдам высокий (>0.1) — вывод требует осторожности.")


# =============================================================
# БЛОК 8. РАСШИРЕННАЯ ВИЗУАЛИЗАЦИЯ
# =============================================================
fig, axes = plt.subplots(2, 3, figsize=(15, 9))
fig.suptitle(f"ANOVA. Вариант {VARIANT}: {STUDY_NAME}",
             fontsize=13, fontweight="bold")

# --- 8.1 Боксплот + точки ---
ax = axes[0, 0]
sns.boxplot(x="group", y="value", data=df, ax=ax, width=0.55,
            hue="group", palette="Set2", legend=False,
            showmeans=True,
            meanprops=dict(marker="D", markerfacecolor="black",
                           markeredgecolor="black", markersize=6))
sns.stripplot(x="group", y="value", data=df, ax=ax,
              color="black", size=3, alpha=0.5, jitter=0.12)
ax.set_title("Боксплот с точками (♦ — среднее)")
ax.set_xlabel("Группа"); ax.set_ylabel("Значение")

# --- 8.2 KDE по группам ---
ax = axes[0, 1]
for g in GROUPS:
    sns.kdeplot(df.loc[df["group"] == g, "value"], ax=ax,
                fill=True, alpha=0.25, label=f"{g}")
ax.set_title("Плотности распределений (KDE)")
ax.set_xlabel("Значение"); ax.set_ylabel("Плотность")
ax.legend(title="Группа", fontsize=8)

# --- 8.3 Средние с 95% ДИ ---
ax = axes[0, 2]
means = df.groupby("group", observed=True)["value"].mean()
sems  = df.groupby("group", observed=True)["value"].sem()
ci_lo = means - t_crit * sems
ci_hi = means + t_crit * sems
ax.errorbar(means.index, means.values,
            yerr=[means.values - ci_lo.values, ci_hi.values - means.values],
            fmt="o", capsize=6, color="darkblue", ecolor="gray")
ax.axhline(grand_mean, ls="--", color="red", alpha=0.7,
           label=f"Общее среднее = {grand_mean:.2f}")
ax.set_title("Средние с 95% ДИ")
ax.set_xlabel("Группа"); ax.set_ylabel("Среднее")
ax.legend(fontsize=8)

# --- 8.4 QQ-график остатков ---
ax = axes[1, 0]
residuals = df["value"] - df.groupby("group", observed=True)["value"].transform("mean")
stats.probplot(residuals, dist="norm", plot=ax)
ax.set_title("QQ-график остатков")
ax.get_lines()[0].set_markerfacecolor("steelblue")
ax.get_lines()[0].set_markeredgecolor("steelblue")
ax.get_lines()[0].set_markersize(4)
ax.get_lines()[1].set_color("red")

# --- 8.5 Матрица попарных разностей ---
ax = axes[1, 1]
diff_matrix = pd.DataFrame(
    [[abs(group_means[gi] - group_means[gj]) if gi != gj else 0
      for gj in GROUPS] for gi in GROUPS],
    index=GROUPS, columns=GROUPS
)
sns.heatmap(diff_matrix, annot=True, fmt=".2f", cmap="YlOrRd",
            cbar_kws={"label": "|Δmean|"}, ax=ax,
            linewidths=0.5, linecolor="white")
ax.set_title(f"Попарные |Δmean| (НСР = {LSD:.3f})")

# --- 8.6 Кросс-валидация: сравнение R² (без labels=) ---
ax = axes[1, 2]
positions = [1, 2]
bp = ax.boxplot([scores_r2_factor, scores_r2_null],
                positions=positions, widths=0.5, patch_artist=True)
ax.set_xticks(positions)
ax.set_xticklabels(["с фактором", "без фактора"])

for patch, color in zip(bp["boxes"], ["#4C72B0", "#DD8452"]):
    patch.set_facecolor(color); patch.set_alpha(0.6)
ax.scatter([1] * len(scores_r2_factor), scores_r2_factor,
           color="black", zorder=3, s=15, alpha=0.6)
ax.scatter([2] * len(scores_r2_null), scores_r2_null,
           color="black", zorder=3, s=15, alpha=0.6)
ax.axhline(0, ls="--", color="gray", alpha=0.5)
ax.set_title(f"R² по фолдам ({CV_N_SPLITS}×{CV_N_REPEATS} CV)")
ax.set_ylabel("R²"); ax.set_xlabel("Модель")

plt.tight_layout(rect=[0, 0, 1, 0.97])
plt.show()


# =============================================================
# БЛОК 9. ИТОГОВЫЙ АВТООТЧЁТ (с честными формулировками)
# =============================================================
print("\n" + "=" * 70)
print("ИТОГОВЫЙ ОТЧЁТ")
print("=" * 70)
print(f"Вариант {VARIANT}: {STUDY_NAME}")
print(f"Групп: {k}, наблюдений: {N}, α = {ALPHA}")
print("-" * 70)

# 1. Предпосылки
norm_ok  = all(p > ALPHA for _, p in shapiro_results.values())
homog_ok = (lev_p > ALPHA)
print("1. Предпосылки:")
print(f"   • Нормальность (Shapiro–Wilk): "
      f"{'✔ не отвергается' if norm_ok else '✘ ОТВЕРГАЕТСЯ'}")
print(f"   • Гомогенность дисперсий (Levene): "
      f"{'✔ не отвергается' if homog_ok else '✘ ОТВЕРГАЕТСЯ'}")

# 2. ANOVA
print(f"\n2. ANOVA: F({df_b}, {df_w}) = {F_manual:.4f}, p = {p_manual:.4g}")
print(f"   → H0 о равенстве средних "
      f"{'ОТВЕРГАЕТСЯ' if p_manual < ALPHA else 'не отвергается'}")

# 3. Апостериорный анализ — с оговоркой про LSD и Тьюки
print(f"\n3. Апостериорный анализ (НСР = {LSD:.4f}):")
print(f"   • По LSD значимы пары: "
      f"{sorted(['–'.join(p) for p in lsd_sig]) if 'lsd_sig' in dir() else 'см. блок 6'}")
print(f"   • По Тьюки значимы пары: "
      f"{significant_pairs_final}")
if 'only_lsd' in dir() and only_lsd:
    print(f"   ⚠ Расхождение: пары {sorted(['–'.join(p) for p in only_lsd])} "
          f"значимы по LSD, но не по Тьюки.")
    print("     Рекомендуется опираться на Тьюки — он защищает от FWER.")

# 4. Кросс-валидация
print(f"\n4. Кросс-валидация ({CV_N_SPLITS}×{CV_N_REPEATS}):")
print(f"   R²(с фактором) = {scores_r2_factor.mean():.3f} ± "
      f"{scores_r2_factor.std():.3f}, "
      f"95% ДИ = [{ci_r2_factor[0]:.3f}; {ci_r2_factor[1]:.3f}]")
print(f"   R²(без фактора) = {scores_r2_null.mean():.3f} ± "
      f"{scores_r2_null.std():.3f}")
print(f"   → {cv_conclusion}.")
if ci_includes_zero and delta_r2 > 0:
    print("   Примечание: широкий ДИ объясняется малым объёмом выборки;")
    print("   основной вывод о значимости фактора опирается на ANOVA.")

print("=" * 70)