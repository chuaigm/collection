<?php
	$conn=mysql_connect("localhost","root","66");
	if($conn==null)
		die("���ݿ�����ʧ��");
	mysql_query("set names 'gb2312'");
	if(!mysql_select_db("chuai"))
	{
		die("ѡ������ʧ��");
	}
?>
