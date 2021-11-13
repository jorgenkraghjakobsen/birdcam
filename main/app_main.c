
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "esp_camera.h"

#include "ota_server.h"

static const char* TAG = "MAIN";

#include "camera_interface.h" 
#include "wifi_interface.h"

uint8_t *wifi_image_buffer = NULL; 
uint8_t *meta = NULL;

void app_main(void)
{   
    // Init WIFI connect to access point - config using idf.py menuconfig

    wifi_init_sta(); 
    ESP_LOGI(TAG,"WIFI init done");
    
    wifi_init_socket();
    ESP_LOGI(TAG,"WIFI socket connect");
    
    camera_init();
    ESP_LOGI(TAG,"Camera init done");
    for(;;){ 
            
        camera_fb_t *fb = esp_camera_fb_get();
        ESP_LOGI(TAG,"H      : %d ",fb->height);
        ESP_LOGI(TAG,"W      : %d ",fb->width);
        ESP_LOGI(TAG,"len    : %d ",fb->len );
        ESP_LOGI(TAG,"Got JPEG convert to RGB888");
            
        void *ptr = NULL;
        uint32_t ARRAY_LENGTH = fb->width * fb->height * 3;  
        ptr = heap_caps_malloc(ARRAY_LENGTH, MALLOC_CAP_SPIRAM); 
        uint8_t *rgb = (uint8_t *)ptr;
        bool jpeg_converted = fmt2rgb888(fb->buf, fb->len, PIXFORMAT_JPEG, rgb);    
        if (!jpeg_converted) ESP_LOGI(TAG,"-Error converting image to RGB- ");
            
        wifi_send_image(fb->buf,fb->len, NULL, 0);
        // Release camera framebuffer
        esp_camera_fb_return(fb);
        heap_caps_free(ptr);

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}