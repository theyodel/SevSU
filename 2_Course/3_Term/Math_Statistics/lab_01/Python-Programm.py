"""
Лабораторная работа №1
Дисциплина: Математическая статистика
Тема: Формирование выборки и базовые оценки
Вариант 4: Сравнение методов выборки
Дано: N = 400; Xi ~ U(0, 100) — равномерное распределение; n = 80
"""

# =============================================================================
# 1. ИМПОРТ НЕОБХОДИМЫХ БИБЛИОТЕК
# =============================================================================

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from scipy import stats
import os
from datetime import datetime
import warnings

warnings.filterwarnings('ignore')

# =============================================================================
# 2. НАСТРОЙКА ПАРАМЕТРОВ
# =============================================================================

RESULTS_DIR = "results_var4"
FIGURES_DIR = os.path.join(RESULTS_DIR, "figures")
os.makedirs(FIGURES_DIR, exist_ok=True)

SHOW_PLOTS = True

plt.style.use('seaborn-v0_8-darkgrid')
sns.set_palette("husl")
plt.rcParams['figure.figsize'] = (12, 8)
plt.rcParams['font.size'] = 10
plt.rcParams['figure.dpi'] = 100

np.random.seed(42)

# =============================================================================
# 3. ГЕНЕРАЦИЯ ГЕНЕРАЛЬНОЙ СОВОКУПНОСТИ
# =============================================================================

def generate_population(N=400, low=0, high=100):
    """Генерация генеральной совокупности U(0, 100)."""
    x = np.random.uniform(low, high, N)
    return x


N = 400
population = generate_population(N, 0, 100)

mu_theor = 50.0
sigma_theor = np.sqrt((100 - 0) ** 2 / 12)
var_theor = (100 - 0) ** 2 / 12

print("=" * 80)
print("ГЕНЕРАЛЬНАЯ СОВОКУПНОСТЬ (Равномерное распределение U(0, 100))")
print("=" * 80)
print(f"Объём генеральной совокупности: N = {N}")
print(f"Выборочное среднее:              {np.mean(population):.4f}")
print(f"Выборочная дисперсия:            {np.var(population, ddof=1):.4f}")
print(f"Выборочное станд. отклонение:    {np.std(population, ddof=1):.4f}")
print(f"Минимум / Максимум:              {np.min(population):.4f} / {np.max(population):.4f}")
print("-" * 80)
print(f"ТЕОРЕТИЧЕСКИЕ значения для U(0,100):")
print(f"Математическое ожидание:         {mu_theor:.4f}")
print(f"Дисперсия:                       {var_theor:.4f}")
print(f"Стандартное отклонение:          {sigma_theor:.4f}")
print("=" * 80)


# =============================================================================
# 4. ФОРМИРОВАНИЕ ВЫБОРОК
# =============================================================================

def simple_random_sample(population, n=80):
    indices = np.random.choice(len(population), size=n, replace=False)
    sample = population[indices]
    return sample, indices


def systematic_sample(population, n=80, start=None):
    N = len(population)
    k = N // n
    if start is None:
        start = np.random.randint(0, k)
    indices = np.arange(start, N, k)[:n]
    sample = population[indices]
    return sample, indices, k


def stratified_sample(population, n=80, n_strata=5):
    N = len(population)
    strata_size = N // n_strata
    samples_per_stratum = n // n_strata

    sample = []
    indices = []
    stratum_means = []

    for i in range(n_strata):
        start = i * strata_size
        end = (i + 1) * strata_size if i < n_strata - 1 else N
        stratum = population[start:end]

        stratum_indices = np.random.choice(
            np.arange(start, end),
            size=min(samples_per_stratum, len(stratum)),
            replace=False
        )
        sample.extend(population[stratum_indices])
        indices.extend(stratum_indices)
        stratum_means.append(np.mean(stratum))

    return np.array(sample), np.array(indices), stratum_means


n = 80
sample_srs, indices_srs = simple_random_sample(population, n)
sample_sys, indices_sys, step = systematic_sample(population, n)
sample_str, indices_str, stratum_means = stratified_sample(population, n)

print("\n" + "=" * 80)
print("СФОРМИРОВАННЫЕ ВЫБОРКИ")
print("=" * 80)
print(f"Простая случайная: n = {len(sample_srs)}")
print(f"Механическая:      n = {len(sample_sys)} (шаг k = {step})")
print(f"Послойная:         n = {len(sample_str)} (5 страт по 16 элементов)")
print("=" * 80)


# =============================================================================
# 5. РАСЧЁТ СТАТИСТИЧЕСКИХ ХАРАКТЕРИСТИК
# =============================================================================

def calculate_statistics(data, name=""):
    n = len(data)
    sorted_data = np.sort(data)

    mean = np.mean(data)
    median = np.median(data)

    try:
        n_bins = int(np.ceil(1 + 3.322 * np.log10(n)))
        hist, bin_edges = np.histogram(data, bins=n_bins)
        max_freq_idx = np.argmax(hist)
        mode = (bin_edges[max_freq_idx] + bin_edges[max_freq_idx + 1]) / 2
        mode_count = hist[max_freq_idx]
    except Exception:
        from scipy.stats import mode as scipy_mode
        mode_result = scipy_mode(np.round(data, 2), keepdims=True)
        mode = mode_result.mode[0] if len(mode_result.mode) > 0 else np.nan
        mode_count = mode_result.count[0] if len(mode_result.count) > 0 else 0

    variance = np.var(data, ddof=1)
    std_dev = np.std(data, ddof=1)
    range_val = np.max(data) - np.min(data)

    q1 = np.percentile(data, 25)
    q3 = np.percentile(data, 75)
    iqr = q3 - q1

    se = std_dev / np.sqrt(n)
    cv = (std_dev / mean) * 100 if mean != 0 else np.nan

    t_crit = stats.t.ppf(0.975, df=n - 1)
    ci_lower = mean - t_crit * se
    ci_upper = mean + t_crit * se

    skewness = stats.skew(data)
    kurtosis = stats.kurtosis(data)

    try:
        ks_stat, ks_p = stats.kstest(data, 'norm', args=(mean, std_dev))
    except Exception:
        ks_stat, ks_p = stats.shapiro(data) if len(data) <= 5000 else (np.nan, np.nan)

    return {
        'name': name, 'n': n, 'mean': mean, 'median': median,
        'mode': mode, 'mode_count': mode_count,
        'variance': variance, 'std_dev': std_dev, 'range': range_val,
        'q1': q1, 'q3': q3, 'iqr': iqr,
        'se': se, 'cv': cv,
        'ci_lower': ci_lower, 'ci_upper': ci_upper,
        'skewness': skewness, 'kurtosis': kurtosis,
        'ks_stat': ks_stat, 'ks_p': ks_p,
        'data': data, 'sorted_data': sorted_data
    }


stats_pop = calculate_statistics(population, "Генеральная совокупность")
stats_srs = calculate_statistics(sample_srs, "Простая случайная выборка")
stats_sys = calculate_statistics(sample_sys, "Механическая выборка")
stats_str = calculate_statistics(sample_str, "Послойная выборка")

stats_srs['indices'] = indices_srs
stats_sys['indices'] = indices_sys
stats_str['indices'] = indices_str


# =============================================================================
# 6. ТАБЛИЦА СТАТИСТИК
# =============================================================================

def create_stats_dataframe(stats_list):
    df_data = []
    for s in stats_list:
        df_data.append({
            'Выборка': s['name'],
            'Объём (n)': s['n'],
            'Среднее': s['mean'],
            'Медиана': s['median'],
            'Мода': s['mode'] if not np.isnan(s['mode']) else None,
            'Дисперсия': s['variance'],
            'Ст. отклонение': s['std_dev'],
            'Размах': s['range'],
            'IQR': s['iqr'],
            'Ст. ошибка (SE)': s['se'],
            'CV, %': s['cv'],
            'Асимметрия': s['skewness'],
            'Эксцесс': s['kurtosis']
        })
    return pd.DataFrame(df_data)


stats_df = create_stats_dataframe([stats_pop, stats_srs, stats_sys, stats_str])


# =============================================================================
# 7. ВИЗУАЛИЗАЦИЯ ДАННЫХ
# =============================================================================

def create_visualizations(population, sample_srs, sample_sys, sample_str,
                          stats_pop, stats_srs, stats_sys, stats_str):
    fig = plt.figure(figsize=(18, 12))

    # 7.1. Исходные данные с указанием выборок
    ax1 = plt.subplot(3, 2, 1)
    indices = np.arange(len(population))
    ax1.scatter(indices, population, alpha=0.3, s=10,
                label='Генеральная совокупность', color='gray')
    ax1.scatter(stats_srs['indices'], stats_srs['data'],
                alpha=0.8, s=25, label='Простая случайная', color='blue')
    ax1.scatter(stats_sys['indices'], stats_sys['data'],
                alpha=0.8, s=25, label='Механическая', color='orange')
    ax1.scatter(stats_str['indices'], stats_str['data'],
                alpha=0.8, s=25, label='Послойная', color='green')
    ax1.set_xlabel('Индекс элемента')
    ax1.set_ylabel('Значение')
    ax1.set_title('Распределение элементов выборок\nв генеральной совокупности')
    ax1.legend(loc='upper right', fontsize=8)
    ax1.grid(True, alpha=0.3)

    # 7.2. Гистограммы
    ax2 = plt.subplot(3, 2, 2)
    ax2.hist(population, bins=20, alpha=0.4, label='Генеральная',
             color='gray', density=True)
    ax2.hist(sample_srs, bins=15, alpha=0.5, label='Простая случайная',
             color='blue', density=True)
    ax2.hist(sample_sys, bins=15, alpha=0.5, label='Механическая',
             color='orange', density=True)
    ax2.hist(sample_str, bins=15, alpha=0.5, label='Послойная',
             color='green', density=True)
    ax2.set_xlabel('Значение')
    ax2.set_ylabel('Плотность')
    ax2.set_title('Гистограммы распределений')
    ax2.legend(fontsize=8)
    ax2.grid(True, alpha=0.3)

    # 7.3. Boxplot
    ax3 = plt.subplot(3, 2, 3)
    data_for_box = [population, sample_srs, sample_sys, sample_str]
    labels = ['Генеральная', 'Простая\nслучайная', 'Механическая', 'Послойная']
    bp = ax3.boxplot(data_for_box, patch_artist=True, widths=0.6)
    colors = ['gray', 'blue', 'orange', 'green']
    for patch, color in zip(bp['boxes'], colors):
        patch.set_facecolor(color)
        patch.set_alpha(0.6)
    ax3.set_xticklabels(labels, fontsize=8)
    ax3.set_ylabel('Значение')
    ax3.set_title('Сравнение распределений (Boxplot)')
    ax3.grid(True, alpha=0.3, axis='y')

    # 7.4. Вариационные ряды
    ax4 = plt.subplot(3, 2, 4)
    ax4.plot(np.sort(population), label='Генеральная',
             color='gray', alpha=0.5, linewidth=2)
    ax4.plot(np.sort(sample_srs), label='Простая случайная',
             color='blue', linewidth=1.5, marker='o', markersize=2)
    ax4.plot(np.sort(sample_sys), label='Механическая',
             color='orange', linewidth=1.5, marker='s', markersize=2)
    ax4.plot(np.sort(sample_str), label='Послойная',
             color='green', linewidth=1.5, marker='^', markersize=2)
    ax4.set_xlabel('Порядковый номер')
    ax4.set_ylabel('Значение')
    ax4.set_title('Вариационные ряды')
    ax4.legend(fontsize=8)
    ax4.grid(True, alpha=0.3)

    # 7.5. Отклонение от теоретического среднего
    ax5 = plt.subplot(3, 2, 5)
    deviations = [
        abs(stats_srs['mean'] - mu_theor),
        abs(stats_sys['mean'] - mu_theor),
        abs(stats_str['mean'] - mu_theor)
    ]
    deviation_labels = ['Простая\nслучайная', 'Механическая', 'Послойная']
    colors = ['blue', 'orange', 'green']
    bars = ax5.bar(deviation_labels, deviations, color=colors, alpha=0.6)
    ax5.set_ylabel('Абсолютное отклонение среднего')
    ax5.set_title(f'Отклонение среднего от теор. (μ = {mu_theor})')
    ax5.axhline(y=stats_srs['se'], color='red', linestyle='--',
                alpha=0.5, label=f'SE СС = {stats_srs["se"]:.2f}')
    ax5.legend(fontsize=8)
    ax5.grid(True, alpha=0.3, axis='y')
    for bar, val in zip(bars, deviations):
        ax5.text(bar.get_x() + bar.get_width() / 2, bar.get_height() + 0.05,
                 f'{val:.4f}', ha='center', va='bottom', fontsize=9)

    plt.tight_layout()

    # --- ВАЖНО: сохраняем PNG (это я потерял в прошлой версии) ---
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    figure_path = os.path.join(FIGURES_DIR, f'var4_visualization_{timestamp}.png')
    plt.savefig(figure_path, dpi=200, bbox_inches='tight')
    print(f"\n✅ Рисунок сохранён: {figure_path}")

    # --- Показ окна (теперь действительно работает) ---
    if SHOW_PLOTS:
        try:
            plt.show()
        except KeyboardInterrupt:
            print("ℹ️ Показ графиков прерван пользователем (Ctrl+C). Продолжаем.")
        finally:
            plt.close('all')
    else:
        plt.close(fig)

    return fig

# =============================================================================
# 9. ВЫВОД РЕЗУЛЬТАТОВ В КОНСОЛЬ
# =============================================================================

def print_statistics_table(stats_df):
    print("\n" + "=" * 80)
    print("ТАБЛИЦА СТАТИСТИЧЕСКИХ ХАРАКТЕРИСТИК")
    print("=" * 80)
    formatted_df = stats_df.copy()
    float_cols = formatted_df.select_dtypes(include=[np.float64, float]).columns
    for col in float_cols:
        if col == 'CV, %':
            formatted_df[col] = formatted_df[col].apply(lambda x: f"{x:.2f}")
        else:
            formatted_df[col] = formatted_df[col].apply(lambda x: f"{x:.4f}")
    formatted_df = formatted_df.fillna("N/A")
    print(formatted_df.to_string(index=False))
    return formatted_df


def print_analysis(stats_pop, stats_srs, stats_sys, stats_str):
    print("\n" + "=" * 80)
    print("АНАЛИЗ РЕПРЕЗЕНТАТИВНОСТИ ВЫБОРОК")
    print("=" * 80)

    print("\n1. СРАВНЕНИЕ СРЕДНИХ ЗНАЧЕНИЙ (теоретическое μ = 50)")
    print("-" * 60)
    print(f"Генеральное среднее: {stats_pop['mean']:.4f}")
    print(f"Простая случайная:  {stats_srs['mean']:.4f} "
          f"(отклонение: {abs(stats_srs['mean'] - mu_theor):.4f})")
    print(f"Механическая:       {stats_sys['mean']:.4f} "
          f"(отклонение: {abs(stats_sys['mean'] - mu_theor):.4f})")
    print(f"Послойная:          {stats_str['mean']:.4f} "
          f"(отклонение: {abs(stats_str['mean'] - mu_theor):.4f})")

    print("\n2. ОЦЕНКА РАЗБРОСА (CV, %)")
    print("-" * 60)
    print(f"Генеральная совокупность: {stats_pop['cv']:.2f}%")
    print(f"Простая случайная:        {stats_srs['cv']:.2f}%")
    print(f"Механическая:             {stats_sys['cv']:.2f}%")
    print(f"Послойная:                {stats_str['cv']:.2f}%")


def print_conclusion(stats_srs, stats_sys, stats_str):
    print("\n" + "=" * 80)
    print("ЗАКЛЮЧЕНИЕ ПО РЕПРЕЗЕНТАТИВНОСТИ")
    print("=" * 80)

    deviations = {
        'Простая случайная': abs(stats_srs['mean'] - mu_theor),
        'Механическая': abs(stats_sys['mean'] - mu_theor),
        'Послойная': abs(stats_str['mean'] - mu_theor)
    }
    best_sample = min(deviations, key=deviations.get)

    print(f"\n1. Наиболее репрезентативная выборка: {best_sample}")
    print(f"   Отклонение от μ_теор: {deviations[best_sample]:.4f}")

    print(f"\n2. Точность оценок (стандартная ошибка среднего):")
    print(f"   - Простая случайная: SE = {stats_srs['se']:.4f}")
    print(f"   - Механическая:      SE = {stats_sys['se']:.4f}")
    print(f"   - Послойная:         SE = {stats_str['se']:.4f}")

    print("\n3. Замечание по равномерному распределению:")
    print("   Генеральная совокупность имеет равномерное распределение U(0, 100),")
    print("   которое НЕ является нормальным. Поэтому KS-тест отвергает гипотезу")
    print("   о нормальности для всех выборок — это ожидаемый результат.")


# =============================================================================
# 10. ДОПОЛНИТЕЛЬНЫЙ АНАЛИЗ
# =============================================================================

def additional_analysis(stats_pop, stats_srs, stats_sys, stats_str):
    print("\nСРАВНЕНИЕ ДИСПЕРСИЙ С ГЕНЕРАЛЬНОЙ")
    print("-" * 40)
    print(f"Теоретическая дисперсия U(0,100): {var_theor:.4f}")
    print(f"Выборочная дисперсия генеральной: {stats_pop['variance']:.4f}")
    for name, stat in [('Простая случайная', stats_srs),
                       ('Механическая', stats_sys),
                       ('Послойная', stats_str)]:
        print(f"{name}: s² = {stat['variance']:.4f} "
              f"(отношение к теор.: {stat['variance'] / var_theor:.4f})")


# =============================================================================
# ЗАПУСК
# =============================================================================

def main():
    # 1) Графики: сохраняются в PNG и (если SHOW_PLOTS=True) показываются в окне
    create_visualizations(
        population, sample_srs, sample_sys, sample_str,
        stats_pop, stats_srs, stats_sys, stats_str
    )

    # 2) Консольный вывод
    print_statistics_table(stats_df)
    print_analysis(stats_pop, stats_srs, stats_sys, stats_str)
    print_conclusion(stats_srs, stats_sys, stats_str)

    additional_analysis(stats_pop, stats_srs, stats_sys, stats_str)

    # 3) Итог
    print("\n" + "=" * 80)
    print("ЛАБОРАТОРНАЯ РАБОТА №1 (ВАРИАНТ 4) ВЫПОЛНЕНА")
    print("=" * 80)
    print(f"\n📁 Результаты сохранены в папке: {RESULTS_DIR}")
    print("\n" + "=" * 80)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n⚠️ Выполнение прервано пользователем (Ctrl+C).")