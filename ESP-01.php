<html>
<head>
<title>ESP-01</title>
</head>
<body>
<?php
    if (! isset($_SERVER['HTTP_X_FORWARDED_FOR'])):
        $ip = $_SERVER['REMOTE_ADDR'];
    else:
        $ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
    endif;

    $id  = "192.168.100." . $_GET['id'];
    $txt = "Meldung";
    if($id == $ip):
        echo '<p>OK - SUCCESS for ' . $ip;
        $txt = "ALARM - Sensor aktiviert - IP: ". $ip;
    else:
        echo '<p> ACCESS DENIED for ' . $ip ;
        $txt = "ALARM - Netzeindringling - IP: " . $id . " / " . $ip;
    endif;

    $sql = "INSERT INTO Alarm (ip4, Bemerkung) VALUES ('" . $ip . "', '" . $txt . "');";
    $con = mysqli_connect("localhost", "DB-UserName", "DB-UserPassword", "IoT");
//    mysql_select_db("IoT") or die(mysql_error());
    if (!mysqli_query($con,$sql)):
         die('Error: ' . mysqli_error($con));
    endif;
    echo "<BR>record added";
// Close connection
    mysqli_close ($con);

// eMail versandt
    $error      = "fehler@youraddress.de";      // Adresse an die evtl Fehlermeldungen gehen.
    $empfaenger = "target1@targetAddress.de, target2@targetAddress.de, target3@targetAddress.dee";
    $betreff    = "ALARM - ALARM - ALARM";      // Betreff
    $nachricht  = $txt;                         // Nachricht
    $from       = "sender@senderAddress";       // Absender Adresse

    $Extra[]   = "From: ".$from;
    $Extra[]   = "Reply-To: IoTa - sender@senderAddress";
    $header    = implode("\n", $Extra);
    $extheader = "-f".$error;

    mail($empfaenger, $betreff, $nachricht, $header, $extheader);
?>
</body>
</html>
