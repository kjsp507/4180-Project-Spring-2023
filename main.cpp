#include "mbed.h"
#include "EthernetInterface.h"
#include "TCPSocketConnection.h"
#include "ifttt.h"

EthernetInterface eth;
RawSerial pc(USBTX, USBRX); // tx, rx

int main()
{
    pc.baud(9600);
    eth.init(); //Use DHCP
    eth.connect();
    printf("IP Address is %s \n\r", eth.getIPAddress());
    TCPSocketConnection socket;

    // Initialize ifttt object, add up to 3 optional values, trigger event.
    IFTTT ifttt("YourEventName","ChangeToYourSecretKey", &socket); // EventName, Secret Key, socket to use
    ifttt.addIngredients("this is awesome","test-ing","data!!!");     // 3 optional Values to send along with trigger.
    ifttt.trigger();

    // Send data using GET
    ifttt.addIngredients("Sending","GET","data");
    ifttt.trigger(IFTTT_GET);

    // Send Data using POST
    ifttt.addIngredients("Sending","POST","things");
    ifttt.trigger(IFTTT_POST);

    eth.disconnect();
    while(1) {
    }
}
