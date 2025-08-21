# OpenBMC Custom Meta Layer and Recipe Setup

This document records the steps we took to create our **own OpenBMC meta-layer** and add a **custom recipe**.  
Purpose: To learn how OpenBMC services and recipes are added to the Yocto build system.

---

## 1. Create Custom Meta Layer
```bash
cd ~/openbmc
. setup qemuarm
bitbake-layers create-layer ../../meta-bhavin
```
This creates a new layer at ../../meta-bhavin.

Layer structure (auto-generated)
```
meta-bhavin/
├── conf/
│   └── layer.conf
├── COPYING.MIT
└── README
```
## 2. Add the Layer to the Build
```
bitbake-layers add-layer ../../meta-bhavin
```
Check:
```
bitbake-layers show-layers
```
Expected output snippet:
```
layer                 path                          priority
meta                  ../poky/meta                  5
meta-bhavin           ../../meta-bhavin             6
```
## 3. Create Custom Recipe
Inside the new layer, we created:
```
meta-bhavin/recipes-example/bhavin-hello/bhavin-hello_0.1.bb
```
Recipe content
```
SUMMARY = "Bhavin custom hello world script"
DESCRIPTION = "Simple script to verify custom recipe integration"
LICENSE = "MIT"

SRC_URI = "file://bhavin-hello.sh"

S = "${WORKDIR}"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/bhavin-hello.sh ${D}${bindir}/bhavin-hello
}
```
Script file
```
meta-bhavin/recipes-example/bhavin-hello/bhavin-hello.sh:
```
```
#!/bin/sh
echo "Hello from Bhavin's custom recipe inside OpenBMC!"
```
## 4. Build the Recipe
```
bitbake bhavin-hello
```
Expected output snippet:
```
NOTE: Tasks Summary: Attempted 1234 tasks of which 1230 didn't need to be rerun
```
## 5. Verify in Image
Run QEMU:
```
runqemu qemuarm nographic
```
Check inside target:
```
which bhavin-hello
bhavin-hello
```
Expected output:
```
/usr/bin/bhavin-hello
Hello from Bhavin's custom recipe inside OpenBMC!
```
Notes
- Even if output not present, steps above serve as reference log.
- Next steps: refine service to systemd integration.
