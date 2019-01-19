#####
#!/bin/sh
# ʹ�÷�����
#	��chmod.��700����
#	./shell_name 1>result.dat 2>&1
#
#######################################

if [ $# -lt 3 ]
then
	echo "Usage: "$0" ��ҵ��� ѧ�� ����ѧ���б�"
	echo "       "$0" 000103 1659999 stu_err_list.txt"
	exit 1
fi

# ͨ����ҵ��ŵõ��ļ���

fileno=$1
stu_err_list="../"$fileno"-"$3

case $fileno in 
	000103)
		filename=linux-makefile
		;; 
	000104)
		filename=linux-static_compile
		;; 
	000105)
		filename=linux-so
		;; 
	000106)
		filename=linux-daemon
		;; 
	000107)
		filename=linux-rpm_service
		;; 
	000108)
		filename=linux-socket-tcp-sync
		;; 
	000109)
		filename=linux-socket-tcp-async
		;; 
	990101)
		filename=linux-static_compile_mysql
		;; 
	*) #default 
		echo "��ҵ��Ŵ���"
		exit 1
		;; 
esac 

# �� student.conf �ж�ȡѧ�Ŷ�Ӧ������������#��ͷ����
stu_no=$2
#awk '/'$stu_no'/&&!/^#/{print $2}' student.conf
stu_name=$(awk '/'$stu_no'/&&!/^#/{print $2}' student.conf)

#echo ${filename}
#echo ${stu_no}"-"${stu_name}

# ��ҵ���Ŀ¼�����������
dir_name=$fileno
if [ ! -d $dir_name ]; then
	echo "��ҵĿ¼["$dir_name"]������"
	exit 1
fi

# ����Ϊ�������
if [ -z $stu_name ]; then
	echo "�޴�ѧ��"
	exit 1
fi

course_no=10106203
file_ext=".tar.bz2"
# ÿ��ѧ������ҵ�ļ�����10106203-165xxxx-����-linux-makefile.tar.bz2
full_filename=$course_no"-"$stu_no"-"$stu_name"-"$filename$file_ext

# ���Դ�ļ���(./000103/��ҵ��)�Ƿ����
check_filename="./"$dir_name"/"$full_filename
if [ ! -e $check_filename ]; then
	echo "��ҵ["$check_filename"]δ�ύ"
	exit 1
fi

cmd_tar="tar -xvjf "$full_filename" 1>/dev/null 2>&1"

# ������ҵ�����Ŀ¼���н�ѹ
cd $dir_name
echo $cmd_tar"("${stu_name}")"
eval $cmd_tar

# ִ����ɺ󣬼��Ŀ¼�Ƿ����
stu_dirname=$stu_no"-"$fileno
if [ ! -d $stu_dirname ]; then
	echo "��ѹ�����ҵĿ¼["$stu_dirname"]������"
	echo $stu_no" "$stu_name >> $stu_err_list
else
	echo "��ѹ�ɹ�"
fi

cd ..
