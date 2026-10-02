#!/usr/bin/env python3
"""
Генерация генеральной совокупности X ~ Poisson(λ=8), N=400,
формирование подвыборок объёмом n = [10, 25, 50, 100, 200]
с указанием индексов элементов и выгрузка результатов в Excel.
"""

import numpy as np
import pandas as pd

# --- Параметры ---
LAMBDA = 8
N = 400
SEED = 42
SAMPLE_SIZES = [10, 25, 50, 100, 200]
OUT_FILE = "ЛР2_Заварзин_ИИб-25-6-о.xlsx"

# --- Генерация генеральной совокупности ---
rng = np.random.default_rng(SEED)
x = rng.poisson(lam=LAMBDA, size=N)

# --- Лист 1: генеральная совокупность ---
df_sample = pd.DataFrame({
    "№ наблюдения": np.arange(1, N + 1, dtype="int64"),
    "X": x.astype("int64")
})

# --- Лист 2: подвыборки с индексами элементов ---
# Для каждой n: столбец "Индекс" (номер элемента в совокупности, 1-based)
# и столбец "X" со значением. Выборка делается по индексам, затем значения
# берутся из x по этим индексам — так индексы и значения гарантированно
# согласованы. Короткие столбцы добиты пустыми ячейками (pd.NA).
max_n = max(SAMPLE_SIZES)
data = {}
for n in SAMPLE_SIZES:
    idx = rng.choice(N, size=n, replace=False) + 1   # 1-based индексы
    vals = x[idx - 1]
    idx_padded = np.full(max_n, pd.NA, dtype="object")
    val_padded = np.full(max_n, pd.NA, dtype="object")
    idx_padded[:n] = idx.astype("int64")
    val_padded[:n] = vals.astype("int64")
    data[f"n = {n} (индекс)"] = idx_padded
    data[f"n = {n} (X)"] = val_padded

df_subsamples = pd.DataFrame(data)

# --- Запись в Excel ---
with pd.ExcelWriter(OUT_FILE, engine="openpyxl") as writer:
    sheets = {
        "Генеральная совокупность": df_sample,
        "Подвыборки": df_subsamples
    }
    for name, df in sheets.items():
        df.to_excel(writer, sheet_name=name, index=False)

    # Автоширина столбцов (устойчиво к пустым столбцам)
    for name, df in sheets.items():
        ws = writer.sheets[name]
        for i, col in enumerate(df.columns, start=1):
            non_na = df[col].dropna().astype(str)
            data_len = non_na.map(len).max() if not non_na.empty else 0
            width = max(data_len, len(str(col))) + 2
            ws.column_dimensions[ws.cell(row=1, column=i).column_letter].width = width

print(f"Готово: {OUT_FILE}")
print(f"Генеральная совокупность: среднее = {x.mean():.3f}, "
      f"дисперсия = {x.var(ddof=1):.3f} (теор. λ = {LAMBDA})")