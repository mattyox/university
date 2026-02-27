# Formulas from L. Rozenblat, Power Supply Design: A Concise Practical Handbook, 2021

import math # sqrt()

# Buck-Boost (Inverting)

# Dane początkowe
V_in = 5.0 # [V]
V_out = -12.0 # [V]
I_out = 0.5 # [A]
f = 300e3 # [kHz]
eta = 0.9

# Tranzystor Q - IRFZ44N
R_DSON = 13.9e-03 # [Ohm]

# Kontroler PWM
I_PK_LIM = 0.0 # [A]

# Dioda D - 30BQ060
V_D = 0.52 # [V]

# Dopuszczalne zmiany napięcia na wyjściu
delta_V_out = 50e-03 # [V]

# Kondensator C - KRM55QR71E336KH01 - https://ds.murata.com/simsurfing/mlcc.html?partnumbers=%5B%22GRM21BZ71H475KE15%22%5D&oripartnumbers=%5B%22GRM21BZ71H475KE15L%22%5D&rgear=suaykx&rgearinfo=com
ESR = 50e-03 # [Ohm]

print("NAPIĘCIE WYJŚCIOWE")

# Współczynnik wypełnienia
D = abs(V_out) / ((eta * abs(V_in)) + abs(V_out))
print(f"Współczynnik wypełnienia D = {(D * 100):.3f} [%]")

# Czas załączenia klucza
t_1 = D / f
print(f"Czas załączenia klucza Q t1 = {t_1:.3e} [s]")

print("\nINDUKCYJNOŚĆ L")

# Średni prąd płynący przez cewkę
I_L_AV = I_out / (1 - D)
print(f"Średni prąd płynący przez cewkę I_LAV = {I_L_AV:.3f} [A]")
print("")

# Indukcyjność cewki
L = (eta * (abs(V_in) ** 2) * (D ** 2)) / (2 * abs(V_out) * I_out * f)
print(f"Indukcyjność cewki L = {L:.3e} [H]")

# Przyrost prądu płynącego przez cewkę
delta_I_L = (abs(V_in) * D) / (L * f)
print(f"Przyrost prądu płynącego przez cewkę delta_I_L = {delta_I_L:.3f} [A]")
print("")

# Praktyczna wartość indukcyjności cewki
L = L * (delta_I_L / (0.3 * I_L_AV))
print(f"Poprawiona indukcyjność cewki L = {L:.3e} [H]")

# Poprawiony przyrost prądu płynącego przez cewkę
delta_I_L = (abs(V_in) * D) / (L * f)
print(f"Poprawiony przyrost prądu płynącego przez cewkę delta_I_L = {delta_I_L:.3f} [A]")

# Warunek CCM
if ((delta_I_L / 2) < I_L_AV):
    print("Warunek CCM spełniony")
else:
    print("Warunek CCM nie spełniony")

print("")

# Katalogowa indukcyjność
L = 22e-06 # [H]
print(f"Katalogowa indukcyjność cewki L = {L:.3e} [H]")

# Przyrost prądu płynącego przez cewkę dla wartości katalogowej
delta_I_L = (abs(V_in) * D) / (L * f)
print(f"Przyrost prądu płynącego przez cewkę dla wartości katalogowej delta_I_L = {delta_I_L:.3f} [A]")
print("")

# Wartość skuteczna prądu płynącego przez cewkę
I_L_RMS = delta_I_L / math.sqrt(12)
print(f"Wartość skuteczna prądu płynącego przez cewkę I_L_RMS = {I_L_RMS:.3f} [A]")

print("\nKLUCZ Q")

# Wartość środkowa impulsu prądowego
I_CV = (I_out * abs(V_out)) / (eta * abs(V_in) * D)
print(f"Wartość środkowa impulsu prądowego I_CV = {I_CV:.3f} [A]")

# Pik prądu
I_Q_PK = I_CV + (delta_I_L / 2)
print(f"Pik prądu I_Q_PK = {I_Q_PK:.3f} [A]")

# Pik napięcia
V_Q_PK = abs(V_in) + abs(V_out)
print(f"Pik napięcia V_Q_PK = {V_Q_PK} [V]")

# Wartość skuteczna prądu
I_Q_RMS = I_CV * math.sqrt(D)
print(f"Wartość skuteczna prądu płynącego I_Q_RMS = {I_Q_RMS:.3f} [A]")

# Straty przewodzenia
P_Q_COND = (I_CV ** 2) * R_DSON * D
print(f"Straty przewodzenia P_Q_COND = {P_Q_COND:.3f} [W]")

# Maksymalny osiągalny prąd obciązenia
I_out_MAX = (I_PK_LIM - (delta_I_L / 2)) / (1 + (abs(V_out) / (eta * abs(V_in))))
print(f"Maksymalny osiągalny prąd obciązenia I_out_MAX = {I_out_MAX:.3f} [A]")

print("\nPROSTOWNIK D")

# Wartość środkowa impulsu prądu
I_D_CV = I_out
print(f"Wartość środkowa impulsu prądu I_D_CV = {I_D_CV} [A]")

# Średnia wartość prądu
I_D_AV = I_out * (1 - D)
print(f"Średnia wartość prądu I_D_AV = {I_D_AV:.3f} [A]")

# Pik prądu
I_D_PK = I_out + (delta_I_L / 2)
print(f"Pik prądu I_D_PK = {I_D_PK:.3f} [A]")

# Pik napięcia wstecznego
V_D_PK = abs(V_out) + abs(V_in)
print(f"Pik napięcia wstecznego V_D_PK = {V_D_PK} [V]")

# Straty przewodzenia
P_D_COND = I_out * V_D * (1 - D)
print(f"Straty przewodzenia P_D_COND = {P_D_COND:.3f} [W]")

print("\nKONDENSATOR WYJŚCIOWY C")

# Prąd kondensatora w czasie t1
I_C = -I_out
print(f"Prąd kondensatora w czasie t1 I_C = {I_C} [A]")

# Okres przełączania
T = 1 / f
print(f"Okres przełączania T = {T:.3e} [s]")

# Prąd pomiędzy t1 i T
I_C = I_L_AV - I_out
print(f"Prąd w pomiędzy t1 i T I_C = {I_C:.3f} [A]")

# Wartość skuteczna prądu
I_C_RMS = I_out * math.sqrt(abs(V_out) / (eta * abs(V_in)))
print(f"Wartość skuteczna prądu I_C_RMS = {I_C_RMS:.3f} [A]")

print("\nPRĄD WEJŚCIOWY")

# Średni prąd
I_IN_AV = I_out * (abs(V_out) / (eta * abs(V_in)))
print(f"Średni prąd I_IN_AV = {I_IN_AV:.3f} [A]")

# Wartość skuteczna komponentu AC prądu
I_IN_AC_RMS = I_out * math.sqrt(abs(V_out) / (eta * abs(V_in)))
print(f"Wartość skuteczna komponentu AC prądu I_IN_AC_RMS = {I_IN_AC_RMS:.3f} [A]")

# Krytyczna wartość prądu wyjściowego
I_out_kryt = ((abs(V_in) * T) / (2 * L)) * D * (1 - D)
print(f"Krytyczna wartość prądu wyjściowego I_out_kryt = {I_out_kryt:.3f} [A]")

# Obciązenie krytyczne
R_L_kryt = abs(V_out) / I_out_kryt
print(f"Obciązenie krytyczne R_L_kryt= {R_L_kryt:.3f} [Ohm]")

print("\nKONDENSATOR WYJŚCIOWY C")

# Pojemność kondensatora
C = delta_I_L / (8 * f * delta_V_out)
print(f"Pojemność kondensatora C = {C:.3e} [F]")

# Zmiany napięcia wyjściowego wywołane przez ESR
delta_V_ESR = ESR * delta_I_L
print(f"Zmiany napięcia wyjściowego wywołane przez ESR delta_V_ESR = {delta_V_ESR:.3e} [V]")
print("")

# Pojemność kondensatora wyznaczona na podstawie symulacji
C = 33e-03
print(f"Pojemność kondensatora wyznaczona na podstawie symulacji C = {C:.3e} [F]")
