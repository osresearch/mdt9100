Configuration for the MDT9100 Mobile Data Terminal.
---

Based on https://github.com/phooky/retrocon and Mac-SE

Disable the HDMi output by editing `/boot/uEnv.txt` to turn on
the emmc-overlay disabled DTB:

  ##BeagleBone Black: HDMI (Audio/Video) disabled:
  dtb=am335x-boneblack-emmc-overlay.dtb

Then reboot.

Compile the DTS into a DTB (on the BBB):

  dtc \
    -O dtb \
    -I dts \
    -o /lib/firmware/MDT9100-TILCDC-00A0.dtbo \
    -b 0 \
    -@ MDT9100-TILCDC-00A0.dts

Load the DTB file with:

  echo MDT9100-TILCDC | sudo tee /sys/devices/platform/bone_capemgr/slots 

Disable screen blanking:

  echo 0 > /sys/class/graphics/fb0/blank




Pinout
----

 Unknown0 12v  Vsync  Gnd |
 Enable  12V  Hsync  Gnd |

The enable pin needs to be driven to +5v.
current draw off is near zero, 1.1A when the CRT is on

Vsync and Hsync are idle +5V
vblank is 3 lines long

