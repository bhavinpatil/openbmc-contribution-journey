# OpenBMC Build Notes (qemuarm)

This document tracks key issues and fixes encountered while building **OpenBMC** for the `qemuarm` target.

---

## Build Environment
- Host: Ubuntu 20.04 (x86_64)
- Target: `qemuarm`
- Image: `obmc-phosphor-image`

---

## Issues Faced

### 1. `libtinyxml2-staticdev` QA Error
During the build, the following error occurred:
```
ERROR: libtinyxml2-11.0.0-r0 do_package_qa: QA Issue:
File /usr/lib/libtinyxml2.a in package libtinyxml2-staticdev
contains reference to TMPDIR [buildpaths]
```

This happens because the static library (`.a`) embeds absolute build paths, which Yocto flags during its QA step.

---

## Fixes Applied

1. **Skipped `buildpaths` QA check** for `libtinyxml2` packages by editing  
   `build/conf/local.conf`:

   ```conf
   INSANE_SKIP:pn-libtinyxml2 += "buildpaths"
   INSANE_SKIP:pn-libtinyxml2-staticdev += "buildpaths"
2. Cleaned and rebuilt the recipe:
    ```
    bitbake -c clean libtinyxml2
    bitbake obmc-phosphor-image
    ```
