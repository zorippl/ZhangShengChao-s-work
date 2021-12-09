# 12.6

## daily report

本日内容：
- 从esp-box中demo_main.c中的程序入手研究相关函数的作用、返回值、参数并做解读记录（配合ESP32 s2编程指南）
- 了解lvgl开源轻量级显示框架（盲区）
- 学习I2S音频总线相关内容（盲区）
- 对《ESP32-C3 实战之快速构建物联网项目》阅览&submit comments
- 补充第一份日报笔记中关于Git部分的扩展内容（分支相关）


明日计划：
- 对《ESP32-C3 实战之快速构建物联网项目》阅览&submit comments
## 1.demo.main.c
所有操作都调用`ESP_ERROR_CHECK()`的宏定义展开进行error code的汇报和定位
1. `bsp_board_init()`初始化核心按钮的GPIO设定、外部中断注册、I2C总线
    1. `gpio_install_isr_service(int intr_alloc_flags)`添加ISR处理程序服务，与`gpio_isr_register(……)`互斥，前者需要用add函数配置启用的端口，后者为所有GPIO中断注册一个全局ISR
    >此处参数为0存在疑问，看似是0代表单个，其他数字代表多个
    2. `gpio_isr_handler_add(gpio_num_t gpio_num, gpio_isr_t isr_handler, void *args)`添加相应中断的GPIO，映射ISR服务函数，可选传递ISR的参数
    3. `bsp_i2c_init(i2c_port_t i2c_num, uint32_t clk_speed)`配置IIC通讯，设置IIC端口组，时钟速率
    >用于触控面板、IMU、audio codec
    >><font color=red>此处时钟速率写作40*1000，猜测是为了可读性，类似于40Mhz、khz</font>
2. `bsp_board_power_ctrl(power_module_t module, bool on)`初始化板载电控，参数为供电域和供电状态
    1. 配置电源控制IO
    2. 检查配置是否成功
    3. 对不同的供电域进行差异化处理（switch）
3. `bsp_spiffs_init(char *partition_label, char *mount_point, size_t max_files)`虚拟文件系统初始化
    ><font color=red>这里大致看明白了，不够深入（驱动层）</font>
4. `bsp_lcd_init()`lcd初始化
    1. `bsp_spi_lcd_init(……)`初始化SPI总线，对各个IO进行配置和初始化
    2. 配置LCD旋转和镜像
    3. 配置LCD背光灯IO状态
    4. 创建互斥量接收LCD刷新事件
5. `bsp_tp_init()`touch pannel触控板初始化
   1. `bsp_tp_prob(tp_vendor_t *p_tp_vendor)`检测触控板信息，根据不同型号执行不同初始化func
   2. `bsp_i2c_add_device(i2c_bus_device_handle_t *i2c_device_handle, uint8_t dev_addr)`增加总线上的i2c设备
6. `lv_port_init()`对lvgl进行配置，定义端口，输入和显示设备，并配置所用计时器
7. `ui_main_start()`设置UI
8. `lv_task_handler()`拉起UI处理句柄（基于给lvgl分配的timer）
9. `app_audio_start()`读取wake.wav放入缓存，创建播放任务的信号量并在core 0创建音频任务`audio_task`于`Core 0`，通过`i2s_wrire()`进行send
10. `app_sr_start()`I2S初始化以及Speech Recognition唤醒词侦测响应任务
    1. `sr_init_task()`中`bsp_i2s_init(i2s_port_t i2s_num, uint32_t sample_rate)`对i2s进行初始化
    2. `sr_init_task()`中`bsp_codec_init(audio_hal_iface_samples_t sample_rate)`对音频采集解码根据检测到的不同规则进行初始化
    3. 创建任务`audio_feed_task`于`Core 1`从i2s总线中读取双声道信息存入缓冲区
    4. 创建任务`audio_detect_task`于`Core 1`从缓冲区读取信息进行检测
    5. 创建任务`sr_handler_task`于`Core 0`进行特定语音识别后的响应命令分发执行
    ><font color=red>这里源码注释有问题</font>
11. `app_pwm_led_init()`对led灯进行配置和初始化
12. `app_network_start(const char *host_name)`开启网络功能，创建任务network_task()于`Core 0`
## 2.freeRTOS in espressif
>https://blog.csdn.net/nippon1218/article/details/79039197  关于信号量不错的博客

>https://www.freertos.org/  官网

1. `xSemaphoreCreateBinary()`创建二进制信号量
> 二值信号量通常用于互斥访问或同步，二值信号量和互斥信号量非常类似，但是还是有一 些细微的差别，互斥信号量拥有优先级继承机制，二值信号量没有优先级继承
2. `xSemaphoreGive()`释放信号量
3. `xSemaphoreTake(xSemaphore, xBlockTime)`获得信号量，参数为句柄和阻塞时间
4. `xEventGroupCreate()`事件标志组可以实现多任务间的任务同步，简单来说就是在不同的任务间传递简单的标志位
   


## 3.I2S总线
### 定义和特点
- I2S（inter-IC Sound）Bus，针对数字音频设备之间的音频数据传输制定的总线标准
- Support for Full duplex/Half duplex mode
- Suppor for Master/Slave mode
- 和PCM相比，I2S更适合立体声系统。I2S的变体也支持多通道的时分复用，因此可以支持多声道
### 信号线
1. 串行始终SCLK，也叫位始终（BCLK），对应数字音频的每一位数据，SCLK都有一个脉冲。SCLK的频率=2 * 采样频率 * 采样位深。
2. 帧时钟LRCK（也称WS），用于切换左右声道的数据。LRCK为‘1’表示传输右声道数据，为“0”则是左声道。LRCK的频率 = 采样频率
3. 串行数据（SDATA），就是用二进制补码表示的音频数据，（MSB ---> LSB：数据由高位到低位依次传输）
4. 一般还有MCLK，主时钟。

### 时序＆格式
分为I2S标准格式（飞利浦规定的格式），左对齐（较少使用）和右对齐（日本格式，普通格式），发送和接收端必须使用相同的数据格式。

## 4.两个C中的文件操作函数积累
- fseek 
```bash
int fseek(FILE *stream, long int offset, int whence)
#功能：重定位文件流指针stream从whence的位置偏移offset个字节
```
- ftell 
```bash
long int ftell(FILE *stream)
#功能：返回文件位置指针当前位置相对于文件首的偏移字节数