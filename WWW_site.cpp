#include "shared_data.h"

String web_site(void){
    String ssid         = sharedData.wifiSSID;
    String password     = sharedData.wifiPassword;
    String user_name    = sharedData.user_name;
    String server_name  = sharedData.server_name;

    String message = ""
    "<!DOCTYPE HTML><html><head>"
    "<title>DLB Setup</title>"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
    "<style>"
    "body{color:white; background-color:#141414; font-family: Helvetica, Verdana, Arial, sans-serif}"
    "h1{text-align:center;}"
    "p{text-align:center;}"
    "div{margin: 5%; background-color:#242424; padding:10px; border-radius:8px;}"
    "br{display: block; margin: 10px 0; line-height:22px; content: " ";}"
    "label{text-align:left; padding:2%;}"
    "input{border-radius: 4px; border: 2px solid #0056a8; width:90%; padding:10px; display:block; margin-right:auto; margin-left:auto;}"
    "input[type=\"submit\"]{font-size: 25px; background-color:#0056a8; color:white; border-radius:8px; height:50px; width:95%;}"
    "</style></head>"
    "<body>"
    "<div>"
    "<h1>Device Setup</h1>"
    "<p>Please enter the following details to setup your device:</p>"
    "<form class=\"registration_form\" method='get' action='setting'>"
    "<label>Wi-Fi Details</label>"
    "<br>"
    "<input type=\"text\" name=\"wifissid\" id=\"wifissid\"  value=\""+String(ssid.c_str())+"\" placeholder=\"Wi-Fi SSID\">"
    "<br>"
    "<input type=\"text\" name=\"wifipass\" id=\"wifipass\" value=\""+String(password.c_str())+"\" placeholder=\"Wi-Fi Password\">"
    "<br>"
    "<label for=\"username\">User Name</label>"
    "<br>"
    "<input type=\"text\" name=\"username\" id=\"username\" value=\""+String(user_name.c_str())+"\" placeholder=\"Client name or ID\">"
    "<br>"
    "<label for=\"username\">Server address</label>"
    "<br>"
    "<input type=\"text\" name=\"server_name\" id=\"server_name\" value=\""+String(server_name.c_str())+"\" placeholder=\"default dlb.com.pl\">"
    "<br>"
    "<br>"
    "<input type=\"submit\" value=\"Submit\">"
    "</form><br>"
    "</div>"
    "</body></html>";
return message;
}