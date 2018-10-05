//mysql_demo.cpp
#include <iostream>	// cin,cout��
#include <iomanip>	// setw��
#include <mysql.h>	// mysql����
using namespace std;

int main(int argc, char* argv[])
{
    MYSQL     *mysql;   
    MYSQL_RES *result;   
    MYSQL_ROW  row;

    /* ��ʼ�� mysql ������ʧ�ܷ���NULL */
    if ((mysql = mysql_init(NULL))==NULL) {
    	cout << "mysql_init failed" << endl;
    	return -1;
    	}

    /* �������ݿ⣬ʧ�ܷ���NULL
       1��mysqldû����
       2��û��ָ�����Ƶ����ݿ���� */
    if (mysql_real_connect(mysql,"localhost","root", "","demo",0, NULL, 0)==NULL) {
    	cout << "mysql_real_connect failed(" << mysql_error(mysql) << ")" << endl;
    	return -1;
    	}

    /* �����ַ���������������ַ����룬��ʹ/etc/my.cnf������Ҳ���� */
    mysql_set_character_set(mysql, "gbk"); 

    /* ���в�ѯ���ɹ�����0�����ɹ���0
       1����ѯ�ַ��������﷨����
       2����ѯ�����ڵ����ݱ� */
    if (mysql_query(mysql, "select * from student")) {
    	cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
    	return -1;
    	}

    /* ����ѯ����洢���������ִ����򷵻�NULL
       ע�⣺��ѯ���ΪNULL�����᷵��NULL */
    if ((result = mysql_store_result(mysql))==NULL) {
    	cout << "mysql_store_result failed" << endl;
    	return -1;
    	}

    /* ��ӡ��ǰ��ѯ���ļ�¼������ */
    cout << "select return " << (int)mysql_num_rows(result) << " records" << endl;

    /* ѭ����ȡ�������������ļ�¼
	   1�����ص���˳����selectָ������˳����ͬ����row[0]��ʼ
	   2���������ݿ�����ʲô���ͣ�C�ж��������ַ��������д�������б�Ҫ����Ҫ�Լ�����ת��
	   3�������Լ�����Ҫ��֯�����ʽ */
    while((row=mysql_fetch_row(result))!=NULL) {
        cout << setiosflags(ios::left);             //��������
        cout << "ѧ�ţ�" << setw(12) << row[0];     //���12λ�������
        cout << "������" << setw(8)  << row[1];     //    8
        cout << "�Ա�" << setw(4)  << row[2];     //    4
        cout << "���䣺" << setw(4)  << row[3];     //    4
        cout << "ϵ����" << setw(4)  << row[4];     //    4
        cout << endl;
        }

    /* �ͷ�result */
    mysql_free_result(result);   

    /* �ر��������� */
    mysql_close(mysql);   

    return 0;
}
