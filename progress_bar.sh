#!/bin/bash

TOTAL=$1
COMPLETED=$2
CURRENT=$3
LOG=$4

LINE=$(tput lines)

move_cursor() {
	echo -ne "\033[$1;$2H"
}

# Calculate the percentage
PERCENTAGE=$((100 * $COMPLETED / $TOTAL))

BAR_LENGTH=50

NUM_EQUALS=$((PERCENTAGE * BAR_LENGTH / 100))

# Create the progress bar

BAR=$(printf "%-${BAR_LENGTH}s" "$BAR")

BAR=$(printf "%-${NUM_EQUALS}s" | tr ' ' '=')

# Fill the bar with = characters

# Move the cursor to the bottom of the terminal
echo -ne "\r$LOG\n"

move_cursor $LINE 0
# Print the progress bar
printf "[%-50s] %d%% - %s" "$BAR" "$PERCENTAGE" "$CURRENT"


# Print a newline if the progress is complete
if [ $COMPLETED -eq $TOTAL ]; then
	echo -ne "\033[2K\r"
fi