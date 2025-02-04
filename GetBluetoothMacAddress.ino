#include <Ps3Controller.h>
void setup()
{
    Serial.begin(115200);
    Ps3.begin();
    String address = Ps3.getAddress();
    Serial.print("The ESP32's Bluetooth MAC address is: "); // to display you mad address
    Serial.println(address);
}

void loop()
{

}
