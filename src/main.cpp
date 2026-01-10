#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
// #include <PulseSensorPlayground.h>

// ===== FAKE DATA =====
unsigned long lastSend = 0;
int fakeBPM = 72; // starting BPM (resting heart rate)

// ===== WIFI CONFIG =====
// const char* ssid = "FRITZ!Box 6660 Cable IH";
// const char* password = "35792990216402215870";
// ===== WIFI CONFIG Sophie N =====
const char *ssid = "FRITZ!Box 6660 Cable IH";
const char *password = "35792990216402215870";

// ===== SUPABASE CONFIG =====
const char *SUPABASE_URL = "https://xqzzmtsccrzyaajbhvbc.supabase.co/rest/v1/pulse_data";
const char *SUPABASE_ANON_KEY = "sb_publishable_fOFeRWtDQ661n7r3DaWfXw_pFyto0o-";
// const char* SUPABASE_URL = "https://YOUR_PROJECT_ID.supabase.co",
// const char* SUPABASE_ANON_KEY =  "PUBLIC_ANON_KEY",
const char *SESSION_ID = "test-session-456";

// ===== FAKE NFC + TIME STATE + ARTWORK_ID=====
int fakeTagCounter = 1;

// start time: 12:37:15
int fakeHour = 12;
int fakeMinute = 37;
int fakeSecond = 15;


int artworkCounter = 1;
int tagsInCurrentArtwork = 0;
const int TAGS_PER_ARTWORK = 20;

String sessionId = "session_001";  // 🔒 fixed for entire run


String generateArtworkId()
{
  char buffer[20];
  sprintf(buffer, "artwork_%03d", artworkCounter);
  return String(buffer);
}

void advanceArtworkIfNeeded()
{
  tagsInCurrentArtwork++;

  if (tagsInCurrentArtwork >= TAGS_PER_ARTWORK)
  {
    artworkCounter++;
    tagsInCurrentArtwork = 0;

    Serial.println("🔄 Switched to new artwork");
  }
}


String formatTime(int h, int m, int s)
{
  char buffer[9];
  sprintf(buffer, "%02d:%02d:%02d", h, m, s);
  return String(buffer);
}

void advanceFakeTime(int secondsToAdd)
{
  fakeSecond += secondsToAdd;

  if (fakeSecond >= 60)
  {
    fakeMinute += fakeSecond / 60;
    fakeSecond = fakeSecond % 60;
  }

  if (fakeMinute >= 60)
  {
    fakeHour += fakeMinute / 60;
    fakeMinute = fakeMinute % 60;
  }

  if (fakeHour >= 24)
  {
    fakeHour = fakeHour % 24;
  }
}

String generateFakeTag()
{
  char buffer[16];
  sprintf(buffer, "tag_%03d", fakeTagCounter);
  fakeTagCounter++;
  return String(buffer);
}



// ================== PULSE SENSOR CODE ==================
// const int PulseWire = 34; // GPIO 34 (ADC)
// const int LED = 2;
// int Threshold = 550;
// PulseSensorPlayground pulseSensor;

// ================== SUPABASE POST ==================
void sendBPMToSupabase(int bpm)
{
  if (WiFi.status() != WL_CONNECTED)
    return;
  
      String tag = generateFakeTag();
  String timeString = formatTime(fakeHour, fakeMinute, fakeSecond);
    String artworkId = generateArtworkId();

  HTTPClient http;
  http.begin(String(SUPABASE_URL));

  http.addHeader("Content-Type", "application/json");
  http.addHeader("apikey", SUPABASE_ANON_KEY);
  http.addHeader("Authorization", String("Bearer ") + SUPABASE_ANON_KEY);

  String body = String("{") +
    "\"tag\":\"" + tag + "\"," +
    "\"session_id\":\"" + sessionId + "\"," +   // 🔒 constant
    "\"time\":\"" + timeString + "\"," +
    "\"artwork_id\":\"" + artworkId + "\"," +   // 🔁 rotating
    "\"bpm\":" + String(bpm) +
  "}";


  int code = http.POST(body);
  Serial.print("Supabase response: ");
  Serial.println(code);

  Serial.println(http.getString());
  http.end();

   // FAKE DATA: advance simulation
  advanceFakeTime(20);
  advanceArtworkIfNeeded();
}

// ================== FAKE BPM ==================
int generateFakeBPM()
{
  // small natural variation
  fakeBPM += random(-3, 4); // -3 to +3

  // clamp to realistic human range
  if (fakeBPM < 55)
    fakeBPM = 55;
  if (fakeBPM > 110)
    fakeBPM = 110;

  return fakeBPM;
}

// ================== SETUP ==================
void setup()
{
  Serial.begin(115200);
  randomSeed(esp_random()); // comment this out for real pulse sensor

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  // ================== PULSE SENSOR CODE ==================
  // pulseSensor.analogInput(PulseWire);
  // pulseSensor.blinkOnPulse(LED);
  // pulseSensor.setThreshold(Threshold);

  // if (pulseSensor.begin()) {
  //   Serial.println("PulseSensor ready");
  // }
}

// ================== LOOP ==================
// unsigned long lastSend = 0;

void loop()
{
  unsigned long now = millis();

  if (now - lastSend >= 20000)
  {
    lastSend = now;

    int bpm = generateFakeBPM();
    Serial.print("FAKE BPM: ");
    Serial.println(bpm);

    sendBPMToSupabase(bpm);
  }

  delay(50);
}

// ================== PULSE SENSOR CODE ==================
// void loop() {
//   if (pulseSensor.sawStartOfBeat()) {
//     int myBPM = pulseSensor.getBeatsPerMinute();
//     Serial.print("BPM: ");
//     Serial.println(myBPM);

//     if (millis() - lastSend > 3000) { // send every 3s
//       sendBPMToSupabase(myBPM);
//       lastSend = millis();
//     }
//   }
//   delay(20);
// }