#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <PulseSensorPlayground.h>

// ===== WIFI CONFIG =====
const char* ssid = "FRITZ!Box 6660 Cable IH";
const char* password = "35792990216402215870";

// ===== SUPABASE CONFIG =====
const char* SUPABASE_URL = "https://xqzzmtsccrzyaajbhvbc.supabase.co/rest/v1/events";
const char* SUPABASE_ANON_KEY = "sb_publishable_fOFeRWtDQ661n7r3DaWfXw_pFyto0o-";

const char* SESSION_ID = "test-session-456";

// ================== PULSE SENSOR ==================
const int PulseWire = 34; // GPIO 34 (ADC)
const int LED = 2;
int Threshold = 550;
PulseSensorPlayground pulseSensor;

// ================== SUPABASE POST ==================
void sendBPMToSupabase(int bpm) {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  http.begin(String(SUPABASE_URL));

  http.addHeader("Content-Type", "application/json");
  http.addHeader("apikey", SUPABASE_ANON_KEY);
  http.addHeader("Authorization", String("Bearer ") + SUPABASE_ANON_KEY);

  String body = String("{") +
                "\"session_id\":\"" + String(SESSION_ID) + "\"," +
                "\"type\":\"pulse\"," +
                "\"artwork_id\":\"painting_3\"," +
                "\"value\":{ \"bpm\":" + String(bpm) + " }" +
                "}";

  int code = http.POST(body);
  Serial.print("Supabase response: ");
  Serial.println(code);

  http.end();
}

// ================== SETUP ==================
void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED);
  pulseSensor.setThreshold(Threshold);

  if (pulseSensor.begin()) {
    Serial.println("PulseSensor ready");
  }
}

// ================== LOOP ==================
unsigned long lastSend = 0;

void loop() {
  if (pulseSensor.sawStartOfBeat()) {
    int myBPM = pulseSensor.getBeatsPerMinute();
    Serial.print("BPM: ");
    Serial.println(myBPM);

    if (millis() - lastSend > 3000) { // send every 3s
      sendBPMToSupabase(myBPM);
      lastSend = millis();
    }
  }
  delay(20);
}