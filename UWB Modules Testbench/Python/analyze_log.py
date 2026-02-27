import re
from datetime import datetime
import argparse
import matplotlib.pyplot as plt
import numpy as np
import os

# Regex to capture timestamps with microseconds (6 digits)
TIMESTAMP_REGEX = r"\[(\d{2}):(\d{2}):(\d{2})\.(\d{6})\]"

def parse_timestamp(line):
    match = re.search(TIMESTAMP_REGEX, line)
    if not match:
        return None
    h, m, s, micros = map(int, match.groups())
    return datetime(1900, 1, 1, h, m, s, micros)

def analyze_log(filepath, keyword=None):
    timestamps = []
    lines = []

    with open(filepath, "r") as f:
        for line in f:
            if keyword and keyword not in line:
                continue
            ts = parse_timestamp(line)
            if ts is not None:
                timestamps.append(ts)
                lines.append(line.strip())

    if len(timestamps) < 2:
        print("Not enough data to analyze.")
        return

    # Calculate time deltas in milliseconds
    deltas = [(timestamps[i] - timestamps[i - 1]).total_seconds() * 1000 for i in range(1, len(timestamps))]
    deltas_np = np.array(deltas)

    # Calculate stats (using all data, no outlier removal)
    mean = deltas_np.mean()
    std = deltas_np.std()

    base_name = os.path.splitext(os.path.basename(filepath))[0]
    summary_file = f"{base_name}_summary.txt"

    with open(summary_file, "w") as out_file:
        print(f"\nAnalyzing {len(lines)} messages", file=out_file)
        print(f"Start: {timestamps[0].time()}", file=out_file)
        print(f"End:   {timestamps[-1].time()}", file=out_file)
        print(f"Duration: {timestamps[-1] - timestamps[0]}", file=out_file)
        print(f"\nMessage Interval Stats (ms):", file=out_file)
        print(f"  Mean:   {mean:.3f}", file=out_file)
        print(f"  Min:    {deltas_np.min():.3f}", file=out_file)
        print(f"  Max:    {deltas_np.max():.3f}", file=out_file)
        print(f"  Stddev: {std:.3f}", file=out_file)

    print(f"\nSaved summary to: {summary_file}")

    # Plot: Delta line plot (all data)
    plt.figure(figsize=(10, 4))
    plt.plot(deltas, marker='o', linestyle='-', color='royalblue', label='Delta (ms)')
    plt.title("Time Delta Between Messages")
    plt.xlabel("Message Index")
    plt.ylabel("Delta (ms)")
    plt.grid(True)
    plt.tight_layout()
    line_plot_file = f"{base_name}_deltas.png"
    plt.savefig(line_plot_file)
    print(f"\nSaved line plot to: {line_plot_file}")
    plt.show()

    # Plot: Histogram (all data, no outliers removed)
    plt.figure(figsize=(6, 4))
    plt.hist(deltas_np, bins=30, color='darkorange', edgecolor='black')
    plt.title("Histogram of Message Intervals (all data)")
    plt.xlabel("Delta (ms)")
    plt.ylabel("Frequency")
    plt.grid(True)
    plt.tight_layout()
    hist_file = f"{base_name}_hist.png"
    plt.savefig(hist_file)
    print(f"Saved histogram to: {hist_file}")
    plt.show()

def main():
    parser = argparse.ArgumentParser(description="Analyze log file timestamps")
    parser.add_argument("filepath", help="Path to the log file")
    parser.add_argument("--keyword", help="Only analyze lines containing this keyword", default=None)
    args = parser.parse_args()

    analyze_log(args.filepath, args.keyword)

if __name__ == "__main__":
    main()

