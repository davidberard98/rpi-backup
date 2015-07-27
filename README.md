# Raspberry Pi Backup Device

I have a Garmin watch which has some memory problem that causes all the activities to be wiped occasionally.  On long trips, I don't want to loose weeks of data so this code, if put on a raspberry pi, will back up a garmin (specifically, a Garmin FR10)

## Specifics

Only checkmount.c is needed -- the other things were used for testing.  When compiling, use

```
gcc -lwiringpi checkmount.c -o checkmount
```

This is currently set up for a Garmin FR10 -- it won't work with any other usb storage.  It looks for this device in /dev/disk/by-id/ and will only match a Garmin FR10.  However, by changing this in checkmount.c (I think it's in 2 places -- checking /dev and mounting) it should work for another specific device.

This was written for a raspberry pi A+ (256mb) running Archlinux.  You'll need to find a way to get it connected to the internet and get gcc as well as [install wiringpi](http://wiringpi.com), which requires make to be installed.  The .system file is specific to systemd/systemctl.

The dbrpi.service file assumes that this is all located in /root/rpi-backup/.  Change this if it's in a different location.  The dbrpi.service should be moved to /etc/systemd/system/ and then type

```
systemctl enable dbrpi.service // Auto-start when booting.
systemctl start dbrpi.service // Start this time.
```
