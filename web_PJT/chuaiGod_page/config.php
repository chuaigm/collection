<?php
	$conn=mysql_connect("localhost","root","66");
	if($conn==null)
		die("数据库连接失败");
	mysql_query("set names 'utf8'");
	if(!mysql_select_db("chuaiGod"))
	{
		die("选择数据失败");
	}
?>
