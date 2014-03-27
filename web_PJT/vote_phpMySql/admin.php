<?php
	require_once("config.php");
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<link href="../style.css" rel="stylesheet" type="text/css" />
<title>PHP+mysql开发的简单投票系统</title>
<style type="text/css">
/*全局样式*/
body { font-family: "宋体"; font-size: 12pt; color: #333333; margin-top: 0px; margin-right: 0px; margin-bottom: 0px; margin-left: 0px;background-image: url(images/Zlppy_Bg.jpg);} 
table { font-family: "宋体"; font-size: 9pt; line-height: 20px; color: #333333}
/*全局样式结束*/
</style>
<script language="javascript">
	function selectAll()
	{
		node=window.document.frm.itm;
		for(i=0;i<node.length;i++)
		{
			node[i].checked=true;
		}
	}
	function cancelAll()
	{
		node=frm.itm;
		for(i=0;i<node.length;i++)
		{
			node[i].checked=false;
		}
	}
	function del()
	{
		node=frm.itm;
		id="";
		for(i=0;i<node.length;i++)
		{
			if(node[i].checked)
			{
				if(id=="")
				{
					id=node[i].value
				}
				else
				{
					id=id+","+node[i].value
				}
			}
		}
		if(id=="")
		{
			alert("您没有选择删除项");
		}
		else
		{
			location.href="?type=del&id="+id
		}
	}
</script>
</head>
<body>
<?php
	if($_POST["Submit"])
	{
		$title=$_POST["title"];
		$sql="update votetitle set votetitle='$title'";
		mysql_query($sql);
		?>
		<script language="javascript">
			alert("修改成功");
		</script>
		<?php
	}
	if($_POST["Submit2"])
	{
		$newitem=$_POST["newitem"];
		$sql="insert into vote (titleid,item,count) values (1,'$newitem',1)";
		mysql_query($sql);
		
	}
?>
<form id="frm" name="frm" method="post" action="" style="margin-bottom:3px;">
  <table width="365" border="0" align="center" cellpadding="5" cellspacing="1" bgcolor="#C2C2C2">
    <tr>
      <td colspan="4" bgcolor="#FFFFFF"><label>
	  <?php
	  	$sql="select * from votetitle";
		$rs=mysql_query($sql);
		$rows=mysql_fetch_assoc($rs);
	  ?>
        <input name="title" type="text" id="title" size="35" value="<?php echo $rows["votetitle"]?>" />
      </label></td>
      <td width="68" align="center" bgcolor="#FFFFFF"><label>
        <input type="submit" name="Submit" value="修改标题" />
      </label></td>
    </tr>
    <tr>
      <th width="30" bgcolor="#FFFFFF">编号</th>
      <th width="45" bgcolor="#FFFFFF">项目</th>
      <th width="52" bgcolor="#FFFFFF">票数</th>
      <th width="50" align="center" bgcolor="#FFFFFF">修改</th>
      <th align="center" bgcolor="#FFFFFF">删除</th>
    </tr>
    <?php
		$sql="select * from vote order by count desc";
		$rs=mysql_query($sql);
		while($rows=mysql_fetch_assoc($rs))
		{
		?>
	<tr>
      <td align="center" bgcolor="#FFFFFF"><input type="checkbox" name="itm" value="<?php echo $rows["id"]?>" /><?php echo $rows["id"]?></td>
      <td align="center" bgcolor="#FFFFFF"><?php echo $rows["item"]?></td>
      <td align="center" bgcolor="#FFFFFF"><?php echo $rows["count"]?></td>
      <td align="center" bgcolor="#FFFFFF"><input type="button" value="修改" onclick="location.href='?type=modify&id=<?php echo $rows["id"]?>'" /></td>
      <td align="center" bgcolor="#FFFFFF"><input type="button" value="删除" onclick="location.href='?type=del&id=<?php echo $rows["id"]?>'"  /></td>
    </tr>
		<?php
		}
	?>
    <tr>
      <td colspan="5" align="center" bgcolor="#FFFFFF">
	  	<input type="button" value="选择全部" onclick="selectAll()" />
		<input type="button" value="取消全部" onclick="cancelAll()" />
	  <input type="button" value="删除所选" onclick="del()" />	  </td>
    </tr>
    <tr>
      <td colspan="3" bgcolor="#FFFFFF"><label>
        <input name="newitem" type="text" id="newitem" />
      </label></td>
      <td colspan="2" bgcolor="#FFFFFF"><label>
        <input type="submit" name="Submit2" value="添加新项" />
      </label></td>
    </tr>
  </table>
</form>

<?php

    if($_GET["type"]=="modify"){
	
	$id=$_GET["id"];
	if($_POST["Submit3"])
	{
		$item=$_POST["itm"];
		$count=$_POST["count"];
		$sql="update vote set item='$item',count=$count where id=$id";
		mysql_query($sql);
	echo "<script language=javascript>alert('修改成功！');window.location='admin.php'</script>";
	}
	$sql="select * from vote where id=$id";
	$rs=mysql_query($sql);
	$rows=mysql_fetch_assoc($rs);
?>
<form id="form1" name="form1" method="post" action="" style="margin-top:2px;">
  <table width="365" border="0" align="center" cellpadding="5" cellspacing="1" bgcolor="#C2C2C2">
    <tr>
      <th colspan="2" bgcolor="#FFFFFF">修改投票项目</th>
    </tr>
    <tr>
      <td align="center" bgcolor="#FFFFFF">名称:</td>
      <td bgcolor="#FFFFFF"><label>
        <input name="itm" type="text" id="itm" value="<?php echo $rows["item"]?>" />
      </label></td>
    </tr>
    <tr>
      <td align="center" bgcolor="#FFFFFF">票数：</td>
      <td bgcolor="#FFFFFF"><label>
        <input name="count" type="text" id="count" value="<?php echo $rows["count"]?>" />
      </label></td>
    </tr>
    <tr>
      <td colspan="2" align="center" bgcolor="#FFFFFF"><label>
        <input type="submit" name="Submit3" value="修改" />
        <input type="reset" name="Submit" value="重置" />
      </label></td>
    </tr>
  </table>
</form>

<?php 
	}
?>
<?php
	if($_GET["type"]=="del"){
	$id=$_GET["id"];
	$sql="delete from vote where id in ($id)";
	mysql_query($sql);
	echo "<script language=javascript>alert('删除成功！');window.location='admin.php'</script>";
	}
?>
</body>
</html>
