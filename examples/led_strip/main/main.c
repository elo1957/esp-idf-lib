#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <led_strip.h>

#define LED_TYPE LED_STRIP_WS2812
#define LED_GPIO 5
#define LED_CHANNEL RMT_CHANNEL_0

static const rgb_t colors[] = {
    { .raw = { 0x0f, 0x0f, 0x0f } },
    { .raw = { 0x00, 0x00, 0x2f } },
    { .raw = { 0x00, 0x2f, 0x00 } },
    { .raw = { 0x2f, 0x00, 0x00 } },
    { .raw = { 0x00, 0x00, 0x00 } },
};

#define COLORS_TOTAL (sizeof(colors) / sizeof(rgb_t))

void test(void *pvParameters)
{
    led_strip_t strip = {
        .type = LED_TYPE,
        .length = CONFIG_LED_STRIP_LEN,
        .gpio = LED_GPIO,
        .channel = LED_CHANNEL,
        .buf = NULL,
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 4, 0)
        .brightness = 255
#endif
    };

    ESP_ERROR_CHECK(led_strip_init(&strip));

    size_t c = 0;
    while (1)
    {
        ESP_ERROR_CHECK(led_strip_fill(&strip, 0, strip.length, colors[c]));
        ESP_ERROR_CHECK(led_strip_flush(&strip));

        vTaskDelay(pdMS_TO_TICKS(1000));

        if (++c >= COLORS_TOTAL)
            c = 0;
    }
}

void app_main()
{
    led_strip_install();
    xTaskCreate(test, "test", configMINIMAL_STACK_SIZE * 5, NULL, 5, NULL);
}

