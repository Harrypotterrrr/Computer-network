# CentOS7 ��������
- [CentOS7 ��������](#centos7-%E5%9F%BA%E6%9C%AC%E9%85%8D%E7%BD%AE)
    - [��װVMware��](#%E5%AE%89%E8%A3%85vmware)
    - [��װsecureCRT](#%E5%AE%89%E8%A3%85securecrt)
    - [�½������������CentOS����ϵͳ](#%E6%96%B0%E5%BB%BA%E8%99%9A%E6%8B%9F%E6%9C%BA%E5%B9%B6%E9%85%8D%E7%BD%AEcentos%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F)
    - [centOS��������](#centos%E4%B8%AD%E6%96%87%E9%85%8D%E7%BD%AE)
    - [ͨ���޸�linux���뷽ʽΪGBK/GB18030��ʹ������ʾ������ʾ����](#%E9%80%9A%E8%BF%87%E4%BF%AE%E6%94%B9linux%E7%BC%96%E7%A0%81%E6%96%B9%E5%BC%8F%E4%B8%BAgbkgb18030%E4%BD%BF%E4%B8%AD%E6%96%87%E6%8F%90%E7%A4%BA%E5%91%BD%E4%BB%A4%E6%98%BE%E7%A4%BA%E6%AD%A3%E5%B8%B8)
    - [centOS��mysql�İ�װ�Լ�����](#centos%E4%B8%ADmysql%E7%9A%84%E5%AE%89%E8%A3%85%E4%BB%A5%E5%8F%8A%E6%93%8D%E4%BD%9C)
    - [�޸�mariaDB���ַ�����Ϊgbk�����������](#%E4%BF%AE%E6%94%B9mariadb%E7%9A%84%E5%AD%97%E7%AC%A6%E7%BC%96%E7%A0%81%E4%B8%BAgbk%E5%B9%B6%E5%BB%BA%E8%A1%A8%E8%BE%93%E5%87%BA)
    - [��װphp�����Լ�ʹApache�����濪��������������php��Apache����](#%E5%AE%89%E8%A3%85php%E6%9C%8D%E5%8A%A1%E4%BB%A5%E5%8F%8A%E4%BD%BFapache%E6%9C%8D%E5%8A%A1%E9%9A%8F%E5%BC%80%E6%9C%BA%E5%90%AF%E5%8A%A8%E5%B9%B6%E9%85%8D%E7%BD%AEphp%E4%B8%8Eapache%E6%9C%8D%E5%8A%A1)
    - [C++����mysql���Գ���](#c%E7%BC%96%E8%AF%91mysql%E6%B5%8B%E8%AF%95%E7%A8%8B%E5%BA%8F)
    - [ʹ��flashFXP�����ʹ������centOS�������������֮�䴫���ļ�](#%E4%BD%BF%E7%94%A8flashfxp%E6%8F%92%E4%BB%B6%E4%BD%BF%E5%8F%AF%E4%BB%A5%E5%9C%A8centos%E8%99%9A%E6%8B%9F%E6%9C%BA%E5%AE%BF%E4%B8%BB%E6%9C%BA%E4%B9%8B%E9%97%B4%E4%BC%A0%E8%BE%93%E6%96%87%E4%BB%B6)

## ��װVMware��

- ��ʦ���õ�14.0�汾��VMware����������ѧ�����Ѱ�װ12.0�汾��VMware�������Ҳ��ٸ���������ӹ��˻��ڣ���֮��ʵ�飬12.0�汾ĿǰΪֹ��14.0�汾��𲻴�

## ��װsecureCRT

- ͬ����ѧ���Ѱ�װ��ϣ����Ѿ���Ϥ�˺ܶ�linux�µĲ���������vim���Լ�Զ�̷��������á��ϴ������ļ��Ȳ��������������Թ�

## �½������������CentOS����ϵͳ

- ����ѧ����ʦ�ṩ�Ѿ����ú�����˿�192.168.80.230��Red Hat��ͬ���ǣ����centOSû�����ã���Ҫ�����ֶ����ã�����ʦ�����İ�װ˵�����Ѿ���ϸ�������˴�û����ͼ����˲��ṩ��ͼ��ʾ��

- �������������һ�����ֵ����⣬���Ǽ�ʹ���ú���������VM8�����Ķ˿�192.168.80.0�� ���Ǵ�������Ժ���Ȼ��ʾ���ܺܺõ������ϣ�����ͼ��
![](./pic/image001.png)

- ���������ϣ��ҷ���ԭ��������ļ�`/etc/sysconfig/network-scripts/`֮�е������ļ�`ifcfg-ens32`��һ�`ONBOOT=no`��Ҫ�ֶ�����Ϊ`yes`��ͨ��`vim` ��`net-work-scripts`�ļ���������`i`��`a`���б༭���޸��������`esc`�˳��༭��������ָ��`:wq` ���沢�Ƴ�vim�༭��

- ������ `ifconfig` �鿴ip���£�
![](./pic/image002.png)

- ��ʾ��������Ч

���ˣ�centOS�������ù����ѽ���

## centOS��������

- �����ʼ�Ұ�װ����Ӣ�İ�Ĳ���ϵͳ������Ӣ�ĸ�����...�������Ƿ�����ʦ����ҵ�������ģ��Լ�Ҫ�����ϵͳ��ʾ����������ʾ�����ò�����Ѱ��������ʾ�ķ����������ϲ鵽�ķ������£�

- ����centOS 7.0�����ϰ汾��˵������������`/etc/locale.conf`��������ļ��У�
![](./pic/image003.png)

- ��vim locale.conf �޸�LANG���ԣ�����Ľ�ͼ�Ѿ��޸�Ϊ���ģ�
![](./pic/image004.png)

- ������Ӣ�Ļ���ת��,��ͼ����Ӣ�� 
![](./pic/image005.png)

- ��ʱΪӢ��״̬

- �����Ҳ鵽������ķ�������ֱ���޸����ԣ�ֱ��������������`LANG='xxx'`;�����޸�ϵͳ���ԣ�����`echo $LANG`������Բ鿴��ǰ����
![](./pic/image006.png)

- ��ͼ�п��Կ�����������utf-8���룬�ı�ΪӢ��utf-8����
ͨ���۲죬����������ʾ��������������
![](./pic/image007.png)


## ͨ���޸�linux���뷽ʽΪGBK/GB18030��ʹ������ʾ������ʾ����

- ��������`locale �Ca`�鿴�����Ѿ���װ�����԰����������԰������£�
![](./pic/image008.png)

- ����Ҫ��ͨ�����������ѡΪ`zh_CN.gbk`�۲���
![](./pic/image009.png)

- �Ѿ����Գɹ���������ʾ��

## centOS��mysql�İ�װ�Լ�����

- ���˺þ�..�����ҵ���mariaDB��������ʽ��
```
systemctl start mariadb.service
```
- ������������mysql����������

```
systemctl enable mariadb.service
```
- ����ֹͣmysql����Ϊ
```
systemctl stop mariadb.service
```
![](./pic/image010.png)

## �޸�mariaDB���ַ�����Ϊgbk�����������

- ��/etc/my.cnf����ļ��м����ɫ��ʾ�������������
![](./pic/image011.png)


- ͨ���������ݿ������ statusָ��鿴��ǰ���ݿ�״̬��
![](./pic/image012.png)

- �ɹ۲쵽���۷���˻������ݿ⻹�пͻ��˱��붼���ó���gbk

- ͨ�� `show databases`�� �Ĳ����������ݿ��б���û��demo���ݿ�

![](./pic/image013.png)

- ͨ�� `create database demo`���������ݿ�
![](./pic/image014.png)


- ͨ��`use demo`���������ݿ�demo
![](./pic/image015.png)


- ��ͨ��`drop XX if exists ..` ɾ���ɱ�Ȼ��ͨ��`create table student`���������ݱ�`student`
![](./pic/image016.png)

- show tables�����ʾ�����ݱ�����
![](./pic/image017.png)

- �����������ݣ�
![](./pic/image018.png)


- ͨ��`select * from student`��ѡ���������ݲ鿴student������Ϣ������ͼ��ʾ
![](./pic/image019.png)


## ��װphp�����Լ�ʹApache�����濪��������������php��Apache����

- ���ȹر����������ǽ�Լ�Selinux��
![](./pic/image020.png)
![](./pic/image021.png)



- �޸�config�ļ���ʹ`selinux=disabled`
![](./pic/image022.png)


- ����ʹcentOS�����������������ο��˴��е����£�[https://blog.csdn.net/zhaolimin66 66/article/details/52897198](https://blog.csdn.net/zhaolimin66%2066/article/details/52897198)��
![](./pic/image023.png)

- ͼΪ�ɹ�ping���ٶ����Ӻ�ķ���

- ͨ��`system control`�����Apache����
![](./pic/image024.png)

- ������������������ip���ɼ��Apache�����Ƿ�����
![](./pic/image025.png)

- ��װPHP����
![](./pic/image026.png)

- ��`yum`���װPHP������չ
![](./pic/image027.png)

- ֮������Apache
![](./pic/image028.png)

- ��mvָ��ı��ļ����� `-> demo.php`
![](./pic/image029.png)

- ����`/etc/httpd/conf` �� `httpd.conf`�ļ� �޸�����
![](./pic/image030.png)

- ����Ĭ���ַ�Ϊgbk
![](./pic/image031.png)

- �����޸���`httpd.conf`, һ��Ҫˢ��һ��apache ��service
![](./pic/image032.png)

- ����һ��ע�����php���룬�������ݿ�Ĳ���..�����������룺
![](./pic/image033.png)

- ����ͷ��������ֵ�ĳ��Լ���root���ݿ������

- ��ʱ�ٽ���`192.168.80.230`
![](./pic/image034.png)

- �ɹ۲쵽��ȷ���

## C++����mysql���Գ���

- ��ǰ����ͨ��������װ�ķ������ͨ��`mount` isoȻ��ͨ��iso��װ��

- �Ƚ�cdrom ���ص�mnt
![](./pic/image035.png)

- ʹ��`df �Ch`ָ��۲�/mnt���سɹ�����ʱmnt Ŀ¼�����ļ��� ÿ��boot�����Ҫ���¹���һ��
![](./pic/image036.png)

- Ȼ����� `/etc/yum.repos.d` Ŀ¼�£��޸� ��CentOS-Media.repo������ļ����ļ�����ʹ������Դ��Ч
![](./pic/image037.png)

- ��CentOS-Media.rep����ͼ��baseurlע�ͣ�����Ϊ`file:///mnt`�� ��ʹ`enabled = 1`������ʵ����ͨ��iso��ʽ��װ
![](./pic/image038.png)

- ��װmysql����
![](./pic/image039.png)

- �������˽⵽��

`--libs`ѡ�� ������mysql�����⣬ ִ��`mysql_config �Clibs`ѡ��
![](./pic/image040.png)

`--cflags`ѡ�� ��ʹ�ñ�Ҫ��include�ļ�
![](./pic/image041.png)

- C++�ļ���Ҫ�ǵ��޸�����
![](./pic/image042.png)

- ����������ͼ���
![](./pic/image043.png)

- ʹ��g++ ����cpp ����ִ���ļ���

    - ����һ�� ��ͨ��makefile

        ����`g++ $(mysql\_config --cflags) demo.cpp $(mysql\_config --libs) -o out` ָ�����
        ![](./pic/image044.png)
        `-I`Ϊ���������-Iָ����·��ȥ����ͷ�ļ�  
        `-L`Ϊ���������-Lָ����·��ȥѰ�ҿ��ļ�

        ���� out���õ����ս��
        ![](./pic/image045.png)

    - ��������ͨ��дmakefile �ļ�

        û��..��֪��Ϊʲô

## ʹ��flashFXP�����ʹ������centOS�������������֮�䴫���ļ�
- ��ѹ��F4����sites/site Manager��������վ����������������
![](./pic/image046.png)

- ע��Ҫ��SFTP���ӷ�ʽ����������ʧ��...
![](./pic/image047.png)

- �ǵø���Flashfxp�ı��뷽ʽ
![](./pic/image048.png)

- ���ӳɹ������ļ�
![](./pic/image049.png)

- �ɹ���ʾ��������ļ���
![](./pic/image050.png)
