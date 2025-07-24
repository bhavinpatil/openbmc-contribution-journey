service="sshd"

if pgrep "$service" > /dev/null; then
echo "$service is running"
else
echo "$service is NOT running!"
fi