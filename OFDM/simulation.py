import numpy as np
from pathlib import Path

# ------------------
# OFDM / NR Parameters
# ------------------
u = 1   # Numerology

M_QAM = 16  # Modulation

B = 20e6    # Bandwidth 20 [MHz]

number_of_symbols = 14

# ------------------
# SNR [dB] Parameter
# ------------------
snr_db = -9 # -10

# ------------------
# Pilot Parameters
# ------------------
pilot_symbol = (1 + 1j) / np.sqrt(2)
pilot_symbols = [2, 11]
pilot_spacing = 6

# ------------------
# Derived parameters
# ------------------
delta_f = (2 ** u) * 15e3   # 30 [kHz]

number_of_active_carriers = int(np.ceil(B / delta_f))   # 667

active_idx = np.arange(number_of_active_carriers)
pilot_idx = active_idx[::pilot_spacing]
data_idx = np.setdiff1d(active_idx, pilot_idx)

bits_per_qam = int(np.log2(M_QAM))  # 4

qam_per_slot = number_of_symbols * len(data_idx)    # 7770
bits_per_slot = qam_per_slot * bits_per_qam # 31080
bytes_per_slot = bits_per_slot // 8 # 3885

number_of_positive_carriers = int(np.floor(number_of_active_carriers / 2))  # 333
number_of_negative_carriers = int(np.ceil(number_of_active_carriers / 2))   # 334

# fft_size = 1 << (number_of_active_carriers - 1).bit_length()  # 1024
fft_size = 1024

# cp_length = int(fft_size / 8)   # 128 - Normal
# cp_length = int(fft_size / 4)   # 256 - Extendet (not used for numerology u = 1)

T_symbol = 1 / delta_f  # 33.3 [us] - Symbol time without Cycling Prefix
T_cp = 2.3e-06  # 2.3 [us] - Cycling Prefix time
cp_length = int(np.ceil(fft_size * (T_cp / T_symbol)))  # 71 - Number of samples used for Cycling Prefix

# ------------------
# Test files
# ------------------

# test_file = "image.png"
test_file = "image.jpg"
# test_file = "text.txt"
# test_file = "bits.bin"

# ------------------
# Utility
# ------------------
def slicer(x):
    return np.where(
        x < -2, -3,
        np.where(
            x < 0, -1,
            np.where(x < 2, +1, +3)
        )
    )

# ------------------
# Main
# ------------------
def main():
    # Cleanup
    path = Path("received_" + test_file)
    try:
        path.unlink()
        print("File deleted")
    except FileNotFoundError:
        print("File does not exist")
    except PermissionError:
        print("Permission denied")
    except OSError as e:
        print(f"Delete failed: {e}")

    # Serial input
    data = np.fromfile(test_file, dtype=np.uint8)

    # Pad input
    number_of_slots = int(np.ceil(len(data) / bytes_per_slot))
    pad_len = number_of_slots * bytes_per_slot - len(data)
    data_padded = np.concatenate([
        data,
        np.zeros(pad_len, dtype=np.uint8)
    ])

    rx_all = []

    # ------------------
    # Slot loop
    # ------------------
    for slot in range(number_of_slots):
        slot_bytes = data_padded[
            slot * bytes_per_slot : (slot + 1) * bytes_per_slot
        ]

        # ------------------
        # Serial-to-parallel (nibbles)
        # ------------------
        high = (slot_bytes >> 4) & 0x0F
        low  = slot_bytes & 0x0F

        nibbles = np.empty(slot_bytes.size * 2, dtype=np.uint8)
        nibbles[0::2] = high
        nibbles[1::2] = low

        # ------------------
        # 16-QAM Modulation
        # ------------------
        i_bits = (nibbles >> 2) & 0b11
        q_bits = nibbles & 0b11

        gray_map = np.array([-3, -1, +3, +1])
        I = gray_map[i_bits]
        Q = gray_map[q_bits]

        qam = (I + 1j * Q) / np.sqrt(10)
        qam = qam.reshape(number_of_symbols, len(data_idx))

        # ------------------
        # OFDM TX
        # ------------------
        tx_slot = []

        for symbol in range(number_of_symbols):
            X_active = np.zeros(number_of_active_carriers, dtype=complex)

            if symbol in pilot_symbols:
                X_active[pilot_idx] = pilot_symbol

            X_active[data_idx] = qam[symbol]

            X = np.zeros(fft_size, dtype=complex)
            X[1 : 1 + number_of_positive_carriers] = X_active[:number_of_positive_carriers]
            X[-number_of_negative_carriers :] = X_active[number_of_positive_carriers:]

            x = np.fft.ifft(X) * fft_size
            x_with_cp = np.concatenate([x[-cp_length:], x])
            tx_slot.append(x_with_cp)

        tx = np.concatenate(tx_slot)

        # ------------------
        # Channel (AWGN)
        # ------------------
        if snr_db is not None:
            snr_lin = 10 ** (snr_db / 10)
            noise_var = 1 / (2 * snr_lin)
            noise = np.sqrt(noise_var) * (
                np.random.randn(*tx.shape) +
                1j * np.random.randn(*tx.shape)
            )
            rx = tx + noise
        else:
            rx = tx.copy()

        # ------------------
        # OFDM RX
        # ------------------
        Y_slot = np.zeros((number_of_symbols, number_of_active_carriers), dtype=complex)
        pointer = 0

        for symbol in range(number_of_symbols):
            received_symbol = rx[pointer + cp_length : pointer + cp_length + fft_size]
            pointer += fft_size + cp_length

            Y = np.fft.fft(received_symbol) / fft_size
            Y_active = np.concatenate([
                Y[1 : 1 + number_of_positive_carriers],
                Y[-number_of_negative_carriers :]
            ])
            Y_slot[symbol] = Y_active

        # ------------------
        # Channel estimation
        # ------------------
        H_pilots = []

        for pilot in pilot_symbols:
            H_pilots.append(Y_slot[pilot, pilot_idx] / pilot_symbol)

        H_pilots = np.mean(H_pilots, axis=0)

        H_estimated = np.interp(
            active_idx,
            pilot_idx,
            H_pilots
        )

        # ------------------
        # Equalization
        # ------------------
        Y_equalized = Y_slot / H_estimated
        qam_hat = Y_equalized[:, data_idx].reshape(-1)

        # ------------------
        # 16-QAM Demod
        # ------------------
        qam_hat *= np.sqrt(10)

        I_hat = slicer(qam_hat.real)
        Q_hat = slicer(qam_hat.imag)

        level_to_bits = np.zeros(7, dtype=np.uint8)
        level_to_bits[-3 + 3] = 0b00
        level_to_bits[-1 + 3] = 0b01
        level_to_bits[+3 + 3] = 0b10
        level_to_bits[+1 + 3] = 0b11

        i_bits_hat = level_to_bits[I_hat + 3]
        q_bits_hat = level_to_bits[Q_hat + 3]

        nibbles_hat = (i_bits_hat << 2) | q_bits_hat
        bytes_hat = (
            (nibbles_hat[0::2] << 4) |
             nibbles_hat[1::2]
        ).astype(np.uint8)

        rx_all.append(bytes_hat)

    # ------------------
    # Reassemble full file
    # ------------------
    data_hat = np.concatenate(rx_all)[:len(data)]

    # ------------------
    # BER / SER
    # ------------------
    bit_diff = np.bitwise_xor(data, data_hat)
    bit_errors = np.unpackbits(bit_diff).sum()
    BER = bit_errors / (len(data) * 8)
    SER = np.mean(nibbles != nibbles_hat)

    print(f"Slots used: {number_of_slots}")
    print(f"Bit errors: {bit_errors}")
    print(f"BER       : {BER:.3e}")
    print(f"SER       : {SER:.3e}")

    data_hat.tofile("received_" + test_file)

if __name__ == "__main__":
    main()
