import re
from datetime import datetime
import argparse
import matplotlib.pyplot as plt
import numpy as np
import os

# Regex to capture timestamps with microseconds
TIMESTAMP_REGEX = r"\[(\d{2}):(\d{2}):(\d{2})\.(\d{6})\]"

def parse_timestamp(line):
    match = re.search(TIMESTAMP_REGEX, line)
    if not match:
        return None
    h, m, s, micros = map(int, match.groups())
    return datetime(1900, 1, 1, h, m, s, micros)

def load_timestamps(filepath, keyword=None):
    timestamps = []
    with open(filepath, "r") as f:
        for line in f:
            if keyword and keyword not in line:
                continue
            ts = parse_timestamp(line)
            if ts:
                timestamps.append(ts)
    return timestamps

def compare_logs(file1, file2, keyword1=None, keyword2=None):
    ts1 = load_timestamps(file1, keyword1)
    ts2 = load_timestamps(file2, keyword2)

    length = min(len(ts1), len(ts2))
    if length == 0:
        print("No matching timestamps found.")
        return

    diffs_ms = [(ts1[i] - ts2[i]).total_seconds() * 1000 for i in range(length)]
    diffs_np = np.array(diffs_ms)

    mean = diffs_np.mean()
    std = diffs_np.std()
    lower_bound = mean - 2 * std
    upper_bound = mean + 2 * std
    inlier_indices = np.where((diffs_np >= lower_bound) & (diffs_np <= upper_bound))[0]

    print(f"\nCompared {length} timestamps")
    print(f"  Mean:   {mean:.3f} ms")
    print(f"  Min:    {diffs_np.min():.3f} ms")
    print(f"  Max:    {diffs_np.max():.3f} ms")
    print(f"  Stddev: {std:.3f} ms")
    print(f"  Outliers (excluded from histogram): {length - len(inlier_indices)}")

    base_name1 = os.path.splitext(os.path.basename(file1))[0]
    base_name2 = os.path.splitext(os.path.basename(file2))[0]
    base_out = f"{base_name1}_vs_{base_name2}"

    # Line plot (all data)
    plt.figure(figsize=(10, 4))
    plt.plot(diffs_np, marker='o', linestyle='-', color='steelblue', label='Delta (ms)')
    plt.title("Timestamp Differences (All Data)")
    plt.xlabel("Message Index")
    plt.ylabel("Difference (ms)")
    plt.grid(True)
    plt.tight_layout()
    line_plot_file = f"{base_out}_delta_all.png"
    plt.savefig(line_plot_file)
    print(f"Saved line plot (all data) to: {line_plot_file}")
    plt.show()

    # Histogram (inliers only)
    plt.figure(figsize=(6, 4))
    plt.hist(diffs_np[inlier_indices], bins=30, color='darkorange', edgecolor='black')
    plt.title("Histogram of Timestamp Differences (Inliers Only)")
    plt.xlabel("Difference (ms)")
    plt.ylabel("Frequency")
    plt.tight_layout()
    hist_plot_file = f"{base_out}_histogram_clean.png"
    plt.savefig(hist_plot_file)
    print(f"Saved histogram to: {hist_plot_file}")
    plt.show()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Compare timestamps from two Arduino log files.")
    parser.add_argument("file1", help="First log file")
    parser.add_argument("file2", help="Second log file")
    parser.add_argument("--filter1", help="Keyword filter for first log", default=None)
    parser.add_argument("--filter2", help="Keyword filter for second log", default=None)
    args = parser.parse_args()

    compare_logs(args.file1, args.file2, args.filter1, args.filter2)
