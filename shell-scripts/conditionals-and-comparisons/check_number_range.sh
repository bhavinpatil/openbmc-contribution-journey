read -p "Enter number: " num

if [ "$num" -lt 10 ]; then
echo "number is less than 10"
elif [ "$num" -le 20 ]; then
echo "number is between 10 to 20"
else
echo "number is greater than 20"
fi
