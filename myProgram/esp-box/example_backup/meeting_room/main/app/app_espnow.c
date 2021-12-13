/* ESPNOW Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

/*
   This example shows how to use ESPNOW.
   Prepare two device, one for sending ESPNOW data and another for receiving
   ESPNOW data.
*/
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_now.h"
#include "esp_crc.h"
#include "app_espnow.h"
#include "app_audio.h"
#include "app_sr.h"
#include "ui_main.h"


#define ESPNOW_MAXDELAY 512
#define CONFIG_ESPNOW_PMK "pmk1234567890123"
#define CONFIG_ESPNOW_LMK "lmk1234567890123"
#define CONFIG_ESPNOW_CHANNEL (1)
#define CONFIG_ESPNOW_SEND_COUNT 100
#define CONFIG_ESPNOW_SEND_DELAY 1000
#define CONFIG_ESPNOW_SEND_LEN 15
#define RANGE_MAX -35
#define RANGE_CNT_MAX 6

// #define PRINT_PAYLOAD()

uint8_t CMD_TURN_ON[] = {0x00, 0x00, 0x00, 0x00, 0x01};
uint8_t CMD_TURN_OFF[] = {0x00, 0x00, 0x00, 0x00, 0x02};
uint8_t CMD_COMIN_IN[] = {0x00, 0x00, 0x00, 0x00, 0x10};
uint8_t HEART_BEAT[] = {0x00, 0x00, 0x00, 0x00, 0xff};

uint8_t test_mac[] = {0x7c, 0xdf, 0xa1, 0xe0, 0x42, 0xed}; //7c:df:a1:e0:42:ed
static const char *TAG = "espnow";

static xQueueHandle s_espnow_queue;
static xQueueHandle s_espnow_rssi_queue;
static example_espnow_send_param_t *s_param;

typedef struct {
    uint16_t frame_head;
    uint16_t duration;
    uint8_t destination_address[6];
    uint8_t source_address[6];
    uint8_t broadcast_address[6];
    uint16_t sequence_control;

    uint8_t category_code;
    uint8_t organization_identifier[3]; // 0x18fe34
    uint8_t random_values[4];
    struct {
        uint8_t element_id;                 // 0xdd
        uint8_t lenght;                     //
        uint8_t organization_identifier[3]; // 0x18fe34
        uint8_t type;                       // 4
        uint8_t version;
        uint8_t body[0];
    } vendor_specific_content;
} __attribute__((packed)) espnow_frame_format_t;

static uint8_t s_example_broadcast_mac[ESP_NOW_ETH_ALEN] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
static uint16_t s_example_espnow_seq[EXAMPLE_ESPNOW_DATA_MAX] = { 0, 0 };
static example_espnow_send_param_t *send_param = NULL;

static void app_espnow_deinit(example_espnow_send_param_t *send_param);

static esp_err_t app_espnow_send_cmd(uint8_t *cmd)
{
    example_espnow_data_t *buf = (example_espnow_data_t *)send_param->buffer;

    memcpy(buf->payload, cmd, CONFIG_ESPNOW_SEND_LEN - sizeof(example_espnow_data_t));
    buf->crc = 0;
    buf->crc = esp_crc16_le(UINT16_MAX, (uint8_t const *)buf, send_param->len);
    
    if (esp_now_send(send_param->dest_mac, send_param->buffer, send_param->len) != ESP_OK) {
        ESP_LOGE(TAG, "Send error");
        return ESP_ERR_NOT_SUPPORTED;
    }

    return ESP_OK;
}

// static void app_print_payload(char *str, int len)
// {
//     char to_print[len] = { [0 ... len-1] = '\0'};
//     for(int i = 0; i < len; i++) {
//         sprintf(to_print, str[i]);
//     }
// }

void app_espnow_turn_on_light(bool on)
{
    example_espnow_data_t *buf = (example_espnow_data_t *)send_param->buffer;
    esp_err_t ret = ESP_OK;
    ESP_LOGI(TAG, "Turn on cmd");

    if(on) {
        ret = app_espnow_send_cmd(CMD_TURN_ON);
        ESP_LOGI(TAG, "Turn on cmd");
    } else {
        ret = app_espnow_send_cmd(CMD_TURN_OFF);
        ESP_LOGI(TAG, "Turn off cmd");
    }

    if(ret != ESP_OK) {
        app_espnow_deinit(send_param);
        return ;
        vTaskDelete(NULL);
    }
    
    for(int i = 0; i < CONFIG_ESPNOW_SEND_LEN - sizeof(example_espnow_data_t); i++) {
        printf("%.2x ", buf->payload[i]);
    }
    printf("\n");

    ESP_LOGI(TAG, "Send dest addr");
    for(int i = 0; i < ESP_NOW_ETH_ALEN; i++) {
        printf("%x ", send_param->dest_mac[i]);
    }
    printf("\n");
}

static void app_espnow_recv_cb(const uint8_t *mac_addr, const uint8_t *data, int len)
{
    example_espnow_event_t evt;
    example_espnow_event_recv_cb_t *recv_cb = &evt.info.recv_cb;

    if (mac_addr == NULL || data == NULL || len <= 0) {
        ESP_LOGE(TAG, "Receive cb arg error");
        return;
    }

    evt.id = EXAMPLE_ESPNOW_RECV_CB;
    memcpy(recv_cb->mac_addr, mac_addr, ESP_NOW_ETH_ALEN);
    recv_cb->data = malloc(len);
    if (recv_cb->data == NULL) {
        ESP_LOGE(TAG, "Malloc receive data fail");
        return;
    }
    memcpy(recv_cb->data, data, len);
    recv_cb->data_len = len;

    wifi_promiscuous_pkt_t *promiscuous_pkt = (wifi_promiscuous_pkt_t *)(data - sizeof(wifi_pkt_rx_ctrl_t) - sizeof(espnow_frame_format_t));
    wifi_pkt_rx_ctrl_t *rx_ctrl = &promiscuous_pkt->rx_ctrl;

    xQueueSend(s_espnow_rssi_queue, rx_ctrl, ESPNOW_MAXDELAY);

    if (xQueueSend(s_espnow_queue, &evt, ESPNOW_MAXDELAY) != pdTRUE) {
        ESP_LOGW(TAG, "Send receive queue fail");
        free(recv_cb->data);
    }
}

/* Parse received ESPNOW data. */
static int app_espnow_data_parse(uint8_t *data, uint16_t data_len)
{
    example_espnow_data_t *buf = (example_espnow_data_t *)data;
    uint16_t crc, crc_cal = 0;

    if (data_len < sizeof(example_espnow_data_t)) {
        ESP_LOGE(TAG, "Receive ESPNOW data too short, len:%d", data_len);
        return -1;
    }

    // *state = buf->state;
    // *seq = buf->seq_num;
    // *magic = buf->magic;
    crc = buf->crc;
    buf->crc = 0;
    crc_cal = esp_crc16_le(UINT16_MAX, (uint8_t const *)buf, data_len);

    // ESP_LOGI(TAG, "cal = %d crc = %d type = %d", crc_cal, crc, buf->type);

    if (crc_cal == crc) {
        return buf->type;
    }

    return -1;
}

static void app_espnow_test_range(int rssi)
{
    static int range_cnt = 0;
    if(rssi < RANGE_MAX) {
        range_cnt ++;
        if(range_cnt > RANGE_CNT_MAX) {
            char *warn = "PUT IT DOWNNNNNNN!";
            ESP_LOGE(TAG, "%s", warn);
            sr_anim_set_text(warn);
            app_audio_send_play_signal(WARNING);
        }
    } else if (range_cnt) {
        range_cnt = 0;
        char *ori = "说 Hi,乐鑫";
        // ui_sr_show_cmd_ori();
        sr_anim_set_text(ori);
    }
    return ;
}

/* Prepare ESPNOW data to be sent. */
static void espnow_data_prepare(example_espnow_send_param_t *send_param)
{
    example_espnow_data_t *buf = (example_espnow_data_t *)send_param->buffer;

    assert(send_param->len >= sizeof(example_espnow_data_t));

    buf->type = IS_BROADCAST_ADDR(send_param->dest_mac) ? EXAMPLE_ESPNOW_DATA_BROADCAST : EXAMPLE_ESPNOW_DATA_UNICAST;
    buf->state = send_param->state;
    buf->seq_num = s_example_espnow_seq[buf->type]++;
    buf->crc = 0;
    buf->magic = send_param->magic;
    /* Fill all remaining bytes after the data with random values */
    esp_fill_random(buf->payload, send_param->len - sizeof(example_espnow_data_t));
    buf->crc = esp_crc16_le(UINT16_MAX, (uint8_t const *)buf, send_param->len);
}

static void espnow_task(void *pvParameter)
{
    example_espnow_event_t evt;
    uint16_t recv_seq = 0;
    int ret;
    uint8_t buffer[20];

    vTaskDelay(5000 / portTICK_RATE_MS);
    ESP_LOGI(TAG, "Start sending broadcast data");

    /* Start sending broadcast ESPNOW data. */
    send_param = (example_espnow_send_param_t *)pvParameter;

    while (xQueueReceive(s_espnow_queue, &evt, portMAX_DELAY) == pdTRUE) {
        switch (evt.id) {
            case EXAMPLE_ESPNOW_RECV_CB:
            {
                example_espnow_event_recv_cb_t *recv_cb = &evt.info.recv_cb;
                wifi_pkt_rx_ctrl_t rx_ctrl;
                xQueueReceive(s_espnow_rssi_queue, &rx_ctrl, portMAX_DELAY);
                ESP_LOGI(TAG, "rssi : %d", rx_ctrl.rssi);

                ret = app_espnow_data_parse(recv_cb->data, recv_cb->data_len);
                example_espnow_data_t *test_buf = (example_espnow_data_t *)recv_cb->data;
                int buffer_len = recv_cb->data_len - sizeof(example_espnow_data_t);
                // printf("len  = %d\n", buffer_len);
                memcpy(buffer, test_buf->payload, buffer_len);
                if(!memcmp(recv_cb->mac_addr, test_mac, 5) && !memcmp(buffer, HEART_BEAT, buffer_len)) {
                    app_espnow_test_range(rx_ctrl.rssi);
                    free(recv_cb->data);
                    break;
                }

                free(recv_cb->data);
                if (ret == EXAMPLE_ESPNOW_DATA_BROADCAST) {
                    ESP_LOGI(TAG, "Receive %dth broadcast data from: "MACSTR", len: %d", recv_seq, MAC2STR(recv_cb->mac_addr), recv_cb->data_len);

                    if(!memcmp(buffer, CMD_COMIN_IN, buffer_len)) {
                        app_audio_send_play_signal(COMINGIN);
                    }
                }
                break;
            }
            default:
                ESP_LOGE(TAG, "Callback type error: %d", evt.id);
                break;
        }
    }
}

esp_err_t app_espnow_init(void)
{
    /* WiFi should start before using ESPNOW */
    // Wi-Fi init
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(ESPNOW_WIFI_MODE));
    ESP_ERROR_CHECK(esp_wifi_start());

    example_espnow_send_param_t *send_param;

    s_espnow_queue = xQueueCreate(ESPNOW_QUEUE_SIZE, sizeof(example_espnow_event_t));
    if (s_espnow_queue == NULL) {
        ESP_LOGE(TAG, "Create mutex s_espnow_queue fail");
        return ESP_FAIL;
    }
    s_espnow_rssi_queue = xQueueCreate(ESPNOW_QUEUE_SIZE, sizeof(wifi_pkt_rx_ctrl_t));
    if (s_espnow_rssi_queue == NULL) {
        ESP_LOGE(TAG, "Create mutex s_espnow_rssi_queue fail");
        return ESP_FAIL;
    }

    /* Initialize ESPNOW and register sending and receiving callback function. */
    ESP_ERROR_CHECK( esp_now_init() );
    ESP_ERROR_CHECK( esp_now_register_recv_cb(app_espnow_recv_cb) );

    /* Set primary master key. */
    ESP_ERROR_CHECK( esp_now_set_pmk((uint8_t *)CONFIG_ESPNOW_PMK) );

    /* Add broadcast peer information to peer list. */
    esp_now_peer_info_t *peer = malloc(sizeof(esp_now_peer_info_t));
    if (peer == NULL) {
        ESP_LOGE(TAG, "Malloc peer information fail");
        vQueueDelete(s_espnow_queue);
        vQueueDelete(s_espnow_rssi_queue);
        esp_now_deinit();
        return ESP_FAIL;
    }
    memset(peer, 0, sizeof(esp_now_peer_info_t));
    peer->channel = CONFIG_ESPNOW_CHANNEL;
    peer->ifidx = ESPNOW_WIFI_IF;
    peer->encrypt = false;
    memcpy(peer->peer_addr, s_example_broadcast_mac, ESP_NOW_ETH_ALEN);
    ESP_ERROR_CHECK( esp_now_add_peer(peer) );
    free(peer);

    /* Initialize sending parameters. */
    send_param = malloc(sizeof(example_espnow_send_param_t));
    memset(send_param, 0, sizeof(example_espnow_send_param_t));
    if (send_param == NULL) {
        ESP_LOGE(TAG, "Malloc send parameter fail");
        vQueueDelete(s_espnow_queue);
        vQueueDelete(s_espnow_rssi_queue);
        esp_now_deinit();
        return ESP_FAIL;
    }
    send_param->unicast = false;
    send_param->broadcast = true;
    send_param->state = 0;
    send_param->magic = esp_random();
    send_param->count = CONFIG_ESPNOW_SEND_COUNT;
    send_param->delay = CONFIG_ESPNOW_SEND_DELAY;
    send_param->len = CONFIG_ESPNOW_SEND_LEN;
    send_param->buffer = malloc(CONFIG_ESPNOW_SEND_LEN);
    if (send_param->buffer == NULL) {
        ESP_LOGE(TAG, "Malloc send buffer fail");
        free(send_param);
        vQueueDelete(s_espnow_queue);
        vQueueDelete(s_espnow_rssi_queue);
        esp_now_deinit();
        return ESP_FAIL;
    }
    memcpy(send_param->dest_mac, s_example_broadcast_mac, ESP_NOW_ETH_ALEN);
    espnow_data_prepare(send_param);
    
    s_param = send_param;
    return ESP_OK;
}

static void app_espnow_deinit(example_espnow_send_param_t *send_param)
{
    free(send_param->buffer);
    free(send_param);
    vQueueDelete(s_espnow_queue);
    vQueueDelete(s_espnow_rssi_queue);
    esp_now_deinit();
}

void app_espnow_start(void) 
{
    xTaskCreate(espnow_task, "espnow_task", 2048 * 2, s_param, 4, NULL);
    return;
}
