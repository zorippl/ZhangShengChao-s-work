# 12.3

## daily report

本日内容：
- 补充昨日笔记中关于Git部分的扩展内容
- 使用ESP32-DevKitC开发板，基于example点亮了外接RGB模块并上传工程到仓库
- 详细阅读了freertos和build文档章节
- 用`xTaskCreatePinnedToCore(……)`双核双任务实现呼吸灯
- 初步阅读esp-box文档，完成example的build(目前没有s3和box，无法烧录)

明日计划：
- 继续学习freertos,box手册
## 1.点灯
对GPIO口进行操作
1. `gpio_reset_pin(gpio_num_t gpio_num)`引脚重置为默认状态
2. `gpio_set_direction()`设定方向
3. `gpio_set_level()`设定状态
4. `skvTaDelay(1000 / portTICK_PERIOD_MS)`延时1s
>出现报错：自己的工程文件夹在命名的时候有空格，导致脚本中交叉编译链接把空格识别成路径断层，build报错，以后要避免

利用乐鑫魔改的`xTaskCreatePinnedToCore(……)`进行双核任务调度完成呼吸灯，并上传工程到仓库
## 2.esp-box
1. 项目clone与编译
```bash
git clone git@github.com:espressif/esp-box.git  //clone到本地
cd esp-box/idf_patch
python3 apply_patch.py -d ~/esp/esp-idf         //运行脚本打补丁
. /path/to/esp-idf/export.sh
```
>出现后续问题：在这里clone的工程有他人维护的子模块,要在clone后外加参数--recursive，不然子模块为空 
2. 成功使用idf.py命令对esp-box的example进行编译
3. 对本日的project push到私人仓库中
>出现问题：其中esp-box clone自别人的project,内有.git隐藏文件，github会将其视为一个子模块
>>解决方案：cd到目录下`rm -r .git`,然后`git rm --cached [文件名]`删除本地仓库中的内容，再执行`add`,`commit`,`push`
>>>`git config --global core.editor "vim`设置默认用vim打开commit后的描述说明文档

## 3.编码规范
1. 单个源文件中函数和变量声明`static`
2. 公共函数和变量一定加前缀如`esp_func()`
3. 静态变量命名`s_xx`
4. 命名尽量避免不必要的缩写
5. 缩进4空格
6. 函数内部不要空行，函数间空行，函数体最大行长度120字符，函数定义必须有单独大括号
7. 条件和循环字后加Space
8. 二元运算符周围空格，一元运算符不用空格 乘法除法可以省略
9. 不用的代码不要注释掉，直接删除，临时禁用某些功能打算恢复的在相邻行说明
10. `tools/format.sh  xxx.c`可以根据上述要求格式化代码
11. 自定义类型以_t为后缀
12. 枚举应该通过typedef定义并命名空间
13. c++中头文件`.hpp` 源文件`.cpp`,与C作区分
14. 待补充……

## 4.文档规范

安装了astyle 和tree,用于运行自动化代码规范脚本和查看目录树