file="log.txt"

if [ -f "$file" ]; then
echo "$file exists"
else
echo "$file does not exists!"
fi

