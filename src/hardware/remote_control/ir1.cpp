#include <stdio.h>
#include <wiringPi.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

#define IRPIN 1  // GPIO 1 Physical 12
#define DATANBR 150.0
#define PERIOD 350 //miliseconds
#define true (1==1)
#define false NOT true
#define MAXSAMPLES 300 * 1000

char data[MAXSAMPLES];
char binary[MAXSAMPLES] = { '\0' };

//Write Data to file
void writeFile(char* data, int len) {
    FILE* fp;
    int i;
    char str[20];

    fp = fopen("/home/pi/projects/IRcode/bin/ARM/Debug/out_power_C.txt", "w");
    for (i = 0; i < len; i++) {
        sprintf(str, "%d,%d\n", i, (char)data[i]);
        fprintf(fp, "%s", str);
    }
    fclose(fp);
}

//Get data from IR receiver
int getData() {
    int i = 0;
    unsigned int startTime = millis(), t;
    while (((t = millis()) - startTime) < PERIOD && i < MAXSAMPLES) {
        data[i++] = (char)digitalRead(IRPIN);
    }
    //this is for pulse analysis purpose only
    //writeFile(data, i-1);
    //exit(0);
    return i - 1;
}

//Set to \0 the binary array
void resetBinary() {
    int i;
    for (i = 0; i < 1000; i++)
        binary[i] = '\0';
}

//Decode the signal from IR receiver
void decode() {
    int i, j = 0;
    double rate;
    int datalen = getData();
    //printf("Data len: %d\n", datalen);
    rate = (double)datalen / PERIOD;
    int pulses[100][100];  //pulses[][0] = value , pulses[][1] = time(us)
    int stop = 0;

    if (datalen < PERIOD)
        return;

    resetBinary();

    for (i = 1; i < datalen; i++) {
        if ((data[i] != data[i - 1]) || (i == datalen - 1)) {
            pulses[j][0] = data[i - 1];             //value
            pulses[j++][1] = (i - stop) * 1000 / (int)rate; //time im us
            stop = i;
        }
    }
    int pulsesCount = j - 1;

    j = 0;
    for (i = 0; i < pulsesCount; i++) {
        if (pulses[i][0] != 1)
            continue;
        if (j != 0 && pulses[i][1] > 2000)
            break;
        else if (pulses[i][1] < 1000)
            binary[j++] = '0';
        else if (pulses[i][1] >= 1000 && pulses[i][1] <= 2000)
            binary[j++] = '1';
    }
    binary[j] = '\0';
    printf("Binary: %s\n", binary);
}

//Reverse string for Binary to decimal conversion
void reverse(char* num) {
    int i, len = (int)strlen(num), j = 0;
    char tmp[len + 1];

    for (i = len - 1; i >= 0; i--)
        tmp[j++] = num[i];

    strncpy(num, tmp, (size_t)len);
}

//Convert binary string to integer
int binaryStr2Dec(char* bin) {
    int i, ret = 0;
    reverse(bin);
    for (i = 0; i < strlen(bin); i++)
        ret += (int)(bin[i] - '0') * (int)pow(2, i);
    return ret;
}

int main(void)
{
    //Setup WiringPi and IR receiver pin
    wiringPiSetup();
    pinMode(IRPIN, INPUT);
    pullUpDnControl(IRPIN, PUD_DOWN);

    printf("Waiting for Signal\n");
    while (true)
    {
        if (digitalRead(IRPIN) == 0) {
            decode();
            printf("Code: 0x%06x\n", binaryStr2Dec(binary));
            delay(250);
        }
    }

    return 0;
}
