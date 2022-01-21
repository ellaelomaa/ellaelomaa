/*
* Harjoitustyö, jossa käsitellään tekstitiedostossa olevia ASCII-grafiikana olevia kuvia.
*/

import java.util.Scanner;
import java.io.*;

public class BimEdit {

    // Luodaan kuva metodissa. Parametreina pääohjelmassa annettu ohjelman nimi sekä
    // pääohjelmassa luodut tyhjt taulukot, jotka täytetään metodissa.
    public static char[][] luoKuva (String nimi, char[] varit, int[] tiedot) {
        // Kutsutaan Scanner-luokkkaa try-lohkon ulkoupuolella, jotta
        // sitä voidaan käyttää myös catch-lohkossa.
        Scanner tiedostonLukija = null;
        
        try {
            // Luodaan tiedostolle olio ja tiedostonlukija
            File tiedosto = new File(nimi);
            tiedostonLukija = new Scanner(tiedosto);
            
            // Täytetään pääohjelmassa luodut taulukot. 
            // Tiedämme etukäteen merkkien järjestyksen, joten taulukot voidaan
            // täyttää helposti lukemalla aina seuraavan rivin merkki.
            tiedot[0] = Integer.parseInt(tiedostonLukija.nextLine());
            tiedot[1] = Integer.parseInt(tiedostonLukija.nextLine());    
            
            varit[0] = tiedostonLukija.nextLine().charAt(0);
            varit[1] = tiedostonLukija.nextLine().charAt(0);         
            
            // Luodaan kuvalle olio.
            char[][] kuva = new char[tiedot[0]][tiedot[1]];
            
            // Alustetaan rivilaskuri.
            int riviInd = 0;
            
            // Luodaan silmukka, jonka avulla taulukko täytetään.
            while (tiedostonLukija.hasNextLine()) {
                // Luodaan String-tyyppinen apumuuttuja. Muuttujaan sijoitetaan
                // tiedostosta luettu rivi, ja String-muuttujan avulla täytetään taulukko.
                String rivi = tiedostonLukija.nextLine();
                
                // Jos rivi on pidempi kuin ennakkoehdoissa annettu, palautetaan null.
                if (rivi.length() > tiedot[1]) {
                    return null;
                }
                else {
                // Alustetaan sarakelaskuri.
                    int sarakeInd = 0;
                    
                    while (sarakeInd < tiedot[1]) {
                        kuva[riviInd][sarakeInd] = rivi.charAt(sarakeInd);
                        sarakeInd++;
                        }
                }
                riviInd++;
            }
            // Kun kaikki rivit on käyty läpi, suljetaan tiedostonlukija.
            tiedostonLukija.close();
            return kuva;
        }
        // Minkä tahansa virheen sattuessa suljetaan tiedostonlukija ja palautaan tyhjäarvo.
        catch (Exception e) {
            return null;
        }
    }
    
    // Luodaan kuvan oikeellisuuden tarkistava metodi.
    public static boolean tarkistaKuva(char[][] kuva, char[] varit, int[] tiedot) {
    
        if (kuva == null) {
            return false;
        }
        // Käydään for-silmukassa solu ja rivi kerrallaan taulukko läpi.
        else {
            for (int riviInd = 0; riviInd < kuva.length; riviInd++) {
                for (int sarakeInd = 0; sarakeInd < kuva[riviInd].length; sarakeInd++) {
        
                // Jos kuvassa on muita kuin ennakkotiedoissa annetut merkit, palautetaan false.
                    if (kuva[riviInd][sarakeInd] != varit[0] && kuva[riviInd][sarakeInd] != varit[1]) {
                        return false;
                    }
                }
            }
            // Jos kuvan rivien määrä ei vastaa ennakkotiedoissa annettua riven määrää,
            // palautetaan false.
            if (kuva.length != tiedot[0]) {
                return false;
            }
            // Muusssa tapauksessa kuva on oikein ja palautetaan true.
            else {
                return true;
            }
        }
    }

    // Luodaan tulostava metodi. Tulosteaan taulukon solut yksi kerrallaan ja vaihdetaan välissä riviä.
    public static void tulosta(char[][] taulu) {
        if (taulu != null) {
            for (int riviInd = 0; riviInd < (taulu.length); riviInd++) {
                for (int sarakeInd = 0; sarakeInd < taulu[riviInd].length; sarakeInd++) {
                        System.out.print(taulu[riviInd][sarakeInd]);                   
                }
                System.out.println();
            }
        }
    }

    // Luodaan kuvan tiedot tulostava metodi.
    public static void info(char[][] kuva, char[] varit, int[] tiedot) {
        
        // Tulostetaan kuvan koko.
        System.out.println(tiedot[0] + " x " + tiedot[1]);
        
        // Alustetaan merkkien laskurit.
        int taustavmaara = 0;
        int korostusvmaara = 0;
        
        // Lasketaan merkkien määrät solu kerrallaan for-silmukassa.
        for (int i = 0; i < kuva.length; i++) {
            for (int b = 0; b < kuva[i].length; b++) {
                if (kuva[i][b] == varit[0]) {
                    taustavmaara++;
                }
                else if (kuva[i][b] == varit[1]) {
                    korostusvmaara++;
                }
            }
        }
        // Tulostetaan laskujen tulokset.
        System.out.println(varit[0] + " " + taustavmaara);
        System.out.println(varit[1] + " " + korostusvmaara);
    }
    
    
    // Luodaan värit vaihtava metodi.
    public static void invert(char[][] kuva, char[] varit, int[] tiedot) {

        // Luodaan sisäkkäiset for-silmukat, jotka käyvät solun kerrallaan läpi.
        for (int riviInd = 0; riviInd < tiedot[0]; riviInd++) {
            for (int sarakeInd = 0; sarakeInd < tiedot[1]; sarakeInd++) {
                // Ehtolauseissa vaihdetaan merkit päikseen.
                if (kuva[riviInd][sarakeInd] == varit[0]) {
                        kuva[riviInd][sarakeInd] = varit[1];
                    }
                else if (kuva[riviInd][sarakeInd] == varit[1]) {
                    kuva[riviInd][sarakeInd] = varit[0];
                }
            }
        }
        // Luodaan apumuuttujat merkkien vaihtoon.
        char vaihto1 = varit[0];
        char vaihto2 = varit[1];
        varit[1] = vaihto1;
        varit[0] = vaihto2;
    }
    
    // Luodaan kuvataulukon kopioiva metodi, jota tarvitaan dilate-metodissa.
    public static char[][] kopioi2dTaulukko(char[][] taulu, int[] tiedot) {
        // Luodaan taululle olio ja annetaan taulun koko.
        char[][] kopio = new char [tiedot[0]][tiedot[1]];
        
        if (tiedot[0] > 0 && tiedot[1] > 0) {
            
            // Täytetään taulu solu kerrallaan. Sijoitetan uuden taulun soluun alkuperäisen
            // taulun samassa kohdassa sijaitseva merkki.
            for (int riviInd = 0; riviInd < tiedot[0]; riviInd++) {
                for (int sarakeInd = 0; sarakeInd < tiedot[1]; sarakeInd++) {
                    kopio[riviInd][sarakeInd] = taulu[riviInd][sarakeInd];
                }
            }
            return kopio;
        }
        else {
            return null;
        }
    }
    
    // Luodaan taulun suurentava ja pienentävä metodi.
    public static void dilate(char[][] kuva, int koko, char a, char b, int[] tiedot) {
        
        // Luodaan toinen samanlainen taulukko, jotta suurentaminen ja pienentäminen onnistuu oikein.
        char[][] kopio = kopioi2dTaulukko(kuva, tiedot);
        
        // Taululle pitää jäädä reunus. Koska annettu koko on kokonaisluku, myös osamäärä jää kokonaisluvuksi.
        int reunus = koko/2;
        
        // Käydään kopioitu, alkuperäisen kanssa samanlainen taulukko solu kerrallaan läpi.
        for (int riviInd = reunus; riviInd < tiedot[0] - reunus; riviInd++) {
            for (int sarakeInd = reunus; sarakeInd < tiedot[1] - reunus; sarakeInd++) {
                
                // Jos merkki on taustamerkki, luodaan merkin korvaava for-silmukka.
                if (kuva[riviInd][sarakeInd] == a) {
                    
                    // Määritellään käsiteltävä alue.
                    for (int ikkunarivi = riviInd - reunus; ikkunarivi < riviInd + reunus + 1; ikkunarivi++) {
                        for (int ikkunasarake = sarakeInd - reunus; 
                            ikkunasarake < sarakeInd + reunus + 1; ikkunasarake++) {
                            
                            // Jos käsiteltävällä alueella on sopiva merkki, korvataan se.
                            if (kopio[ikkunarivi][ikkunasarake] == b) {
                                kuva[riviInd][sarakeInd] = b;
                            }
                        }
                    }
                }
            }
        }
    }
                

    public static void main(String[] args) {
        Scanner lukija = new Scanner(System.in);
        
        System.out.println("-----------------------");
        System.out.println("| Binary image editor |");
        System.out.println("-----------------------");
        
     // Jos komentoriviparametria ei ole, tulostetaan virheilmoitus ja lopetetaan ohjelma.
        if (args.length == 0) {
            System.out.println("Invalid command-line argument!");
            System.out.println("Bye, see you soon.");
        }
        
        // Jos komentoriviparametrina on vain ohjelman nimi, tai parametrina on
        // ohjelman nimi ja "echo", jatketaan tulostamista.
        else if (args.length == 1 || (args.length == 2 && args[1]== "echo")) {
            
            // Sijoitetaan komentoriviparametrin ensimmäinen osa tiedoston nimen kertovaan muuttujaan.
            String tiedostoNimi = args[0];
            
            // Alustetaan kuvasta kertovat taulukot, jotta ne voidaan täyttää metodissa ja käyttää muualla.
            int[] tiedot = new int[2];
            char[] varit = new char[2];
            char[][] kuva;
            
            // Luodaan kuva metodissa.
            kuva = luoKuva(tiedostoNimi, varit, tiedot);
            
            // Tarkistetaan kuva. Jos kuva ok, jatketaan.
            boolean kuvaOk = tarkistaKuva(kuva, varit, tiedot);
            
            if (kuvaOk) {
                
                // Luodaan boolean-tyyppinen lippumuuttuja.
                boolean jatketaan = true;
                
                while (jatketaan) {
                    System.out.println("print/info/invert/dilate/erode/load/quit?");
                    
                    // Luetaan käyttäjän komento,
                    String syote = lukija.nextLine();
                    
                    // Jaetaan syöte välilyönnin kohdalta, jotta myöhemmin voidaan lukea ikkunan koko.
                    String[] komento = syote.split(" ");
                    
                    // Oikeita yksiosaisia komentoja on viisi. Mikäli jokin näistä tunnistetaan,
                    // suoritetaan metodi. Muussa tapauksessa komento on väärä.
                    if (komento.length == 1) {
                        if (komento[0].equals("quit")) {
                            System.out.println("Bye, see you soon.");
                            jatketaan = false;
                            break;
                        }
                        else if (komento[0].equals("print")) {
                            tulosta(kuva);
                        }
                        else if (komento[0].equals("info")) {
                            info(kuva, varit, tiedot);
                        }
                        else if (komento[0].equals("invert")) {                           
                            invert(kuva, varit, tiedot);
                        }
                        
                        // Load-komento lukee komentoparametrina annetun kuvan uudestaan ja lataa sen.
                        // Kuva siis palautuu ennalleen.
                        else if (komento[0].equals("load")) {
                            kuva = luoKuva(tiedostoNimi, varit, tiedot);
                        }    
                        else {
                            System.out.println("Invalid command!");
                        }
                    }
                    
                    // Jos komennon osia on kaksi, ensimmäisen osan pitää olla joko "dilate" tai "erode"
                    // ja toisen osan kokonaisluku.
                    // Muussa tapauksessa komento on väärä ja tulostetaan virheilmoitus.
                    else if (komento.length == 2) {
                        
                        // Alustetaan ikkunan koosta kertova kokonaistyyppinen
                        // muuttuja ehtolausekkeiden ulkopuolella, jotta sitä voidaan käyttää
                        // molemmissa ehtolauseissa.
                        int koko;
                        
                        if (komento[0].equals("dilate")) {
                            koko = Integer.parseInt(komento[1]);
                            
                            // Kutsutaan metodia vain jos annettu koko on suurempi kuin kaksi ja pariton.
                            if (koko > 2 && koko % 2 == 1 && koko < tiedot[0] && koko < tiedot[1]) {
                                dilate(kuva, koko, varit[0], varit[1], tiedot);
                            }
                            // Muussa tapauksessa koko on väärä. 
                            else {
                                System.out.println("Invalid command!");
                            }
                        }
                        else if (komento[0].equals("erode")) {
                            koko = Integer.parseInt(komento[1]);
                            if (koko > 2 && (koko % 2 == 1) && koko < tiedot[1] && koko < tiedot[0]) {
                                dilate(kuva, koko, varit[1], varit[0], tiedot);
                            }
                            else {
                                System.out.println("Invalid command!");
                            }
                        }
                        // Jos komento ei ole "dilate" tai "erode", komento on väärä.
                        else {
                            System.out.println("Invalid command!");
                        }
                    }
                    else {
                        System.out.println("Invalid command!");
                    }
                }
            }
            // Mikäli tarkistaKuva-metodi palautti falsen, kuvatiedostossa on virhe. 
            // Palautetaan virheilmoitus ja lopetetaan ohjelma.
            else {
                System.out.println("Invalid image file!");
                System.out.println("Bye, see you soon.");
            }
        }
        else {
            System.out.println("Invalid command-line argument!");
            System.out.println("Bye, see you soon.");
        }
    }
}
