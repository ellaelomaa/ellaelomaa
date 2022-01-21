/* Kurssin harjoitustyö.
 * Rasse2
 * Kuvaus:
 * Ohjelma lukee tiedostosta riveittän Rassen linjaston tiedot. Linjastoon
 * tallennetaan linjan nimi, pysäkin nimi, ja pysäkin etäisyys linjan
 * lähtöpysäkistä.Tiedostoon nämä tiedot on eroteltu puolipisteellä toisistaan.
 * Ohjelma myös tarkistaa, onko tiedosto oikein muotoiltu. Mikäli tiedoston
 * tai pysäkin nimeen kuuluu useampi kuin yksi osa, laitetaan nimi
 * lainausmerkkeihin. Tiedostossa linjan pysäkkien järjestyksellä ei ole
 * väliä, kunhan ne tallennetaan ohjelman tietorakenteeseen oikein.
 *
 * Käyttäjä syöttää ohjelmaan komentoja. Komennoilla pitää olla 0-3 parametria.
 * Mikäli komento on virheellinen tai siinä mainittua linjaa tai pysäkkiä ei
 * löydy, tulostetaan virheilmoitus. Komentoja on yhdeksän erilaista:
 * 1. QUIT päättää ohjelman suorituksen.
 * 2. LINES tulostaa kaikki linjat aakkosjärjestyksessä allekkain.
 * 3. LINE <linja> tulostaa kaikki tietyn linjan pysäkit reitin
 * mukaisessa järjestyksessä.
 * 4. STOPS tulostaa kaikki linjaston pysäkit aakkosjärjestyksessä allekkain.
 * 5. STOP <pysäkki> tulostaa kaikki linjat, joille tietty pysäkki kuuluu.
 * 6. DISTANCE<linja><pysäkki1><pysäkki2> laskee tietyllä linjalla
 * kahden annetun pysäkin välisen etäisyyden.
 * 7. ADDLINE <linja> lisää linjastoon linjan.
 * 8. ADDSTOP <linja>pysäkki><etäisyys> Lisää tietylle linjalle uuden pysäkin
 * ja sen etäisyyden aloituspysäkistä oikeaan kohtaan reitille.
 * 9. REMOVE <pysakki> poistaa tietyn pysäkin kaikilta linjoilta.
 *
 * Ohjelman toimintoa jatketaan niin kauan kunnes käyttäjä syöttää komennon
 * QUIT. Komentojen kirjainten koolla ei ole merkitystä. Muutoksia ei
 * kirjoiteta tiedostoon, vaan en toteutetaan ainoastaan ohjelman omassa
 * tietorakenteessa.
 *
**/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>

using namespace std;

using Rasse = map<string, vector<pair<string, string>>>;
const string ERROR = "Error: Stop/line already exists.";
const string ALREADY_EXISTS = "Error: Stop/line already exists.";
const string LINE_ERROR = "Error: Line could not be found.";
const string STOP_ERROR = "Error: Stop could not be found.";
const string FORMAT_ERROR = "Error: Invalid format in file.";

// Funktio, jossa luettavan tiedoston rivit lisätään vektoriin,
// jota käytetään tietorakenteseen tallentamiseen.
vector<string> split(const string& line, const char sep) {
    string temp = line;
    vector<string> result;
    string new_part = "";

    // Jatketaan silmukkaa niin kauan kuin erottajamerkki löytyy.
    while (temp.find(sep) != std::string::npos) {

        // Lisätään apumuuttujaan merkkijono rivin alusta
        // erottajamerkkiin asti.
        new_part = temp.substr(0, temp.find(sep));

        // Poistetaan rivin alusta merkkejä erottajamerkkiin asti,
        // erottajamerkki mukaan lukien.
        temp = temp.substr(temp.find(sep)+1, temp.size());

        // Listäään apumuuttujan merkkijono vektoriin.
        result.push_back(new_part);
    }

    if (temp.find(sep) == std::string::npos) {
        // Mikäli erottajamerkkiä ei löydy, mutta rivi ei ole tyhjä.
        if (!temp.empty()) {
            new_part = temp.substr(0);
            result.push_back(new_part);
            // Tarkistetaan palautettavan vektorin koko, ja lisätään loppuun 0,
            // jos vektorilta puuttuu kolmas osa.
            if (result.size() == 2) {
                new_part = "0";
                result.push_back(new_part);

            }

        }
        else {
            if (result.size() == 2) {
                new_part ="0";
                result.push_back(new_part);

            }
        }
    }

    return result;
}

// Funktio, jossa tulostetaan kaikkien linjojen nimet.
void printAllLines(const Rasse& rasse) {
    cout << "All tramlines in alphabetical order:" << endl;
    // Käydään tietorakenteen avaimet lävitse.
    for (auto&x : rasse) {
        // Tulostetaan vain sanakirjan avain, eli linjan nimi.
        // Sanakirja järjestyy itsestään aakkosjärjestykseen.
        cout << x.first << endl;
    }
}

// Funktio, jossa käyttäjän syöte pilkotaan vektoriin.
// Vektorin osia käytetään komentojen tunnistamisessa pääohjelmassa.
vector<string> splitCommand(string command) {
    // Alustetaan muuttujat ja palautettava vektori.
    vector<string> result;
    string temp = command;
    char sep = ' ';
    string new_part = "";

    // Jatketaan niin kauan, kuin käyttäjän syöttämiä osia löytyy.
    while (temp.find(sep) != std::string::npos) {
        // Jaetaan syöte osiin välilyönnin kohdalta.
        // Mikäli komentovektori on tyhjä, ensimmäinen käsiteltävä osa on komento.
        if (result.size() == 0) {
            new_part = temp.substr(0, temp.find(sep));
            // Muutetaan komento isoiksi kirjaimiksi.
            transform(new_part.begin(), new_part.end(), new_part.begin(),
                      ::toupper);
            // Poistetaan jo käsitelty komennon osa merkkijonosta.
            temp = temp.substr(temp.find(sep)+1, temp.size());
            result.push_back(new_part);

        }
        else {
            if (temp.at(0) == '"') {
                temp = temp.substr(1, temp.size());
                string::size_type second = temp.find('"');
                new_part = temp.substr(0, second);
                if (second == temp.size()-1) {
                    temp.clear();
                }
                else {
                    temp = temp.substr(second + 2, temp.size());
                }
                result.push_back(new_part);


            }
            else {
                // Lisätään uusi osa vektoriin.
                new_part = temp.substr(0, temp.find(sep));
                // Muokataan rivin kopiota siten, että
                // poistetaan apumuuttujaan jo lisätty osa.
                temp = temp.substr(temp.find(sep)+1, temp.size());
                result.push_back(new_part);
            }
        }
    }
    // Mikäli välimerkkejä ei enää löydy, lisätään rivin loppu vektoriin.
    if (temp.find(sep) == std::string::npos) {
       // Tarkistetaan, onko apumuuttuja tyhjä.
        if (!temp.empty()) {
           // Mikäli komentovektori on tyhjä, komentoon kuuluu vain yksi osa.
           // Muutetaan se isoiksi kirjaimiksi.
           if (result.size() == 0) {
               new_part = temp.substr(0);
               transform(new_part.begin(), new_part.end(), new_part.begin(),
                         ::toupper);
               result.push_back(new_part);
           }
           // Muussa tapauksessa lisätään osa sellaisenaan.
           else {
               if (temp.at(0) == '"') {
                   new_part = temp.substr(1, temp.size() - 1);
                   result.push_back(new_part);
               }
               else {
                   new_part = temp.substr(0);
                   result.push_back(new_part);
               }
           }
       }
    }
    return result;
}

// Funktio, joka tulostaa linjan kaikki pysäkit allekkain.
void printLine(const Rasse& rasse, string lineName) {
    // Tarkistetaan, löytyykö käyttäjän syöttämää linjaa tietorakenteesta.
    if (rasse.find(lineName) != rasse.end()) {
        cout << "Line " << lineName << " goes through these stops " <<
                                       "in the order they are listed:" << endl;
        // Käydään tietorakenteen linjat läpi ja etsitään oikea.
        for (auto line : rasse) {
            // Verrataan linjojen nimiä käyttäjän syötteeseen.
            if (line.first == lineName) {
                // Käydään valitun linjan kaikki pysäkit läpi.
                for (auto stop : line.second) {
                    // Tulostetaan pysäkit, jos niitä löytyy.
                    if (!stop.first.empty()) {
                        cout << " - " << stop.first << " : " <<
                                stop.second << endl;
                    }
                }
            }
        }
    }
    // Mikäli linjaa ei löydy, tulostetaan virheilmoitus.
    else {
        cout << LINE_ERROR << endl;
    }
}

// Funktio, joka lisää vektoriin kaikki pysäkit.
// Käytetään printAllStops-funktiossa.
vector<string> allStops(const Rasse& rasse) {
    // Luodaan vektori pysäkeistä.
    vector<string> allRasse;
    // Käydään kaikki linjat läpi.
    for (auto line : rasse) {
        // Käydään kaikki linjan pysäkit läpi.
        for (auto stop : line.second) {
            // Mikäli pysäkin nimeä ei vielä kaikki pysäkit listaavasta
            // vektorista löydy, lisätään se sinne.
            if (find(allRasse.begin(), allRasse.end(), stop.first) ==
                    allRasse.end()) {
                if (!stop.first.empty()) {
                    allRasse.push_back(stop.first);
                }
            }
        }
    }

    return allRasse;
}

// Funktio, joka tulostaa kaikki pysäkit allekkain aakkosjärjestyksessä.
void printAllStops(const Rasse& rasse) {
    // Luodaan vektori pysäkeistä.
    vector<string> stops = allStops(rasse);

    // Lajitellaan vektori.
    sort(stops.begin(), stops.end());

    cout << "All stops in alphabetical order:" << endl;
    // Tulostetaan vektori.
    for (string stop : stops) {
        cout << stop << endl;
    }
}

// Funktio, joka selvittää onko tietyllä linjalla tiettyä pysäkkiä.
bool findStopOnLine(const Rasse& rasse, string lineName, string stopName) {
    // Luodaan vektori, jonne lisätään kaikki linjan pysäkit.
    vector<string> lineRasse;
    for (auto line : rasse) {
        if (line.first == lineName) {
            for (auto stop : line.second) {
                if (find(lineRasse.begin(), lineRasse.end(), stop.first)
                        == lineRasse.end()) {
                    lineRasse.push_back(stop.first);
                }
            }
        }
    }
    // Tarkistetaan löytyikö pysäkkiä linjalta ja sijoitetaan tulos muuttujaan.
    bool found = find(lineRasse.begin(), lineRasse.end(), stopName) !=
            lineRasse.end();

    return found;
}

// Funktio, joka tulostaa kaikki linjat, joilta tietty pysäkki löytyy.
void printLinesWhereStop(const Rasse& rasse, string stopName) {
    vector<string> foundLines;
    // Kutsutaan funktiota, joka listaa kaikki pysäkit.
    vector<string> stops = allStops(rasse);

    // Etsitään käyttäjän syöttämää pysäkkiä pysäkkien listalta.
    if (find(stops.begin(), stops.end(), stopName) != stops.end()) {
        cout << "Stop " << stopName << " can be found on the following lines:"
             << endl;
        // Käydään kaikki linjat läpi ja etsitään pysäkki.
        for (auto line : rasse) {
            for (auto stop : line.second) {
                if (stop.first == stopName) {
                    // Lisätään pysäkki löydettyjen linjojen vektoriin.
                    foundLines.push_back(line.first);
                }
            }
        }
        // Lajitellaan vektori.
        sort(foundLines.begin(), foundLines.end());

        // Tulostetaan vektori.
        for (string lineName : foundLines) {
            cout << "- " << lineName << endl;
        }
    }
    // Mikäli pysäkkiä ei löytynyt, tulostetaan virheilmoitus.
    else {
        cout << STOP_ERROR << endl;
    }
}

// Funktio, joka laskee kahden pysäkin välisen etäisyyden.
void distance(const Rasse& rasse, string lineName, string firstStop,
              string secondStop) {
    if (rasse.find(lineName) != rasse.end()) {
        // Tarkistetaan löytyykö ensimmäinen pysäkki linjalta.
        if (findStopOnLine(rasse, lineName, firstStop)) {
            // Tarkistetaan löytyykö toinen pysäkki linjalta.
            if (findStopOnLine(rasse, lineName, secondStop)) {
                cout << "Distance between " << firstStop << " and "
                     << secondStop << " is ";
                if (firstStop == secondStop) {
                    cout << "0" << endl;
                }
                else {
                    // Alustetaan muuttujat.
                    double first = 0;
                    double second = 0;
                    double result = 0;
                    for (auto line : rasse) {
                        // Mikäli linjan nimi vastaa komennon linjaa, käydään
                        // sen pysäkit läpi.
                        if (line.first == lineName) {
                            // Jokaisen pysäkin nimeä verrataan parametreina
                            // annettuihin nimiin. Mikäli merkkijonot
                            // vastaavat toisiaan, sijoitetaan ne muuttujiin.
                            for (auto stop : line.second) {
                                if (stop.first == firstStop) {
                                    first = stod(stop.second);
                                }
                                else if (stop.first == secondStop) {
                                    second = stod(stop.second);
                                }
                            }
                        }
                    }
                    // Lasketaan etäisyys ja tulostetaan vastaus.
                    if (first > second) {
                        result = first - second;
                        cout << result << endl;
                    }
                    else if (second > first) {
                        result = second - first;
                        cout << result << endl;
                    }
                }

            }
            else {
                cout << STOP_ERROR << endl;
            }
        }
        else {
            cout << STOP_ERROR << endl;
        }
    }
    else {
        cout << LINE_ERROR << endl;
    }
}

// Funktio, joka lisää linjan tietorakenteeseen.
void addLine(Rasse& rasse, string lineName) {
    // Tarkistetaan löytyykö samanniminen linja jo tietorakenteesta.
    if (rasse.count(lineName) == 0) {
        // Lisätään linja ja alustetaan linjan tiedot tyhjillä merkkijonoilla.
        rasse.insert( { lineName, {{"", ""}} });
        cout << "Line was added." << endl;
    }
    else {
        // Mikäli linja jo löytyy, tulostetaan virheilmoitus
        cout << ALREADY_EXISTS << endl;
    }
}

bool checkDistance(const Rasse& rasse, string lineName) {
    // Luodaan vektori, jonne lisätään kaikki linjan pysäkkien etäisyydet
    // aloituspysäkistä.
    vector<string> distances;
    for (auto line : rasse) {
        if (line.first == lineName) {
            for (auto stop : line.second) {
                if (find(distances.begin(), distances.end(), stop.first) ==
                        distances.end()) {
                    distances.push_back(stop.second);
                    cout << stop.second << endl;
                }
            }
        }
    }
    int duplicates = 0;
    for (auto check : distances) {
        if (find(distances.begin(), distances.end(), check) !=
                distances.end()) {
            duplicates++;
            cout << duplicates << endl;
        }
    }

    if (duplicates == 0) {
        return false;
    }
    else {
        return true;
    }
}

// Apufunktio, jota käytetään addStop-funktion lajittelussa.
bool sortbysec (const pair<string, string> &a, const pair<string, string> &b) {
    return (a.second < b.second);
}

// Funktio, jossa tietylle linjalle lisätään uusi pysäkki.
void addStop(Rasse& rasse, string lineName, string stopName, string distance) {
    bool success = false;

    // Tarkistetaan löytyykö linjalta jo samanniminen pysäkki.
    if (rasse.find(lineName) != rasse.end()) {
        // Kutsutaan funktiota, joka etsii linjalta pysäkin.
        bool alreadyExists = findStopOnLine(rasse, lineName, stopName);
        // Jatketaan, jos pysäkkiä ei linjalta löydy.
        if (!alreadyExists) {
            // Käydään tietorakenne silmukalla läpi.
            for (auto line : rasse) {
                // Valitaan oikea linja.
                if (line.first == lineName) {
                    rasse.at(lineName).push_back(make_pair(stopName, distance));
                    sort(rasse.at(lineName).begin(), rasse.at(lineName).end(),
                         sortbysec);
                    success = true;
                }
            }
        }
        // Mikäli pysäkki löytyy, tulostetaan virheilmoitus.
        else {
            cout << ALREADY_EXISTS << endl;
        }
    }
    // Mikäli linjaa ei löydy, tulostetaan virheilmoitus.
    else {
        cout << LINE_ERROR << endl;
    }

    if (success == true ) {
        cout << "Stop was added." << endl;
    }
}

// Funktio, jossa poistetaan tietty pysäkki kaikilta linjoilta.
void removeStop(Rasse& rasse, string stopName) {
    // Muuttuja, jota käytetään onnistumisviestin tulostamisen apuna.
    bool success = false;
    vector<string> listOfRasse = allStops(rasse);
    // Katsotaan löytyykö pysäkkiä linjastosta.
    if (find(listOfRasse.begin(), listOfRasse.end(), stopName) !=
            listOfRasse.end()) {
        // Käydään linja kerrallaan läpi.
        for (auto &line : rasse) {

            // Käydään jokaisen linjan jokainen pysäkki läpi.
            for (std::size_t i = 0; i < line.second.size(); ++i) {

                // Tarkistetaan, vastaako pysäkin nimi poistettavan
                // pysäkin nimeä.
                if (line.second.at(i).first == stopName) {
                    line.second.erase(line.second.begin() + i);
                    // Muutetaan apumuuttujan tila todeksi.
                    success = true;
                }
            }
        }
    }
    // Mikäli pysäkkiä ei löydy, tulostetaan virheilmoitus.
    else {
        cout << STOP_ERROR << endl;
    }

    // Mikäli ainakin yksi poistettava pysäkki löytyi,
    // tulostetaan onnistumisviesti.
    if (success == true ) {
        cout << "Stop was removed from all lines." << endl;
    }
}

// The most magnificent function in this whole program.
// Prints a RASSE
void print_rasse()
{
    std::cout <<
                 "=====//==================//===\n"
                 "  __<<__________________<<__   \n"
                 " | ____ ____ ____ ____ ____ |  \n"
                 " | |  | |  | |  | |  | |  | |  \n"
                 " |_|__|_|__|_|__|_|__|_|__|_|  \n"
                 ".|                  RASSE   |. \n"
                 ":|__________________________|: \n"
                 "___(o)(o)___(o)(o)___(o)(o)____\n"
                 "-------------------------------" << std::endl;
}

// Funktio, jossa tiedosto luetaan ja tallennetaan tietorakenteeseen.
bool parseFile(Rasse& rasse, string fileName, vector<string> parts) {
    ifstream fileObj(fileName);
    // Avataan tiedosto onnistuneesti.
    if (fileObj) {
        // Toistorakenteessa luetaan tiedoston rivit.
        string line = "";
        while(getline(fileObj, line)) {
            /*
             * Jaetaan rivi vektoriin split-funktiossa, joka myös
             * lisää rivin tiedot tietorakenteeseen.
             * Parametreiksi annetaan rivi ja
             * erottajamerkiksi puolipiste.
            */
            parts = split(line, ';');
            if (parts.size() != 3) {
                cout << FORMAT_ERROR << endl;
                return false;
            }
            else {
                // Sijoitetaan rivin osat muuttujiin.
                string line = parts.at(0);
                string stop = parts.at(1);
                string distance = parts.at(2);

                // Mikäli linjaa ei vielä löydy, luodaan sellainen.
                if (rasse.count(line) == 0) {
                    rasse[line] = {};
                }

                // Lisätään oikealle linjalle pysäkki+etäisyys -pari.
                rasse.at(line).push_back(make_pair(stop, distance));
            }

        }
        return true;
    }
    else {
        cout << "Error: File could not be read." << endl;
        return false;
    }
}

// Funktio, joka lukee käyttäjän syötteet ja niiden perusteella ohjaa
// muihin funktioihin.
bool readCommand(Rasse& rasse) {
    while (true) {
    string command = "";
    cout << "tramway> ";
    getline(cin, command);

    // Jaetaan käyttäjän antama syöte osiin funktiossa.
    vector<string> commandParts = splitCommand(command);
        if (commandParts.size() == 1) {
            if (commandParts.at(0) == "LINES") {
                printAllLines(rasse);
            }
            else if (commandParts.at(0) == "QUIT") {
                return false;
            }
            else if (commandParts.at(0) == "STOPS") {
                printAllStops(rasse);
            }
            else {
                cout << "Error: Invalid input." << endl;
            }
        }
        else if (commandParts.size() == 2) {
            if (commandParts.at(0) == "LINE") {
                string lineName = commandParts.at(1);
                printLine(rasse, lineName);
            }
            else if (commandParts.at(0) == "STOP") {
                string stopName = commandParts.at(1);
                printLinesWhereStop(rasse, stopName);
            }
            else if (commandParts.at(0) == "ADDLINE") {
                string line = commandParts.at(1);
                addLine(rasse, line);
            }
            else if (commandParts.at(0) == "REMOVE") {
                string stop = commandParts.at(1);
                removeStop(rasse, stop);
            }
            else {
                cout << "Error: Invalid input." << endl;
            }
        }
        else if (commandParts.size() == 4) {
            if (commandParts.at(0) == "DISTANCE") {
                string line = commandParts.at(1);
                string firstStop = commandParts.at(2);
                string secondStop = commandParts.at(3);
                distance(rasse, line, firstStop, secondStop);
            }
            else if (commandParts.at(0) == "ADDSTOP") {
                string line = commandParts.at(1);
                string stopName = commandParts.at(2);
                string distance = commandParts.at(3);
                addStop(rasse, line, stopName, distance);
            }
            else {
                cout << "Error: Invalid input." << endl;
            }
        }
        else {
            cout << "Error: Invalid input." << endl;
        }
    }

}

//Funktio, joka tarkistaa tietorakenteeseen tallennettujen
// tietojen oikeellisuuden.
bool checkRasseValidity(Rasse& rasse) {
    // Luodaan vektorit, johon tallennetaan jo löydetyt nimet vertailuun.
    vector<string> lineNames;
    vector<string> stopNames;
    vector<string> distances;
    // Muuttuja, joka laskee virheiden määrän. Tämän avulla palautetaan
    // true tai false.
    int errors = 0;

    // Aluksi käyn kaikki linjojen nimet läpi. Olisin voinut lisätä
    // kaikki tarkastukset
    // sisäkkäisiin for-each -silmukoihin, mutta ymmärsin koodia paremmin, kun
    // jaoin ne kahteen osaan.
    for (auto line : rasse) {
        if (find(lineNames.begin(), lineNames.end(), line.first) ==
                lineNames.end()) {
            // Tarkistetaan, onko linjan nimi tyhjä.
            if (line.first != "") {
                lineNames.push_back(line.first);
            }
            else {
                errors++;
            }
        }
        else {
            errors++;
        }
    }
    // Sitten käydään pysäkkien tiedot samalla tavalla läpi.
    for (auto line : rasse) {
        for (auto stop : line.second) {
            if (find(stopNames.begin(), stopNames.end(), stop.first) ==
                    stopNames.end()) {
                if (stop.first != "") {
                    stopNames.push_back(stop.first);
                    if (find(distances.begin(), distances.end(), stop.second)
                            == distances.end()) {
                        distances.push_back(stop.second);
                    }
                    else {
                        errors++;
                    }
                }
                else {
                    errors++;
                }
            }
            else {
                errors++;
            }
        }
        // Tyhjennetään apuvektorit joka linjan jälkeen.
        stopNames.clear();
        distances.clear();
    }

    if (errors > 0) {
        cout << FORMAT_ERROR << endl;
        return false;
    }
    else {
        return true;
    }
}

// Short and sweet main.
int main()
{
    print_rasse();
    cout << "Give a name for input file: ";
    string fileName = "";
    getline(cin, fileName);

    // Valitaan tietorakenteeksi map, jossa ensimmäinen tieto on linjan nimi ja
    // toinen pysäkin nimestä ja etäisyydestä muodostuva vektori. Tallennetaan
    // etäisyys myöskin merkkijonona, joka voidaan muuttaa numeroksi,
    // koska etäisyys voi tiedostossa olla joko null, double tai int.
    map<string, vector<pair<string, string>>> rasse;
    vector<string> parts;

    // Tiedoston avaaminen epäonnistuu.
    if (!parseFile(rasse, fileName, parts)) {
        return EXIT_FAILURE;
    }

    if (checkRasseValidity(rasse)) {
        // Jatketaan käyttäjän komentojen lukemista, kunnes palautetaan false.)
        if (!readCommand(rasse)) {
            return EXIT_SUCCESS;
        }
    }
    else if (!checkRasseValidity(rasse)) {
        return EXIT_FAILURE;
    }
}
