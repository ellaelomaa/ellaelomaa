<?php
    session_start();
    if (!isset($_SESSION['rooli']) ||  $_SESSION['rooli'] != 'admin') {
        echo "503 Not authorized\n";
        exit;
    }

    $y_tiedot = "dbname=ryhma4 user=ryhma4 password=5RIis2az7lbHYRTD";
    
    if (!$yhteys = pg_connect($y_tiedot)) {
        die("Tietokantayhteyden luominen epäonnistui.");
    }
?>

<html>
    <head>
        <title>Raportit</title>
        <link rel="stylesheet" type="text/css" href="tyylit.css?d=<?php echo time(); ?>">
    </head>

    <?php include("header.php");
          include("topnav.php"); ?>

    <body>
        <form action="raportit.php" method="post">
            <?php if (isset($virhe)) echo '<p style="color:red">'.$virhe.'</p>'; ?>
            <?php if (isset($viesti)) echo '<p style="color:green">'.$viesti.'</p>'; ?>

            <p>
                <td>Lähettämättömät tilaukset</td>
                <td><input type="submit" name="avoimetTilaukset" value="Avaa yhteenveto"/></td>
            </p>

        <div id="content-wrap">
        <?php

            if (isset($_POST["avoimetTilaukset"])) {
            $_SESSION['tilausid'] = array();
            $tilaukset = pg_query("SELECT * FROM tilaus WHERE tila = 'vastaanotettu'");
            while ($rivi = pg_fetch_row($tilaukset)) {
                $_SESSION['tilausid'][] = $rivi[0];
            }
            header('Location: tilaus.php');
            }
            
            pg_close($yhteys);
        ?>
    </div>
    </body>
    <?php include('footer.php'); ?>
</html>
