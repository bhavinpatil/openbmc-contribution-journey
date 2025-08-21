# Bhavin Virtual Switch D-Bus Service

This document describes how to deploy and manage the **Bhavin Virtual Switch D-Bus Service** on an OpenBMC QEMU image. This service exposes a virtual switch over D-Bus and allows toggling its state using standard D-Bus method calls.

## 1. Files

The main executable:
```
/usr/bin/bhavin_virtual_switch
```
Systemd unit file:
```
/etc/systemd/system/bhavin-virtual-switch.service
```

---

## 2. Systemd Setup

To make the service start automatically and manage it via systemd, the following unit file is used:

```
[Unit]
Description=Bhavin Virtual Switch D-Bus Service
After=dbus.service
Requires=dbus.service

[Service]
Type=simple
User=root
ExecStart=/usr/bin/bhavin_virtual_switch
Restart=always
RestartSec=5

[Install]
WantedBy=multi-user.target
```
Commands
Reload systemd to detect the new service:
```
sudo systemctl daemon-reload
```
Enable service on boot:
```
sudo systemctl enable bhavin-virtual-switch.service
```
Start service immediately:
```
sudo systemctl start bhavin-virtual-switch.service
```
Check service status:
```
sudo systemctl status bhavin-virtual-switch.service
```
## 3. Verify D-Bus Registration
The service should appear on the system D-Bus:
```
busctl list | grep Bhavin
```
Check the object tree:
```
busctl tree xyz.openbmc_project.Bhavin.VirtualSwitchService
```
## 4. Usage Example
Toggle the virtual switch state using D-Bus method calls:
```
# Toggle switch0
busctl call xyz.openbmc_project.Bhavin.VirtualSwitchService \
/xyz/openbmc_project/bhavin/virtualswitch/switch0 \
xyz.openbmc_project.Bhavin.VirtualSwitch.Control Toggle
```
Sample output:
```
Switched state: ON
Switched state: OFF
```
## 5. Notes
- The service automatically restarts on failure (Restart=always).
- Ensure dbus.service is active before starting this service (After=dbus.service).
- This service is hardware-independent and can run fully in QEMU-based OpenBMC images.
