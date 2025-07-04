import os
import pandas as pd
import numpy as np
from math import sqrt
import matplotlib.pyplot as plt
import seaborn as sns

# ---- Configuration ----

base_path = r"C:\Users\stijn\OneDrive - Universiteit Utrecht\ERP Muon Tomography\Measurements"  # Replace with your actual path
trigger_locations = {
    "Corner1": (0, 0), #left front
    "Corner3": (0, 50), # left back
    "Corner2": (50, 0), #right front
    "Corner4": (50, 50), #right back
    "Center":  (25, 25),
    "Midleft": (0, 25),      # between PMT1 and PMT3
    "Midright": (50, 25),    # between PMT2 and PMT4
}

pmt_positions = {
    1: (0, 0),
    2: (50, 0),
    3: (0, 50),
    4: (50, 50),
}

# PMT-specific color mapping
pmt_colors = {
    'PMT1': '#e60049',
    'PMT2': '#0bb4ff',
    'PMT3': '#ffa300',
    'PMT4': '#9b19f5',
}


plt.rcParams.update({
    'font.size': 16,        # Larger font
    'axes.titlesize': 22,   # Title font size
    'axes.labelsize': 22,   # Axis label size
    'xtick.labelsize': 20,  # X tick label size
    'ytick.labelsize': 20,  # Y tick label size
    'legend.fontsize': 18,  # Legend font
    'lines.linewidth': 3, # Thicker lines
    'lines.markersize': 8,   # Larger markers
})

# ---- Processing ----

records = []

for folder_name, trigger_pos in trigger_locations.items():
    folder_path = os.path.join(base_path, folder_name)
    if not os.path.isdir(folder_path):
        print(f"Folder not found: {folder_path}")
        continue

    files = [f for f in os.listdir(folder_path) if f.endswith('.csv') and f.startswith('scope_')]
    
    for file in files:
        try:
            # Filename format: scope_###_pmt.csv
            parts = file.replace('.csv', '').split('_')
            if len(parts) != 3:
                continue

            measurement_id = int(parts[1])
            pmt_number = int(parts[2])

            pmt_pos = pmt_positions.get(pmt_number)
            if not pmt_pos:
                continue

            distance_cm = sqrt((trigger_pos[0] - pmt_pos[0])**2 + (trigger_pos[1] - pmt_pos[1])**2)

            file_path = os.path.join(folder_path, file)
            df = pd.read_csv(file_path, skiprows=1).iloc[1:1500]
            time = df.iloc[:, 0]
            voltage = df.iloc[:, 1]

            peak_idx = voltage.idxmin()
            peak_time = time.loc[peak_idx]
            peak_voltage = abs(voltage.loc[peak_idx])
            
            # --- Filter out weak signals ---
            if abs(voltage.loc[peak_idx]) < 0.005:
                continue  # Skip low-amplitude signals likely due to noise

            records.append({
                'trigger_position': folder_name,
                'measurement_id': measurement_id,
                'pmt': f'PMT{pmt_number}',
                'distance_cm': distance_cm,
                'peak_time_s': peak_time,
                'amplitude': peak_voltage,
                'pmt_number': pmt_number
            })

        except Exception as e:
            print(f"Error reading {file}: {e}")

# ---- Convert to DataFrame ----

df = pd.DataFrame(records)

# ---- Time Offset Calculation ----
df['time_offset_s'] = np.nan
for group, group_df in df.groupby(['trigger_position', 'measurement_id']):
    # Find PMT at distance 0 (i.e., the PMT nearest the trigger)
    ref = group_df.loc[group_df['distance_cm'].idxmin()]
    ref_time = ref['peak_time_s']
    mask = (df['trigger_position'] == group[0]) & (df['measurement_id'] == group[1])
    df.loc[mask, 'time_offset_s'] = df.loc[mask, 'peak_time_s'] - ref_time

# ---- Plots ----
#%%
# Create directory for saving plots
plot_save_path = os.path.join(base_path, "plots")
os.makedirs(plot_save_path, exist_ok=True)

# 1. Amplitude vs Distance
# --- Average Amplitude vs Distance with Error Bars ---

plt.figure(figsize=(10, 6))

# Compute mean and std dev of amplitude per PMT and distance
avg_df = df.groupby(['pmt', 'distance_cm']).agg(
    mean_amplitude=('amplitude', 'mean'),
    std_amplitude=('amplitude', 'std'),
    count=('amplitude', 'count')
).reset_index()

# Optional: Use standard error instead of std dev
avg_df['sem_amplitude'] = avg_df['std_amplitude'] / np.sqrt(avg_df['count'])

# Plot each PMT line with error bars
plt.figure(figsize=(10, 6))

for pmt_name, group in avg_df.groupby('pmt'):
    color = pmt_colors.get(pmt_name, 'black')
    plt.errorbar(
        group['distance_cm'],
        group['mean_amplitude'],
        yerr=group['std_amplitude'],
        label=pmt_name,
        fmt='o-',
        capsize=4,
        color=color,
    )

plt.title('Average Amplitude vs Distance (Per PMT)', size=28)
plt.xlabel('Distance from Trigger (cm)', size=28)
plt.ylabel('Mean Amplitude (V)', size=28)
plt.grid(True)
plt.legend(fontsize=25)
plt.tight_layout()
plt.savefig(os.path.join(plot_save_path, 'amplitude_vs_distance.jpg'), dpi=600)
plt.show()



# 2. Time Offset vs Distance
# ---- Time Offset Stats ----

time_stats_df = df.groupby(['pmt', 'distance_cm']).agg(
    mean_time_offset=('time_offset_s', 'mean'),
    std_time_offset=('time_offset_s', 'std'),
    count=('time_offset_s', 'count')
).reset_index()

# Optional: Compute SEM
time_stats_df['sem_time_offset'] = time_stats_df['std_time_offset'] / np.sqrt(time_stats_df['count'])

# 2. Average Time Offset vs Distance with Error Bars
plt.figure(figsize=(10, 6))

for pmt_name, group in time_stats_df.groupby('pmt'):
    color = pmt_colors.get(pmt_name, 'black')
    plt.errorbar(
        group['distance_cm'],
        group['mean_time_offset']/1*10**9,
        yerr=group['sem_time_offset']/1*10**9,
        label=pmt_name,
        fmt='o-',
        capsize=8,
        color=color
    )

plt.title('Average Time Offset vs Distance (Per PMT)', size=28)
plt.xlabel('Distance from Trigger (cm)', size=28)
plt.ylabel('Mean Time Offset (ns)', size=28)
plt.grid(True)
plt.legend(fontsize=25)
plt.tight_layout()
plt.savefig(os.path.join(plot_save_path, 'time_offset_vs_distance.jpg'), dpi=600)
plt.show()

#%%
# --- Merge the amplitude and time offset data ---
merged_df = avg_df.merge(time_stats_df, on=['pmt', 'distance_cm'], how='inner')

# --- Plot: Amplitude vs Time Offset with Error Bars ---
plt.figure(figsize=(10, 6))

for pmt_name, group in merged_df.groupby('pmt'):
    plt.errorbar(
        group['mean_time_offset'],         # X: time offset
        group['mean_amplitude'],           # Y: amplitude
        xerr=group['sem_time_offset'],     # X error bars
        yerr=group['sem_amplitude'],       # Y error bars
        fmt='o',
        capsize=8,
        label=pmt_name
    )

plt.xlabel('Mean Time Offset (nanos)')
plt.ylabel('Mean Amplitude (V)')
plt.title('Amplitude vs Time Offset with Error Bars (Per PMT)')
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()

#%%
#Threshold plots
x_3 = np.array([20, 40, 60, 80, 100, 120, 140, 160, 180, 200])
y_3 = np.array([2152, 1330, 1044, 782, 622, 429, 309, 226, 148, 116])

x_4 = np.array([20, 40, 60, 80, 100, 120, 140])
y_4 = np.array([1820, 1190, 760, 561, 360, 263, 187])

font1 = {'size' : 28}
font2 = {'size' : 28}

pmt_colors = {
    'Trigger arm PMT (upper)': '#e60049',
    'Trigger arm PMT (lower)': '#0bb4ff',
    }
plt.figure(figsize=(10, 6))   
plt.title('Number of pulses per threshold in 5 minutes', fontdict = font2)
plt.xlabel('Threshold (mV)', fontdict = font1)
plt.ylabel('Number of Pulses', fontdict = font1)

plt.plot(x_3, y_3, 'o', ls = 'solid', color = '#e60049', label = 'Trigger arm PMT (upper)')
plt.plot(x_4, y_4, 'o', ls = 'solid', color = '#0bb4ff', label = 'Trigger arm PMT (lower)')

plt.legend(fontsize =25)
plt.grid()
plt.tight_layout()
plt.savefig(os.path.join(plot_save_path, 'number_of_pulses_threshold.jpg'), dpi=600)
plt.show()
