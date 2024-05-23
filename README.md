# IOT-project-smart-safe
 IoT projekt Smart Safe predstavlja osnovu za funkcioniranje pametnih sefova, razvijen kao dio fakultetskih obaveza. Projekt sluzi kao jednostavna simulacija pametnog sefa.

## Opis projekta:
- IoT projekt Smart Safe predstavlja osnovu za funkcioniranje pametnih sefova, razvijen kao dio fakultetskih obaveza. Projekt sluzi kao jednostavna simulacija pametnog sefa.

## Glavne funkcionalnosti:
- Daljinsko otvaranje i zatvaranje sefa.
- Generiranje sigurnosnih kodova za pristup sefu.
- Praćenje statusa sefa u realnom vremenu.

## Upotreba

Kada se povezujete na WiFi mrežu putem mikrokontrolera, važno je obratiti pažnju na nekoliko stvari:

1. Naziv WiFi mreže (SSID): Zamijenite "NazivVašeWiFiMreže" sa tačnim nazivom vaše WiFi mreže. Ovaj naziv treba da odgovara nazivu WiFi mreže na koju želite da se povežete.

2. WiFi šifra (password): Zamijenite "VašaWiFiŠifra" sa vašom WiFi šifrom. Ova šifra je potrebna za uspostavljanje sigurne veze sa WiFi mrežom.

Nakon što ste unijeli tačne podatke za WiFi mrežu, mikrokontroler će koristiti ove informacije kako bi se uspješno povezao na WiFi mrežu prilikom pokretanja koda. Ovo osigurava da vaš pametni sef bude dostupan na mreži i može komunicirati sa web sučeljem ili mobilnom aplikacijom za kontrolu.

<pre>
    #define WIFI_SSID "NazivVaseWiFiMreze"
    #define WIFI_PASSWORD "VasaWiFiSifra"
</pre>
