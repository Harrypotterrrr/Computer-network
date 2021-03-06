# CentOS深度配置
- [CentOS深度配置](#centos%E6%B7%B1%E5%BA%A6%E9%85%8D%E7%BD%AE)
    - [Linux通过samba共享目录及文件](#linux%E9%80%9A%E8%BF%87samba%E5%85%B1%E4%BA%AB%E7%9B%AE%E5%BD%95%E5%8F%8A%E6%96%87%E4%BB%B6)
    - [Windows中盘符进阶](#windows%E4%B8%AD%E7%9B%98%E7%AC%A6%E8%BF%9B%E9%98%B6)
    - [在Linux中新增一个网卡](#%E5%9C%A8linux%E4%B8%AD%E6%96%B0%E5%A2%9E%E4%B8%80%E4%B8%AA%E7%BD%91%E5%8D%A1)
    - [在Linux中新增一个硬盘](#%E5%9C%A8linux%E4%B8%AD%E6%96%B0%E5%A2%9E%E4%B8%80%E4%B8%AA%E7%A1%AC%E7%9B%98)
    - [在Linu中创建普通用户](#%E5%9C%A8linu%E4%B8%AD%E5%88%9B%E5%BB%BA%E6%99%AE%E9%80%9A%E7%94%A8%E6%88%B7)
    - [在Linux中设置账户密码复杂性](#%E5%9C%A8linux%E4%B8%AD%E8%AE%BE%E7%BD%AE%E8%B4%A6%E6%88%B7%E5%AF%86%E7%A0%81%E5%A4%8D%E6%9D%82%E6%80%A7)
    - [普通用户磁盘配额设置](#%E6%99%AE%E9%80%9A%E7%94%A8%E6%88%B7%E7%A3%81%E7%9B%98%E9%85%8D%E9%A2%9D%E8%AE%BE%E7%BD%AE)
    - [利用Shell编写简单脚本](#%E5%88%A9%E7%94%A8shell%E7%BC%96%E5%86%99%E7%AE%80%E5%8D%95%E8%84%9A%E6%9C%AC)
        - [学习Shell](#%E5%AD%A6%E4%B9%A0shell)
        - [完成Shell脚本程序](#%E5%AE%8C%E6%88%90shell%E8%84%9A%E6%9C%AC%E7%A8%8B%E5%BA%8F)

## Linux通过samba共享目录及文件

- 在`192.168.80.230`根目录建立共享文件夹 d1651574
![](./pic/1-1.png)
通过`chmod -R 777 /d1651574` 给该目录设置最高共享权限
用`ls -l -d /d1651574` 查看其权限
![](./pic/1-2.png)

- 在安装前检查samba服务状态，发现没开启
![](./pic/1-3.png)
- 设置开机启动samba服务，并重启
![](./pic/1-4.png)
- 再次检查发现samba服务已开启
![](./pic/1-5.png)
- 在`/etc/samba`目录下修改 `smb.conf` 文件，
```
# 备份smb.conf文件
cp /etc/samb/smb.conf /etc/samb/smb.conf/bak 
# 修改smb.conf文件
vim /etc/samba/smb.conf
```
- 在smb.conf中添加下图所示内容：    
    - 其中`[d1651574]`为要共享文件夹名称
    - `browseable`为可读性
    - `writable`为可写性
    - `valid users` 为可通过samba该共享文件夹的共享用户，`haolin` 为下步骤所设置

![](./pic/1-6.png)

- 使用SMB服务的`smbpasswd`命令给系统用户设置SMB密码
```
# 添加一个叫 haolin 的系统用户
useradd haolin
# 给haolin 设置SMB密码
smbpasswd -a haolin
```
![](./pic/1-7.png)

- 通过`This PC`地址栏(Windows)访问192.168.80.230(Linux虚拟机)
![](./pic/1-8.png)
- 双击进入发现可以访问
![](./pic/1-9.png)
- 文件夹也可以打开，发现为空文件夹
![](./pic/1-10.png)

- Windows上建立映射盘符，如下图操作
![img-w50](./pic/1-11.png)
![](./pic/1-12.png)
- 可以看到映射盘符对应的虚拟机上的文件夹以及文件夹内容
![](./pic/1-13.png)
![](./pic/1-14.png)

- 在映射盘中分别新建`harry.txt` 与 `HARRY.txt`发现冲突，说明Windows中映射Linux的盘符对大小写不敏感
![](./pic/1-15.png)
- 文件夹的结果也能显示对大小写不敏感
![](./pic/1-16.png)

- 设置字符可以显示中文，在`/etc/samb/smb.conf`中的`global`中修改如下
![](./pic/1-17.png)

观察在目录下创建`贾昊霖.txt` 通过`ls`查看
![](./pic/1-18.png)
![](./pic/1-19.png)

- 关闭虚拟机以后，选择克隆虚拟机
![](./pic/1-20.png)
- 我选择的是创建完整克隆
![](./pic/1-21.png)
- 进入修改网络配置
```
vim /etc/sysconfig/network-service/ipcfg-ens32
```
![](./pic/1-22.png)
- 修改ip为`192.168.80.231`
![](./pic/1-23.png)

    并观察修改成功
    ![](./pic/1-24.png)

- 进入`192.168.80.231` 虚拟机，通过samba客户端服务指令，访问目标地址的共享文件夹
```
smbclient -U haolin //192.168.80.230/d1651574
```
    - 其中 haolin 为该地址的samba用户名称
    - 后面为地址
    - 观察成功访问!
![](./pic/1-25.png)

- 为了使只有Windows可以访问，而某些ip端访问不了，在`/etc/samba/smb.conf`文件`global`段添加如下内容
![](./pic/1-26.png)
- 再次通过samba-client访问，发现访问失败
![](./pic/1-27.png)
- 而Windows端可以正常访问
![](./pic/1-28.png)
- 且有修改权限
![](./pic/1-29.png)


## Windows中盘符进阶

- 现在Windows中新建共享文件夹
![](./pic/2-1.png)
- 设置其共享属性，可看到其共享权限级别(permission level: Read/Write)
![](./pic/2-3.png)
- 设置成功
![](./pic/2-4.png)


- 查看Windows字符编码
![](./pic/2-5.png)
为936,即gbk编码
- 在CentOS上挂载该目录
```
mount -t cifs //192.168.80.1/w1651574 /mnt/1651574 -o username=JiaHL,iocharset=cp936,vers=2.0
```
![](./pic/2-9.png)
观察`w1651574`文件中的文件，正常

- 在Windows下建立文件
![](./pic/2-6.png)
- 在CentOS上观察，并查看其其权限，正常
![](./pic/2-8.png)  

- 通过`umount /mnt/w1651574`取消映射需要保证当前没有运行挂载盘中的程序
![](./pic/2-7.png)

- 通过简单的测试，发现大小写均不敏感
![](./pic/2-10.png)
![](./pic/2-11.png)

- 先取消映射，在`/mnt/w1651574`目录下保留文件，删除Windows目录中的文件，再映射观察
![](./pic/2-12.png)
上图显示了取消映射后再创建文件夹以及文件
![](./pic/2-13.png)
上图显示了重新建立挂在后，发现Windows文件夹中没有上述的文件与文件夹，但在Linux下仍然保留

- 使重启后开机自动挂载，在`/etc/fstab`目录下加入
```
//192.168.80.1/w1651574 /mnt/w1651574 cifs defaults.username=JiaHL,password=xxxxxx,iocharset=cp396,vers=2.0 0 0
```
![](./pic/2-14.png)
- 检验reboot开机后,自动挂载成功
![](./pic/2-15.png)


## 在Linux中新增一个网卡
- 关闭虚拟机后，在VMware中新增一张网卡，如下图所示
![](./pic/3-1.png)
- 通过`ifconfig`指令观察增设的显卡
![](./pic/3-2.png)
- 进入 `/etc/sysconfig/network-script` 目录，copy一份`ifcfg-ens32` 到`ifcfg-ens34`
![](./pic/3-3.png)
- 修改`ifcfg-ens34`,如下图，将新网卡的ip修改为`192.168.81.1`
![](./pic/3-4.png)
其中UUID是用UUID Generator 生成：![http://www.uuid.online/](http://www.uuid.online/)
- 分别`ping 192.168.80.1` 和 `ping 192.168.81.1`，观察结果，发现成功
![](./pic/3-5.png)


## 在Linux中新增一个硬盘
- 通过`fdisk -l`指令观察目前硬盘，可看到有一个硬盘 `sda`
![](./pic/4-1.png)
- 如下图所示，为虚拟机添加新虚拟硬盘，
![](./pic/4-2.png)
![](./pic/4-3.png)
![](./pic/4-4.png)
![](./pic/4-5.png)
![](./pic/4-6.png)
![](./pic/4-7.png)
![](./pic/4-8.png)
- 再次通过`fdisk -l`指令观察目前硬盘，可看到有`sda`,`sdb`两个硬盘
![](./pic/4-9.png)
- 通过`lsblk`命令列出可用设备信息
![](./pic/4-10.png)
- 通过`fdisk /etc/sdb` 对新硬盘进行划分
![](./pic/4-11.png)

    - 命令输入`n`进行分区操作
    - 从`p` 主分区 与 `e` 扩展分区中选择，我选择扩展分区选项
    - 分区好选择1，起始扇区以及结束扇区保持默认(即选择了最大扇区，将)，
    - 以下为分区规则 
  
            1.一块磁盘上主分区和扩展分区的个数总和不能超过4个

            2.一块磁盘上最多只能有1个扩展分区

            3.扩展分区中可以创建逻辑分区，个数限制视磁盘类型而定
![](./pic/4-12.png)
- 对该拓展分区再分成两个逻辑分区，分区号分别为`5`,`6`，起始结束扇区区间为start~20000000与20000000~end
![](./pic/4-13.png)
![](./pic/4-14.png)
- 通过 `p`命令观察已分区的情况，可观察到sdb1为一开始分的扩展分区，sdb5与sdb6为sdb1所分的两个逻辑分区
![](./pic/4-15.png)
- 通过`w`命令保存分区结果
![](./pic/4-16.png)
- 通过`partprobe`将分区情况更新同步至操作系统
![](./pic/4-17.png)
- 对磁盘进行格式化
```
mkfs -t ext4 /dev/sdb5
mkfs -t ext4 /dev/sdb6
```
其中ext4 为Linux下的文件系统，此外还有ex1,ex2,ex3，对应Windows下有FTS32，NTFS
![](./pic/4-18.png)
![](./pic/4-19.png)

- 在目录建立`disk-1651574-1`与`disk-1651574-2`
![](./pic/4-20.png)

- 将分区得到的两个硬盘映射到新建的两个文件夹
![](./pic/4-21.png)
- 通过`df -TH` 观察挂载情况，挂载以及分区容量一切正常
![](./pic/4-22.png)

## 在Linu中创建普通用户

- 创建`stu`组
![](./pic/5-1.png)
- 通过创建用户，然后给已有用户增加工作组
![](./pic/5-2.png)
- 创建用户的同时增加工作组
![](./pic/5-3.png)
- 用`cat /etc/passwd` 指令观察所有用户
![](./pic/5-4.png)
- 用`cat /etc/group` 指令观察所有工作组
![](./pic/5-5.png)

- 为用户设置密码
![](./pic/5-6.png)

- 注意如果直接用`userdel xxxxx`删除用户会在`/home`目录下留下用户目录，再次创建相同用户会显示如下提示
![](./pic/5-7.png)
- 因此删除用户选择`uerdel -rf xxxxxx`指令
![](./pic/5-8.png)

- 锁定、解锁用户，使用户无法登陆
![](./pic/5-9.png)

- 通过`who`指令观察在线用户(分别在vmware以及secureCRT上登陆root与u1651574)
![](./pic/5-10.png)

- 通过修改`/etc/sudoers`文件，将用户权限提升为和root相同
![](./pic/5-11.png)
- 登陆赋予权限的`u1651574`执行管理员命令，如useradd，并在命令前增加`sudo`
![](./pic/5-12.png)
- 当将`/etc/sudoers`文件最终u1651574权限删除，再执行管理员的命令
![](./pic/5-13.png)


## 在Linux中设置账户密码复杂性

- 修改`/etc/pam.d/system-auth`文件，如下图修改
![](./pic/6-1.png)
    - 参数分别为:
    - retry:允许尝试次数
    - difok:与原密码最少不同字符数
    - minlen:最少密码长度
    - ucredit:最少大写字母数
    - lcredit:最少小写字母数
    - dcredit:最少数字数
    - ocredit:最少其他字符数
- 通过`chage -l xxxxx`命令，查看用户及其密码信息
![](./pic/6-2.png)
其他`chage`指令详解可参照:![https://blog.csdn.net/Jason_Deng_2017/article/details/78181153](https://blog.csdn.net/Jason_Deng_2017/article/details/78181153)
- 修改`/etc/pam.d/system-auth`文件，加入`remember = 3`，使新密码不能与不能与之前3次相同
![](./pic/6-3.png)
- 通过`chage -d 0 u1651574` 设定用户登陆后强制修改密码
![](./pic/6-4.png)
![](./pic/6-5.png)
- 通过`chage -M 90 u1651574` 设置u1651574账户在90天后强制修改密码
![](./pic/6-6.png)
经观察此时间与系统时间无关


## 普通用户磁盘配额设置

- 观察开机后的挂载情况，无自动挂载sdb5,sdb6
![](./pic/7-1.png)
- 进入`/etc/fstab`，修改如下
![](./pic/7-2.png)
- 通过指令`mount | grep /disk-1651574-1`，观察挂在是否成功,且支持quota功能
![](./pic/7-3.png)
- 分别在`/disk-1651574-1`与`/disk-16515744-2`文件夹中使用`quotacheck -ug .`命令，创建`aquota.user`与`aquota.group`文件
![](./pic/7-4.png)
- 通过`quotaon /dev/sdb5` 与`quotaon /dev/sdb6`启动quota功能

- 配置quota硬盘配额
```
edquota -u u1651574 # 配置用户磁盘配额
edquota -g stu      # 配置组磁盘配额
```
![](./pic/7-5.png)
- 其中参数含义如下
    - blocks :目前使用者在`/dev/xxx`所使用的空间，单位:KB (不要修改)
    - soft   :soft limit 磁盘空间限定值 单位：KB (需要设定)
    - hard   :hard limit 磁盘空间限定值 单位： KB (需要设定)
    - inodes :目前使用者使用掉的inodes,由于不容易使用inode控制,所以不要修改它。
    - soft   :soft limit 文件限制数量  (根据需要修改)
    - hard   :hard limit 文件限制数量   (根据需要修改)
    - soft limit :最低限制容量，在宽限期(grace period)之内，使用容量可以超过soft limit，但必须在宽限期之内将使用容量降低到soft limit以下。
    - hard limit :最终限制容量，如果使用者在宽限期内继续写入数据，到达hard limit将无法再写入。
    - 宽限时间：使用容量超过soft limit，宽限时间自动启动,使用者将容量降低到soft limit以下，宽限时间自动关闭，如果使用者没有在宽限时间内将容量降低到soft limit，那么他将无法再写入数据，即使使用容量没有到达hard limit 

- 进入`u1651574` 账户，并在`/disk-1651574-1`目录下创建`haolin.txt`文件夹,随意加入文字   
![](./pic/7-6.png)
- 通过`quota u1651574`查看其占有的容量
![](./pic/7-7.png)
- 或者通过`repquota -aug`查看更详细的所有用户的占用配额
![](./pic/7-8.png)

- 关闭quota服务，并删除配额文件,如果直接删除`aquota.user`与`aquota.group`文件，会报告如下错误提示
![](./pic/7-9.png)
- 因此应先通过`quotaoff -avgu`指令，关闭quota功能，再删除

## 利用Shell编写简单脚本
### 学习Shell
- [Shell学习网站](http://c.biancheng.net/cpp/shell/)
- 通过 `cat` 命令来查看当前 Linux 系统的可用 Shell
![](./pic/8-1.png)
- 查看当前 Linux 的默认 Shell
![](./pic/8-2.png)
- 常见的shell有 `sh`、`bash`、`csh`、`tcsh`、`ash` 等，`bash`是 Linux 的默认 shell
- shell的简单程序运行
```
# cd 进入在当前目录下，给test.sh权限
chmod +x ./test.sh 
# 运行test.sh
./test.sh
```
但发现会报错
![](./pic/8-3.png)
查明原因发现是在Windows下编辑的程序每一行最后换行符为\n\r，与Linux下\n不同
- 通过`cat -A test.sh`查看文件所有字符
![](./pic/8-4.png)
- 通过 `sed -i 's/\r//' test.sh` 命令将所有的`'\r'` 替换成`''`
![](./pic/8-5.png)
- 再次通过`cat -A test.sh`查看文件所有字符
![](./pic/8-6.png)
发现末尾字符已被替换成$
- 运行成功！
![](./pic/8-7.png)

### 完成Shell脚本程序
```shell
#! /bin/bash
#author: Haolin Jia
#date: 2018-9-24
if [ $# -ne 2 ];then
    echo "命令错误, user_add.sh [file_input] [file_output]"
    exit 1
fi

file_input=`cat < $1`
file_output=$2
echo -n "" > $file_output

IFS_old=$IFS
IFS=$'\n'

for line in $file_input;do
    # echo $line
    IFS=$' '
    ctr=0
    for word in $line;do
        if [ ${word:0:1} = "#" ];then
            continue
        fi
        if [ $ctr -eq 0 ];then
            user=u$word
        fi
        if [ $ctr -eq 1 ];then
            name=$word
        fi
        ctr=$[ $ctr+1 ]
    done

    if [ $ctr -le 2 ];then
        i=`cat /etc/passwd | cut -f1 -d':' | grep -w $user -c`
        if [ $i -ge 1 ];then
            echo ${user} has existed in the system
            userdel -rf $user
            echo ${user} has been deleted and prepared to be rebuilt...
        fi
        adduser -G stu $user 
        chage -d 0 $user

        echo ${user}[${name}] has been built
        pwd=`date +%s%N | md5sum | head -c 10`
        echo "$user $pwd">> $file_output
    fi
    IFS=$IFS_old
done
```
- 当用户不存在系统中时，程序运行如下
![](./pic/8-10.png)
- 当用户存在系统中，程序会检测到用户已存在，删除并重新建立，并给出提示
![](./pic/8-11.png)
- 程序所需的输入文件为`student.conf`与`initpwd.dat`,文件的权限可通过`chmod 777 xxxxx`来增加到最大
![](./pic/8-9.png)
- 可观察到程序运行结束后，用`cut -d : -f 1 /etc/passwd`中账户已创建
![](./pic/8-8.png)
