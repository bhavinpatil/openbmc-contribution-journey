SUMMARY = "Example recipe to demonstrate tasks in bitbake"
HOMEPAGE = "https://github.com/bhavinpatil/openbmc-contribution-journey"
MAINTAINER = "Bhavin Patil <bhavinpatil56@gmail.com>"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"


# ====================================================================================================================
# ====================================================================================================================
# ========================== TASK 1 : Fetch ==========================================================================

# fetch (do_fecth) : fetches the source code e.g. git, local, wget, etc 
SRC_URI = "git://github.com/bhavinpatil/openbmc-contribution-journey.git;protocol=https;branch=main"
SRCREV = "${AUTOREV}"

# AUTOREV - used to get the latest commit hash automaticalluy we can use it for local development purpose but on production level hardcoded hash are prefered.

# black@bhavin:~/openbmc/build/qemuarm$ bitbake -c do_fetch taskexample -v -------- command to run fetch task explicitly

# black@bhavin:~/openbmc/build/qemuarm$ ll downloads/git2/github.com.bhavinpatil.openbmc-contribution-journey.git*





# ====================================================================================================================
# ====================================================================================================================
# ========================== TASK 2 : Unpack =========================================================================
# Unpack (do_unpack) : unpacks the source code downloaded from fetch task into a working directory
#                    :  if we run the unpack task , fetch task gets executed automatically, 

S = "${WORKDIR}/git"
# black@bhavin:~/openbmc/build/qemuarm$ bitbake -e taskexample | grep ^WORKDIR=
# WORKDIR="/home/black/openbmc/build/qemuarm/tmp/work/cortexa15t2hf-neon-openbmc-linux-gnueabi/taskexample/1.0"

# black@bhavin:~/openbmc/build/qemuarm$ bitbake -c do_unpack taskexample -v -------- command to run unpack task explicitly




# ====================================================================================================================
# ====================================================================================================================
# ========================== TASK 3 : Patch =========================================================================
# Patch (do_patch) : locates the patches if available and applies them to the source code 

# locates the patche files and applies them to the source code if available
# if there are no patches available, this task is skipped

# 1. first make the changes to local and commit before pushing prepare the patch file like below
#     black@bhavin:~/openbmc-contribution-journey$ git format-patch -1 HEAD
#     0001-Add-THANKYOU-message-to-taskexample.c-for-enhanced-u.patch

# 2. copy the patch file to recipe folder under files directory then run do_unpack then do_patch, patch files will be unpacked under sources-unpack directory
# NOTE: taskexample-1.0-r0 do_unpack: Unpacking /home/black/openbmc/meta-bhavin/recipes-example
# /task-example/files/0001-Add-THANKYOU-message-to-taskexample.c-for-enhanced-u.patch to 
# /home/black/openbmc/build/qemuarm/tmp/work/cortexa15t2hf-neon-openbmc-linux-gnueabi/taskexample/1.0/sources-unpack/

# SRC_URI:append = " file://0001-Add-THANKYOU-message-to-taskexample.c-for-enhanced-u.patch"
# SRC_URI:append = " file://0001-Updated-taskexample.c-file-to-include-local-header-f.patch"
# SRC_URI:append = " file://0001-Again-Updated-the-taskexample.c-file-for-header-issu.patch"
# SRC_URI:append = " file://0001-Updated-taskexample.c-file.patch"
# SRC_URI:append = " file://*.patch"

# black@bhavin:~/openbmc/build/qemuarm$ bitbake -c do_patch taskexample -v -------- command to run patch task explicitly

# NOTE: taskexample-1.0-r0 do_patch: Applying patch '0001-Add-THANKYOU-message-to-taskexample.c-for-enhanced-u.patch' 
# (../../meta-bhavin/recipes-example/task-example/files/0001-Add-THANKYOU-message-to-taskexample.c-for-enhanced-u.patch)



# ====================================================================================================================
# ====================================================================================================================
# ========================== TASK 4 : Configure =========================================================================
# Configure (do_configure) : configures the source code for compilation


do_configure(){
    echo "#define WELCOME y" > ${S}/cplusplus/meta-bhavin-sample-code/taskexample.h # This '>' will write in the file 
    echo "#define THANKYOU y" >> ${S}/cplusplus/meta-bhavin-sample-code/taskexample.h # This '>>' will append the new content with old in the file
}

# black@bhavin:~/openbmc/build/qemuarm$ bitbake -c do_configure taskexample -v

# + cd /home/black/openbmc/build/qemuarm/tmp/work/cortexa15t2hf-neon-openbmc-linux-gnueabi/taskexample/1.0/git
# + do_configure
# + echo #define WELCOME y

# + echo #define THANKYOU y

# ====================================================================================================================
# ====================================================================================================================
# ========================== TASK 5 : Compile =========================================================================
# Compile (do_compile) : compiles the source code in compilation directory

EXAMPLE_SRC_URI = "${S}/cplusplus/meta-bhavin-sample-code"
# black@bhavin:~/openbmc/build/qemuarm$ bitbake -e taskexample | grep ^EXAMPLE_SRC_URI=
# EXAMPLE_SRC_URI="/home/black/openbmc/build/qemuarm/tmp/work/cortexa15t2hf-neon-openbmc-linux-gnueabi/taskexample/1.0/git/cplusplus/meta-bhavin-sample-code"1

do_compile(){
    ${CC} ${CFLAGS} ${LDFLAGS} ${EXAMPLE_SRC_URI}/taskexample.c -o ${EXAMPLE_SRC_URI}/taskexample
}

# black@bhavin:~/openbmc/build/qemuarm$ bitbake -c do_compile taskexample -v


# ====================================================================================================================
# ====================================================================================================================
# ========================== TASK 6 : Install =========================================================================
# Install (do_install) : copies files from the compilation directory to a temporary directory (D) which will be packaged later.

# install tasks copies files that we have to packaged into the holding area means to the destination dir, from current working dir i.e. ${S} to ${D} 

# ${D} ----
# black@bhavin:~/openbmc/build/qemuarm$ bitbake -e taskexample | grep ^D=
# D="/home/black/openbmc/build/qemuarm/tmp/work/cortexa15t2hf-neon-openbmc-linux-gnueabi/taskexample/1.0/image"

do_install(){
    install -d ${D}${bindir} #-- bindir : /usr/bin/
# at first we dont have that folder but we creates once we run do_install and above line create the folder /usr/bin/ then it will copies the compile file
    install -m 0755 ${EXAMPLE_SRC_URI}/taskexample ${D}${bindir}
}

# we copies the binary or compiled file to the /usr/bin in rootfs of the image we builds

# black@bhavin:~/openbmc/build/qemuarm$ bitbake -c do_install taskexample -v

# after running above command, we get below image dir

# black@bhavin:~/openbmc/build/qemuarm/tmp/work/cortexa15t2hf-neon-openbmc-linux-gnueabi/taskexample/1.0$ ll image/usr/bin/
# total 20
# drwxr-xr-x 2 black black  4096 Sep 18 09:18 ./
# drwxr-xr-x 3 black black  4096 Sep 18 09:18 ../
# -rwxr-xr-x 1 black black 12076 Sep 18 09:18 taskexample*



# ====================================================================================================================
