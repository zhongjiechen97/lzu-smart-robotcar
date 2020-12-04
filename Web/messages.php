<?php
    include "conn.php";
    $name=$_POST['name'];
    $email=$_POST['email'];
    $message=$_POST['message'];
    $sql="INSERT INTO `messages`(`id`, `name`, `email`, `message`) VALUES (null,'$name','$email','$message')";
    $conn->query($sql);
    echo "留言成功<a href='index.php'>返回首页</a>";
  

