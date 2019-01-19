#####
#!/bin/sh
# ʹ�÷�����
#	��chmod.��700����
#	./shell_name 1>result.dat 2>&1
#
#######################################

if [ $# -lt 2 ]
then
	echo "Usage: "$0" ��ҵ��� ����ѧ���б�"
	echo "       "$0" 000103 stu_err_list.txt"
	exit 1
fi

fileno=$1
stu_err_list=$2
rm -rf $stu_err_list

# �� student.conf �ж�ȡѧ�ź�����������#��ͷ����
stu_no=($(awk '!/^#/{print $1}' student.conf))

for i in "${!stu_no[@]}";
do
	echo -e "\n-----------------------------\n"
	./tar_one_student.sh $fileno ${stu_no[i]} $stu_err_list
#	pause
done
