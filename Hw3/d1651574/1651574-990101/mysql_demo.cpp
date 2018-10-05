//mysql_demo.cpp
#include <iostream>	// cin,cout等
#include <iomanip>	// setw等
#include <mysql.h>	// mysql特有
using namespace std;

int main(int argc, char* argv[])
{
    MYSQL     *mysql;   
    MYSQL_RES *result;   
    MYSQL_ROW  row;

    /* 初始化 mysql 变量，失败返回NULL */
    if ((mysql = mysql_init(NULL))==NULL) {
    	cout << "mysql_init failed" << endl;
    	return -1;
    	}

    /* 连接数据库，失败返回NULL
       1、mysqld没运行
       2、没有指定名称的数据库存在 */
    if (mysql_real_connect(mysql,"localhost","root", "","demo",0, NULL, 0)==NULL) {
    	cout << "mysql_real_connect failed(" << mysql_error(mysql) << ")" << endl;
    	return -1;
    	}

    /* 设置字符集，否则读出的字符乱码，即使/etc/my.cnf中设置也不行 */
    mysql_set_character_set(mysql, "gbk"); 

    /* 进行查询，成功返回0，不成功非0
       1、查询字符串存在语法错误
       2、查询不存在的数据表 */
    if (mysql_query(mysql, "select * from student")) {
    	cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
    	return -1;
    	}

    /* 将查询结果存储起来，出现错误则返回NULL
       注意：查询结果为NULL，不会返回NULL */
    if ((result = mysql_store_result(mysql))==NULL) {
    	cout << "mysql_store_result failed" << endl;
    	return -1;
    	}

    /* 打印当前查询到的记录的数量 */
    cout << "select return " << (int)mysql_num_rows(result) << " records" << endl;

    /* 循环读取所有满足条件的记录
	   1、返回的列顺序与select指定的列顺序相同，从row[0]开始
	   2、不论数据库中是什么类型，C中都当作是字符串来进行处理，如果有必要，需要自己进行转换
	   3、根据自己的需要组织输出格式 */
    while((row=mysql_fetch_row(result))!=NULL) {
        cout << setiosflags(ios::left);             //输出左对齐
        cout << "学号：" << setw(12) << row[0];     //宽度12位，左对齐
        cout << "姓名：" << setw(8)  << row[1];     //    8
        cout << "性别：" << setw(4)  << row[2];     //    4
        cout << "年龄：" << setw(4)  << row[3];     //    4
        cout << "系部：" << setw(4)  << row[4];     //    4
        cout << endl;
        }

    /* 释放result */
    mysql_free_result(result);   

    /* 关闭整个连接 */
    mysql_close(mysql);   

    return 0;
}
