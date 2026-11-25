#pragma once
// ======================================================================
//  Your settings. Edit these, then upload.
// ======================================================================

// ---- Wi-Fi (2.4 GHz network; the ESP32 can't use 5 GHz) ----
#define WIFI_SSID      "AAB3 Hyperoptic 1Gb Fibre 2.4Ghz"
#define WIFI_PASSWORD  "4Qe2224FH46A"
#define WIFI_SSID_2      "GiantFrog"
#define WIFI_PASSWORD_2  "TiggyDinoSausage2025"

// ---- Nightscout ----
// No trailing slash.
#define NS_URL         "https://nightscoutmaster-rq2s.onrender.com"
// Leave empty: your site is readable without a token.
// If you lock it down later, put a read-only token here.
#define NS_TOKEN       ""
// Only show readings from this sensor (your site has two uploading).
#define NS_DEVICE      "Libre2"

// ---- Time zone (for the clock) ----
// UK time with automatic summer time. Other examples:
//   Ireland: "IST-1GMT0,M10.5.0,M3.5.0/1"   Central Europe: "CET-1CEST,M3.5.0,M10.5.0/3"
#define TZ_INFO        "GMT0BST,M3.5.0/1,M10.5.0"

// ---- Behaviour ----
#define FETCH_INTERVAL_S   60   // how often to fetch new readings
#define STALE_MINUTES      15   // grey out the reading if it's older than this
