#include <iostream>
#include <wiringPi.h>

using namespace std;


#define LED 27
#define SERVO 15
int main()
{
    int x = 180;
    cout << "Test du blink" << endl;
    wiringPiSetup();
    wiringPiSetupSys();


    pinMode(SERVO,OUTPUT);
    pinMode(LED,OUTPUT);
    digitalWrite(LED,1);
    digitalWrite(SERVO,0);
/*
    while(1)
    {
        for(x=0;x<=100;x++)
        {
            cout << "1" << endl;
            delay(2);
            digitalWrite(SERVO,0);
            delay(15);
            digitalWrite(SERVO,1);
        }

        for(x=0;x<=100;x++)
        {
            cout << "2" << endl;
            delay(1);
            digitalWrite(SERVO,0);
            delay(15);
            digitalWrite(SERVO,1);
        }
    }
    */
/*
    for(;;)
    {
        digitalWrite(27,1);
        delay(500);
        digitalWrite(27,0);
        delay(500);
    }
*/
}
