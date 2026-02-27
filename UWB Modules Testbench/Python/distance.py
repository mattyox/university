import re

def calculate_means(log_file):
    a1_values = []
    a2_values = []
    
    # Open the log file and read line by line
    with open(log_file, 'r') as file:
        for line in file:
            # Regular expression to extract A1 and A2 values from the line
            match = re.match(r'.*\sA1:(\d+\.\d+)\sA2:(\d+\.\d+)', line)
            if match:
                a1 = float(match.group(1))
                a2 = float(match.group(2))
                a1_values.append(a1)
                a2_values.append(a2)
    
    # Calculate the means
    mean_a1 = sum(a1_values) / len(a1_values) if a1_values else 0
    mean_a2 = sum(a2_values) / len(a2_values) if a2_values else 0
    
    return mean_a1, mean_a2

# Example usage
# log_file = 'path_to_your_log_file.txt'  # Change this to the actual path of your log file
log_file = 'test_log_delay.txt'
mean_a1, mean_a2 = calculate_means(log_file)
print(f'Mean of A1: {mean_a1}')
print(f'Mean of A2: {mean_a2}')
