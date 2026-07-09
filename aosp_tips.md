# AOSP 开发环境实战总结

> 这是一份"血泪经验"总结，记录了从搭建 AOSP 开发环境到最终稳定运行模拟器的完整过程。


## 一、核心知识点

### 1. `out/` 目录删除后，环境会变

`out/` 不只是存放编译产物的目录，还存储了构建系统的状态（`.ninja_log`、`.ninja_deps` 等）。删除 `out/` 会触发一次"从零开始的完整构建"，可能导致：

- 产物路径变化（例如 `sdk_phone_x86_64` → `emulator_x86_64`）
- 构建系统重新解析所有依赖，耗时增加
- 模拟器找不到镜像，需要手动设置 `ANDROID_PRODUCT_OUT`

**正确做法**：日常开发不要删除 `out/`，用增量编译（`make -j8` 或 `mmm`）。只有在切换分支或遇到编译异常时才用 `make clobber` 清理。

### 2. 加速工具会导致 VMware 网络崩溃

游戏加速器、Watt Toolkit、网络代理等工具会修改系统的网络路由表、LSP 或代理设置，可能导致：

- VMware DHCP Service 被停用
- 虚拟机无法获取 IP（`state DOWN`，`NO-CARRIER`）
- SMB 共享连接中断
- 模拟器启动失败（IP 地址解析问题）

**如果虚拟机突然断网，先检查是否开启了加速工具。**

### 3. 模拟器启动的关键条件

| 条件 | 说明 |
|------|------|
| 编译目标 | `sdk_phone_x86_64` |
| 内核 | `kernel-5.4`（`kernel-5.10` 不支持 lz4 解压） |
| 产物路径 | 必须设置 `ANDROID_PRODUCT_OUT` 指向正确位置 |
| 网络 | 必须正常，否则模拟器的 modem 模块会报错 |

### 4. Android 12 中 `sdk_phone_x86_64` 的产物路径

在 Android 12 中，`lunch sdk_phone_x86_64` 的输出目录不再是 `sdk_phone_x86_64`，而是 `emulator_x86_64`。这是 AOSP 的别名机制，不是错误。启动模拟器时必须将 `ANDROID_PRODUCT_OUT` 设置为 `emulator_x86_64`。


## 二、完整命令速查

### 1. AOSP 编译与启动

```bash
# 进入源码目录并设置环境
cd ~/aosp_local/aosp
source build/envsetup.sh
lunch sdk_phone_x86_64

# 设置产物路径（Android 12 必须）
export ANDROID_PRODUCT_OUT=~/aosp_local/aosp/out/target/product/emulator_x86_64

# 启动模拟器
emulator

# 快速启动脚本
cat > ~/start_emulator.sh << 'SCRIPT'
#!/bin/bash
cd ~/aosp_local/aosp
source build/envsetup.sh
lunch sdk_phone_x86_64
export ANDROID_PRODUCT_OUT=~/aosp_local/aosp/out/target/product/emulator_x86_64
emulator
SCRIPT
chmod +x ~/start_emulator.sh


####################################################################


## 六、VMware 网络修复（完整版）

### 问题现象

- 虚拟机无法上网，`ip addr show` 显示 `ens33: state DOWN`
- 宿主机和虚拟机之间无法 ping 通
- SMB 共享连接失败

### 根因

加速工具（Watt Toolkit / 游戏加速器）占用了 VMware 的虚拟网络资源，导致：
- VMware DHCP Service 被停用
- VMware NAT Service 异常
- 虚拟网卡无法获取 IP

### 完整解决步骤

#### 步骤 1：退出加速工具

完全退出 Watt Toolkit 或任何游戏加速器，确保不在后台运行。

#### 步骤 2：重启 VMware 网络服务（Windows）

以管理员身份打开 CMD，执行：

```cmd
net start "VMware NAT Service"
net start "VMware DHCP Service"

步骤 3：重置 Ubuntu 虚拟网卡
在 Ubuntu 终端执行：

bash
sudo ip link set ens33 down
sudo ip link set ens33 up
sudo dhclient -v
步骤 4：验证网络
bash
ip addr show ens33
# 确认有 inet 192.168.x.x 的 IP

ping -c 4 8.8.8.8
# 确认外网连通

ping -c 4 192.168.0.200
# 确认宿主机连通（替换为你的 Windows IP）
七、SMB 共享挂载（完整版）
问题现象
Ubuntu 无法访问 Windows 共享文件夹

mount 命令报错或超时

前置条件
虚拟机网络正常（能 ping 通宿主机）

Windows 已开启文件夹共享

Windows 防火墙已允许“文件和打印机共享”

完整解决步骤
步骤 1：查看 Windows 共享
bash
smbclient -L //192.168.0.200 -U 你的Windows用户名
系统会提示输入密码（Windows 登录密码），成功后会列出所有共享文件夹。

步骤 2：创建挂载点
bash
sudo mkdir -p /mnt/share
步骤 3：挂载共享文件夹
bash
sudo mount -t cifs //192.168.0.200/share /mnt/share -o username=你的Windows用户名,password=你的Windows密码,vers=3.0
参数说明：

参数	说明
//192.168.0.200/share	Windows 共享路径（替换 IP 和共享名）
/mnt/share	Ubuntu 本地挂载点
username	Windows 登录用户名
password	Windows 登录密码
vers=3.0	使用 SMB 3.0 协议
步骤 4：验证挂载
bash
ls /mnt/share
# 能看到 Windows 共享文件夹的内容

mount | grep cifs
# 显示当前挂载的 CIFS 共享
步骤 5：开机自动挂载（可选）
编辑 /etc/fstab：

bash
sudo nano /etc/fstab
添加一行：

text
//192.168.0.200/share /mnt/share cifs username=你的用户名,password=你的密码,vers=3.0 0 0
八、常用排查命令汇总
网络排查
bash
# 查看网卡状态和 IP
ip addr show

# 查看路由表
ip route

# 测试连通性
ping -c 4 8.8.8.8
ping -c 4 192.168.0.200

# 查看 DNS 配置
cat /etc/resolv.conf

# 查看网络服务状态
systemctl status systemd-networkd
模拟器排查
bash
# 清理残留进程
pkill -9 emulator
pkill -9 qemu
rm -rf ~/.android/avd/*.lock
rm -rf /tmp/android-*

# 检查端口占用
ss -tulpn | grep -E "5554|5555"

# 查看模拟器日志
emulator -verbose -no-window 2>&1 | tee emulator.log
VMware 排查
bash
# Windows 检查 VMware 服务状态（管理员 CMD）
sc query "VMware NAT Service"
sc query "VMware DHCP Service"

# 重启 VMware 服务
net stop "VMware NAT Service" && net start "VMware NAT Service"
net stop "VMware DHCP Service" && net start "VMware DHCP Service"
text

3. **保存文件并提交**：
   ```bash
   cd ~/aosp_learning_config
   git add aosp_tips.md
   git commit -m "补充 VMware 网络修复和 SMB 共享挂载完整步骤"
   git push
这样你的总结文档就包含了完整的网络修复、SMB 挂载和常用排查命令，以后遇到类似问题可以直接翻这份记录。

