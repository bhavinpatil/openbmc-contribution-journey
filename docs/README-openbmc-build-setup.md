# OpenBMC Build Environment Setup (Tested on Ubuntu 20.04+)

This guide walks you through setting up a working OpenBMC build environment on Ubuntu (20.04 or 22.04) using QEMU as the target machine. It includes steps to install required Python, GCC, and G++ versions and resolve common errors.

---

## 📦 Prerequisites

### 1. Install System Dependencies

```
sudo apt update
sudo apt install -y \
  git curl wget vim unzip bc \
  bzip2 gawk gcc make \
  libssl-dev libncurses-dev flex \
  libelf-dev python3-pip python3-setuptools \
  libsdl1.2-dev xterm xz-utils zstd \
  cpio rsync
```
🐍 Step 1: Install Python 3.10+
BitBake requires Python 3.9+.

Add PPA and install Python 3.10:
```
sudo add-apt-repository ppa:deadsnakes/ppa -y
sudo apt update
sudo apt install -y python3.10 python3.10-venv python3.10-dev
```
Set Python 3.10 as default:
```
sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.8 1
sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.10 2
sudo update-alternatives --config python3
```

➡️ Select the option for Python 3.10 when prompted.

Confirm:
```
python3 --version
# Should show Python 3.10.x
```

⚙️ Step 2: Upgrade G++ and GCC to Version 10+
BitBake checks for C++20 support, so you need at least G++ 10.

Add toolchain PPA:
```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt update
```

Install GCC and G++ 10:
```
sudo apt install -y gcc-10 g++-10
```

Set GCC/G++ 10 as default:
```
# Set G++
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 100
sudo update-alternatives --config g++

# Set GCC
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 100
sudo update-alternatives --config gcc
```

Confirm versions:
```
g++ --version
gcc --version
# Both should be 10.x (e.g. 10.5.0)
```

📥 Step 3: Clone OpenBMC
```
git clone https://github.com/openbmc/openbmc.git
cd openbmc
```

⚙️ Step 4: Initialize the Build Environment
Setup for qemuarm (no hardware needed):
```
source setup qemuarm
```

🟢 You should see:

```
Machine qemuarm found in meta-phosphor
```
This will create the directory:

```
./build/qemuarm/
```

Optional: Check machine config

```
cat build/qemuarm/conf/local.conf | grep MACHINE

# MACHINE ??= "qemuarm"
```

🧱 Step 5: Build the Image

```
cd build/qemuarm
export TEMPLATECONF=meta-phosphor/conf/templates/default
source ../../openbmc-env

bitbake obmc-phosphor-image
```
⏳ This may take 30–90+ minutes depending on system resources.

🏃 Step 6: Run the Image (QEMU)
After the build completes:
```
runqemu qemuarm
```
