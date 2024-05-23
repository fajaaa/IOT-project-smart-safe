#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <Keypad.h>
#include <Servo.h>
#include <string>

#define WIFI_SSID "Plati manje"
#define WIFI_PASSWORD "mozemoze"
#define API_KEY "AIzaSyBllCwGnuoAan_qfy6kMFF10bnnUksIcLY"
const char* DATABASE_URL = "https://smart-sef-default-rtdb.europe-west1.firebasedatabase.app/";

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signUpOk = false;

const byte ROWS = 4;
const byte COLS = 4;

char keys[4][4] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 16, 5, 4, 0 };
byte colPins[COLS] = { 2, 14, 12, 13 };

Keypad tastatura = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

Servo myServo;

void setup() {
  Serial.begin(9600);
  myServo.attach(15);
  myServo.write(0);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("signUp OK");
    signUpOk = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

bool cp = false;
int ispravan_pass = 0;
int pozicijaNiza = 0;

const int duzina = 6;
char lozinka[duzina] = "";

bool door = false;
String strSaBaze = "";

void loop() {
  char dugme = tastatura.getKey();
  if (dugme == '#') {
    Skini_iz_base_string();
    if (strcmp(lozinka, strSaBaze.c_str()) == 0) {
      Serial.println("_____ISPRAVNA SIFRA_____\n");
      Posatavi_u_bazu(true, "Senzor/enable");
      while (door == false) {
        Skini_iz_base_bool();
        delay(1600);
        if (door == true)
          Serial.println("Vrata su spremna za otvaranje");
      }
      Okreni_180();
      while (door == true) {
        Skini_iz_base_bool();
        delay(1600);
      }
      Okreni_0();
      delay(1200);
      Posatavi_u_bazu(false, "Senzor/enable");
      PostaviPrazanNiz();
    } else {
      Serial.println("Sifra je netacna !!!");
      PostaviPrazanNiz();
    }
  } else if (dugme != '\0') {
    DodajNiz(pozicijaNiza, dugme);
    Serial.print(lozinka);
    Serial.println("   ovo je lozinka");
  }
}

void Skini_iz_base_bool() {
  if (Firebase.RTDB.getBool(&fbdo, "Senzor/ispravan_pass")) {
    if (fbdo.dataType() == "boolean") {
      door = fbdo.boolData();
      Serial.println("usojesno povlacenje iz" + fbdo.dataPath() + " : " + strSaBaze + " (" + fbdo.dataType() + ") ");
    }
  } else {
    Serial.println("greska .. ---> " + fbdo.errorReason());
  }
}

void Skini_iz_base_string() {
  if (Firebase.RTDB.getString(&fbdo, "Senzor/text/pass")) {
    if (fbdo.dataType() == "string") {
      strSaBaze = fbdo.stringData();
      Serial.println("usojesno povlacenje iz" + fbdo.dataPath() + " : " + strSaBaze + " (" + fbdo.dataType() + ") ");
    }
  } else {
    Serial.println("greska .. ---> " + fbdo.errorReason());
  }
}

void Posatavi_u_bazu(bool var, String putanja) {
  cp = var;
  if (Firebase.RTDB.setInt(&fbdo, putanja, cp)) {
    Serial.println();
    Serial.print(cp);
    Serial.print(" -sacuvano u: " + fbdo.dataPath());
    Serial.println(" (" + fbdo.dataType() + ") ");
  } else {
    Serial.println("greska: " + fbdo.errorReason());
  }
  delay(100);
}

void PostaviPrazanNiz() {
  memset(lozinka, 0, duzina);
  pozicijaNiza = 0;
}

void DodajNiz(int& pozicija, char karakter) {
  if (duzina - 1 > pozicija && karakter != '\0') {
    lozinka[pozicija] = char(karakter);
    pozicija++;
  } else {
    PostaviPrazanNiz();
  }
}

void Okreni_0() {
  myServo.write(0);
  delay(1000);
}

void Okreni_180() {
  myServo.write(180);
  delay(500);
}
