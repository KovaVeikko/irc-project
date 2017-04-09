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
Asiakkaan main-funktio hakee ip-osoitteen annetulle DNS-nimelle ja yhdistää palvelimeen käyttäjän ilmoittamaan porttiin. Aluksi asiakasohjelma lähettää palvelimelle /USER-komennon, jolla se ilmoittaa käyttäjän nimen palvelimelle. Tämän jälkeen funktio kuuntelee ikuisessa silmukassa sekä käyttäjältä että palvelimelle avatusta pistokkeesta tulevaa io-virtaa. Käyttäjää ja palvelinta kuunnellaan vuorotellen Select()-funktion avulla, ja käyttäjän lähettämät viestit lähetetään palvelimelle avattuun pistokkeeseen.

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
Ohjelma perustuu IRC-protokollaan (<a href="https://tools.ietf.org/html/rfc1459">RFC 1459</a>), joskin toteuttaa vain pienen osan siitä.

- Ohjelmassa käytettävät komennot (ks. 3.4) ovat vastaavat kuin IRC:ssa, mutta eivät kaikilta osin vastaa protokollassa määriteltyä toiminnallisuutta. Suurinta osaa IRC:ssa määritellyistä komennoista ei ole sisällytetty tähän ohjelmaan. Ohjelmassa ei muun muassa ole toimintoa yksityisviestien lähettämiseen asiakkaalta asiakkaalle.

- Ohjelmassa on toteutettu asiakkaalta palvelimelle viestintä sekä palvelimelta asiakkaille viestintä samalla ajatuksella kuin IRC:ssa, mutta toimintoja on yksinkertaistettu. Esimerkiksi asiakasohjelman lähettämät tiedot palvelimelle rekisteröityessä eivät sisällä kaikkia protokollassa määriteltyjä tietoja. Lisäksi palvelimelle liitytään suoraan ohjelman käynnistyessä ilman CONNECT/ komentoa.

- Kanavat ovat yksikertaistuksia IRC:n kanavista, eivätkä sisällä mm. kuvausta tai ylläpitäjiä. Myöskään palvelimesta tai käyttäjistä ei ole saatavilla yhtä kattavia tietoja kuin IRC:ssa.

### 5. Testaus
Ohjelma on testattu toimivaksi Ubuntu-käyttöjärjestelmässä käyttäen localhost-osoitetta.

Seuraavat tilanteet on testattu:
- asiakkaan liittyminen palvelimelle localhostissa
- nimen vaihtaminen /NICK komennolla
- kanavien listaus /LIST komennolla
- olemassa olevalle kanavalle liittyminen /JOIN komennolla
- kanavalta poistuminen /PART komennolla
- uuden kanavan perustaminen ja sille liittyminen /JOIN komennolla
- kolmen yhtäaikaisen asiakkaan keskustelu kanavalla
- /PART komento kun ei olla millään kanavalla
- /JOIN komento kun ollaan jo kanavalla
- viestin lähetys ku ei olla kanavalla
- asiakkaan yhteyden katkaisu (palvelin pysyy toiminnassa)

### 6. Toiminta
##### Kuljetuskerroksen protokolla
Ohjelma käyttää kuljetuskerroksen protokollana TCP:tä. Tämä protokolla valittiin ensisijaisesti siksi, että se on yleisesti käytössä oleva protokolla vastaavissa sovelluksissa. Tämän ohjelman tapauksessa TCP:n etu erityisesti UDP:hen verrattuna on, että TCP takaa luotettavan kommunikaation, eli viestit menevät läpi kokonaisina, vaikka väliaikaisia kommunikaatiohäiriöitä ilmenisi. Toisaalta mahdollisista häiriöistä aiheutuvat viiveet, joita UDP:n tapauksessa ei olisi, eivät ole tämän ohjelman toiminnan kannalta merkityksellisiä.

##### IPv6 yhteensopivuus
Palvelinohjelma on tehty toimimaan myös IPv6-osoitteiden kanssa, mutta ei testattu ajanpuutteen vuoksi. Samasta syystä asiakasohjelmaan ei toteutettu tukea IPv6:lle.

##### Asiakkaan äkillinen yhteyden katkaisu
Äkillisen asiakkaan poistumisen ei ole havaittu aiheuttavan ongelmia palvelimella. Testeissä palvelin on toiminut normaalisti ja siihen on voinut liittyä uudestaan, kun asiakasohjelma on sammutettu brute forcella.

##### Usean yhtäaikaisen asiakkaan liittyminen
Palvelin tukee useaa samanaikaista käyttäjää, eikä monen yhtäaikaa tapahtuvan liittymisen pitäisi aiheuttaa ongelmia, koska sekä palvelin että asiakas odottavat riittävän kauan vastausta. Tätä ei ole kuitenkaan testattu kuin muutamalla n. sekunnin sisään liitetyllä asiakkaalla.


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
