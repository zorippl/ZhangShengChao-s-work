# 12.10 & 12.13

## daily report
本日内容：
- exp-box 触控驱动代码
- esp-box中lvgl框架学习（以出厂demo和手册demo为例）
- 搭建番茄钟app界面
- 烧录五个例程并进行功能整理

### Pomodoro项目
- 构建番茄钟界面,设置定时arc控件等
- 滚动设置定时选项进行定时
- 到达时间后语音提示
- 触摸后开始休息计时
>明天会附上开源链接和效果图


### esp-box中lvgl学习（以出厂demo为例）
- 得益于QT5.9的经验结合开发app以及前些日子的文档学习，已经对lvgl的各项结构控件有所了解

### esp-box中Speech Recognition 部分学习(以出厂demo为例)
- 梳理语音处理数据流和处理结构
---
### 五个例程功能整理
| 分支路径 | 功能 | 备注 |
| ------ | ------ | ------ |
| feature/add_mp3_Player_demo : mp3_demo | MP3播放器 |  |
| feature/add_esp_box_lite : lite_demo | 蓝牙HID设备接入 |  |
| feature/meeting_room_demo : meeting_room | 会议解决方案 | 待了解 |
| feature/add_cam_receiver : cam_receiver | 图传解决方案 | 没法测 |
| master : factory_demo | 语音识别唤醒/命令执行 |  |

#### MP3播放器
- 支持离线播放预存的.mp3音频文件
- 支持触摸bar调节音量大小、对曲目进行切换倒换 

#### 出厂例程
- 高唤醒率的离线语音唤醒，（改变唤醒词）
- 支持高识别率的离线中英文命令词识别（自定义命令词）
- 端到端一站式接入云平台（rainmaker）
- 支持触控和语音调节LED扩展板状态
- Pmod™ 兼容接口支持多种外设扩展

#### 会议方案
- 长按Media打开播放控制器，支持
- 更好看的四格dev ctrl GUI
- 带有当前fps和CPU使用率刷新显示
- 具体func待定--与刘中伟对接

#### 蓝牙HID设备接入
- 连接蓝牙鼠标进行交互控制

#### 图传方案
- 未测