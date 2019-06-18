![MikroE](http://www.mikroe.com/img/designs/beta/logo_small.png)
---
# G2C - Matrix RGB demo application

#### Internal information 
- **Version**        : v1.0.0
- **Date**           : 3/28/2019
- **Author**         : Nemanja Medakovic
- **Libstock**       : {LIBSTOCK_LINK}

## Project description

Ovaj demo projekat ima za cilj da olaksa reklamiranje odredjene robe, akcija, popusta, rasprodaje, snizenja u radnjama, prodavnicama, shopping centrima itd.

## Project goals 

Glavna ideja ovog projekta je da ustedi vreme i napor da se odredjena reklama postavi na RGB panel/panele.
Noviteti ili promene u marketingu zelejnog produkta ili akcije sada se mogu veoma lako i brzo izvrsiti na nacin koji pokazuje ovaj demo projekat.
Pristupacnost RGB panela, koji mogu biti postavljeni visoko i na nepristupacnim mestima, nije neophodna, jer se izmene vrse softverski preko cloud aplikacije.
Ovim je postignut u velikoj meri olaksan marketing.

## Project solved

Korisnik ima potpunu kontrolu dizajniranja RGB panel reklame preko cloud aplikacije unosenjem odredjenih parametara kroz aplikaciju.

### Design
 
Matrix RGB klik upravlja sa 1 ili vise panela (u nasem demo projektu konkretno su koriscena 4 RGB panela) slanjem odredjenih komandi koje vrse dizajniranje i kontrolu RGB panela.
Nacin dizajniranja i komande koje se salju Matrix RGB kliku se unose preko parametara (aktuatora) u cloud aplikaciji.
Korisnik preko web browser-a (cloud aplikacije) moze u svakom trenutku i na svakom mestu da preko aplikacije posalje parametre za Matrix RGB klik i upravljanje panelom.
Time je reseno da za svako novo reprogramiranje panela i postavaljanja nove reklame korisnik ne mora da skida panel i da trosi dodatno vreme na programiranje.

### Where it is used

- Shopping radnje
- Shopping centri
- Prodavnice
- Marketi
- bilo koji drugi vidovi trzista i marketinga

### Hardware

**System**

 - EasyMx PRO v7 for STM32 ARM
 - MCU kartica STM32F107VC za pomenuti sistem

**Click boards**

 - Cloud (G2C) click
 - Matrix RGB click

**Other hardware**

 - RGB panel 6mm pitch, sa IDC i power kablom, komada 4 
 - DC napajanje, 50W, 5V, 0-10A (http://napajanje.rs/spd/RS-50-5/Mean-Well-RS-50-5-50W-5V-0-10A) 

## Instructions - hardware

Postaviti pomenutu MCU karticu na STM32 ARM sistem i dovesti napajanje sistema.
Spojiti napajanje sa RGB panelima preko power kablova dobijenih uz svaki RGB panel.
Takodje medjusobno povezati svaki RGB panel uz pomoc IDC kablova takodje dobijenih uz svaki RGB panel.
Semu povezivanja RGB panela preko IDC kablova moguce naci na sledecem linku (http://gitlab/fw-clicks/matrixRGB).
Jedan IDC kabel ide sa prvog RGB panela na Matrix RGB klik.
Tako povezan Matrix RGB klik se postavlja na prvi mikrobus na STM32 razvojnom sistemu, a G2C (Cloud) klik se postavlja na drugi mikrobus.

### Software

 - mikroC PRO for ARM
 - cloud aplikacija (https://app.clickcloud.io/#/login)

**Library Packages**

 - [G2C click](https://libstock.mikroe.com/projects/view/2628/g2c-click)
 - [Matrix RGB click](https://libstock.mikroe.com/projects/view/1840/matrix-rgb-click)
 - [mikroSDK](https://libstock.mikroe.com/projects/view/2249/mikrosdk) 

**Architecture**

|        | CEC | MSP | STM | KINETIS | TIVA | AVR | DSPIC | FT90x | PIC | PIC32 |
|--------|-----|-----|-----|---------|------|-----|-------|-------|-----|-------|
|Basic   |  /  |  /  |  /  |    /    |  /   |  /  |   /   |   /   |  /  |   /   |
|C       |  /  |  /  |  X  |    /    |  /   |  /  |   /   |   /   |  /  |   /   |
|Pascal  |  /  |  /  |  /  |    /    |  /   |  /  |   /   |   /   |  /  |   /   |

## Instructions - cloud 

Kreirati device sa vec postojecim (kreiranim) Matrix RGB manifestom.
Omoguciti sve navedene aktuatore za Matrix RGB, a zatim izgenerisani device key i password uneti kao parametre u kodu.
Napraviti dashboard za kreirani device koji ce sluziti za unosenje parametara preko clouda i kontrolu RGB panela.
Zatim se preko G2C (Cloud) klika konektovati na cloud i proveriti da li je kreirani device-a konektovan (stanje device-a).

## Instructions - software

 - Skinuti i instalirati 'MikroC PRO for ARM' kompajler
 - Skinuti G2C - Matrix RGB demo project
 - Instalirati pakete za (koristiti mikroe package manager):
   - mikroSDK
   - G2C click
   - Matrix RGB click
 - Pokrenuti 'MikroC PRO for ARM'
 - Otvoriti G2C - Matrix RGB demo project
 - Proveriti da li su parametri za device key i device password zamenjeni sa izgenerisanim parametrima sa cloud-a
 - Izbildovati projekat
 - Isprogramirati G2C - Matrix RGB demo projekat na 'easyMx PRO v7 for STM32 ARM' sistem

## Project and click ranges 

 - G2C click board
   - Operating temperature - (-40℃ to 85℃)
   - Operating voltage - (3.0V to 3.6V)
 - Matrix RGB click board
   - Operating temperature - (-40℃ to 85℃)
   - Operating voltage - (3.0V to 3.6V)
 - RGB panels
   - DC source - 5V max

## Project Test 

FW test je izvrsen nekoliko puta u toku i na kraju izrade FW-a za pomenuti projekat pri konstantnom DC izvoru napajanja od 5V.
Korisceno je vise tipova DC napajanja, medju kojima je i onaj nabrojan u opisu HW-a (gore pomenut). 
Prilikom testiranja projekta na RGB panelima je demo primer generisao razlicite slike, animacije i boje, prilikom kojih su paneli vukli promenljivu struju, 
koja je isla i do reda nekoliko ampera. Krajnji kod je odlicno usaglasen sa cloud aplikacijom i samim hardverom, sto podrazumeva vremenski odziv na zadatu komandu sa cloud-a, 
zatim prihvatanje raznih komandi sa cloud-a i efikasnost izvrsavanja dizajniranja RGB panela na zeljeni nacin preko cloud-a, kao i to da kompletno povezan hardver na pomenuti nacin 
nije ni u kakvom smislu pravio problem.

## Project upgrades

Unapredjivanje funkcionalnosti projekta je moguce izvrsiti dodavanjem (oduzimanjem po potrebi korisnika) broja RGB panela, koji mogu biti vezani ne samo u kvadratnoj formaciji, 
kako je radjeno u nasem primeru, vec i na red horizontalno, vertikalno, moguce je menjati raspored i broj panela.
Povecanjem broja RGB panela povecava se i rezolucija (koja u nasem slucaju iznosi 64x64, jer su koriscena 4 panela rezolucije 32x32 u kvadtatnoj formaciji vezana na red).
Takodje moguce je povecati broj osmisljenih pattern-a (animacija) same reklame, kao i broj mogucih boja i drugih parametara aktuatora sa cloud-a, koji zapravo upravljaju ovim demo primerom (reklamom).

---
---