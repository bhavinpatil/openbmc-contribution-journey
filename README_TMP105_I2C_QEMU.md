# Overview

This document summarizes the steps we followed to attempt running a TMP105 temperature sensor inside an OpenBMC QEMU image (qemuarm) and explains why it failed. It also provides guidance for next steps to achieve a working I²C sensor emulation.

## 1. Environment Setup

- Host: Ubuntu Server inside VMware
- OpenBMC source: ~/openbmc
- Target QEMU machine: qemuarm
- Tools used: runqemu, bitbake, qemu-system-arm, dtc
- Goal: Add a TMP105 I²C temperature sensor to the virtual QEMU board to test OpenBMC D-Bus service interaction.

## 2. Investigations

1. Checked available I²C devices in QEMU:
```
qemu-system-arm -machine virt -device help | grep i2c
```

Output showed TMP105 is a supported device if attached to a valid I²C bus:
```
name "tmp105", bus i2c-bus
...
```

- This confirmed that QEMU knows TMP105 exists.
- But TMP105 requires a bus (i2c-bus).

## 3. Attempted Methods
### 3.1 Using i2c-bus
```
runqemu qemuarm nographic slirp \
  qemuparams="-device i2c-bus,id=i2c0 \
  -device tmp105,bus=i2c0,address=0x48"
```

Error:
```
Failed to run qemu: qemu-system-arm: -device i2c-bus,id=i2c0: 'i2c-bus' is not a valid device model name
```

- Reason: QEMU does not have a standalone i2c-bus device model compiled in.

### 3.2 Using i2c-gpio
```
runqemu qemuarm nographic slirp \
  qemuparams="-device i2c-gpio,bus=virtio-mmio-bus,id=i2c0 \
  -device tmp105,bus=i2c0,address=0x48"
```

Error:
```
Failed to run qemu: qemu-system-arm: -device i2c-gpio,bus=virtio-mmio-bus,id=i2c0: 'i2c-gpio' is not a valid device model name
```

- Reason: The i2c-gpio QEMU device model is not compiled into the current QEMU build.

### 3.3 Using TMP105 directly
```
runqemu qemuarm nographic slirp \
  qemuparams="-device tmp105,address=0x48"
```

Error:
```
No 'i2c-bus' bus found for device 'tmp105'
```

- Reason: TMP105 cannot exist without a bus.

### 3.4 Device Tree Investigation

- Converted QEMU dump DTB to DTS:
```
dtc -I dtb -O dts /tmp/qemu-virt.dtb -o /tmp/qemu-virt.dts
```

- Checked for I²C nodes:
```
cat /tmp/qemu-virt.dts | grep i2c
```

- Result: No I²C bus exists in the virt machine.
- Conclusion: The default virt machine in QEMU does not have any I²C buses enabled, so TMP105 cannot attach.

## 4. Summary of Current Status
```
Attempt	Command	Result	Reason
i2c-bus	-device i2c-bus	Failed	i2c-bus not a compiled device
i2c-gpio	-device i2c-gpio	Failed	i2c-gpio not compiled in current QEMU
tmp105 alone	-device tmp105	Failed	No I²C bus exists for it to attach
DTB inspection	dtc -I dtb	No I²C node	virt machine doesn’t provide one
```
- Key takeaway: TMP105 requires an I²C bus, and the current qemuarm image does not provide one, nor does the default QEMU build support the GPIO-based I²C bus we need.

## 5. Next Steps to Enable TMP105 Emulation
### Option 1: Rebuild QEMU with I²C GPIO Support

1. Clone the latest QEMU from upstream
2. Configure QEMU with:
```
./configure --target-list=arm-softmmu --enable-i2c-gpio
make -j$(nproc)
sudo make install
```

3. Test the bus creation:
```
qemu-system-arm -machine virt -device i2c-gpio,bus=virtio-mmio-bus,id=i2c0 \
-device tmp105,bus=i2c0,address=0x48 -nographic
```

4. Once it works, integrate with OpenBMC using runqemu.

### Option 2: Continue Development Without TMP105

- Skip the TMP105 sensor for now.
- Focus on mock D-Bus services, GPIO, LEDs, and virtual sensors.
- Once QEMU has I²C support, TMP105 can be added easily.

### Option 3: Use a Different QEMU Machine

- Some QEMU boards (e.g., vexpress-a9) may have prebuilt I²C buses.
- This requires building a compatible OpenBMC image for that machine.

## 6. References

- QEMU Device Models: https://www.qemu.org/docs/master/system/devices/i2c.html
- OpenBMC QEMU Integration: https://github.com/openbmc/openbmc/blob/master/meta-phosphor/README.md
- Device Tree Compiler (DTC): https://github.com/dtc-linux/dtc

## 7. Conclusion

Currently, TMP105 cannot be emulated in the default OpenBMC QEMU virt machine because:

1. No I²C bus exists in the DTB.
2. Required QEMU I²C devices (i2c-gpio) are not compiled.

The recommended solution is to rebuild QEMU with i2c-gpio support or switch to a machine with prebuilt I²C support. Meanwhile, development can continue using mock services without hardware dependency.
This README can serve as a reference guide for anyone attempting to emulate I²C devices in OpenBMC QEMU.
