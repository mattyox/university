import matplotlib.pyplot as plt

# ----- DANE -----
# Napięcie wejściowe
U = [0, 1, 2, 3, 4, 5]

# Źródło prądowe
I_meas = [14, 187, 359, 524, 671, 805]
I_theory = [0, 165, 330, 495, 660, 825]

# ----- WYKRES -----
plt.figure(figsize=(10, 6))

plt.plot(U, I_meas, 'o-', markersize=8, linewidth=4, label="Pomiary")
plt.plot(U, I_theory, 'o-', markersize=8, linewidth=4, label="Teoria")

# Wymuszenie osi od 0
plt.xlim(left=0)
plt.ylim(bottom=0)

# Opisy osi
plt.xlabel("Napięcie wejściowe [V]", fontsize=14)
plt.ylabel("Prąd wyjściowy [μA]", fontsize=14)

# Gruba siatka jak na Twoim przykładzie
plt.grid(True, which='both', linestyle='--', alpha=0.4)

# Legenda u góry
plt.legend(fontsize=14, loc='upper center')

plt.tight_layout()
plt.show()
