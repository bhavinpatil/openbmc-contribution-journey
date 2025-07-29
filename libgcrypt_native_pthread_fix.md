# Fixing `pthread_create` Link Error in `libgcrypt-native` during OpenBMC Build

This guide explains how to fix a linker error related to `pthread_create` and `pthread_join` during the build of `libgcrypt-native` in OpenBMC on **Ubuntu Server**.

---

## 🧠 When Does This Error Occur?

You may encounter this error on minimal Ubuntu Server setups when building OpenBMC:

```
undefined reference to `pthread_create`
undefined reference to `pthread_join`
```

The full log may look like:

```
/home/.../ld: t-thread-local.o: in function `main`:
t-thread-local.c:(.text.startup+0x19f): undefined reference to `pthread_create`
t-thread-local.c:(.text.startup+0x1e8): undefined reference to `pthread_join`
collect2: error: ld returned 1 exit status
make[2]: *** [Makefile:885: t-thread-local] Error 1
```

This happens because the native build of `libgcrypt` does not link `-lpthread` explicitly, and some server environments (unlike Ubuntu Desktop) do not implicitly include it.

---

## ✅ Fix Steps

Follow these steps in order to patch and resolve this issue **cleanly**:

---

### 🔁 Step 1: Clean the broken build

```bash
bitbake -c clean libgcrypt-native
```

This ensures you are starting from a clean state.

---

### ✍️ Step 2: Patch the build using `.bbappend`

1. Navigate to your OpenBMC source directory (e.g. `~/openbmc`).

2. Create a `.bbappend` file:

```bash
mkdir -p meta-phosphor/recipes-support/libgcrypt
nano meta-phosphor/recipes-support/libgcrypt/libgcrypt_1.11.1.bbappend
```

3. Paste the following content into the file:

```bitbake
EXTRA_OECONF:append:class-native = " LIBS=-lpthread"
```

✅ This appends `-lpthread` only for the **native build** of `libgcrypt`, fixing the link error.

Save and exit.

---

### 🔃 Step 3: Rebuild

Now build the fixed component first:

```bash
bitbake libgcrypt-native
```

If that succeeds, continue with the full image build:

```bash
bitbake obmc-phosphor-image
```

---

## ✅ Done!

This fix ensures a clean build without modifying upstream recipes and avoids hacks like editing Makefiles directly.

---

## 📌 Notes

- This issue is more common on **Ubuntu Server** because of its minimal dev package set.
- The `.bbappend` approach keeps your changes localized and clean.
- Always clean before applying recipe-level changes: `bitbake -c clean <recipe>`

---

## 📂 File Created: `meta-phosphor/recipes-support/libgcrypt/libgcrypt_1.11.1.bbappend`

This file tells Yocto to link pthreads explicitly for the host toolchain.
