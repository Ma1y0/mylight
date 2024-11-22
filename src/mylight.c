#include <dirent.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PATH 500

// Finds the first available backlight device
char *find_device(void) {
  static char device_path[MAX_PATH];

  // Open the directory which should include the device
  DIR *dir;
  dir = opendir("/sys/class/backlight/");
  if (dir == NULL) {
    perror("Failed to open /sys/class/backlight/");
    return NULL;
  }

  // Iterates over the dir until it finds the first eligible file
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_name[0] != '.') { // Skips . and ..
      snprintf(device_path, MAX_PATH, "/sys/class/backlight/%s", entry->d_name);
      closedir(dir);
      return device_path;
    }
  }

  closedir(dir);

  perror("The directory is empty?");
  return NULL;
}

// Finds the value
int get_value(char *device_path, char *name) {
  if (device_path == NULL) {
    perror("The `device_path` can't be a NULL");
    return -1;
  }

  if (name == NULL) {
    perror("The `name` can't be a NULL");
    return -1;
  }

  char value[20];

  // Create the file path
  char path[MAX_PATH];
  snprintf(path, MAX_PATH, "%s/%s", device_path, name);

  // Open the file
  FILE *fd = fopen(path, "r");
  if (fd == NULL) {
    perror("Can't open the file");
    return -1;
  }

  // Read from the file
  if (fgets(value, sizeof(value), fd) == NULL) {
    perror("Failed to read the file");
    fclose(fd);
    return -2;
  }

  fclose(fd);

  return strtol(value, NULL, 10);
}

// The `new_value` should be between 0-100
int set_brightness(char *device_path, int new_value) {
  // Checks the backlight devicwhile ((opt = getopt(argc, argv, "gs:h")) != -1)
  // {e path
  if (device_path == NULL) {
    return -1;
  }

  // Constructs the path
  char path[MAX_PATH];
  snprintf(path, MAX_PATH, "%s/brightness", device_path);

  // Opens the file
  FILE *fd = fopen(path, "w");
  if (fd == NULL) {
    perror("Failed to open the file");
    return -2;
  }

  // Convert the percentage to a value
  int value = get_value(device_path, "max_brightness") * (new_value / 100.0);

  // Sets the brightness
  if (fprintf(fd, "%d", value) < 0) {
    perror("Failed to write the brightness");
    return -3;
  }

  fclose(fd);
  return 0;
}

int main(int argc, char *argv[]) {
  char *device_path = find_device();
  if (device_path == NULL) {
    return -1;
  }

  // Prints the brightness value when no args are passed
  if (argc == 1) {
    int brightness = get_value(device_path, "brightness");
    float percentage = round(brightness * 100.0 / 255.0);
    printf("Brightness: %.0f\n", percentage);
    return 0;
  }

  // Changes the brightness
  if (argc == 2) {
    int value = strtol(argv[1], NULL, 10);
    if (value < 0 || value > 100) {
      perror("The value should be between 0 and 100");
      return -2;
    }

    if (set_brightness(device_path, value) < 0) {
      return -3;
    }

    int brightness = get_value(device_path, "brightness");
    float percentage =
        round(brightness * 100.0 / get_value(device_path, "max_brightness"));
    printf("Brightness: %.0f\n", percentage);
    return 0;
  }
}
