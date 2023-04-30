#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "esp_wifi.h"

#include "data.h"

#define NAMESPACE_NAME "storage"

void app_main() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
        if (ret != ESP_OK) {
            ESP_LOGW("main", "Error (%s) initializing NVS!", esp_err_to_name(ret));
            return;
        }
    }

    nvs_handle_t my_handle;
    if (nvs_open(NAMESPACE_NAME, NVS_READWRITE, &my_handle) != ESP_OK) {
        ESP_LOGW("main", "Error (%s) opening NVS!", esp_err_to_name(ret));
        return;
    }



    if (nvs_set_str(my_handle, "sta_ssid", STA_SSID) != ESP_OK) {
        ESP_LOGW("main", "Error (%s) writing STA SSID data to NVS!", esp_err_to_name(ret));
        return;
    }

    if (nvs_set_str(my_handle, "sta_passwd", STA_PASSWD) != ESP_OK) {
        ESP_LOGW("main", "Error (%s) writing STA Password data to NVS!", esp_err_to_name(ret));
        return;
    }

    if (nvs_set_str(my_handle, "user_passwd", USER_PASSWD) != ESP_OK) {
        ESP_LOGW("main", "Error (%s) writing User Password data to NVS!", esp_err_to_name(ret));
        return;
    }

    // mac { 0xC8, 0xF0, 0x9E, 0xF3, 0x86, 0x01 }
    uint64_t mac = 0;
    uint8_t my_mac[6] = MAC_ADDR;

    for (int i = 0; i < 6; i++) {
        mac |= (uint64_t)my_mac[i] << (8 * i);
    }

    if (nvs_set_u64(my_handle, "sta_mac", mac) != ESP_OK) {
        ESP_LOGW("main", "Error (%s) writing MAC address data to NVS!", esp_err_to_name(ret));
        return;
    }

    nvs_close(my_handle);

    char *sta_ssid = NULL;    
    char *sta_passwd = NULL;    
    char *user_passwd = NULL;
    my_mac[0] = 0;
    my_mac[1] = 0;
    my_mac[2] = 0;
    my_mac[3] = 0;
    my_mac[4] = 0;

    nvs_handle_t my_handle2;
    if (nvs_open(NAMESPACE_NAME, NVS_READWRITE, &my_handle2) != ESP_OK) {
        ESP_LOGW("main", "Error (%s) opening NVS!", esp_err_to_name(ret));
        return;
    }

    size_t required_size = 0;  // value will default to 0, if not set yet in NVS
    if(nvs_get_str(my_handle2, "sta_ssid", NULL, &required_size) != ESP_OK) {
        ESP_LOGW("main", "Error (%s) reading STA SSID data from NVS!", esp_err_to_name(ret));
        return;
    } else {
        sta_ssid = malloc(required_size);
        if (nvs_get_str(my_handle2, "sta_ssid", sta_ssid, &required_size) != ESP_OK) {
            ESP_LOGW("main", "Error2 (%s) reading STA SSID data from NVS!", esp_err_to_name(ret));
            return;
        }
    }

    required_size = 0;  // value will default to 0, if not set yet in NVS
    if(nvs_get_str(my_handle2, "sta_passwd", NULL, &required_size) != ESP_OK) {
        ESP_LOGW("main", "Error (%s) reading STA Password data from NVS!", esp_err_to_name(ret));
        return;
    } else {
        sta_passwd = malloc(required_size);
        if (nvs_get_str(my_handle2, "sta_passwd", sta_passwd, &required_size) != ESP_OK) {
            ESP_LOGW("main", "Error2 (%s) reading STA Password data from NVS!", esp_err_to_name(ret));
            return;
        }
    }

    required_size = 0;  // value will default to 0, if not set yet in NVS
    if(nvs_get_str(my_handle2, "user_passwd", NULL, &required_size) != ESP_OK) {
        ESP_LOGW("main", "Error (%s) reading User Password data from NVS!", esp_err_to_name(ret));
        return;
    } else {
        user_passwd = malloc(required_size);
        if (nvs_get_str(my_handle2, "user_passwd", user_passwd, &required_size) != ESP_OK) {
            ESP_LOGW("main", "Error2 (%s) reading User Password data from NVS!", esp_err_to_name(ret));
            return;
        }
    }

    mac = 0;
    if (nvs_get_u64(my_handle2, "sta_mac", &mac) != ESP_OK) {
        ESP_LOGW("main", "Error (%s) reading MAC address data from NVS!", esp_err_to_name(ret));
        return;
    } else {
        for (int i = 0; i < 6; i++) {
            my_mac[i] = (uint8_t)(mac >> (8 * i));
        }
    }

    ESP_LOGI("main", "sta_ssid: %s", sta_ssid);
    ESP_LOGI("main", "sta_passwd: %s", sta_passwd);
    ESP_LOGI("main", "user_passwd: %s", user_passwd);
    ESP_LOGI("main", "sta_mac: %02X:%02X:%02X:%02X:%02X:%02X", my_mac[0], my_mac[1], my_mac[2], my_mac[3], my_mac[4], my_mac[5]);

    nvs_close(my_handle2);
}
