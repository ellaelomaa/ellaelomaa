<?php
    session_start();

    $y_tiedot = "dbname=ryhma4 user=ryhma4 password=5RIis2az7lbHYRTD";
    
    if (!$yhteys = pg_connect($y_tiedot))
        die("Tietokantayhteyden luominen epäonnistui.");

    if(isset($_POST['kirjaudu']) && !empty($_POST)) {
        $email = $_POST['email'];
        $salasana = $_POST['salasana'];

        if (empty($_POST['email'])) {
            $viesti = "Sähköpostiosoite ei voi olla tyhjä.";
        }
        elseif (empty($_POST['salasana'])) {
            $viesti = "Salasana ei voi olla tyhjä.";
        }
        else {
            $emailTarkistus = pg_prepare($yhteys, "kysely", "SELECT email FROM kayttaja WHERE email=$1");
            $emailTarkistus = pg_execute($yhteys, "kysely", array($email));

            if ($emailTarkistus === false) {
                $viesti = "Sähköpostiosoite tai salasana väärin.";
            }
            else {
                // Haetaan tietokantaan tallennettu salasana.
                // $salasanaTarkistus = pg_query("SELECT salasana FROM kayttaja WHERE email = '$email'");
                $salasanaTarkistus = pg_prepare($yhteys, "kysely2", "SELECT salasana FROM kayttaja WHERE email = $1");
                $salasanaTarkistus = pg_execute($yhteys, "kysely2", array($email));
                $salasanaRivi = pg_fetch_assoc($salasanaTarkistus);

                // Haetaan tietokantaan tallennettu, jokaiselle käyttäjälle
                // yksilöllinen salt.
                $salt = pg_query("SELECT salt FROM kayttaja WHERE email = '$email'");
                $hashRivi = pg_fetch_assoc($salt);

                // Hashataan nyt syötetty salasana käyttäjän suolalla.
                $hash = crypt($salasana, $hashRivi['salt']);

                if (!$hashRivi['salt']) {
                    $hash = $salasana;
                }

                // Verrataan tietokantaan tallennettua suolattua
                // salasanaan nyt syötettyyn suolattuun salasanaan.
                if($salasanaRivi['salasana'] != $hash) {
                    $viesti = "Sähköpostiosoite tai salasana väärin.";
                }
                else {
                    $rooliTarkastus = pg_query("SELECT rooli FROM kayttaja WHERE email = '$email'");
                    $salasanaRivi = pg_fetch_assoc($rooliTarkastus);
                    
                    $tulos = pg_query("SELECT kayttaja_id FROM kayttaja WHERE email = '$email'");
                    $kayttaja_id = pg_fetch_row($tulos);
                    $_SESSION['id'] = $kayttaja_id[0];
                    
                    if ($salasanaRivi['rooli'] == 'asiakas') {
                        $_SESSION['rooli'] = 'asiakas';
                        $_SESSION['ostoskori'] = array();
                        header("Location: haku.php");
                    }
                    else if ($salasanaRivi['rooli'] == 'yllapitaja') {                    
                        $kysely = pg_prepare($yhteys, "kysely3",
                            "SELECT divari.divari_id 
                             FROM yllapitaja, kayttaja, divari 
                             WHERE 
                                divari.divari_id = yllapitaja.divari_id AND
                                yllapitaja_id =  kayttaja_id AND 
                                kayttaja.email = $1");
                        $kysely = pg_execute($yhteys, "kysely3", array($email));

                        $temp = pg_fetch_row($kysely);
                        $divariId = $temp[0];

                        $_SESSION['divari_id'] = $divariId;
                        $_SESSION['rooli'] = 'admin';
                        header("Location: admin.php");
                    }
                }
            }
        }
        unset($email, $salasana);
    }
    else if (isset($_POST['rekisteroidy'])) {
        header("Location: rekisteroityminen.php");
    }


    pg_close($yhteys);
?>

<html>
    <head>
        <title>Kirjaudu sisään</title>
        <link rel="stylesheet" type="text/css" href="tyylit.css?d=<?php echo time(); ?>">
    </head>

    <?php include("header.php")?>

    <body>
        <form action="kirjautuminen.php" method="post">

        <?php if (isset($viesti)) echo '<p style="color:red">'.$viesti.'</p>'; ?>

            <table>
                <tr>
                    <td>Sähköpostiosoite: </td>
                    <td><input type="email" name="email" value=""/></td>
                </tr>

                <tr>
                    <td>Salasana: </td>
                    <td><input type ="password" name="salasana" value="" /></td>
                </tr>
            </table>

                <input type="hidden" name="tallenna" value="hidden"/>
                <input type="submit" name="kirjaudu" value="Kirjaudu"/>
        </form>
        <a href="rekisteroityminen.php">Rekisteröidy</a>
    </body>
    
</html>
