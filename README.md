完整详细记录
我把从开始到现在的完整过程写进 README.md：

第一步：先更新内容
bash
cd ~/aosp_learning_config
替换 README.md 为完整记录
bash
cat > README.md << 'EOF'
# AOSP 学习之旅 - 完整记录

## 一、环境信息

| 项目 | 详情 |
|------|------|
| 宿主机 | 联想 R9000P（2025款），AMD 处理器，32GB 内存 |
| 虚拟机 | VMware Workstation 16，Ubuntu 22.04 |
| 虚拟机配置 | 12核 CPU，24GB 内存，650GB 虚拟硬盘 |
| 源码版本 | Android 12.1.0_r5 |
| 源码位置 | `~/aosp_local/aosp/` |
| 编译目标 | `sdk_phone_x86_64`（最终成功） |
| 编译时间 | 约 3-4 小时 |


## 二、最终能跑的编译流程

### 步骤 1：清理旧编译产物

```bash
cd ~/aosp_local/aosp
make clobber
步骤 2：设置编译环境
bash
source build/envsetup.sh
lunch sdk_phone_x86_64
步骤 3：开始编译
bash
make -j8
步骤 4：启动模拟器
bash
emulator
步骤 5：验证 ADB 连接
bash
adb devices
# 输出：emulator-5554   device
三、踩过的坑（完整记录）
坑 1：错误使用 aosp_x86_64-eng
现象：

text
init: [libfs_mgr]ReadDefaultFstab(): failed to find device default fstab
init: Failed to mount required partitions early
reboot: Restarting system with command 'bootloader'
原因：aosp_x86_64-eng 没有正确打包 fstab 到 ramdisk。

解决：切换到 sdk_phone_x86_64。

坑 2：kernel-5.10 无法解压 ramdisk
现象：

text
RAMDISK: lz4 image found at block 0
RAMDISK: lz4 decompressor not configured!
Kernel panic - not syncing: Could not decompress initial ramdisk image.
原因：kernel-5.10 不支持 lz4 解压。

解决：使用 kernel-5.4。

坑 3：嵌套虚拟化无法启用
现象：

text
VMware Workstation 在此主机上不支持嵌套虚拟化。
模块"HV"启动失败。
原因：Windows 的 Hyper-V/虚拟机平台功能占用硬件虚拟化资源。

解决：

关闭 Windows 的 Hyper-V 和虚拟机平台功能

执行 bcdedit /set hypervisorlaunchtype off

重启 Windows

VMware 中勾选"虚拟化 Intel VT-x/EPT 或 AMD-V/RVI"

手动编辑 .vmx 文件，添加：

text
vhv.enable = "TRUE"
hypervisor.cpuid.v0 = "FALSE"
坑 4：userdata-qemu.img 缺失
现象：

text
qemu-system-x86_64: Could not open '.../userdata-qemu.img': No such file or directory
解决：

bash
cd out/target/product/sdk_phone_x86_64/
dd if=/dev/zero of=userdata-qemu.img bs=1M count=800
坑 5：AVD 冲突
现象：

text
emulator: ERROR: Running multiple emulators with the same AVD is an experimental feature.
解决：清理残留进程和锁文件

bash
pkill -9 emulator
pkill -9 qemu
rm -f ~/.android/avd/*.lock
rm -rf /tmp/android-*
四、关键结论
项目	结论
编译目标	必须用 sdk_phone_x86_64，不能用 aosp_x86_64-eng
内核版本	必须用 kernel-5.4，不能用 kernel-5.10
嵌套虚拟化	必须启用 KVM（AMD-V/Intel VT-x）
用户数据	userdata-qemu.img 必须存在
五、代码修改记录
本次实验没有修改任何 AOSP 源码。

所有问题都是通过以下方式解决的：

切换编译目标（aosp_x86_64-eng → sdk_phone_x86_64）

切换内核版本（kernel-5.10 → kernel-5.4）

配置嵌套虚拟化（VMware + Windows 设置）

手动创建缺失的镜像文件

六、VMware 配置
文件位置：D:\my-install\vmware-ubuntu22.0\Ubuntu 64 位.vmx

添加以下配置：

text
vhv.enable = "TRUE"
hypervisor.cpuid.v0 = "FALSE"
七、Windows 配置
以管理员身份执行：

powershell
bcdedit /set hypervisorlaunchtype off
关闭 Windows 功能：

Hyper-V

虚拟机平台

Windows 虚拟机监控程序平台

八、快速启动脚本
bash
#!/bin/bash
cd ~/aosp_local/aosp
source build/envsetup.sh
lunch sdk_phone_x86_64
emulator
九、总结
一句话结论：用 sdk_phone_x86_64 编译，用 kernel-5.4 启动，AOSP 原始代码就能跑模拟器。

十、验证命令
bash
# 启动模拟器
emulator

# 查看日志
adb logcat | grep SystemServer

# 进入 shell
adb shell
EOF

text

---

## 第二步：添加并提交

```bash
git add README.md
git commit -m "完整详细记录：编译过程、踩坑记录、解决方案"
第三步：推送到 GitHub
bash
git remote add origin https://github.com/dengchaohui/aosp-learning-journey.git
git branch -M main
git push -u origin main
第四步：查看结果
浏览器打开：
https://github.com/dengchaohui/aosp-learning-journey

执行上面的命令，把输出发给我。

详细记录写到哪里
详细记录已经写到 README.md 文件里了。现在执行以下命令提交并推送：

bash
cd ~/aosp_learning_config
git add README.md
git commit -m "完整详细记录：编译过程、踩坑记录、解决方案"
git remote add origin https://github.com/dengchaohui/aosp-learning-journey.git
git branch -M main
git push -u origin main
执行完后，去 https://github.com/dengchaohui/aosp-learning-journey 就能看到完整的 README.md 了。

# AOSP 学习笔记


更新 README.md，加入走过的弯路
bash
cd ~/aosp_learning_config
bash
cat > README.md << 'EOF'
# AOSP 模拟器编译完整记录

> **最终状态：模拟器成功运行 ✅**
> 
> 编译目标：`sdk_phone_x86_64`
> 
> Android 版本：12.1.0_r5


## 📋 目录

1. [环境信息](#环境信息)
2. [最终成功的编译流程](#最终成功的编译流程)
3. [走过的弯路（完整记录）](#走过的弯路完整记录)
4. [踩过的坑和解决方案](#踩过的坑和解决方案)
5. [关键结论](#关键结论)
6. [VMware 配置](#vmware-配置)
7. [Windows 配置](#windows-配置)
8. [快速启动脚本](#快速启动脚本)


## 一、环境信息

| 项目 | 详情 |
|------|------|
| 宿主机 | 联想 R9000P（2025款），AMD 处理器，32GB 内存 |
| 虚拟机 | VMware Workstation 16，Ubuntu 22.04 |
| 虚拟机配置 | 12核 CPU，24GB 内存，650GB 虚拟硬盘 |
| 源码版本 | Android 12.1.0_r5 |
| 源码位置 | `~/aosp_local/aosp/` |
| 编译目标 | `sdk_phone_x86_64`（最终成功） |
| 编译时间 | 约 3-4 小时 |


## 二、最终成功的编译流程

```bash
cd ~/aosp_local/aosp
make clobber
source build/envsetup.sh
lunch sdk_phone_x86_64
make -j8
emulator
三、走过的弯路（完整记录）
弯路 1：错误地使用 aosp_x86_64-eng
时间：2026-07-06 下午至晚上

一开始我以为 aosp_x86_64-eng 是标准的模拟器编译目标，花了大量时间反复编译这个目标。

结果：编译完成后，模拟器启动报 fstab 找不到，系统反复重启。浪费了约 4-5 小时。

教训：aosp_x86_64-eng 是通用 x86 目标，fstab 打包逻辑与模拟器不匹配。模拟器应该用 sdk_phone_x86_64。

弯路 2：手动修改 fstab
时间：2026-07-06 晚上

看到 fstab 找不到的错误后，我尝试手动解压 ramdisk，添加 fstab 文件，再重新打包。

尝试过的操作：

解压 ramdisk，手动复制 fstab.ranchu 到根目录

重新打包 ramdisk

用新的 ramdisk 启动模拟器

结果：依然报同样的错误，因为问题不在 fstab 文件本身，而是整个编译目标的打包逻辑有问题。

教训：方向错了，再怎么努力也没用。应该先确认编译目标是否正确。

弯路 3：在 kernel-5.10 上浪费大量时间
时间：2026-07-06 晚上

模拟器启动时内核报错：

text
RAMDISK: lz4 image found at block 0
RAMDISK: lz4 decompressor not configured!
Kernel panic - not syncing: Could not decompress initial ramdisk image.
我一开始以为是自己编译配置的问题，反复尝试不同的编译参数。

结果：后来才发现是 kernel-5.10 本身不支持 lz4 解压，换成 kernel-5.4 就解决了。浪费了约 1-2 小时。

教训：遇到内核错误，先尝试切换内核版本。

弯路 4：在嵌套虚拟化上折腾了很久
时间：2026-07-06 下午至晚上

VMware 中启用嵌套虚拟化时，一直报错：

text
VMware Workstation 在此主机上不支持嵌套虚拟化。
模块"HV"启动失败。
尝试过的操作：

手动编辑 .vmx 文件

修改 Windows 功能设置

执行各种 bcdedit 命令

结果：折腾了约 3-4 小时，最终发现需要彻底关闭 Windows 的 Hyper-V 和虚拟机平台功能，重启后才能生效。

教训：

Windows 的虚拟化功能会占用硬件资源，必须先关闭

vhv.enable = "TRUE" 和 hypervisor.cpuid.v0 = "FALSE" 都要加

修改后必须重启 Windows

弯路 5：反复创建 userdata-qemu.img
时间：2026-07-06 晚上

模拟器启动时反复报错找不到 userdata-qemu.img，我每次都用 dd 命令重新创建。

bash
dd if=/dev/zero of=userdata-qemu.img bs=1M count=800
结果：每次重新编译或清理后，这个文件又消失了，需要重新创建。反复创建了约 5-6 次。

教训：

make clobber 会删除所有编译产物，包括 userdata

编译完成后要检查 userdata 是否存在

可以用 -wipe-data 参数让模拟器自动创建

弯路 6：误以为 sdk_phone_x86_64 不存在
时间：2026-07-06 晚上

在 lunch 菜单里没看到 sdk_phone_x86_64，我一度以为这个目标不存在。

结果：后来发现 lunch sdk_phone_x86_64 直接输入就行，不需要在菜单里选。浪费了约 30 分钟。

教训：lunch 命令可以直接指定目标名称，不一定要从菜单选。

弯路 7：在 AVD 冲突上浪费时间
时间：2026-07-06 晚上

模拟器报错：

text
emulator: ERROR: Running multiple emulators with the same AVD is an experimental feature.
我尝试了各种方法：

清理 .android/avd/ 下的锁文件

杀掉所有 qemu 进程

用 -avd none 参数

结果：后来发现只要彻底清理残留进程（pkill -9 emulator 和 pkill -9 qemu），然后正常启动就行。浪费了约 1 小时。

教训：启动前必须先清理残留进程。

弯路 8：总时间浪费统计
弯路	浪费时间
使用错误的编译目标	4-5 小时
手动修改 fstab	1-2 小时
kernel-5.10 问题	1-2 小时
嵌套虚拟化配置	3-4 小时
userdata 反复创建	30 分钟
误以为目标不存在	30 分钟
AVD 冲突	1 小时
总计	约 12-15 小时
四、踩过的坑和解决方案
坑 1：fstab 找不到
现象：

text
init: [libfs_mgr]ReadDefaultFstab(): failed to find device default fstab
init: Failed to mount required partitions early
reboot: Restarting system with command 'bootloader'
原因：aosp_x86_64-eng 没有正确打包 fstab 到 ramdisk。

解决：切换到 sdk_phone_x86_64。

坑 2：kernel-5.10 无法解压 ramdisk
现象：

text
RAMDISK: lz4 image found at block 0
RAMDISK: lz4 decompressor not configured!
Kernel panic - not syncing: Could not decompress initial ramdisk image.
原因：kernel-5.10 不支持 lz4 解压。

解决：使用 kernel-5.4。

坑 3：嵌套虚拟化无法启用
现象：

text
VMware Workstation 在此主机上不支持嵌套虚拟化。
模块"HV"启动失败。
原因：Windows 的 Hyper-V/虚拟机平台功能占用硬件虚拟化资源。

解决：

关闭 Windows 的 Hyper-V 和虚拟机平台功能

执行 bcdedit /set hypervisorlaunchtype off

重启 Windows

VMware 中勾选"虚拟化 Intel VT-x/EPT 或 AMD-V/RVI"

手动编辑 .vmx 文件，添加：

text
vhv.enable = "TRUE"
hypervisor.cpuid.v0 = "FALSE"
坑 4：userdata-qemu.img 缺失
现象：

text
qemu-system-x86_64: Could not open '.../userdata-qemu.img': No such file or directory
解决：

bash
cd out/target/product/sdk_phone_x86_64/
dd if=/dev/zero of=userdata-qemu.img bs=1M count=800
坑 5：AVD 冲突
现象：

text
emulator: ERROR: Running multiple emulators with the same AVD is an experimental feature.
解决：

bash
pkill -9 emulator
pkill -9 qemu
rm -f ~/.android/avd/*.lock
rm -rf /tmp/android-*
五、关键结论
项目	结论
编译目标	必须用 sdk_phone_x86_64，不能用 aosp_x86_64-eng
内核版本	必须用 kernel-5.4，不能用 kernel-5.10
嵌套虚拟化	必须启用 KVM（AMD-V/Intel VT-x）
用户数据	userdata-qemu.img 必须存在
代码修改	不需要修改任何 AOSP 源码
六、VMware 配置
文件位置：D:\my-install\vmware-ubuntu22.0\Ubuntu 64 位.vmx

添加以下配置：

text
vhv.enable = "TRUE"
hypervisor.cpuid.v0 = "FALSE"
七、Windows 配置
以管理员身份执行：

powershell
bcdedit /set hypervisorlaunchtype off
关闭 Windows 功能：

Hyper-V

虚拟机平台

Windows 虚拟机监控程序平台

八、快速启动脚本
bash
#!/bin/bash
cd ~/aosp_local/aosp
source build/envsetup.sh
lunch sdk_phone_x86_64
emulator
九、一句话总结
用 sdk_phone_x86_64 编译，用 kernel-5.4 启动，AOSP 原始代码就能跑模拟器。

前面浪费的 12-15 小时，都是因为用了错误的编译目标（aosp_x86_64-eng）和错误的内核（kernel-5.10）。

十、验证命令
bash
# 启动模拟器
emulator

# 查看日志
adb logcat | grep SystemServer

# 进入 shell
adb shell
EOF

text

---

## 提交并推送

```bash
git add README.md
git commit -m "添加完整记录：包含走过的弯路和浪费的时间"
git push
执行完去 https://github.com/dengchaohui/aosp-learning-journey 查看。


