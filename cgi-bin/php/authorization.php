<?php
    print_r("Content-Type: text/html\n\n");
    $str = getenv('HTTP_QUERY_STRING=');
    $list = explode("&", $str);
    $login = $list[0];
    $message = $list[1];
    $list2 = explode("=", $login);
    $login = $list2[1];
    $list3 = explode("=", $message);
    $message = $list3[1];
    echo "Hello ".$login."!\r\n";
    echo "Your message: ".$message."!\r\n";
    exit;

?>