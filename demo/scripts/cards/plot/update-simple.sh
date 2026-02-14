#!/bin/bash
# Plot Card: Simplest possible update test

NAME="simple"

# Create plot with initial data (10 values)
PAYLOAD=$(echo -n "10,20,30,40,50,60,70,80,90,100" | base64 -w0)
printf '\033]666666;run -c plot -x 0 -y 0 -w 40 -h 8 -r --name %s;--grid --min 0 --max 100;%s\033\\' "$NAME" "$PAYLOAD"
echo
sleep 1

# Update: shift left by 1, append 5
PAYLOAD=$(echo -n "5" | base64 -w0)
printf '\033]666666;update --name %s;advance=1;%s\033\\' "$NAME" "$PAYLOAD"
sleep 0.5

# Update: shift left by 1, append 95
PAYLOAD=$(echo -n "95" | base64 -w0)
printf '\033]666666;update --name %s;advance=1;%s\033\\' "$NAME" "$PAYLOAD"
sleep 0.5

# Update: shift left by 1, append 50
PAYLOAD=$(echo -n "50" | base64 -w0)
printf '\033]666666;update --name %s;advance=1;%s\033\\' "$NAME" "$PAYLOAD"

echo
echo "Expected final: 40,50,60,70,80,90,100,5,95,50"
