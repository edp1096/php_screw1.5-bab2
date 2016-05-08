<?php
// php판 php_screw 인코딩 스크립트
require_once("my_screw.php");

function screwSource($org_data) {
  global $header, $seed_key, $seed_key_len;

  $org_data = gzcompress($org_data);
  $datalen = strlen($org_data);
  for ($i = 0 ; $i < $datalen ; $i++) {
    $org_data[$i] = chr($seed_key[($datalen - $i) % $seed_key_len] ^ (~ord($org_data[$i])));
  }
  $result = $header.$org_data;
  //echo $org_data;
  
  return $result;
}

function descrewSource($org_data) {
  global $header_len, $seed_key, $seed_key_len;

  $org_data = substr($org_data, $header_len, strlen($org_data));
  $datalen = strlen($org_data);

  for ($i = 0 ; $i < $datalen ; $i++) {
    $org_data[$i] = chr($seed_key[($datalen - $i) % $seed_key_len] ^ (~ord($org_data[$i])));
  }
  $result = gzuncompress($org_data);
  //echo $org_data;

  return $result;
}

function screwFile($fname) {
  $org_data = '';

  $fp = fopen($fname, "rb");
  $fsize = filesize("test.php");

  while (!feof($fp)) {
    $org_data .= fgetc($fp);
  }
  fclose($fp);
  
  $org_data = screwSource($org_data);
  
  $fp = fopen($fname.'.screwed', 'wb');
  fwrite($fp, $org_data);
  fclose($fp);  
}

function descrewFile($fname) {
  $org_data = '';
  
  $fp = fopen($fname, "rb");
  $fsize = filesize("test.php");

  while (!feof($fp)) {
    $org_data .= fgetc($fp);
  }
  fclose($fp);
  
  $org_data = descrewSource($org_data);

  $fp = fopen($fname.'.descrewed', 'wb');
  fwrite($fp, $org_data);
  fclose($fp);

}

//screwFile("test.php");
//descrewFile("test.php.screwed");

?>
