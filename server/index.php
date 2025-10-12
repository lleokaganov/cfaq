<?php

function sdie($s,$err="HTTP/1.0 200 OK") { // header("HTTP/1.0 204 No Content");
    header($err);
    header('Content-Type: text/plain');
    header('Access-Control-Allow-Origin: *');
    header('Connection: close');
    header('Content-Length: '.strlen($s));
    die($s);
}

// file_put_contents("iglu.txt",print_r($_GET,1));
// file_put_contents("server.txt",print_r($_SERVER,1));

if(isset($_GET['justfile'])) {
    $name=preg_replace("/[^0-9a-z\-\_\.\/]/si",'',$_GET['justfile']);
    $name=$f=str_replace('..','',$name);
    $name="/R/lleo.me/ESP8266/".$name;
    sdie( file_get_contents($name) );
}

// https://lleo.me/ESP8266/index.php?ip=%3C%3Cip%3E%3E&chip=%3C%3Cchip%3E%3E&soft=%3C%3Csoft%3E%3E&file=calibrate.htm

if(isset($_GET['firmware'])) { $_GET['soft']="firmware"; $_GET['file']="firmware.bin"; }

if(isset($_GET['soft'])) {

    $d=$_GET['soft'];
    // if($d=='%3C%3Csoft%3E%3E') $d='default5'; // Р—РђР•Р‘РђР›Рћ!

    $d=preg_replace("/[^0-9a-z\-\_]/si",'',$d);
// die("OK[$d}");
    if(!is_dir($d)) sdie('',"HTTP/1.0 404 Not Found");

    // конкретный файл
    if(isset($_GET['file'])) { $f=$_GET['file']; $f=str_replace('..','',$f);
	$fl=$d."/".ltrim($f,"/");


	if(!file_exists($fl)) sdie('',"HTTP/1.0 404 Not Found");

	if(isset($_GET['md5'])) sdie(md5_file($fl));

    file_put_contents('d.txt',$fl." [".md5_file($fl)."]");


	header('Content-Description: File Transfer');
	header('Content-Type: application/octet-stream');
	header('Content-Disposition: attachment; filename="'.basename($fl).'"');
	header('Expires: 0');
	// header('Cache-Control: must-revalidate');
	// header('Pragma: public');
	header('Content-Length: '.filesize($fl)); // no fuckin chunked
	readfile($fl);
	exit;
    }

    // возможные схемы обновления
    if(isset($_GET['softs'])) {
        $g=glob($d."/*");
    }

    $g=glob($d."/*");
    $o="OK\n"; foreach($g as $l) { $l0=basename($l); if(!is_dir($l) && $l0!='off') $o.=$l0." ".md5_file($l)." ".filesize($l)."\n"; }
    sdie($o);
}


if(isset($_GET['unixtime'])) { die(time()); }

// возможные схемы обновления
if(isset($_GET['softs'])) {
        $g=glob("*",GLOB_ONLYDIR);
	sdie(implode("\n",$g));
}

    sdie('',"HTTP/1.0 404 Not Found");

?>