// wifi debug
#include <string.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "lwip/sockets.h"
#include "esp_log.h"

#define WIFI_SSID "NINJA_SIMA_ESP32"
#define WIFI_PASS "memristorxd"
#define PORT 3333
#define BUF_SIZE 1024

#define DEBUG_ENABLED 0 // use 1 to enabled debug and 0 to disable debug

#if DEBUG_ENABLED
#define print(...) print_impl(__VA_ARGS__)
void print(const char *fmt, ...);
#else
#define print(...)
#endif

void wifi_init(void);
void tcp_server_task(void *arg);