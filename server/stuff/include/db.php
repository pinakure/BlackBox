<?PHP
/* Secure Header, call this function when connection is needed */


function dbquery($query, $db) {
	return(mysql_query($query, $db));	
}

function dbfree($result) {
	mysql_free_result($result);
}

function dbrowcount($result) {
	return(mysql_num_rows($result));
}

function dbfetch($result) {
	return(mysql_fetch_array($result));
}

function dbclose($db) {
	mysql_close($db);
}

function dbopen() {
	$sql_server   = 'hl103.dinaserver.com';
	$sql_username = 'pinakure';
	$sql_password = 'alienhunter';
	
	$sql_database = 'aser';
	
	$db = mysql_connect($sql_server , $sql_username , $sql_password);
	
	mysql_select_db($sql_database, $db);
    return($db);
}

function dbvalue($table, $id, $field) {
	$ret = 0;
	$db = dbopen();	
	$result = dbquery("SELECT ".$field." FROM ".$table." WHERE guid='".$id."';", $db);
	$ret = mysql_result($result,0,0);
	dbfree($result);	
	dbclose($db);	
	return $ret;
}

?>