#!/bin/bash
# Plot Card: Test streaming updates with advance

# Create a plot with a named buffer
NAME="test-plot"
BUFFER_SIZE=50

# Initial data: 50 zeros
INITIAL=""
for i in $(seq 1 $BUFFER_SIZE); do
    [ -n "$INITIAL" ] && INITIAL="$INITIAL,"
    INITIAL="${INITIAL}0"
done
PAYLOAD=$(echo -n "$INITIAL" | base64 -w0)

# Create the plot with initial data
printf '\033]666666;run -c plot -x 0 -y 0 -w 60 -h 10 -r --name %s;--grid --axes --min 0 --max 100;%s\033\\' "$NAME" "$PAYLOAD"
echo

sleep 1

# Send 20 updates with advance=1
for i in $(seq 1 20); do
    # Random value 0-100
    VAL=$((RANDOM % 100))
    PAYLOAD=$(echo -n "$VAL" | base64 -w0)

    printf '\033]666666;update --name %s;advance=1;%s\033\\' "$NAME" "$PAYLOAD"

    sleep 0.2
done

echo "Done"
