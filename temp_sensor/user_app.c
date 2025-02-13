#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int fd;
    char buf[32];
    int temperature_raw;

    fd = open("/dev/bmp280", O_RDONLY);
    if (fd < 0) {
        perror("Failed to open /dev/bmp280");
        return -1;
    }

    int bytes_read = read(fd, buf, sizeof(buf));
    if (bytes_read > 0) {
        buf[bytes_read] = '\0'; // Null terminate for safety
        temperature_raw = atoi(buf); // Convert string to integer
        printf("Temperature: %.2f°C\n", temperature_raw / 100.0);
    } else {
        perror("Failed to read temperature");
    }

    close(fd);
    return 0;
}

