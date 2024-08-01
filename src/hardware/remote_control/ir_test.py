import gpiod

# Open the GPIO chip (e.g., gpiochip4 for Raspberry Pi 5)
chip = gpiod.Chip("/dev/gpiochip4")

# Initialize a variable to hold the selected line
selected_line = None

# Iterate over the lines available on the chip
for line in chip:
    # Check if the line has the desired label
    if line.name == "gpio18":  # Replace "gpio18" with the label of your GPIO pin
        selected_line = line
        break

# Check if a line with the desired label was found
if selected_line is None:
    print("GPIO line not found!")
    chip.close()
    exit()

# Request the selected line for input
selected_line.request(consumer="example", type=gpiod.LINE_REQ_DIR_IN)

# Main loop
try:
    while True:
        # Read the value of the selected GPIO line
        value = selected_line.get_value()
        print("GPIO value:", value)
except KeyboardInterrupt:
    # Clean up
    selected_line.release()
    chip.close()
