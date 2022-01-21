<?php
// luodaan tietokantayhteys ja ilmoitetaan mahdollisesta virheestä

$y_tiedot = "dbname=ryhma4 user=ryhma4 password=5RIis2az7lbHYRTD";

if (!$yhteys = pg_connect($y_tiedot))
   die("Tietokantayhteyden luominen epäonnistui.");

// isset funktiolla jäädään odottamaan syötettä.
// POST on tapa tuoda tietoa lomaketta (tavallaan kutsutaan lomaketta).
// Argumentti tallenna saadaan lomakkeen napin nimestä.

if (isset($_POST['tallenna']) && !empty($_POST))
{
    // kerätään inputit muuttujiin.
    $sposti = pg_escape_string($_POST['sposti']);
    $salasana = $_POST['salasana'];
    $salasana2 = $_POST['salasana2'];
    $etunimi = pg_escape_string($_POST['etunimi']);
    $sukunimi = pg_escape_string($_POST['sukunimi']);
    $osoite = pg_escape_string($_POST['osoite']);
    $puhnro = pg_escape_string($_POST['puhnro']);

    //Tarkistetaan, löytyykö salasanasta tarpeeksi monenlaisia merkkejä.
    $lowercase = preg_match('@[A-Z]@', $salasana);
    $uppercase = preg_match('@[a-z]@', $salasana);
    $number = preg_match('@[0-9]@', $salasana);
    $special = preg_match('@[^\w]@', $salasana);

    // Generoidaan jokaiselle käyttäjälle oma suola.
    $Allowed_Chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789./';
    $Chars_Len = 63;
    $Salt_Length = 21;
    $salt = "";
    $Blowfish_Pre='$2y$12$';
    $Blowfish_End='$';

    for ($i = 0; $i < $Salt_Length; $i++) {
        $salt.= $Allowed_Chars[mt_rand(0, $Chars_Len) ];
    }

    $bcrypt_salt = $Blowfish_Pre . $salt . $Blowfish_End;

    // Hashataan syötetty salasana.
    $hash = crypt($salasana, $bcrypt_salt);

    // jos kenttiin on syötetty jotain, lisätään tiedot kantaan
    
    $tiedot_ok = (trim($sposti) != '' && 
        trim($salasana) != '' &&
        trim($salasana2) != '' &&
        trim($etunimi) != '' && 
        trim($sukunimi) != '' && 
        trim($osoite) != '' && 
        trim($puhnro) != '');

    if ($salasana !== $salasana2) {
        $viesti = "Salasanat eivät vastaa toisiaan.";
        $tiedot_ok = false;
    }

    if (strlen($salasana) < 8) {
        $viesti = "Salasanan vähimmäispituus on 8 merkkiä.";
        $tiedot_ok = false;
    }

    if(!$uppercase || !$lowercase || !$number || !$special) {
        $viesti = "Salasanassa täytyy olla ainakin yksi pieni kirjain(a-z), yksi iso kirjain(A-Z), yksi numero ja yksi erikoismerkki.";
        $tiedot_ok = false;
    }

    else if ($tiedot_ok) {

        $kysely1 = pg_prepare($yhteys, "kysely1", "INSERT INTO kayttaja VALUES(DEFAULT, $1, $2, $3, $4) RETURNING kayttaja_id");
        $kysely1 = pg_execute($yhteys, "kysely1", array($sposti, $hash, "asiakas", $bcrypt_salt));
    
        if ($kysely1 === false) {
            $viesti = 'Operaatio epäonnistui - sähköpostiosoite on jo rekisteröity!';
        }
        else {
            $id = pg_fetch_row($kysely1)[0];
            $kysely2 = pg_prepare($yhteys, "kysely2", "INSERT INTO asiakas VALUES($1, $2, $3, $4, $5)");
            $kysely2 = pg_execute($yhteys, "kysely2", array($id, $osoite, $etunimi, $sukunimi, $puhnro));
            if ($kysely2 === false) {
               $viesti = 'Operaatio epäonnistui - asiakastiedoissa jotain häikkää!';
               //jos asiakkaan luonti epäonnistui, yritetään tuhota myös kayttaja, jotta sähköpostiosoitetta voi uudelleenyrittää
               $kysely3 = "DELETE FROM kayttaja WHERE kayttaja_id = $id";
               pg_query($kysely3);
            }
            else {
               $ok_viesti = 'Operaatio onnistui - <a href="kirjautuminen.php">Kirjaudu sisään</a> käyttääksesi palvelua!';
               unset($sposti, $salasana, $salasana2, $etunimi, $sukunimi, $osoite, $puhnro);
            }
        }    
    }
    else
        $viesti = 'Annetut tiedot puutteelliset - tarkista, ole hyvä!';
        unset($sposti, $salasana, $salasana2, $etunimi, $sukunimi, $osoite, $puhnro);

}

// suljetaan tietokantayhteys

pg_close($yhteys);

?>

<html>
    <head>
        <title>Rekisteröidy</title>
        <link rel="stylesheet" type="text/css" href="tyylit.css?d=<?php echo time(); ?>">
    </head>

    <?php include("header.php"); ?>

    <body>
        <form action="rekisteroityminen.php" method="post">

        <h2>Uuden käyttäjän rekisteröityminen</h2>

        <?php if (isset($viesti)) echo '<p style="color:red">'.$viesti.'</p>';
            if (isset($ok_viesti)) echo '<p style="color:green">'.$ok_viesti.'</p>'; ?>

        <p>Täytä kaikki kentät. Salasanan on oltava vähintään 8 merkkiä pitkä ja siinä täytyy olla ainakin yksi pieni kirjain(a-z), yksi iso kirjain(A-Z), yksi numero ja yksi erikoismerkki.</p>
        <table border="0" cellspacing="0" cellpadding="3">
            <tr>
                <td>Sähköpostiosoite</td>
                <td><input type ="text" pattern="[^@\s]+@[^@\s]+\.[^@\s]+" name="sposti" value="" /></td>
            </tr>
            <tr>
                <td>Salasana</td>
                <td><input type="password" name="salasana" value="" /></td>
            </tr>
            <tr>
                <td>Salasana uudelleen</td>
                <td><input type="password" name="salasana2" value="" /></td>
            </tr>
            <tr>
                <td>Etunimi</td>
                <td><input type="text" name="etunimi" value="" /></td>
            </tr>
            <tr>
                <td>Sukunimi</td>
                <td><input type="text" name="sukunimi" value="" /></td>
            </tr>
        <tr>
                <td>Osoite</td>
                <td><input type="text" name="osoite" value="" /></td>
            </tr>
        <tr>
                <td>Puhelinnumero</td>
                <td><input type="tel" name="puhnro" value="" /></td>
            </tr>

        </table>

        <br />

        <input type="hidden" name="tallenna" value="jep" />
        <input type="submit" value="Rekisteröi" />
        <br><a href="kirjautuminen.php">Palaa kirjautumissivulle</a>
        </form>
    </body>

    <?php include('footer.php'); ?>
</html>
