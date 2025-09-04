# README: Cross Compilation and File Transfer Between Host and OpenBMC QEMU (qemuarm)

This document explains how to cross-compile C++ programs for OpenBMC QEMU (qemuarm) and how to transfer files between the host machine and the QEMU guest.
---

## 1. Setting up Cross Compiler (SDK)

If you don’t already have a cross-compiler for OpenBMC, you can generate and install the SDK using the following steps:
```bash
cd ~/openbmc/build/qemuarm
. setup qemuarm
bitbake obmc-phosphor-image -c populate_sdk
```

⚠️ Note: This step may take around 3–4 hours to complete on the first run.

After completion, an SDK installer will be generated inside:
```bash
~/openbmc/build/qemuarm/tmp/deploy/sdk/
```

Example output:
```bash
black@bhavin:~/openbmc/build/qemuarm/tmp/deploy/sdk$ ls -al
-rwxr-xr-x 2 black black 327491737 Aug 29 10:23 oecore-obmc-phosphor-image-x86_64-cortexa15t2hf-neon-qemuarm-toolchain-nodistro.0.sh
...
```

Install the SDK

Run the generated .sh installer:
```bash
./oecore-obmc-phosphor-image-x86_64-cortexa15t2hf-neon-qemuarm-toolchain-nodistro.0.sh
```

It will ask for an installation path (default: /usr/local/oecore-x86_64).

Setup Environment

After installation, set up the environment:
```bash
source /usr/local/oecore-x86_64/environment-setup-armv7at2hf-neon-oe-linux-gnueabi
```
Verify Compiler

Check the compiler variable:
```bash
echo $CXX
```

You should see the cross-compiler binary. Now you can compile your C++ program for QEMU ARM:
```bash
$CXX -std=c++11 -O2 -o bhavin-demo-service main.cpp $(pkg-config --cflags --libs dbus-1)
```

## 2. Start QEMU

Use the following command to boot the OpenBMC QEMU ARM image:

```bash
runqemu qemuarm nographic slirp
```
This will boot the QEMU machine and drop you directly into the root shell of the OpenBMC guest.

## 3. Transfer File From Host to QEMU
From the host side, open another terminal and use the scp command to copy your file into the QEMU guest.

Example: transferring bhavin_virtual_switch from the host directory ~/code to the guest /root:

```bash
scp -P 2222 ~/code/bhavin-demo-service root@127.0.0.1:/root/
```
## 4. Transfer File From QEMU to Host (Optional)
If you want to copy a file back from the QEMU guest to the host:
```
scp -P 2222 root@127.0.0.1:/root/somefile ~/code/
```
## 5. Verify Inside QEMU
After transferring, verify inside the QEMU guest:
```
ls -l /root/
```
If the binary is not executable, run:
```
chmod +x /root/bhavin-demo-service
./bhavin-demo-service &
busctl list | grep openbmc_project
```
