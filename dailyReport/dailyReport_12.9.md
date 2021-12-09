# 12.9

## daily report
本日内容：
- 《ESP32-C3 实战之快速构建物联网项目》进展沟通会议（11:00-12:00 304 addax）
- ESP RainMaker配置，并用手机控制S2-saola-1板载LED
- 阅读esp32-c3/S3 datasheet中的资源
- esp-box中基于lvgl的界面制作
- esp-box中Speech Cognition相关了解

明日计划：
- 继续修改调试box现有功能

### Rainmaker配置
1. 克隆工程到本地并编译烧录
```shell
git clone --recursive https://github.com/espressif/esp-rainmaker.git
cd /path/to/esp-rainmaker/examples/switch/
idf.py set-target esp32s2
idf.py build
idf.py erase_flash
idf.py flash monitor
```
>遇到问题I (755) esp_claim: Initialising Self Claiming. This may take time.缺少连接到rainmaker的凭据，可以通过设备连接Wi-Fi后运行的`Claiming service`获取
>>解决方法如下：
```shell
//检查安装python3
python3 --version 

//检查安装pip
curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
python3 get-pip.py

//根据需求表自动安装依赖
cd /path/to/esp-rainmaker/cli/
pip3 install -r requirements.txt

//Host Driven Claiming (ESP32)
cd /path/to/esp-rainmaker/cli
./rainmaker.py login
./rainmaker.py claim $ESPPORT
```
2. 安装ios/android ESP rainmaker app
3. 扫描QR码添加设备进行控制

#### API文档
https://docs.espressif.com/projects/esp-rainmaker/en/latest/c-api-reference/index.html

### esp-box中lvgl学习（以出厂demo为例）
<font color=#008000>LVGL（Light and Versatile Graphics Library）是一个免费的开源图形库，提供了创建具有易于使用的图形元素、漂亮的视觉效果和低内存占用的嵌入式 GUI 所需的一切。</font>

出厂demo中起界面函数内容如下：
```c
esp_err_t ui_main_start(void)
{
    if (NULL == lv_scr_act()) {
        ESP_LOGE(TAG, "LVGL not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    ui_mute_init();                     //Mute Button

    ui_sr_anim_init();                  //Speech Cognition UI

    boot_animate_start(lv_scr_act());   //Boot Animation

    ui_clock_update_start();            //Clock Refreshing

    return ESP_OK;
}
```
在主函数while（1），终端方程或操作系统任务中每数毫秒周期性调用lv_task_handler()刷新界面。


### esp-box中Speech Recognition 部分学习(以出厂demo为例)
feed音频数据
`afe_handle->feed() `函数输入到 AFE 中进行处理
fetch音频数据 `用户调用 afe_handle->fetch() `函数可以获取处理完成的单通道音频。

fetch 的数据采样点数目（采样点数据类型为 int16）可以通过 afe_handle->get_fetch_chunksize 获取。
使用 `afe_handle->get_channel_num()` 可以获取需要传入` afe_handle->feed()` 函数的 mic 数据通道数。（不含参考回路通道）

#### API文档
https://docs.lvgl.io/master/index.htmls
