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
