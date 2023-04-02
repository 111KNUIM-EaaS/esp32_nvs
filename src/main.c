#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"

esp_err_t init_nvs();
esp_err_t get_data(char **ssid, char **passwd);
esp_err_t set_data(const char *ssid, const char *passwd);

void app_main() {
    init_nvs();
    
    char *ssid = NULL;    
    char *passwd = NULL;

    esp_err_t ret = get_data(&ssid, &passwd);
    if(ret == ESP_OK) {
        ESP_LOGI("main", "SSID: %s", ssid);
        ESP_LOGI("main", "Password: %s", passwd);
    } else {
        ESP_LOGW("main", "Error (%s) reading data from NVS!", esp_err_to_name(ret));
        ret = set_data("ssid", "passwd");
        if(ret != ESP_OK) {
            ESP_LOGW("main", "Error (%s) writing data to NVS!", esp_err_to_name(ret));
        }
    }

}

esp_err_t init_nvs() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    return ret;
}

esp_err_t get_data(char **ssid, char **passwd) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) return err;

    size_t required_size = 0;  // value will default to 0, if not set yet in NVS
    err = nvs_get_str(my_handle, "ssid", NULL, &required_size);
    if (err != ESP_OK) return err;
    if (required_size > 0) {
        *ssid = malloc(required_size);
        err = nvs_get_str(my_handle, "ssid", *ssid, &required_size);
        if (err != ESP_OK) return err;
    }

    required_size = 0;  // value will default to 0, if not set yet in NVS
    err = nvs_get_str(my_handle, "passwd", NULL, &required_size);
    if (err != ESP_OK) return err;
    if (required_size > 0) {
        *passwd = malloc(required_size);
        err = nvs_get_str(my_handle, "passwd", *passwd, &required_size);
        if (err != ESP_OK) return err;
    }

    // Close
    nvs_close(my_handle);
    return ESP_OK;
}

esp_err_t set_data(const char *ssid, const char *passwd) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) return err;

    err = nvs_set_str(my_handle, "ssid", ssid);
    if (err != ESP_OK) return err;

    err = nvs_set_str(my_handle, "passwd", passwd);
    if (err != ESP_OK) return err;

    // Commit
    err = nvs_commit(my_handle);
    if (err != ESP_OK) return err;

    // Close
    nvs_close(my_handle);
    return ESP_OK;
}