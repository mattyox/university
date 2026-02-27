import matplotlib.pyplot as plt

# Dane
f = [0.55, 1, 2, 3, 4, 5, 5.05, 5.1, 5.15, 5.2, 5.5, 6, 7]
K = [1, 1, 0.933, 0.867, 0.833, 0.733, 0.7, 0.7, 0.7, 0.667, 0.667, 0.6, 0.533]

plt.figure(figsize=(10, 6))

# Wykres z linią + kropkami
# plt.plot(f, K, color="#0094FF", linewidth=5, marker='o', markersize=10, markerfacecolor='white', markeredgewidth=3)
plt.plot(f, K, 'o-', markersize=8, linewidth=4)

plt.xscale('log')

# Siatka
plt.grid(True, which='both', linestyle='--', alpha=0.4)

# Opisy osi
plt.xlabel("Częstotliwość [kHz]", fontsize=14)
plt.ylabel("Wzmocnienie K", fontsize=14)

plt.tight_layout()
plt.show()
