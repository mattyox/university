import matplotlib.pyplot as plt

def plot_logic(signals, labels, step=1):
    """
    signals: dict of {signal_name: [0/1 values]}
    step: time step width
    """
    fig, ax = plt.subplots(figsize=(10, 2 + len(signals)))

    time = range(len(next(iter(signals.values()))) + 1)

    colors = ["tab:blue", "tab:orange", "tab:green", "tab:purple", "tab:brown"]

    for idx, (name, values) in enumerate(signals.items()):
        y_offset = idx * 2

        # Extend values for step plot
        extended = values + [values[-1]]

        color = colors[idx % len(colors)]  # ← pick ONE color

        ax.step(
            [t * step for t in time],
            [v + y_offset for v in extended],
            where="post",
            color=color
        )

        # Signal label
        ax.text(-0.5, y_offset + 0.5, name, va="center", ha="center", fontweight="bold")
        ax.set_xlim(left=-step)

    ax.set_yticks([])

    max_t = (len(time) - 1) * step
    odd_ticks = list(range(step, max_t + 1, 2 * step))

    ax.set_xticks(odd_ticks)
    ax.set_xticklabels([str(i) for i in range(1, len(odd_ticks) + 1)])

    # --- top axis: one label per logic value ---
    ax_top = ax.secondary_xaxis("top")

    num_samples = len(signals["TCK"])

    # First top label: above first logic value
    positions = [odd_ticks[0] - step / 2]

    # Center of each logic value
    positions += [t + step for t in odd_ticks[:-1]]

    ax_top.set_xticks(positions)
    ax_top.tick_params(length=0)
    ax_top.set_xticklabels(labels)
    ax_top.set_xlabel("Stan TAP", labelpad=20, fontweight="bold")
    ax_top.xaxis.label.set_y(1.25)

    ax.set_xlabel("Cykl zegara", labelpad=15, fontweight="bold")
    # ax.set_title("Logic Timing Diagram")
    ax.grid(True, axis="x", linestyle="--", alpha=0.5)

    plt.tight_layout()
    plt.show()


# Task 2 and 3
# Test-Logic-Reset (0)-> Run-Test/Idle (1)-> Select-DR-Scan (1)-> Select-IR-Scan (0)-> Capture-IR (0)-> Shift-IR (0, 0, 0, 0) (1)-> Exit1-IR (1)-> Update-IR
# signals = {
#     "TDO": [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
#     "TDI": [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0],
#     "TMS": [0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1],
#     "TCK": [1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1],
# }

# labels = ["F", "C", "7", "4", "E", "A", "A", "A", "A", "9", "D"]

# Task 6
# Select-DR-Scan (0)-> Capture-DR (0)-> Shift-IR (0, 0, 0)-> Exit1-DR (1)-> Update-DR
signals = {
    "TMS":      [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1],
    "Update":   [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    "Shift":    [0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0],
    "Clock":    [1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1],
    "TCK":      [1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1],
    
}

labels = ["7", "6", "2", "2", "2", "2", "1", "5"]

plot_logic(signals, labels)
