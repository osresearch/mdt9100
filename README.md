![MDT9100 running `top`](https://farm5.staticflickr.com/4785/40754929641_e54ecd3eb8_z_d.jpg)

Configuration for the MDT9100 Mobile Data Terminal.
---

## Setting up the CRT driver

This process has been updated for Debian 9.3. The cape manager is going away;
we now rely on uboot to handle device tree overlays.

### Build the device tree overlay file

Our "raw" DTS file uses `#include` directives that rely on the bb.org device tree overlay
repository. If you want to regenerate the DTS file from the "raw" version, you'll need to
retrieve the necessary headers and run the GCC preprocessor.

```console
git clone https://github.com/beagleboard/bb.org-overlays.git
gcc -E -nostdinc -Ibb.org-overlays -undef -D__DTS__ -x assembler-with-cpp \ 
    -o MDT9100-TILCDC-00A0.dts MDT9100-TILCDC-00A0.raw.dts
```

The preprocessed file is already checked in to this repository for your
convenience.

Once the DTS file has been preprocessed, it needs to be compiled by `dtc`.

```console
dtc -O dtb -I dts -o /lib/firmware/MDT9100-TILCDC-00A0.dtbo -@ MDT9100-TILCDC-00A0.dts
```

### Edit `/boot/uEnv.txt`

You'll need to make two changes to the `/boot/uEnv.txt` file:

1. Disable HDMI video by uncommenting the line:

```
disable_uboot_overlay_video=1
```

2. Add the overlay file as a "custom cape":

```
uboot_overlay_addr4=/lib/firmware/MDT9100-TILCDC-00A0.dtbo
```

Save your changes and reboot your Beaglebone.

### Helpful hints

Initially the Beaglebone should bring up a console in the CRT's framebuffer. However,
it will stop sending signals to the CRT quickly; you can disable screen blanking with
the following line:

```shell
echo 0 > /sys/class/graphics/fb0/blank
```

Ideally screen blanking/saving should be down with the monitor enable pin.

For more information: https://trmm.net/MDT9100
