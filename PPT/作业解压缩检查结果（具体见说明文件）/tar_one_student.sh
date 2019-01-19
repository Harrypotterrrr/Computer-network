#####
#!/bin/sh
# 使用方法：
#	用chmod.加700属性
#	./shell_name 1>result.dat 2>&1
#
#######################################

if [ $# -lt 3 ]
then
	echo "Usage: "$0" 作业编号 学号 错误学生列表"
	echo "       "$0" 000103 1659999 stu_err_list.txt"
	exit 1
fi

# 通过作业编号得到文件名

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
		echo "作业编号错误"
		exit 1
		;; 
esac 

# 从 student.conf 中读取学号对应的姓名，忽略#开头的行
stu_no=$2
#awk '/'$stu_no'/&&!/^#/{print $2}' student.conf
stu_name=$(awk '/'$stu_no'/&&!/^#/{print $2}' student.conf)

#echo ${filename}
#echo ${stu_no}"-"${stu_name}

# 作业编号目录不存在则错误
dir_name=$fileno
if [ ! -d $dir_name ]; then
	echo "作业目录["$dir_name"]不存在"
	exit 1
fi

# 姓名为空则错误
if [ -z $stu_name ]; then
	echo "无此学生"
	exit 1
fi

course_no=10106203
file_ext=".tar.bz2"
# 每个学生的作业文件名：10106203-165xxxx-张三-linux-makefile.tar.bz2
full_filename=$course_no"-"$stu_no"-"$stu_name"-"$filename$file_ext

# 检查源文件名(./000103/作业名)是否存在
check_filename="./"$dir_name"/"$full_filename
if [ ! -e $check_filename ]; then
	echo "作业["$check_filename"]未提交"
	exit 1
fi

cmd_tar="tar -xvjf "$full_filename" 1>/dev/null 2>&1"

# 进入作业编号子目录进行解压
cd $dir_name
echo $cmd_tar"("${stu_name}")"
eval $cmd_tar

# 执行完成后，检查目录是否存在
stu_dirname=$stu_no"-"$fileno
if [ ! -d $stu_dirname ]; then
	echo "解压后的作业目录["$stu_dirname"]不存在"
	echo $stu_no" "$stu_name >> $stu_err_list
else
	echo "解压成功"
fi

cd ..
