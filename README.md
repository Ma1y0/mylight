# Install
```sh
sudo make install
```
# Changing brightness without sudo
### 1. Add an udev Rule
Identify your backlight device name by running:

```bash
ls /sys/class/backlight/
```

Replace `<your_device>` below with the name of your backlight device (e.g., `amdgpu_bl2`). Then create a new udev rule file at `/etc/udev/rules.d/99-backlight-brightness.rules` with the following content:

```bash
KERNEL=="<your_device>", SUBSYSTEM=="backlight", RUN+="/bin/chgrp backlight /sys/class/backlight/<your_device>/brightness", RUN+="/bin/chmod 0664 /sys/class/backlight/<your_device>/brightness"
```

### 2. Create the Backlight Group
Run the following command to create a group named `backlight`:

```bash
sudo groupadd backlight
```

### 3. Add Yourself to the Group
Add yourself  to the `backlight` group:

```bash
sudo usermod -aG backlight $USER
```

### 4. Apply Changes
Reboot your computer for the changes to take effect.
