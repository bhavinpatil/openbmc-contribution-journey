# README: File Transfer Between Host and OpenBMC QEMU (qemuarm)

This document explains how to share files from the **host machine** to the **OpenBMC QEMU (qemuarm) guest** when running with `slirp` networking.

---

## 1. Start QEMU

Use the following command to boot the OpenBMC QEMU ARM image:

```bash
runqemu qemuarm nographic slirp
```
This will boot the QEMU machine and drop you directly into the root shell of the OpenBMC guest.

## 2. Transfer File From Host to QEMU
From the host side, open another terminal and use the scp command to copy your file into the QEMU guest.

Example: transferring bhavin_virtual_switch from the host directory ~/code to the guest /root:

```bash
scp -P 2222 ~/code/bhavin_virtual_switch root@127.0.0.1:/root/
```
## 3. Transfer File From QEMU to Host (Optional)
If you want to copy a file back from the QEMU guest to the host:
```
scp -P 2222 root@127.0.0.1:/root/somefile ~/code/
```
## 4. Verify Inside QEMU
After transferring, verify inside the QEMU guest:
```
ls -l /root/
```
If the binary is not executable, run:
```
chmod +x /root/bhavin_virtual_switch
./bhavin_virtual_switch &
busctl list | grep openbmc_project
```
