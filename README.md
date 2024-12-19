# Install
```sh
sudo make install
```
# Changing brightness without sudo
### 1. Add a udev Rule
Create a new udev rule file at `/etc/udev/rules.d/99-backlight-brightness.rules` with the following content:

```bash
KERNEL=="amdgpu_bl2", SUBSYSTEM=="backlight", RUN+="/bin/chgrp backlight /sys/class/backlight/amdgpu_bl2/brightness", RUN+="/bin/chmod 0664 /sys/class/backlight/amdgpu_bl2/brightness"
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
