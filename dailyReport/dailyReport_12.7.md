# 12.7

## daily report

本日内容：
- 对《ESP32-C3 实战之快速构建物联网项目》阅览&submit comments(5~16章)，问题在lab进行comment
- 根据第六章教程和idf中wifi/station例程学习esp32中Wi-Fi组件相关处理,并用本地路由测试



明日计划：
- 上课请假

## station_example_main.c
    
1. `nvs_flash_init()`初始化非易失性存储，如果失败可调用`nvs_flash_erase()`擦除NVS，然后再次初始化。
2. `esp_netif_init()`创建 LwIP 核心任务并初始化与 LwIP 相关的工作。

>esp32刚烧写后用 `idf.py flash monitor` 会出现以下的错误：

    原因：程序需要跑在单核CPU上，却用双核编译

    解决：如果 ESP32-DevKitC板载的是 ESP32-SOLO-1模组，请务必在烧写示例程序之前在menuconfig中使能单核模式（CONFIG_FREERTOS_UNICORE）。

     在Component config --->  FreeRTOS --->Run FreeRTOS only on first core
