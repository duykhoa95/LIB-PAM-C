<?php
if(isset($_GET['username']) && $_GET['password'])
{
	$username = $_GET['username'];
	$password = $_GET['password'];
	$mang = array();
	$i=0;
	//echo $code;
	//echo $username;
	//echo "</br>";
	$path='dulieu.txt';
	if(!file_exists($path))
	{
		echo 'File không tôn tại';
	}
	else
	{
		$fp = @fopen($path,"r");
		if(!$fp)
		{
			echo 'mở file không thành công';
		}
		else
		{
			
			while(!feof($fp))
			{
				  $a =fgets($fp);
				 $mang[$i] = explode("#",$a); 
				//var_dump($mang[$i]);
				//print_r($mang[$i]);
				//print("</br>");
				if( (trim($mang[$i][1])==trim($password)) && (trim($mang[$i][0])==trim($username)))
				{
					echo "true";
				}
				else
				{
					//echo $mang[$i][0];
					//echo $mang[$i][1];
					//echo "</br>";
					//echo "false";
				}
				$i+=1;
			}
			
		}
		fclose($fp);
	}
}
?>
