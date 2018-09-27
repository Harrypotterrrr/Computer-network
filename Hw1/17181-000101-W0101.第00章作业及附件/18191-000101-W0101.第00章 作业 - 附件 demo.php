<?php
    $startTime=microtime (true);
    //打开数据库
    $mysqli=new mysqli('localhost','root','root123','demo');
    if (mysqli_connect_errno()){
        echo "连接失败，原因为：".mysqli_connect_error();
        exit();
        }
    //设置中文字符集
    $result = $mysqli->query ("set names gbk");
    //读取student表
    $query = "select * from student";
    $result = $mysqli->query ($query);
    if (!$result->num_rows) {
        echo "数据为空!";
        exit();
        }
    echo '<table align="center" width="50%" border="3" cellpadding="0" cellspacing="0">';
    echo '<caption align="left">列表</caption>';
    echo '<tr><th>学号</th><th>姓名</th><th>性别</th><th>年龄</th><th>系部</th></tr>';
    while($row=$result->fetch_assoc()){
        echo '<tr><td>'.$row["sno"].'</td><td>'.$row["sname"].'</td><td>'.$row["ssex"].'</td><td>'.$row["sage"].'</td><td>'.$row["sdept"].'</td></tr>';
        }
    $result->close();
    echo '</table>';
    		
    //关闭数据库
    $mysqli->close();
            
    $stopTime=microtime (true);
    echo "打开网页耗时：".($stopTime - $startTime)."秒。";
?>
