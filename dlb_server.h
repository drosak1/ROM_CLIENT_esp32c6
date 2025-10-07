#ifndef dlb_server_h
#define dlb_server_h

#include "Arduino.h"
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <HTTPUpdate.h>

class dlb_server {
public:
  dlb_server(WebServer * server, int logger_firmware_version);
  bool send_event(String addres);
  void set_macAddress();
  bool get_credential(String addres);
  bool update(String addres);
  bool have_fingerprint = false;
  bool have_update_version = false;
  String get_http_buff(String addres);
  String get_https_buff(String addres);

private:
  char fingerprint[128];
  int update_version = 0;
  int logger_firmware_version_=100000;
  const char* www_username = "admin";
  const char* www_password = "dlb";
  WebServer * server_;
  const char* root_ca = R"EOF(
  -----BEGIN CERTIFICATE-----
  MIIF... (root CA PEM)
  -----END CERTIFICATE-----
  )EOF";
};

#endif