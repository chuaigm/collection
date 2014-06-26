<?php
	ob_start();
	session_start();
	require_once("config.php");
?>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf8" />
<link href="style.css" rel="stylesheet" type="text/css" />
<title>欢迎投票 by chuaigm</title>
<style type="text/css">
/*全局样式*/
body { font-family: "宋体"; font-size: 12pt; color: #333333; margin-top: 0px; margin-right: 0px; margin-bottom: 0px; margin-left: 0px;background-image: url(images/test_bk.jpg);} 
table { font-family: "宋体"; font-size: 12pt; line-height: 30px; color: #333333}
a:link { font-size: 9pt; color: #333333; text-decoration: none} 
a:visited { font-size: 9pt; color: #333333; text-decoration: none} 
a:hover { font-size: 9pt; color: #E7005C; text-decoration: underline} 
a:active { font-size: 9pt; color: #333333; text-decoration: none} 
/*全局样式结束*/
</style>
<script language="javascript">
	function check()
	{
		node=frm.itm;
		flag=false;
		for(i=0;i<node.length;i++)
		{
			if(node[i].checked)
			{
				flag=true;
			}
		}
		if(!flag)
		{
			alert("您没有选择")
			return false;
		}
		return true;
	}
</script>

<?php

    if($_POST["submit"]){
	
	if($_SESSION["vote"]==session_id())
	{
		?>
		<script language="javascript">
			alert("您已经投票了");
			location.href="index.php";
		</script>
		<?php
		exit();
	}
// add 加入对投票者IP的记录
	$remote_ip=$_SERVER["REMOTE_ADDR"];
	$sql="select * from voter_ip";
	$rs=mysql_query($sql);
	while($row=mysql_fetch_assoc($rs))
	{
		if($row["user_ip"]==$remote_ip)
		{
		?>
		<script language="javascript">
			alert("您的IP已经投过票了");
			location.href="index.php";
		</script>
		<?php
		exit();
		}
	}
	$sql="insert into voter_ip (id, v_count, user_ip) values (NULL, 1, '$remote_ip')";
	$rs=mysql_query($sql);
// add end
	$id=$_POST["itm"];
	$sql="update vote set count=count+1 where id=$id";
	if(mysql_query($sql))
	{
		$_SESSION["vote"]=session_id();
	?>
	<script language="javascript">alert("投票成功,点确定查看结果");location.href="index.php?id=ck";</script>
	<?php
	}
	else
	{
	?>
	<script language="javascript">alert("投票失败");location.href="index.php";</script>
	<?php
	}
}
?>

</head>
<body>
<form name="frm" action="" method="post" onsubmit=return(check()) style="margin-bottom:5px;">
<table width="365" border="0" align="center" cellpadding="5" cellspacing="1" bgcolor="#C2C2C2">
<tr>
	<th bgcolor="#FFFFCC">
	<?php
		$sql="select * from votetitle";
		$rs=mysql_query($sql);
		$row=mysql_fetch_assoc($rs);
		echo $row["votetitle"];
	?>	</th>
</tr>
<?php
	$sql="select * from vote";
	$rs=mysql_query($sql);
	while($rows=mysql_fetch_assoc($rs))
	{
	?>
	<tr>
		                             <!--checkbox-->
	  <td bgcolor="#FFFFFF"><input type="radio" name="itm" value="<?php echo $rows["id"]?>" />&nbsp;&nbsp;<?php echo $rows["item"]?></td>
	</tr>
	<?php
	}
?>
<tr>
	<td align="center" bgcolor="#FFFFFF"><input type="submit" name="submit" value="投票"/>
	<input type="button" value="查看结果" onClick="location.href='index.php?id=ck'" /></td>
</tr>
</table>
</form>
<?php if($_GET["id"]=="ck"){?>

<?php
	$sql="select sum(count) as 'total' from vote";
	$rs=mysql_query($sql);
	$rows=mysql_fetch_assoc($rs);
	$sum=$rows["total"];	 //得出总票数
	$sql="select * from vote";
	$rs=mysql_query($sql);
?>
<table width="365" border="0" align="center" cellpadding="5" cellspacing="1" bgcolor="#C2C2C2">
<tr>
	<th bgcolor="#FFFFFF">项目</th>
	<th bgcolor="#FFFFFF">票数</th>
	<th bgcolor="#FFFFFF">百分比</th>
</tr>
<?php
	while($rows=mysql_fetch_assoc($rs))
	{
	?>
	<tr>
		<td bgcolor="#FFFFFF"><?php echo $rows["item"]?></td>
		<td bgcolor="#FFFFFF"><?php echo $rows["count"]?></td>
		<td bgcolor="#FFFFFF">
			<?php
			if($sum==0)
			{
				$per=0;
			} else {
				$per=$rows["count"]/$sum;
			}
				$per=number_format($per,4);
			?>
			<img src="100.jpg" height="4" width="<?php echo $per*100?>" />
			<?php echo $per*100?>%		</td>
	</tr>
	<?php
	}
?>
<tr>
		<td bgcolor="#FFFFFF">总票数</td>
		<td bgcolor="#FFFFFF"><?php echo $sum ?></td>
		<td bgcolor="#FFFFFF"></td>
</tr>
</table>
<div align="center">
<a href="index.php">隐藏结果</a>
</div>
<?php } ?>
</body>
</html>
