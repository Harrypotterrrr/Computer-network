#! /bin/bash
#author: Haolin Jia
#date: 2018-9-24
if [ $# -ne 2 ];then
    echo "ÃüÁî´íÎó, user_add.sh [file_input] [file_output]"
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