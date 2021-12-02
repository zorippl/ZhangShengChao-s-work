# 12.2

## 1.git相关学习
### 部署gitlab环境
- 安装SSH:
```bash
sudo apt-get update
sudo apt-get install openssh-server
```

- 注册gitlab账号并login
启用root账户
```bash
sudo passwd root
su      //切换到root
exit    //登出root
```
- 安装postfix用于邮件发送
```bash
sudo apt-get install curl openssh-server ca-certificates postfix
```
- 信任Gitlab的GPC公钥
```bash
curl https://packages.gitlab.com/gpg.key 2> /dev/null | sudo apt-key add - &>/dev/null
```
- 进入root用户新建表，加入一行源
```bash
vim /etc/apt/sources.list.d/gitlab-ce.list
deb https://mirrors.tuna.tsinghua.edu.cn/gitlab-ce/ubuntu xenial main
```
- 安装gitlab-ce
```bash
sudo apt-get update
sudo apt-get install gitlab-ce
```

- 配置GitLab IP地址
```bash
sudo apt install net-tools  //安装网络工具
ifconfig                    //查看本机ip为192.168.1.100
在文本中修改"externval_url"之后的域名，可以是本机IP或指向本机IP的域名（注意要带有“https://”）
sudo gitlab-ctl reconfigure //重新配置gitlab，使得生效
```
>遇到报错，It seems you haven't specified an initial root password while configuring the GitLab instance.进入root下运行成功解决.

- 打开sshd和postfix服务
```bash
service sshd start
service postfix start
```
>为了使 GitLab 社区版的 Web 界面可以通过网络进行访问，我们需要允许 80 端口通过防火墙，这个端口是 GitLab 社区版的默认端口。为此需要运行下面的命令
>>sudo iptables -A INPUT -p tcp -m tcp --dport 80 -j ACCEPT
- 验证运行情况
`sudu gitlab-ctl status`得到以下反馈，服务正常
```bash
run: alertmanager: (pid 29647) 264s; run: log: (pid 29386) 321s
run: gitaly: (pid 28585) 427s; run: log: (pid 27711) 529s
run: gitlab-exporter: (pid 29619) 266s; run: log: (pid 29297) 339s
run: gitlab-workhorse: (pid 28551) 428s; run: log: (pid 28134) 474s
run: grafana: (pid 29666) 263s; run: log: (pid 29542) 282s
run: logrotate: (pid 27628) 542s; run: log: (pid 27636) 541s
run: nginx: (pid 29601) 268s; run: log: (pid 28195) 466s
run: node-exporter: (pid 29611) 267s; run: log: (pid 29286) 343s
run: postgres-exporter: (pid 29658) 264s; run: log: (pid 29434) 313s
run: postgresql: (pid 27852) 524s; run: log: (pid 27865) 523s
run: prometheus: (pid 29628) 265s; run: log: (pid 29362) 325s
run: puma: (pid 29582) 272s; run: log: (pid 28067) 487s
run: redis: (pid 27658) 536s; run: log: (pid 27676) 533s
run: redis-exporter: (pid 29621) 266s; run: log: (pid 29334) 331s
run: sidekiq: (pid 29552) 280s; run: log: (pid 28102) 479s
```
>tips: gitlab暂无访问权限,就此打住
---
### GitHub使用
- 安装SSH keys
```bash
cd ~/.ssh
ls
ssh-keygen -t rsa -C "github对应的邮箱地址"

/*得到如下结果*/
zhangshengchao@FA001237:~/.ssh$ ssh-keygen -t rsa -C "876229130@qq.com"
Generating public/private rsa key pair.
Enter file in which to save the key (/home/zhangshengchao/.ssh/id_rsa): id_rsa
Enter passphrase (empty for no passphrase): 
Enter same passphrase again: 
Your identification has been saved in id_rsa.
Your public key has been saved in id_rsa.pub.
The key fingerprint is:
SHA256:uLYLHsLVSo3UuNKLHplUcMJFwGpgIvpUmeHSFaAlAuE 876229130@qq.com
The key's randomart image is:
+---[RSA 2048]----+
|=+=+B=o.         |
|=ooX++           |
|*Eo.* .          |
|o..= = .         |
|.oo * + S        |
| o.B o .         |
|  B = o          |
| . + + .         |
|  . . o.         |
+----[SHA256]-----+
```
```bash
cat id_rsa.pub                                          //将秘钥cp至个人github账号下添加

zhangshengchao@FA001237:~/.ssh$ ssh -T git@github.com   //访问成功
Hi zorippl! You've successfully authenticated, but GitHub does not provide shell access.
```
## 2.linux基本操作
- Linux思想为万物皆文件的思想，严格通过权限去限制读写访问运行等情况
- 文件的颜色代表了文件的状态&形式
  
### 一些常用指令
```bash
cd                                  //当前目录
cd ..                               //上一级目录
cd ~                                //用户主目录
cd /                                //根目录
pwd                                 //显示当前工作目录
ls [parameter]                      //查看当前目录下内容 -l详细显示 -a包括隐藏
cp [parameter] 源文件/目录 目标目录    //-r 递归copy
rm [parameter] 文件/目录             //-r 递归rm
touch                               //新建一个文件
```

## 3.ESP-IDF环境搭建与测试

1. 首先安装git工具，克隆ESP-IDF到本地 `git clone -b v4.3.1 --recursive https://github.com/espressif/esp-idf.git`
2. 切换到esp-idf目录下运行提供的shell脚本进行各种交叉编译器,工具组件的下载和安装`./install.sh`,其中遇到了一些工具缺失的报错,安装了python python3 python3-pip后得以解决.
3. 将hello_word cp到外部目录后运行脚本为当前目录配置各种工具的PATH环境变量`. $HOME/esp/esp-idf/export.sh`.
4. 将ESP32开发板连接至PC USB,使用指令‘ls /dev/tty*’查看设备连接前后新增的外设得到串口ID为`/dev/ttyUSB0`.
5. 在目标目录下运行脚本`idf.py set-target esp32`设置目标芯片，`idf.py menuconfig`可以启动工程配置窗口，对基本内容进行配置.
>遇到报错,大致意思是读取串口失败,立刻联想到是权限问题,搜索到指令chmod改变串口设备的权限
```bash
chmod [可选项] <mode> <file...>      此处操作/dev/ttyUSB0
-rw------- (600)                    只有拥有者有读写权限。
-rw-r--r-- (644)                    只有拥有者有读写权限；而属组用户和其他用户只有读权限。
-rwx------ (700)                    只有拥有者有读、写、执行权限。
-rwxr-xr-x (755)                    拥有者有读、写、执行权限；而属组用户和其他用户只有读、执行权限。
-rwx--x--x (711)                    拥有者有读、写、执行权限；而属组用户和其他用户只有执行权限。
-rw-rw-rw- (666)                    所有用户都有文件读、写权限。
-rwxrwxrwx (777)                    所有用户都有读、写、执行权限。
```
6. 

//申请了两块板卡