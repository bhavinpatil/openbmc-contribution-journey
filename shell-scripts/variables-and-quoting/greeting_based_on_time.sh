#!/bin/bash
hour=$(date +%H)
echo $date
echo $hour

if [ "$hour" -lt 12 ]; then
greet="Good Morning"

elif [ "$hour" -lt 18 ]; then
greet="Good Afternoon"

else
greet="Good Evening"

fi

echo "$greet, Bhavin!"