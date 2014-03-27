<?php
	$conn=mysql_connect("localhost","root","66");
	if($conn==null)
		die("数据库连接失败");
	mysql_query("set names 'gb2312'");
	if(!mysql_select_db("chuai"))
	{
		die("选择数据失败");
	}
?>
