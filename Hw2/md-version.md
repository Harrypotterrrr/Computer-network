# CentOS�������
- [CentOS�������](#centos%E6%B7%B1%E5%BA%A6%E9%85%8D%E7%BD%AE)
    - [Linuxͨ��samba����Ŀ¼���ļ�](#linux%E9%80%9A%E8%BF%87samba%E5%85%B1%E4%BA%AB%E7%9B%AE%E5%BD%95%E5%8F%8A%E6%96%87%E4%BB%B6)
    - [Windows���̷�����](#windows%E4%B8%AD%E7%9B%98%E7%AC%A6%E8%BF%9B%E9%98%B6)
    - [��Linux������һ������](#%E5%9C%A8linux%E4%B8%AD%E6%96%B0%E5%A2%9E%E4%B8%80%E4%B8%AA%E7%BD%91%E5%8D%A1)
    - [��Linux������һ��Ӳ��](#%E5%9C%A8linux%E4%B8%AD%E6%96%B0%E5%A2%9E%E4%B8%80%E4%B8%AA%E7%A1%AC%E7%9B%98)
    - [��Linu�д�����ͨ�û�](#%E5%9C%A8linu%E4%B8%AD%E5%88%9B%E5%BB%BA%E6%99%AE%E9%80%9A%E7%94%A8%E6%88%B7)
    - [��Linux�������˻����븴����](#%E5%9C%A8linux%E4%B8%AD%E8%AE%BE%E7%BD%AE%E8%B4%A6%E6%88%B7%E5%AF%86%E7%A0%81%E5%A4%8D%E6%9D%82%E6%80%A7)
    - [��ͨ�û������������](#%E6%99%AE%E9%80%9A%E7%94%A8%E6%88%B7%E7%A3%81%E7%9B%98%E9%85%8D%E9%A2%9D%E8%AE%BE%E7%BD%AE)
    - [����Shell��д�򵥽ű�](#%E5%88%A9%E7%94%A8shell%E7%BC%96%E5%86%99%E7%AE%80%E5%8D%95%E8%84%9A%E6%9C%AC)
        - [ѧϰShell](#%E5%AD%A6%E4%B9%A0shell)
        - [���Shell�ű�����](#%E5%AE%8C%E6%88%90shell%E8%84%9A%E6%9C%AC%E7%A8%8B%E5%BA%8F)

## Linuxͨ��samba����Ŀ¼���ļ�

- ��`192.168.80.230`��Ŀ¼���������ļ��� d1651574
![](./pic/1-1.png)
ͨ��`chmod -R 777 /d1651574` ����Ŀ¼������߹���Ȩ��
��`ls -l -d /d1651574` �鿴��Ȩ��
![](./pic/1-2.png)

- �ڰ�װǰ���samba����״̬������û����
![](./pic/1-3.png)
- ���ÿ�������samba���񣬲�����
![](./pic/1-4.png)
- �ٴμ�鷢��samba�����ѿ���
![](./pic/1-5.png)
- ��`/etc/samba`Ŀ¼���޸� `smb.conf` �ļ���
```
# ����smb.conf�ļ�
cp /etc/samb/smb.conf /etc/samb/smb.conf/bak 
# �޸�smb.conf�ļ�
vim /etc/samba/smb.conf
```
- ��smb.conf�������ͼ��ʾ���ݣ�    
    - ����`[d1651574]`ΪҪ�����ļ�������
    - `browseable`Ϊ�ɶ���
    - `writable`Ϊ��д��
    - `valid users` Ϊ��ͨ��samba�ù����ļ��еĹ����û���`haolin` Ϊ�²���������

![](./pic/1-6.png)

- ʹ��SMB�����`smbpasswd`�����ϵͳ�û�����SMB����
```
# ���һ���� haolin ��ϵͳ�û�
useradd haolin
# ��haolin ����SMB����
smbpasswd -a haolin
```
![](./pic/1-7.png)

- ͨ��`This PC`��ַ��(Windows)����192.168.80.230(Linux�����)
![](./pic/1-8.png)
- ˫�����뷢�ֿ��Է���
![](./pic/1-9.png)
- �ļ���Ҳ���Դ򿪣�����Ϊ���ļ���
![](./pic/1-10.png)

- Windows�Ͻ���ӳ���̷�������ͼ����
![img-w50](./pic/1-11.png)
![](./pic/1-12.png)
- ���Կ���ӳ���̷���Ӧ��������ϵ��ļ����Լ��ļ�������
![](./pic/1-13.png)
![](./pic/1-14.png)

- ��ӳ�����зֱ��½�`harry.txt` �� `HARRY.txt`���ֳ�ͻ��˵��Windows��ӳ��Linux���̷��Դ�Сд������
![](./pic/1-15.png)
- �ļ��еĽ��Ҳ����ʾ�Դ�Сд������
![](./pic/1-16.png)

- �����ַ�������ʾ���ģ���`/etc/samb/smb.conf`�е�`global`���޸�����
![](./pic/1-17.png)

�۲���Ŀ¼�´���`�����.txt` ͨ��`ls`�鿴
![](./pic/1-18.png)
![](./pic/1-19.png)

- �ر�������Ժ�ѡ���¡�����
![](./pic/1-20.png)
- ��ѡ����Ǵ���������¡
![](./pic/1-21.png)
- �����޸���������
```
vim /etc/sysconfig/network-service/ipcfg-ens32
```
![](./pic/1-22.png)
- �޸�ipΪ`192.168.80.231`
![](./pic/1-23.png)

    ���۲��޸ĳɹ�
    ![](./pic/1-24.png)

- ����`192.168.80.231` �������ͨ��samba�ͻ��˷���ָ�����Ŀ���ַ�Ĺ����ļ���
```
smbclient -U haolin //192.168.80.230/d1651574
```
    - ���� haolin Ϊ�õ�ַ��samba�û�����
    - ����Ϊ��ַ
    - �۲�ɹ�����!
![](./pic/1-25.png)

- Ϊ��ʹֻ��Windows���Է��ʣ���ĳЩip�˷��ʲ��ˣ���`/etc/samba/smb.conf`�ļ�`global`�������������
![](./pic/1-26.png)
- �ٴ�ͨ��samba-client���ʣ����ַ���ʧ��
![](./pic/1-27.png)
- ��Windows�˿�����������
![](./pic/1-28.png)
- �����޸�Ȩ��
![](./pic/1-29.png)


## Windows���̷�����

- ����Windows���½������ļ���
![](./pic/2-1.png)
- �����乲�����ԣ��ɿ����乲��Ȩ�޼���(permission level: Read/Write)
![](./pic/2-3.png)
- ���óɹ�
![](./pic/2-4.png)


- �鿴Windows�ַ�����
![](./pic/2-5.png)
Ϊ936,��gbk����
- ��CentOS�Ϲ��ظ�Ŀ¼
```
mount -t cifs //192.168.80.1/w1651574 /mnt/1651574 -o username=JiaHL,iocharset=cp936,vers=2.0
```
![](./pic/2-9.png)
�۲�`w1651574`�ļ��е��ļ�������

- ��Windows�½����ļ�
![](./pic/2-6.png)
- ��CentOS�Ϲ۲죬���鿴����Ȩ�ޣ�����
![](./pic/2-8.png)  

- ͨ��`umount /mnt/w1651574`ȡ��ӳ����Ҫ��֤��ǰû�����й������еĳ���
![](./pic/2-7.png)

- ͨ���򵥵Ĳ��ԣ����ִ�Сд��������
![](./pic/2-10.png)
![](./pic/2-11.png)

- ��ȡ��ӳ�䣬��`/mnt/w1651574`Ŀ¼�±����ļ���ɾ��WindowsĿ¼�е��ļ�����ӳ��۲�
![](./pic/2-12.png)
��ͼ��ʾ��ȡ��ӳ����ٴ����ļ����Լ��ļ�
![](./pic/2-13.png)
��ͼ��ʾ�����½������ں󣬷���Windows�ļ�����û���������ļ����ļ��У�����Linux����Ȼ����

- ʹ�����󿪻��Զ����أ���`/etc/fstab`Ŀ¼�¼���
```
//192.168.80.1/w1651574 /mnt/w1651574 cifs defaults.username=JiaHL,password=xxxxxx,iocharset=cp396,vers=2.0 0 0
```
![](./pic/2-14.png)
- ����reboot������,�Զ����سɹ�
![](./pic/2-15.png)


## ��Linux������һ������
- �ر����������VMware������һ������������ͼ��ʾ
![](./pic/3-1.png)
- ͨ��`ifconfig`ָ��۲�������Կ�
![](./pic/3-2.png)
- ���� `/etc/sysconfig/network-script` Ŀ¼��copyһ��`ifcfg-ens32` ��`ifcfg-ens34`
![](./pic/3-3.png)
- �޸�`ifcfg-ens34`,����ͼ������������ip�޸�Ϊ`192.168.81.1`
![](./pic/3-4.png)
����UUID����UUID Generator ���ɣ�![http://www.uuid.online/](http://www.uuid.online/)
- �ֱ�`ping 192.168.80.1` �� `ping 192.168.81.1`���۲��������ֳɹ�
![](./pic/3-5.png)


## ��Linux������һ��Ӳ��
- ͨ��`fdisk -l`ָ��۲�ĿǰӲ�̣��ɿ�����һ��Ӳ�� `sda`
![](./pic/4-1.png)
- ����ͼ��ʾ��Ϊ��������������Ӳ�̣�
![](./pic/4-2.png)
![](./pic/4-3.png)
![](./pic/4-4.png)
![](./pic/4-5.png)
![](./pic/4-6.png)
![](./pic/4-7.png)
![](./pic/4-8.png)
- �ٴ�ͨ��`fdisk -l`ָ��۲�ĿǰӲ�̣��ɿ�����`sda`,`sdb`����Ӳ��
![](./pic/4-9.png)
- ͨ��`lsblk`�����г������豸��Ϣ
![](./pic/4-10.png)
- ͨ��`fdisk /etc/sdb` ����Ӳ�̽��л���
![](./pic/4-11.png)

    - ��������`n`���з�������
    - ��`p` ������ �� `e` ��չ������ѡ����ѡ����չ����ѡ��
    - ������ѡ��1����ʼ�����Լ�������������Ĭ��(��ѡ���������������)��
    - ����Ϊ�������� 
  
            1.һ�����������������չ�����ĸ����ܺͲ��ܳ���4��

            2.һ����������ֻ����1����չ����

            3.��չ�����п��Դ����߼����������������Ӵ������Ͷ���
![](./pic/4-12.png)
- �Ը���չ�����ٷֳ������߼������������ŷֱ�Ϊ`5`,`6`����ʼ������������Ϊstart~20000000��20000000~end
![](./pic/4-13.png)
![](./pic/4-14.png)
- ͨ�� `p`����۲��ѷ�����������ɹ۲쵽sdb1Ϊһ��ʼ�ֵ���չ������sdb5��sdb6Ϊsdb1���ֵ������߼�����
![](./pic/4-15.png)
- ͨ��`w`�����������
![](./pic/4-16.png)
- ͨ��`partprobe`�������������ͬ��������ϵͳ
![](./pic/4-17.png)
- �Դ��̽��и�ʽ��
```
mkfs -t ext4 /dev/sdb5
mkfs -t ext4 /dev/sdb6
```
����ext4 ΪLinux�µ��ļ�ϵͳ�����⻹��ex1,ex2,ex3����ӦWindows����FTS32��NTFS
![](./pic/4-18.png)
![](./pic/4-19.png)

- ��Ŀ¼����`disk-1651574-1`��`disk-1651574-2`
![](./pic/4-20.png)

- �������õ�������Ӳ��ӳ�䵽�½��������ļ���
![](./pic/4-21.png)
- ͨ��`df -TH` �۲��������������Լ���������һ������
![](./pic/4-22.png)

## ��Linu�д�����ͨ�û�

- ����`stu`��
![](./pic/5-1.png)
- ͨ�������û���Ȼ��������û����ӹ�����
![](./pic/5-2.png)
- �����û���ͬʱ���ӹ�����
![](./pic/5-3.png)
- ��`cat /etc/passwd` ָ��۲������û�
![](./pic/5-4.png)
- ��`cat /etc/group` ָ��۲����й�����
![](./pic/5-5.png)

- Ϊ�û���������
![](./pic/5-6.png)

- ע�����ֱ����`userdel xxxxx`ɾ���û�����`/home`Ŀ¼�������û�Ŀ¼���ٴδ�����ͬ�û�����ʾ������ʾ
![](./pic/5-7.png)
- ���ɾ���û�ѡ��`uerdel -rf xxxxxx`ָ��
![](./pic/5-8.png)

- �����������û���ʹ�û��޷���½
![](./pic/5-9.png)

- ͨ��`who`ָ��۲������û�(�ֱ���vmware�Լ�secureCRT�ϵ�½root��u1651574)
![](./pic/5-10.png)

- ͨ���޸�`/etc/sudoers`�ļ������û�Ȩ������Ϊ��root��ͬ
![](./pic/5-11.png)
- ��½����Ȩ�޵�`u1651574`ִ�й���Ա�����useradd����������ǰ����`sudo`
![](./pic/5-12.png)
- ����`/etc/sudoers`�ļ�����u1651574Ȩ��ɾ������ִ�й���Ա������
![](./pic/5-13.png)


## ��Linux�������˻����븴����

- �޸�`/etc/pam.d/system-auth`�ļ�������ͼ�޸�
![](./pic/6-1.png)
    - �����ֱ�Ϊ:
    - retry:�����Դ���
    - difok:��ԭ�������ٲ�ͬ�ַ���
    - minlen:�������볤��
    - ucredit:���ٴ�д��ĸ��
    - lcredit:����Сд��ĸ��
    - dcredit:����������
    - ocredit:���������ַ���
- ͨ��`chage -l xxxxx`����鿴�û�����������Ϣ
![](./pic/6-2.png)
����`chage`ָ�����ɲ���:![https://blog.csdn.net/Jason_Deng_2017/article/details/78181153](https://blog.csdn.net/Jason_Deng_2017/article/details/78181153)
- �޸�`/etc/pam.d/system-auth`�ļ�������`remember = 3`��ʹ�����벻���벻����֮ǰ3����ͬ
![](./pic/6-3.png)
- ͨ��`chage -d 0 u1651574` �趨�û���½��ǿ���޸�����
![](./pic/6-4.png)
![](./pic/6-5.png)
- ͨ��`chage -M 90 u1651574` ����u1651574�˻���90���ǿ���޸�����
![](./pic/6-6.png)
���۲��ʱ����ϵͳʱ���޹�


## ��ͨ�û������������

- �۲쿪����Ĺ�����������Զ�����sdb5,sdb6
![](./pic/7-1.png)
- ����`/etc/fstab`���޸�����
![](./pic/7-2.png)
- ͨ��ָ��`mount | grep /disk-1651574-1`���۲�����Ƿ�ɹ�,��֧��quota����
![](./pic/7-3.png)
- �ֱ���`/disk-1651574-1`��`/disk-16515744-2`�ļ�����ʹ��`quotacheck -ug .`�������`aquota.user`��`aquota.group`�ļ�
![](./pic/7-4.png)
- ͨ��`quotaon /dev/sdb5` ��`quotaon /dev/sdb6`����quota����

- ����quotaӲ�����
```
edquota -u u1651574 # �����û��������
edquota -g stu      # ������������
```
![](./pic/7-5.png)
- ���в�����������
    - blocks :Ŀǰʹ������`/dev/xxx`��ʹ�õĿռ䣬��λ:KB (��Ҫ�޸�)
    - soft   :soft limit ���̿ռ��޶�ֵ ��λ��KB (��Ҫ�趨)
    - hard   :hard limit ���̿ռ��޶�ֵ ��λ�� KB (��Ҫ�趨)
    - inodes :Ŀǰʹ����ʹ�õ���inodes,���ڲ�����ʹ��inode����,���Բ�Ҫ�޸�����
    - soft   :soft limit �ļ���������  (������Ҫ�޸�)
    - hard   :hard limit �ļ���������   (������Ҫ�޸�)
    - soft limit :��������������ڿ�����(grace period)֮�ڣ�ʹ���������Գ���soft limit���������ڿ�����֮�ڽ�ʹ���������͵�soft limit���¡�
    - hard limit :�����������������ʹ�����ڿ������ڼ���д�����ݣ�����hard limit���޷���д�롣
    - ����ʱ�䣺ʹ����������soft limit������ʱ���Զ�����,ʹ���߽��������͵�soft limit���£�����ʱ���Զ��رգ����ʹ����û���ڿ���ʱ���ڽ��������͵�soft limit����ô�����޷���д�����ݣ���ʹʹ������û�е���hard limit 

- ����`u1651574` �˻�������`/disk-1651574-1`Ŀ¼�´���`haolin.txt`�ļ���,�����������   
![](./pic/7-6.png)
- ͨ��`quota u1651574`�鿴��ռ�е�����
![](./pic/7-7.png)
- ����ͨ��`repquota -aug`�鿴����ϸ�������û���ռ�����
![](./pic/7-8.png)

- �ر�quota���񣬲�ɾ������ļ�,���ֱ��ɾ��`aquota.user`��`aquota.group`�ļ����ᱨ�����´�����ʾ
![](./pic/7-9.png)
- ���Ӧ��ͨ��`quotaoff -avgu`ָ��ر�quota���ܣ���ɾ��

## ����Shell��д�򵥽ű�
### ѧϰShell
- [Shellѧϰ��վ](http://c.biancheng.net/cpp/shell/)
- ͨ�� `cat` �������鿴��ǰ Linux ϵͳ�Ŀ��� Shell
![](./pic/8-1.png)
- �鿴��ǰ Linux ��Ĭ�� Shell
![](./pic/8-2.png)
- ������shell�� `sh`��`bash`��`csh`��`tcsh`��`ash` �ȣ�`bash`�� Linux ��Ĭ�� shell
- shell�ļ򵥳�������
```
# cd �����ڵ�ǰĿ¼�£���test.shȨ��
chmod +x ./test.sh 
# ����test.sh
./test.sh
```
�����ֻᱨ��
![](./pic/8-3.png)
����ԭ��������Windows�±༭�ĳ���ÿһ������з�Ϊ\n\r����Linux��\n��ͬ
- ͨ��`cat -A test.sh`�鿴�ļ������ַ�
![](./pic/8-4.png)
- ͨ�� `sed -i 's/\r//' test.sh` ������е�`'\r'` �滻��`''`
![](./pic/8-5.png)
- �ٴ�ͨ��`cat -A test.sh`�鿴�ļ������ַ�
![](./pic/8-6.png)
����ĩβ�ַ��ѱ��滻��$
- ���гɹ���
![](./pic/8-7.png)

### ���Shell�ű�����
```shell
#! /bin/bash
#author: Haolin Jia
#date: 2018-9-24
if [ $# -ne 2 ];then
    echo "�������, user_add.sh [file_input] [file_output]"
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
- ���û�������ϵͳ��ʱ��������������
![](./pic/8-10.png)
- ���û�����ϵͳ�У�������⵽�û��Ѵ��ڣ�ɾ�������½�������������ʾ
![](./pic/8-11.png)
- ��������������ļ�Ϊ`student.conf`��`initpwd.dat`,�ļ���Ȩ�޿�ͨ��`chmod 777 xxxxx`�����ӵ����
![](./pic/8-9.png)
- �ɹ۲쵽�������н�������`cut -d : -f 1 /etc/passwd`���˻��Ѵ���
![](./pic/8-8.png)
