<?php
    $startTime=microtime (true);
    //�����ݿ�
    $mysqli=new mysqli('localhost','root','root123','demo');
    if (mysqli_connect_errno()){
        echo "����ʧ�ܣ�ԭ��Ϊ��".mysqli_connect_error();
        exit();
        }
    //���������ַ���
    $result = $mysqli->query ("set names gbk");
    //��ȡstudent��
    $query = "select * from student";
    $result = $mysqli->query ($query);
    if (!$result->num_rows) {
        echo "����Ϊ��!";
        exit();
        }
    echo '<table align="center" width="50%" border="3" cellpadding="0" cellspacing="0">';
    echo '<caption align="left">�б�</caption>';
    echo '<tr><th>ѧ��</th><th>����</th><th>�Ա�</th><th>����</th><th>ϵ��</th></tr>';
    while($row=$result->fetch_assoc()){
        echo '<tr><td>'.$row["sno"].'</td><td>'.$row["sname"].'</td><td>'.$row["ssex"].'</td><td>'.$row["sage"].'</td><td>'.$row["sdept"].'</td></tr>';
        }
    $result->close();
    echo '</table>';
    		
    //�ر����ݿ�
    $mysqli->close();
            
    $stopTime=microtime (true);
    echo "����ҳ��ʱ��".($stopTime - $startTime)."�롣";
?>
