# ESP32-S3-Cube Meeting Room Demo

This is the meeting room demo for ESP32-S3-Box development board. 

IDF v5.0-dev-19-gfe2c978b33

## How to Use

### Hardware Required

* A ESP32-S3-Box
* An USB cable for power supply and programming
* A 5mm infrared LED (e.g. IR333C) used to transmit encoded IR signals 

example connection:

|        | ESP32-S3-Box |
| :----: | :-----------: |
| IR333C |     IO 41     |

### Build and Flash

Run `idf.py flash -p /dev/ttyACM0 monitor` to build, flash and monitor the project.

(To exit the serial monitor, type `Ctrl-]`.)

## Example Output

Run this example, you will see the following output log:

```
SPIWP:0xee
mode:DIO, clock div:1
load:0x3fcd0108,len:0x51c
load:0x403b6000,len:0x93c
load:0x403ba000,len:0x2c48
entry 0x403b6164
I (847) opi psram: vendor id : 0x0d (AP)
I (847) opi psram: dev id    : 0x02 (generation 3)
I (848) opi psram: density   : 0x03 (64 Mbit)
I (851) opi psram: good-die  : 0x01 (Pass)
I (856) opi psram: Latency   : 0x01 (Fixed)
I (861) opi psram: VCC       : 0x01 (3V)
I (865) opi psram: SRF       : 0x01 (Fast Refresh)
I (871) opi psram: BurstType : 0x01 (Hybrid Wrap)
I (876) opi psram: BurstLen  : 0x01 (32 Byte)
I (881) opi psram: Readlatency  : 0x02 (10 cycles@Fixed)
I (887) opi psram: DriveStrength: 0x00 (1/1)
W (892) PSRAM: DO NOT USE FOR MASS PRODUCTION! Timing parameters will be updated in future IDF version.
I (903) spiram: Found 64MBit SPI RAM device
I (907) spiram: SPI RAM mode: sram 80m
I (912) spiram: PSRAM initialized, cache is in normal (1-core) mode.
I (919) cpu_start: Pro cpu up.
I (922) cpu_start: Starting app cpu, entry point is 0x4037963c
0x4037963c: call_start_cpu1 at /home/lijiaiq/esp/esp-idf-wifi-ok/components/esp_system/port/cpu_start.c:156

I (0) cpu_start: App cpu up.
I (1217) spiram: SPI SRAM memory test OK
I (1226) cpu_start: Pro cpu start user code
I (1226) cpu_start: cpu freq: 240000000
I (1226) cpu_start: Application information:
I (1226) cpu_start: Project name:     factory_demo
I (1227) cpu_start: App version:      5bdc4c87-dirty
I (1227) cpu_start: Compile time:     Nov 24 2021 16:51:33
I (1227) cpu_start: ELF file SHA256:  ec0b3b0c3194862c...
I (1228) cpu_start: ESP-IDF:          v5.0-dev-19-gfe2c978b33-dirty
I (1228) heap_init: Initializing. RAM available for dynamic allocation:
I (1228) heap_init: At 3FCB0AE0 len 0002F520 (189 KiB): D/IRAM
I (1228) heap_init: At 3FCE0000 len 0000EE34 (59 KiB): STACK/DRAM
I (1229) heap_init: At 600FE000 len 00001FF0 (7 KiB): RTCRAM
I (1229) spiram: Adding pool of 8192K of external SPI memory to heap allocator
I (1229) spi_flash: detected chip: gd
I (1230) spi_flash: flash io: qio
I (1230) sleep: Configure to isolate all GPIO pins in sleep state
I (1231) sleep: Enable automatic switching of GPIO sleep configuration
I (1231) cpu_start: Starting scheduler on PRO CPU.
I (0) cpu_start: Starting scheduler on APP CPU.
I (1232) spiram: Reserving pool of 8K of internal memory for DMA/internal allocations
I (1242) gpio: GPIO[1]| InputEn: 1| OutputEn: 0| OpenDrain: 0| Pullup: 1| Pulldown: 0| Intr:3 
I (1252) gpio: GPIO[46]| InputEn: 0| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0 
I (1502) bsp_spiffs: Partition size: total: 3848081, used: 3776546
I (1562) bsp_spiffs: Partition size: total: 956561, used: 169425
I (1562) gpio: GPIO[4]| InputEn: 0| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0 
I (1562) gpio: GPIO[48]| InputEn: 0| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0 
I (1682) gpio: GPIO[45]| InputEn: 0| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0 
I (1682) bsp_tp: Detected touch panel at 0x24. Vendor : Parade Tech
I (1722) gpio: GPIO[3]| InputEn: 1| OutputEn: 0| OpenDrain: 0| Pullup: 1| Pulldown: 0| Intr:0 
I (1722) pp: pp rom version: e7ae62f
I (1722) net80211: net80211 rom version: e7ae62f
I (1732) wifi:wifi driver task: 3fce6a70, prio:23, stack:3584, core=0
I (1732) system_api: Base MAC address is not set
I (1732) system_api: read default base MAC address from EFUSE
I (1732) wifi:wifi firmware version: f84e709
I (1732) wifi:wifi certification version: v7.0
I (1732) wifi:config NVS flash: enabled
I (1732) wifi:config nano formating: disabled
I (1732) wifi:Init data frame dynamic rx buffer num: 4
I (1732) wifi:Init management frame dynamic rx buffer num: 4
I (1732) wifi:Init management short buffer num: 32
I (1732) wifi:Init static tx buffer num: 4
I (1732) wifi:Init tx cache buffer num: 16
I (1732) wifi:Init static tx FG buffer num: 2
I (1732) wifi:Init static rx buffer size: 1600
I (1732) wifi:Init static rx buffer num: 4
I (1732) wifi:Init dynamic rx buffer num: 4
I (1732) wifi_init: rx ba win: 2
I (1732) wifi_init: tcpip mbox: 32
I (1732) wifi_init: udp mbox: 6
I (1732) wifi_init: tcp mbox: 6
I (1732) wifi_init: tcp tx win: 5744
I (1732) wifi_init: tcp rx win: 5744
I (1732) wifi_init: tcp mss: 1440
I (1732) phy_init: phy_version 302,cc45c1a,Oct 21 2021,14:55:07
I (1782) wifi:set rx active PTI: 0, rx ack PTI: 0, and default PTI: 0
I (1782) wifi:mode : softAP (7c:df:a1:e0:40:dd)
I (1782) wifi:Total power save buffer number: 2
I (1782) wifi:Init max length of beacon: 752/752
I (1782) wifi:Init max length of beacon: 752/752
I (1782) ESPNOW: espnow [version: 1.0] init
I (2002) I2S: DMA Malloc info, datalen=blocksize=1280, dma_buf_count=6
I (2002) I2S: DMA Malloc info, datalen=blocksize=1280, dma_buf_count=6
I (2002) I2S: I2S0, MCLK output by GPIO2
I (2002) codec: Detected codec at 0x40. Name : ES7210
I (2002) codec: Detected codec at 0x18. Name : ES8311
I (2002) ES7210: ES7210 in Slave mode
I (2012) ES7210: Enable ES7210_INPUT_MIC1
I (2012) ES7210: Enable ES7210_INPUT_MIC2
I (2012) ES7210: Enable ES7210_INPUT_MIC3
I (2012) ES7210: Enable ES7210_INPUT_MIC4
I (2022) ES7210: The ES7210_CLOCK_OFF_REG01 value before stop is 0
I (2022) ES7210: Enable ES7210_INPUT_MIC1
I (2022) ES7210: Enable ES7210_INPUT_MIC2
I (2022) ES7210: Enable ES7210_INPUT_MIC3
I (2022) ES7210: Enable ES7210_INPUT_MIC4
I (2032) DRV8311: ES8311 in Slave mode
model_name: hilexin7q8 model_data: /srmodel/hilexin7q8/wn7q8_data
accelate model: 1
0x3d859f3c
MC Quantized-8 wakeNet7: wakeNet7Q8_v2_hilexin_5_0.97_0.90, mode:2, p:3, (Oct 28 2021 17:15:22)
Initial TWO-MIC auido front-end for speech recognition, mode:0, (Oct 22 2021 10:56:29)
model_name: mn3cn model_data: /srmodel/mn3cn/mn3cn_data
layer0 6 26
layer1 8 64
layer2 6 96
layer3 6 128
layer4 4 264
0x3da2499c
SINGLE_RECOGNITION: V3.0 CN; core: 0; (Oct 28 2021 17:15:23)
I (13182) MN: ---------------------SPEECH COMMANDS---------------------
I (13182) MN: Command ID0, phrase 0: xiang shang
I (13182) MN: Command ID1, phrase 1: xiang xia
I (13182) MN: Command ID2, phrase 2: xiang zuo
I (13182) MN: Command ID3, phrase 3: xiang you
I (13182) MN: Command ID4, phrase 4: que ding
I (13182) MN: Command ID5, phrase 5: kai ji
I (13182) MN: Command ID6, phrase 6: zeng da yin liang
I (13182) MN: Command ID7, phrase 7: jian xiao yin liang
I (13182) MN: Command ID8, phrase 8: da kai dian deng
I (13182) MN: Command ID9, phrase 9: guan bi dian deng
I (13192) MN: ---------------------------------------------------------

I (13192) app_sr: Audio detection start. Reset memories :
         Biggest    Free   Total
Internal    32768   48419  271171
SPI RAM   2097152 3996791 8388607

```

## Function Introduction

1. *espnow_cube_test*  sends a espnow message to simulate that csi detects someone coming into the meeting room. When ESP-Box receives this message, it starts to play audio.
   
2. *espnow_cube_freq* sends espnow messages frequently, ESP-Box receives these messages and estimates the distance between them. When the distance is over the threshold, it starts to play warning audio.
   
3. In the control interface, when you press the light button, it will send a espnow message to open the light in the meeting room. (fake message, just to simulate the situation)
   
4. Long press the media button, the remote interface appears. It uses IR to control the projector. (this is true, but just for epson)

## Attention

something else to do to build the project:

components/bsp/boards/esp32_s3_box.h
105: change
> ```#define GPIO_RMT_IR         (GPIO_NUM_41)```

components/esp-sr/CMakeLists.txt
30 :

> ```# IF (IDF_VER MATCHES "v4.")```

33 :

> ```# ENDIF (IDF_VER MATCHES "v4.")```


## TODO 

- update lvgl to 8.1
- update UI to BOX master