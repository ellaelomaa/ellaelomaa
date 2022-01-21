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

    $divariId = pg_escape_string($_SESSION['divari_id']);

    // Etsitään onko divarilla omaa tietokantaa.
    $tulos = pg_prepare($yhteys, "kantakysely", "SELECT omakanta FROM public.divari WHERE divari_id = $1");
    $tulos = pg_execute($yhteys, "kantakysely", array($divariId));
    $omakanta = pg_fetch_row($tulos)[0];

    // Jos divarilla ei ole omaa tietokantaa, käytetään keskusdivarin kantaa.
    if(!$omakanta) {
        $omakanta = "public";
    }

    $isbnVertailuXML = pg_prepare($yhteys, "isbnVertailuXML", "SELECT teos_id, isbn FROM $omakanta.teos WHERE isbn = $1");
    $lisaaTeostauluun = pg_prepare($yhteys, "lisaaTeostauluun", "INSERT INTO $omakanta.teos (isbn, nimi, tyyppi, t_etunimi, 
            t_sukunimi, julkaisuvuosi) VALUES($1, $2, $3, $4, $5, $6) RETURNING teos_id");
    $lisaaKappaletauluun = pg_prepare($yhteys, "lisaaKappaletauluun", "INSERT INTO $omakanta.myyntikappale(divari_id, tila, hinta, paino, teos_id)
            VALUES($1, $2, $3, $4, $5)"); 

    if (isset($_POST['lataa'])) {
        $nimi = $_POST['tiedosto'];
        $osat = explode(".", $nimi);
        if ($osat[1] !== "xml") {
            $virhe = "Virheellinen tiedostomuoto.";
        }
        else {
            if (file_exists($nimi)) {
                $teokset = simplexml_load_file($nimi);
                if ($teokset === false) {
                    $virhe = "Virheellinen XML.";
                }
                else {
                    $viesti = "Tiedosto avattu onnistuneesti.";
                
                    $virheellisetTeokset = "";

                    foreach ($teokset->teos as $teos) {
                        // Alustetaan muuttujat.
                        $nimi = pg_escape_string($teos->nimi);
                        $etunimi = pg_escape_string($teos->etunimi);
                        $sukunimi = pg_escape_string($teos->sukunimi);
                        $isbn = pg_escape_string($teos->isbn);
                        if (empty($etunimi) && empty($sukunimi)) {
                           $sukunimi = pg_escape_string($teos->tekija);
                        }
            
                        $isbnVirheviestiteksti = "ISBN on väärässä muodossa teoksessa/teoksissa:";
                        // Tarkistetaan, että ISBN on oikeassa muodossa.
                        $isbnVirhe = false;

                        $isbn = str_replace("-", "", $isbn);
                        $isbn = str_replace(" ", "", $isbn);
                
                        // Syötteiden tarkistus.
                        if (!empty($isbn) && (!ctype_digit($isbn) || (strlen($isbn) != 10 && strlen($isbn) != 13))) {
                            $virheellisetTeokset = $virheellisetTeokset . "<br /> \n" .  $nimi;
                            $isbnVirheviesti = $isbnVirheviestiteksti . $virheellisetTeokset;
                            $isbnVirhe = true;
                        }

                        if (empty($isbn)) {
                            $virheellisetTeokset = $virheellisetTeokset . "<br /> \n" .  $nimi;
                            $isbnVirheviesti = $isbnVirheviestiteksti . $virheellisetTeokset;
                            $isbnVirhe = true;
                        }
            
                        if (empty($nimi)) {
                            $nimiVirheviesti = "Ainakin yhden teoksen nimi on tyhjä.";
                        }
            
                        // Varmistetaan, että nimet alkavat isolla alkukirjaimella.
                        $nimi = ucfirst($nimi);
                        $etunimi = strtolower($etunimi);
                        $sukunimi = strtolower($sukunimi);
                        $etunimi = ucwords($etunimi);
                        $sukunimi = ucwords($sukunimi);
            
                        // Katsotaan, ovatko XML:n tiedot virheelliset.
                        if ($isbnVirhe === true || empty($nimi)) {
                            $virhe = "XML virheellinen.";
                        }
                        else {
                        // Katsotaan löytyykö ISBN jo tietokannasta.
                            $isbnHaku = pg_execute($yhteys, "isbnVertailuXML", array($isbn));
            
                            // Jos teosta ei löydy, lisätään se teostauluun.
                            if (pg_num_rows($isbnHaku) == 0) {
                                // Lisätään teos teostauluun.
                                $lisaaTeostauluun = pg_execute($yhteys, "lisaaTeostauluun", array($isbn, $nimi, "", $etunimi, $sukunimi, '0'));
                                $teosId = pg_fetch_row($lisaaTeostauluun)[0];
                            }
                            else {
                                // Teos löytyy jo kannasta. Haetaan sen id.
                                $isbnHaku = pg_execute($yhteys, "isbnVertailuXML", array($isbn));
                                $tulos = pg_fetch_row($isbnHaku);
                                $teosId = $tulos[0];  
                            }
                            // Lisätään myyntikappale myyntikappaletauluun nide kerrallaan.
                            foreach ($teos->nide as $nide) {
                                    $hinta = pg_escape_string($nide->hinta);
                                    $paino = pg_escape_string($nide->paino);
                                    // Mikäli painoa ei ole niteelle erikseen merkitty,
                                    // asetetaan sen oletusarvoksi maksimipaino eli 2kg.
                                    if ($paino == '') {
                                        $paino = 2000;
                                    }
            
                                    // Muutetaan strig-tyyppiset muuttujat.
                                    $hinta = floatval($hinta);
                                    $paino = intval($paino);
                                    $tulos2 = pg_execute($yhteys, "lisaaKappaletauluun", array($divariId, "saatavilla", $hinta, $paino, $teosId));   
                                    if ($tulos2 == false) {
                                        $virhe = "XML:n lisääminen ei onnistunut.";
                                    }
                                    else {
                                        $viesti = "XML:n lisääminen onnistui.";   
                                    }     
                            } 
                        }    
                    }
                    $virheellisetTeokset = "";
                }
            }
            else {
            $virhe = "Tiedoston avaaminen epäonnistui.";
            }
        }
    }

    if (isset($_POST['siirra'])) {
        $xml = $_POST["xml"];
        // Ladataan teksti XML-objektiksi.
        $teokset = simplexml_load_string($xml);
        if ($teokset === false) {
            $virhe = "Virheellinen XML.";
        }
        else {
            $virheellisetTeokset = "";

            foreach ($teokset->teos as $teos) {
                // Alustetaan muuttujat.
                $nimi = pg_escape_string($teos->nimi);
                $etunimi = pg_escape_string($teos->etunimi);
                $sukunimi = pg_escape_string($teos->sukunimi);
                $isbn = pg_escape_string($teos->isbn);
                if (empty($etunimi) && empty($sukunimi)) {
                   $sukunimi = pg_escape_string($teos->tekija);
                }

                $isbnVirheviestiteksti = "ISBN on väärässä muodossa teoksessa/teoksissa:";
                // Tarkistetaan, että ISBN on oikeassa muodossa.
                $isbnVirhe = false;
                if (!empty($isbn) && (!ctype_digit($isbn) || strlen($isbn) != 10 && strlen($isbn) != 13)) {
                    $virheellisetTeokset = $virheellisetTeokset . "<br /> \n" .  $nimi;
                    $isbnVirheviesti = $isbnVirheviestiteksti . $virheellisetTeokset;
                    $isbnVirhe = true;
                }

                if (empty($isbn)) {
                    $virheellisetTeokset = $virheellisetTeokset . "<br /> \n" .  $nimi;
                    $isbnVirheviesti = $isbnVirheviestiteksti . $virheellisetTeokset;
                    $isbnVirhe = true;
                }

                if (empty($nimi)) {
                    $nimiVirheviesti = "Ainakin yhden teoksen nimi on tyhjä.";
                }

                // Varmistetaan, että nimet alkavat isolla alkukirjaimella.
                $nimi = ucfirst($nimi);
                $t_etunimi = strtolower($etunimi);
                $t_sukunimi = strtolower($sukunimi);
                $t_etunimi = ucwords($etunimi);
                $t_sukunimi = ucwords($sukunimi);

                // Katsotaan, ovatko XML:n tiedot virheelliset.
                if ($isbnVirhe === true || empty($nimi)) {
                    $virhe = "XML virheellinen.";
                }
                else {
                // Katsotaan löytyykö ISBN jo tietokannasta.
                    $isbnHaku = pg_execute($yhteys, "isbnVertailuXML", array($isbn));

                    // Jos teosta ei löydy, lisätään se teostauluun.
                    if (pg_num_rows($isbnHaku) == 0) {
                        // Lisätään teos teostauluun.
                        $lisaaTeostauluun = pg_execute($yhteys, "lisaaTeostauluun", array($isbn, $nimi, "", $etunimi, $sukunimi, '0'));
                        $teosId = pg_fetch_row($lisaaTeostauluun)[0];
                    }
                    else {
                        // Teos löytyy jo kannasta. Haetaan sen id.
                        $isbnHaku = pg_execute($yhteys, "isbnVertailuXML", array($isbn));
                        $tulos = pg_fetch_row($isbnHaku);
                        $teosId = $tulos[0];
                    }
                    // Lisätään myyntikappale myyntikappaletauluun nide kerrallaan.
                    foreach ($teos->nide as $nide) {
                            $hinta = pg_escape_string($nide->hinta);
                            $paino = pg_escape_string($nide->paino);
                            // Mikäli painoa ei ole niteelle erikseen merkitty,
                            // asetetaan sen oletusarvoksi maksimipaino eli 2kg.
                            if ($paino == '') {
                                $paino = 2000;
                            }

                            // Muutetaan strig-tyyppiset muuttujat.
                            $hinta = floatval($hinta);
                            $paino = intval($paino);
                            $tulos2 = pg_execute($yhteys, "lisaaKappaletauluun", array($divariId, "saatavilla", $hinta, $paino, $teosId));   
                            if ($tulos2 == false) {
                                $virhe = "XML:n lisääminen ei onnistunut.";
                            }
                            else {
                                $viesti = "XML:n lisääminen onnistui.";   
                            }     
                    }            
                }    
            }
            $virheellisetTeokset = "";
        }
    }

    pg_close($yhteys);
?>

<html>
    <head>
        <title>Tuo XML-data tietokantaan</title>
        <link rel="stylesheet" type="text/css" href="tyylit.css?d=<?php echo time(); ?>">
    </head>

    <?php include("header.php");
          include("topnav.php"); ?>

    <body>
        <div id="content-wrap">
            <form action="xml.php" method="post">
                <?php if (isset($virhe)) echo '<p style="color:red">'.$virhe.'</p>'; ?>
                <?php if (isset($isbnVirheviesti)) echo '<p style="color:red">'.$isbnVirheviesti.'</p>'; ?>
                <?php if (isset($nimiVirheviesti)) echo '<p style="color:red">'.$nimiVirheviesti.'</p>'; ?>
                <?php if (isset($viesti)) echo '<p style="color:green">'.$viesti.'</p>'; ?>

                <p>
                Varmistathan, että XML-tiedostossa kaikista teoksista löytyy ISBN. Teokset, joissa ei ole ISBN:ää, täytyy lisätä erikseen Lisää teos -valikossa.
                </p>
                
                <p>
                Lataa tiedosto tai kopioi se alla olevaan kenttään.
                </p>

                <p><input type="file" name="tiedosto"></p>

                <p>
                    <td><input type="submit" name="lataa" value="Lataa tiedosto tietokantaan"/></td>
                </p>

                <textarea name="xml" rows="30" cols="50">
                </textarea>

                <p>
                    <td><input type="submit" name="siirra" value="Siirrä tietokantaan"/></td>
                </p>
                
                <input type="hidden" name="tallenna" value="hidden"/>
            </form>
        </div>
    </body>
    <?php include('footer.php'); ?>
</html>
