# 11.10
### 一.关于工程的创建

>第一步：创建工程
>>    cd ~/esp
cp -r $IDF_PATH/examples/get-started/hello_world . 
使用这个来copy库中的例程
ESP-IDF 编译系统不支持带有空格的路径。
 
>第二步：查看串口 创建链接
>>ls /dev/tty*
使用两次看多了什么串口
确认串口链接未用
 
>第三步：配置
>>cd ~/esp/hello_world
idf.py set-target esp32s2
idf.py menuconfig
>>运行工程配置文件 menuconfig
 
>第四步：编译
>>idf.py build
>>>报错：ninja: build stopped: subcommand failed.
>>>证书过期失败了
 
>注：环境变量的更改
>>. $HOME/esp/esp-idf/export.sh
cd ~/esp/hello_world
​​​​​​​
idf.py build

>修改path路径
>>lijunru@FA001245:~/esp/hello_world$ export IDF_PATH=~/esp/esp-idf-test/esp-idf/

>>$ echo $IDF_PATH
>>查询我的IDF_PATH路径

>烧录 使用idf.py flash
>>idf.py -p PORT [-b BAUD] flash 
从的

>fullclean 
>> rm-f build

>添加ESP-LOT-SOLUTION环境变量
export IOT_SOLUTION_PATH=$HOME/esp/esp-iot-solution

### git仓库的使用

```bash
git branch          //创建分支命令
git checkout       //切换分支命令
git merge           //合并分支命令
git pull           //拉取代码下来
git branch -a      //查看所有的分支
```

### 关于串口连接
* ls /dev/tty*

* 打开usb串口 sudo chmod 777 /dev/ttyUSB0

**************************
**************************

# 11.11

### 如何创建一个自己的工程
1. 使用idf工具创建例程
idf.py create-project

Q:在vscood里面没法使用 idf.py


2. git 的使用
   * git clone 网址

3. monitor //开启监视器
   *  ctrl + ]  //关闭监视器

### 对于出现 fullclean 问题
* rm -rf build 移除build文件夹 

* 换芯片需要
  * cd ~/esp/esp-idf
  * ./install.sh esp32s2

## 11.16 && 11.17 

### 编写 ESP32-S3-USB-OTG 文档 
* [乐鑫写作规范 Espressif-Manual-of-Style.pdf](https://espressifsystems.sharepoint.com/sites/Documentation/Internal%20Documents/Espressif-Manual-of-Style.pdf)

* 完成

**************************
**************************
# 11.18 && 11。19

### [跑 USB_HOST 示例程序](https://github.com/espressif/esp-iot-solution/tree/usb/add_usb_solutions/examples/usb/host)

1. USB Camerra
```
格式视频捕获：
        宽/高：320/480
        像素格式：'MJPG'
        字段：无
        每行字节数：0
        图片尺寸：614400
        色彩空间：sRGB
        传递函数：默认（映射到 sRGB）
        YCbCr/HSV 编码：默认（映射到 ITU-R 601）
        量化：默认（映射到全范围）
        旗帜
```
2. USB CDC

3. USB_WIFI_TRANS
   * 例程跑通了
   * 文档已经翻译成英语，推到内部仓库了。为internal分支

### [与人脸识别关联起来做个DEMO](https://github.com/espressif/esp-who)

* 
### [四个 example 的 readme 翻译成英文](https://github.com/espressif/esp-iot-solution/tree/usb/add_usb_solutions/examples/usb/host)
 * [翻译网站](https://www.deepl.com/zh/translator)
**四个文档翻译完成，并且推到gitlab**

### 其他：
   * [ehr系统](https://myehr.espressif.cn:8090/)
  
### ROS系统 

启动：roscore

结束：ctrl+c

# 11.23

### 人脸识别+usb摄像头
1.freertos的学习

* 基本语法    
```c
Queues:队列，用来通信的主要方式
xStreamBufferSend():将数据发送到任务的流缓冲区
xStreamBufferSendFromISR():将数据发送到中断服务的流缓冲区
xStreamBufferSend():
xStreamBufferReceive() 用于从任务的流缓冲区读取数据。
xStreamBufferReceiveFromISR() 用于从中断服务例程 (ISR) 的流缓冲区读取数据。
xStreamBufferReceive() 允许指定阻止时间。如果在调用 
xStreamBufferReceive() 时，从流缓冲区读取非零阻止时间但缓冲区为空，则任务将置为“被阻止”状态，直到流缓冲区中有可用的指定数据量，或者阻止时间结束。
```

* Usage And APIs
```c
xQueneCreate(UBaseType_t uxQueueLength,UBaseType_t uxItemsize):创建一个Queue,uxQueueLength:能够存储最大的Ltems的数量。uxltemSize:一个Ltem的大小/bytes。
xQueueSendToBack()/xQueueSend()：向队列中插入数据分两组 API，一组是往队列的尾部插入 git commit -m "11.25 add readme_en for ESP32_S3_USB_OTG "
xQueueSendToFront()：另一组是往队列的头部插入
   xQueue：队列创建时候的句柄，用于表示向哪个队列写入数据；
   pvItemToQueue：放入队列的 Item 的指针；
   xTicksToWait：如果队列为满，则阻塞的最大时间；当被设置为 0 的时候，不阻塞，如果队列为满，则直接返回；如果设置为portMAX_DELAY 的话，意味着如果队列为满，则会挂起这个任务；
xQueueReceive(): 用于从队列中读数据；读出数据并将数据从 Queue 中删除
uxQueueMessagesWaiting():该函数用于查询当前指定的队列中有多少个 Item

```

# 12.1

### 读取摄像头设备的参数
