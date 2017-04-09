# Tietoliikenneprojekti - dokumentaatio

### 1. Johdanto
Tämä tietoliikenneprojekti sisältää C-kielellä ohjelmoidut asiakas- ja palvelinohjelmat, joiden avulla voidaan käydä reaaliaikaista tesktiviestintää internetissä usean yhtäaikaisen käyttäjän ryhmissä. Ohjelman käyttö perustuu IRC-protokollaan ja sitä käytetään UNIX-ympäristössä terminaali-ohjelmalla.

### 2. Toteutus ja rakenne
Ohjelma sisältää erilliset ohjelmat palvelimelle ja asiakkaalle.

#### Palvelimen ohjelmamoduulit
##### main
Palvelimen main-funktio kuuntelee TCP-yhteyksiä kaikista osoitteista ikuisessa silmukassa. Aina kun jostain tulee yhteyspyyntö palvelimelle, palvelin avaa uuden pistokkeen ja alkaa kuunnella tietoliikennettä asiakkaalta. Jokaista yhteyttä varten avataan oma säie (thread), jotta niitä voidaan kuunnellaan rinnakkain.

##### client
Kun asiakas yhdistyy palvelimelle, luodaan uusi Client-rakenne, joka sisältää asiakkaan keskeiset tiedot, kuten nimen, nkyisen kanavan sekä pistokkeen ja säikeen kuvaajat. Client-rakenteet tallennetaan pino-tietorakenteeseen (stack), jotta ne voidaan käydä tarvittaessa läpi.

##### connect
Connect-moduulissa on toteutettu logiikka asiakkaan lähettämien komentojen vastaanottamiseen. Tässä moduulissa olevaa funktiota client_thread kutsutaan main-funktiosta jokaiselle palvelimelle liittyvälle asiakkaalle. Jos asiakas lähettää komennon, funktio reagoi kutsumalla moduulista command_handlers sopivaa funktiota, joka huolehtii, että asiakkaan komento käsitellään oikein ja asiakas saa vastauksen.

##### channel
Palvelimella olevat kanavat tallennetaan Channel-rakenteeseen, joka pitää kirjaa kanavalla olevista asiakkaista. Client sisältää kanavan nimen sekä muistiosoitteen pinoon, johon on tallennettu kanavalle liittyneet asiakkaat. Aina kun asiakas liittyy kanavalle, lisätään asiakas kyseisen kanavan pinoon ja poistuessa asiakas vastaavasti poistetaan pinosta. Uusi kanava luodaan, jos asiakas liittyy kanavalle, jota ei vielä ole.


##### command_handlers
command_handlers moduuli sisältää funktiot, jotka huolehtivat asiakkaan lähettämien komentojen täytäntöönpanosta ja asiakkaalle vastaamisesta.

##### utility
utility sisältää sekalaisen joukon apufuktioita toistuviin tehtäviin.

##### config
config sisältää palvelimen asetuksia, kuten tervetuloa-viestin, oletusnimen, valmiina olevat kanavat yms.

#### Asiakkaan ohjelmamoduulit
##### main



- TCP kommunikaatio
- useamman threadin socket

### 3. Käyttöohjeet
##### 3.1. Käännä C-tiedostot
Asiakas- ja palvelinohjelmat ovat hakemistoissa <i>/client</i> ja <i>/server</i>. Kummallakin ohjelmalla on oma make-tiedosto, ja ne käännetään suorittamalla ohjelman hakemistossa komento <code>make</code>.

><code>cd client/ && make</code>

><code>cd server/ && make</code>

##### 3.2. Käynnistä palvelin
Palvelin käynnistetään suorittamalla <i>server/</i> hakemistossa <code>./main [portti]</code>. Portti on se palvelimen portti, johon ohjelma odottaa yhteyksiä käynnistyttyään. Esimerkiksi:

> <code>./main 8000</code>

##### 3.3. Käynnistä asiakasohjelma
Kun palvelin on käynnissä, asiakasohjelmalla voidaan ydistää palvelimeen. Tätä varten on avattava toinen terminaali-ikkuna. Ohjelma käynnistetään <i>client/</i> hakemistossa komennolla <code>./main [osoite] [portti] [nimimerkki]</code>. Osoite on palvelimen DNS-osoite, portti on se portti, jota palvelin asetettiin kuuntelemaan ja nimimerkki on nimi, jolla käyttäjä näkyy muille käyttäjille. Esimerkiksi:

> <code>./main localhost 8000 Veikko</code>

##### 3.4. Keskustelu ohjelmassa
Keskustelu ohjelmassa tapahtuu kanavilla (Channel). Kun asiakasohjelma on yhdistänyt palvelimelle, ohjelmaa käytetään seuraavien komentojen avulla:


| Komento           | Toiminto                                 |
| ------------------| -----------------------------------------|
| /NICK [nimimerkki]| Vaihtaa nimimerkin                       |
| /LIST             | Listaa kaikki kanavat                    |
| /JOIN [kanava]    | Liittää käyttäjän kanavalle              |
| /PART             | Poistaa käyttäjän nykyiseltä kanavaltaan |

Kun käyttäjä on liittynyt kanavalle komennolla <code>/JOIN</code>, keskustelu tapahtuu kirjoittamalla viestin ja painamalla enteriä.

##### 3.5 Esimerkki asiakasohjelman käytöstä

```
% ./main localhost 8001 Veikko

### Hello! Welcome to chat. ###

List of commands:
/NICK [nickname] - change your nickname
/LIST            - List all channels
/JOIN [channel]  - Join channel
/PART            - Leave your current channel

% /NICK VK
You are now known as VK

% /LIST
--- Channels:
Kanava3 (1)
HappyGarden (0)

/JOIN Kanava3
--- You are now on channel Kanava3
--- members are:
VK
Joona

% Moro!
Joona: no moi!

% /PART
--- Left channel
```

### 4. Protokolla

### 5. Testaus

### 6. Toiminta
