<?php
header("Content-type:application/json");
//连接数据库
include "conn.php";
//设置数据库字符集
//查询数据库
$result = $conn->query("select * from `data` order by `time` desc limit 50");
$num = $result->num_rows;
if ($num) {
    //echo json_encode($result->fetch_array());
    echo json_encode($result->fetch_all(MYSQLI_ASSOC));
}
?>
