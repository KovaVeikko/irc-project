# Tietoliikenneprojekti - dokumentaatio

Veikko Kovanen & Joona Karjula, 2017

### 1. Johdanto
Tämä tietoliikenneprojekti sisältää C-kielellä ohjelmoidut asiakas- ja palvelinohjelmat, joiden avulla voidaan käydä reaaliaikaista tesktiviestintää internetissä usean yhtäaikaisen käyttäjän ryhmissä. Ohjelman käyttö perustuu IRC-protokollaan ja sitä käytetään UNIX-ympäristössä terminaali-ohjelmalla.

### 2. Toteutus ja rakenne
Ohjelma sisältää erilliset ohjelmat palvelimelle (server) ja asiakkaalle (client).

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
Asiakkaan main-funktio hakee ip-osoitteen annetulle DNS-nimelle ja yhdistää palvelimeen käyttäjän ilmoittamaan porttiin. Aluksi asiakasohjelma luo pistokkeen, joka käyttää IPv4 - protokollaa (AF_INET) ja TCP-protokollaa (SOCK_STREAM). Paluuarvo on pistokkeen tunniste, tai -1 jos luominen ei onnistunut. Sen jälkeen ohjelmalle kerrotaan, että osoiteperhe on IPv4 ja muutetaan IP -osoite binääriseksi. Asiaksohjelma ei siis tällähetkellä osaa käsitellä IPv6 osoitteita. Tässä vaiheessa ohjelma avaa TCP yhteyden palvelimeen ja lähettää virhearvon, jos yhteyttä ei saada muodostettua. Tämän jälkeen asiakasohjelma lähettää palvelimelle /USER-komennon, jolla se ilmoittaa käyttäjän nimen palvelimelle. Ilmoituksen jälkeen funktio kuuntelee ikuisessa silmukassa sekä käyttäjältä että palvelimelle avatusta pistokkeesta tulevaa io-virtaa. Käyttäjää sekä palvelinta kuunnellaan vuorotellen Select()-funktion avulla. Käyttäjän lähettämät viestit lähetetään palvelimelle avattuun pistokkeeseen tai pistokkeeseen tulleet viestit tulevat näkyviin käyttäjän näytölle. Ohjelma kuuntelee palvelinta siis niin kauan, kuin ohjelma suljetaan.

##### utility
utility sisältää hostname_to_ip-funktion, joka tekee DNS-kyselyn ja muuttaa DNS-nimen ip-osoitteeksi.

##### config
config sisältää asiakasohjelman asetuksia.

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

Jos osoite ei ole oikein määritetty, lähetetään virheviesti "Failed to get host by name". Tämä tarkoittaa sitä, että kyseistä osoitetta ei ole olemassa, koska IP -osoitetta ei saatu haettua. Jos taas portti on väärin, tulee käyttäjälle lukemaan "connect error: Connection refused". Tällöin IP -osoitteella ei ole pistoketta, joka kuuntelee tätä porttia.

##### 3.4. Keskustelu ohjelmassa
Keskustelu ohjelmassa tapahtuu kanavilla (Channel). Kun asiakasohjelma on yhdistänyt palvelimelle, ohjelmaa käytetään seuraavien komentojen avulla:


| Komento           | Toiminto                                 |
| ------------------| -----------------------------------------|
| /NICK [nimimerkki]| Vaihtaa nimimerkin                       |
| /LIST             | Listaa kaikki kanavat                    |
| /JOIN [kanava]    | Liittää käyttäjän kanavalle              |
| /PART             | Poistaa käyttäjän nykyiseltä kanavaltaan |

Kun käyttäjä on liittynyt kanavalle komennolla <code>/JOIN</code>, keskustelu tapahtuu kirjoittamalla viestin ja painamalla enteriä. Viestit tulevat näkymään niin, että viestin edessä on nimimerkki, jolta viesti on saapunut. Käyttäjä erottaa omat viestinsä helposti, koska oman viestin edessä ei ole nimimerkkiä. Tähän olisi voinut laittaa myös näkymään asiakkaan oma nimimerkki, mutta päädyimme tähän, koska asiakas erottaa omat viestinsä näin helpommin. Asiakkaan nimimerkki näkyy kuitenkin toisille käyttäjille.

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
Ohjelma perustuu IRC-protokollaan (<a href="https://tools.ietf.org/html/rfc1459">RFC 1459</a>), joskin toteuttaa vain pienen osan siitä.

- Ohjelmassa käytettävät komennot (ks. 3.4) ovat vastaavat kuin IRC:ssa, mutta eivät kaikilta osin vastaa protokollassa määriteltyä toiminnallisuutta. Suurinta osaa IRC:ssa määritellyistä komennoista ei ole sisällytetty tähän ohjelmaan. Ohjelmassa ei muun muassa ole toimintoa yksityisviestien lähettämiseen asiakkaalta asiakkaalle.

- Ohjelmassa on toteutettu asiakkaalta palvelimelle viestintä sekä palvelimelta asiakkaille viestintä samalla ajatuksella kuin IRC:ssa, mutta toimintoja on yksinkertaistettu. Esimerkiksi asiakasohjelman lähettämät tiedot palvelimelle rekisteröityessä eivät sisällä kaikkia protokollassa määriteltyjä tietoja. Lisäksi palvelimelle liitytään suoraan ohjelman käynnistyessä ilman CONNECT/ komentoa.

- Kanavat ovat yksikertaistuksia IRC:n kanavista, eivätkä sisällä mm. kuvausta tai ylläpitäjiä. Myöskään palvelimesta tai käyttäjistä ei ole saatavilla yhtä kattavia tietoja kuin IRC:ssa.

### 5. Testaus
Ohjelma on testattu toimivaksi Ubuntu-käyttöjärjestelmässä käyttäen localhost-osoitetta.

Seuraavat tilanteet on testattu:
- Asiakkaan liittyminen palvelimelle localhostissa
- Nimen vaihtaminen /NICK komennolla
- Kanavien listaus /LIST komennolla
- Olemassa olevalle kanavalle liittyminen
- Uuden kanavan perustaminen ja sille liittyminen /JOIN komennolla
- /JOIN komento kun ollaan jo kanavalla
- Kanavalta poistuminen /PART komennolla
- Viiden yhtäaikaisen asiakkaan keskustelu kanavalla
- /PART komento kun ei olla millään kanavalla
- Viestin lähetys kun ei olla kanavalla
- Asiakkaan yhteyden katkaisu
- Palvelimen sammuttaminen
- Useiden kanavien ylläpito

### 6. Toiminta
##### Kuljetuskerroksen protokolla
Ohjelma käyttää kuljetuskerroksen protokollana TCP:tä. Tämä protokolla valittiin ensisijaisesti siksi, että se on yleisesti käytössä oleva protokolla vastaavissa sovelluksissa. Tämän ohjelman tapauksessa TCP:n etu erityisesti UDP:hen verrattuna on, että TCP takaa luotettavan kommunikaation, eli viestit menevät läpi kokonaisina, vaikka väliaikaisia kommunikaatiohäiriöitä ilmenisi. Toisaalta mahdollisista häiriöistä aiheutuvat viiveet, joita UDP:n tapauksessa ei olisi, eivät ole tämän ohjelman toiminnan kannalta merkityksellisiä.

##### IPv6 yhteensopivuus
Palvelinohjelma on tehty toimimaan myös IPv6-osoitteiden kanssa, mutta ei testattu ajanpuutteen vuoksi. Samasta syystä asiakasohjelmaan ei toteutettu tukea IPv6:lle.

##### Asiakkaan äkillinen yhteyden katkaisu
Äkillinen asiakkaan poistuminen palvelimelta aiheuttaa versiossamme palvelimen kaatumisen. Tätä testasimme sulkemalla asiakasohjelman brute forcella. Palvelin antaa lähettää vielä muutaman viestin jäljellä olevien asiakkaiden välillä, kunnes kaatuu. Tämä voi johtua siitä, että socket jää edelleen päälle ja palvelin yrittää syöttää sinne tavaraa siinä kuitenakaan onnistumatta.

##### Usean yhtäaikaisen asiakkaan liittyminen
Palvelin tukee useaa samanaikaista käyttäjää, eikä monen yhtäaikaa tapahtuvan liittymisen pitäisi aiheuttaa ongelmia, koska sekä palvelin että asiakas odottavat riittävän kauan vastausta. Tätä ei ole kuitenkaan testattu kuin muutamalla n. sekunnin sisään liitetyllä asiakkaalla.


##### Komennot
Komennot (/NICK, /LIST, /PART, /JOIN) pitää kirjoittaa isolla, jotta. Nimen vaihtaminen onnistuu useaan kertaan.
- Kanavien listaus /LIST komennolla
  - Komennot pitää kirjoittaa isolla. Listaaminen onnistuu vaivattomasti ja ilmoittaa kaikki valmiiksi luodut kanavat, sekä kanavalla olevien asiakkaiden lukumäärän.
- Olemassa olevalle kanavalle liittyminen, Uuden kanavan perustaminen ja sille liittyminen /JOIN komennolla
  - Komennolla joko liitytään tai perustetaan ja liitytään kanavalle.
- Kanavalta poistuminen /PART komennolla
  - /PART ei vielä poista muodostettua kanavaa, mutta poistaa käyttäjän kyseiseltä kanavalta.
- Viiden yhtäaikaisen asiakkaan keskustelu kanavalla
  - Käyttäjien lisääminen onnistuu vaivattomasti ja pystyy käsittelemään useampia käyttäjiä. Tätä testattu enimmillään viidellä käyttäjällä yhtäaikaa.
- /PART komento kun ei olla millään kanavalla
  - Käyttäjä saa viestin, jossa hänelle kerrotaan, ettei hän ole liittynyt yhteenkään kanavaan.
- /JOIN komento kun ollaan jo kanavalla
  - Käyttäjä siirtyy uudelle määritetylle kanavalle
- Viestin lähetys kun ei olla kanavalla
  - Käyttäjä saa viestin, jossa hänelle kerrotaan, ettei hän ole liittynyt yhteenkään kanavaan.
- asiakkaan yhteyden katkaisu
  - Palvelin ei valitettavasti kykene tällähetkellä käsittelemään tilannetta, jossa asiakas sulkee yhteyden.
- Palvelimen sammuttaminen
- Useiden kanavien ylläpito
  - Palvelin pystyy välittämään viestejä asikkaiden välillä usealla kanavalla samanaikaisesti


### 7. Puutteet
Ohjelma jäi joiltakin osin keskeneräiseksi ja siihen jäi bugeja, joita ei ehditty korjaamaan. Syynä tähän on tiukka aikataulu ja allekirjoittaneiden kokemattomuus C-ohjelmoinnissa. Tunnistetut puutteet on listattu alla.


| Puute             | Kuvaus                                   |
| ------------------| -----------------------------------------|
| accept() bugi     | Joskus asiakkaan käynnistämienn aiheuttaa palvelimen kaatumisen accept()-funktioon, joka päättyy virheeseen "Invalid argument"|
| muistivuodot      | Ohjelma sisältää jonkin verran dynaamiseen muistinvaraukseen liittyviä muistivuotoja |
| käytettävyys      | Asiakasohjelman käytettävyys on hankalaa |
| asiakkaan poistuminen | Asiakkaan poistumiseen palvelimelta ei ole toteutettu fuktiota, eli asiakasta ei poisteta palvelimen tiedoista vaikka yhteys katkeaisi|
| tyhjät kanavat    | tyhjien kanavien poistamiseen ei ole toteutettu funktiota |

Vaikka projekti jäi monilta osin keskeneräisiksi, keskeiset toiminnot, eli asiakkaan liittyminen palvelimelle ja keskustelu muiden asiakkaiden kassa pitäisi kuitenkin toimia.
