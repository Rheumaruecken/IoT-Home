<?php
  // Manfred Nebel
  header("Content-type: image/png");
  $imgWidth     = '200';
  $imgHeight    = '100';
  $KlimaBild    = imageCreateTruecolor($imgWidth, $imgHeight);
  $colorBlue    = imageColorAllocate($KlimaBild, 0, 50, 255);
  $colorBlack   = imageColorAllocate($KlimaBild, 0, 0, 0);
  $colorGreen   = imageColorAllocate($KlimaBild, 0, 255, 0);
  $colorWhite   = imageColorAllocate($KlimaBild, 255, 255, 255);
  $colorRed     = imageColorAllocate($KlimaBild, 255, 125, 125);

  $x = 199;
  $y = 0;
  $z = 10;
  $dbhost = 'localhost';
  $dbuser = 'netzClient';
  $dbpass = '1#l0;Osgdr';
  $dbname = 'IoT';
  $sql = 'SELECT datum, temp FROM Klima ORDER BY datum DESC LIMIT 96;';

  imageColorAllocate($bild, 0, 0, 0);    // Hintergrundfarbe
  imageFill($KlimaBild, 0, 0, $colorWhite);
  imageRectangle($KlimaBild, 6, 0, $imgWidth-1, $imgHeight-1, $colorBlack);
// Verbindung herstellen
  $mysqli = new mysqli($dbhost, $dbuser, $dbpass, $dbname);
  if ($mysqli->connect_errno) {
    exit();
  }
mysqli_query($mysqli, "SET NAMES 'utf8'");

  if ($result = $mysqli->query($sql)) {
    while ($row = $result->fetch_assoc()) {
      $z = 2*$row["temp"];
      imageline($KlimaBild, $x-1, 100, $x-1, 100-$z, $colorBlue);               // Werte zeichnen
      imageline($KlimaBild, $x-2, 100, $x-2, 100-$z, $colorBlue);
      $x = $x - 2;
    }
    $result->free();;
  }

  $mysqli->close();
  ImageString($KlimaBild, 10, $imgWidth/2, 0, "50", $colorRed);
  ImageString($KlimaBild, 10, $imgWidth/2, $imgHeight-15, "0", $colorRed);
  imagePng($KlimaBild);
?>
