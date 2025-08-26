# meta-bhavin – Custom OpenBMC Layer

This layer demonstrates how to extend an existing OpenBMC D-Bus service by
adding a new property to the `xyz.openbmc_project.Logging.Settings` interface.

## Overview

OpenBMC uses YAML interface definitions (`.interface.yaml`) to auto-generate
D-Bus bindings (via sdbusplus). By extending these YAML files, developers can
add new properties, methods, or signals to existing services.

In this example, we added a custom property named **`CustomNote`** alongside the
existing **`QuiesceOnHwError`** property in the Logging Settings interface.

---

## Layer Setup

### 1. Create a Custom Meta-Layer
```bash
bitbake-layers create-layer meta-bhavin
bitbake-layers add-layer ../meta-bhavin
```

Verify it appears:
```bash
bitbake-layers show-layers
```

### 2. Directory Structure
```bash
meta-bhavin
├── conf
│   └── layer.conf
├── recipes-phosphor
│   └── dbus
│       ├── phosphor-dbus-interfaces
│       │   └── xyz/openbmc_project/Logging/Settings.interface.yaml
│       └── phosphor-dbus-interfaces_%.bbappend

```
## Files
phosphor-dbus-interfaces_%.bbappend
```bitbake
# Point FILESEXTRAPATHS so Yocto can find our files
FILESEXTRAPATHS:prepend := "${THISDIR}/phosphor-dbus-interfaces/xyz/openbmc_project/Logging/:"
# Include our custom YAML file in SRC_URI
SRC_URI += "file://Settings.interface.yaml"
# After patch, copy YAML into the source tree
do_patch:append() {
    import os
    import shutil
    
    # Debug: List what's in WORKDIR
    workdir = d.getVar('WORKDIR')
    bb.note("WORKDIR is: " + workdir)
    bb.note("WORKDIR contents: " + str(os.listdir(workdir)))
    
    # Check common locations where files might be extracted
    possible_locations = [
        workdir + '/Settings.interface.yaml',
        workdir + '/sources-unpack/Settings.interface.yaml',
        workdir + '/git/Settings.interface.yaml',
    ]
    
    # Also check if there are any .yaml files anywhere
    for root, dirs, files in os.walk(workdir):
        for file in files:
            if file.endswith('.yaml'):
                bb.note("Found YAML file: " + os.path.join(root, file))
    
    src_file = None
    for loc in possible_locations:
        bb.note("Checking location: " + loc)
        if os.path.exists(loc):
            src_file = loc
            bb.note("Found file at: " + src_file)
            break
    
    if src_file is None:
        bb.error("Could not find Settings.interface.yaml in any expected location")
        return
    
    # Create directory
    os.makedirs(d.getVar('S') + '/yaml/xyz/openbmc_project/Logging', exist_ok=True)
    
    # Copy file
    shutil.copy2(src_file, d.getVar('S') + '/yaml/xyz/openbmc_project/Logging/')
    bb.note("File copied successfully from: " + src_file)
}
```

Settings.interface.yaml
```yaml
description: >
    Implement to control different behaviors on the Logging service. See
    properties below for more information.
properties:
    - name: QuiesceOnHwError
      type: boolean
      default: false
      description: >
          If true, a hardware failure within the system will result in a Quiesce
          of the BMC state. If false, no action is taken.

    - name: CustomNote
      type: string
      default: "BhavinTest"
      description: >
          A test property added by Bhavin to verify custom changes in the
          Logging Settings interface.
```

## Build and Deploy

Rebuild phosphor-dbus-interfaces:
```bash
bitbake -c cleanall phosphor-dbus-interfaces
bitbake phosphor-dbus-interfaces
```
Now check:
```bash
black@bhavin:~/openbmc$ cat build/qemuarm/tmp/work/cortexa15t2hf-neon-openbmc-linux-gnueabi/phosphor-dbus-interfaces/1.0+git/build/gen/xyz/openbmc_project/Logging/Settings/server.hpp | grep Custom
        /** Get value of CustomNote */
        /** Set value of CustomNote with option to skip sending signal */
        /** Set value of CustomNote */
        /** @brief sd-bus callback for get-property 'CustomNote' */
        static int _callback_get_CustomNote(
        /** @brief sd-bus callback for set-property 'CustomNote' */
        static int _callback_set_CustomNote(
black@bhavin:~/openbmc$
```

Rebuild full image:
```bash
bitbake obmc-phosphor-image
```

Boot into QEMU with the new image.

## Validation

Check the new property at runtime:

```bash
busctl introspect xyz.openbmc_project.Settings /xyz/openbmc_project/logging/settings
```

Read property:
```bash
busctl get-property xyz.openbmc_project.Settings \
  /xyz/openbmc_project/logging/settings \
  xyz.openbmc_project.Logging.Settings CustomNote
# → s "BhavinTest"
```

Write property:
```bash
busctl set-property xyz.openbmc_project.Settings \
  /xyz/openbmc_project/logging/settings \
  xyz.openbmc_project.Logging.Settings CustomNote s "HelloWorld"
```
