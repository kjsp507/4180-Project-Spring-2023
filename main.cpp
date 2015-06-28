#include "mbed.h"
#include "EthernetInterface.h"
#include "HTTPClient.h"
 
EthernetInterface eth;
HTTPClient http;
char str[512] = {};
char json[125] = {};
char eventName[] = "helloworld";
char key[] = "ChangeThisToTheKeyProvidedByIFTTonTheMakerChannel";
char value1[] = {"A"}, value2[] = {"B"}, value3[] = {"C"};

int main() 
{
    eth.init(); //Use DHCP
    eth.connect();
    printf("IP Address is %s \n\r", eth.getIPAddress());
    
    //GET 
    printf("GETing data... \n\r");
    sprintf(str, "https://maker.ifttt.com/trigger/%s/with/key/%s?value1=%s&value2=%s&value3=%s",eventName,key,value1,value2,value3);
    printf("String is : %s\n\r",str);
    int ret = http.get(str, str, 128); 
    if (!ret)
    {
      printf("Page fetched successfully - read %d characters\n\r", strlen(str));
      printf("Result: %s\n", str);
    }
    else
    {
      printf("Error - ret = %d - HTTP return code = %d \n\r", ret, http.getHTTPResponseCode());
    }
    
    //POST
    HTTPMap map;
    HTTPText inText(str, 512);
    map.put("value1", value1);
    map.put("value2", value2);
    map.put("value3", value3);
    sprintf(str, "https://maker.ifttt.com/trigger/%s/with/key/%s",eventName,key);
    printf("String is : %s\n\r",str);
    printf("POSTing data ....\n\r");
    ret = http.post(str, map, &inText); 
    if (!ret)
    {
      printf("Executed POST successfully - read %d characters \n\r", strlen(str));
      printf("Result: %s \n\r", str);
    }
    else
    {
      printf("Error - ret = %d - HTTP return code = %d \n\r", ret, http.getHTTPResponseCode());
    }
    
    
   
    eth.disconnect();  
 
    while(1) {
    }
}
