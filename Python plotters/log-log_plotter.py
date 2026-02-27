import matplotlib.pyplot as plt

# ----- DANE -----
# Częstotliwości w kHz
f = [0.1, 0.5, 1, 10, 50, 100, 150, 200]
# f = [0.55, 1, 2, 3, 4, 5, 5.05, 5.1, 5.15, 5.2, 5.5, 6, 7]

# Szeregowy
R_meas = [3448276, 602410, 295858, 29078, 6098, 2309, 4237, 4695]
R_theory = [3030303, 606061, 303030, 30303, 6061, 3030, 2020, 1515]

# Równoległy
# R_meas = [3125000, 628931, 312500, 28571, 6757, 2227, 1524, 1277]
# R_theory = [3030303, 606061, 303030, 30303, 6061, 3030, 2020, 1515]

# Biliniowy
# R_meas = [1176471, 236967, 118203, 8547, 1764, 770, 629, 651]
# R_theory = [757576, 151515, 75758, 7576, 1515, 758, 505, 379]

# ----- WYKRES -----
plt.figure(figsize=(10, 6))

plt.plot(f, R_meas, 'o-', markersize=8, linewidth=4, label="Pomiary")
plt.plot(f, R_theory, 'o-', markersize=8, linewidth=4, label="Teoria")

# Skale logarytmiczne
plt.xscale('log')
plt.yscale('log')

# Opisy osi
plt.xlabel("Częstotliwość [kHz]", fontsize=14)
plt.ylabel("Rezystancja zastępcza [Ω]", fontsize=14)

# Gruba siatka jak na Twoim przykładzie
plt.grid(True, which='both', linestyle='--', alpha=0.4)

# Legenda u góry
plt.legend(fontsize=14, loc='upper center')

plt.tight_layout()
plt.show()
