#include <SPI.h>

void setup() {
    Serial.begin(115200);

    SPI.begin();
    pinMode(SS, OUTPUT);
    digitalWrite(SS, HIGH); // Ensure SS is high
}

bool ledOn = false;

byte dataToSend[16];

void loop() {
    // send 0..15 to match slave's tx_buf (the slave verifies that it received this pattern)

    if (Serial.available() > 1) {
        memset(dataToSend, 0, 16);
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();

        if (cmd.length() > 0) {
            char buf[cmd.length() + 1];
            cmd.toCharArray(buf, sizeof(buf));
            const char* delim = " ,\t";
            char* token = strtok(buf, delim);
            int j = 0;
            while (token != NULL && j < 16) {
                long val = strtol(token, NULL, 16);
                dataToSend[j++] = (byte)(val & 0xFF);
                token = strtok(NULL, delim);
            }
            writeBytes(dataToSend);
        }
    }

    // dataToSend[0] = 0x20;
    // dataToSend[1] = ledOn ? 0x01 : 0x00;

    // for (int i = 0; i <= 0xF; i++) {
    //     dataToSend[i] = i;
    //     dataToSend[i] <<= 4;
    //     dataToSend[i] |= i;
    // }
}

void writeBytes(const byte bytes[16]) {
    byte rx[16];

    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    digitalWrite(SS, LOW);
    for (int i = 0; i < 16; i++) {
        // capture byte returned by slave while sending
        rx[i] = SPI.transfer(bytes[i]);
    }
    digitalWrite(SS, HIGH);
    SPI.endTransaction();

    // print received bytes for debugging
    Serial.print("rx: ");
    for (int i = 0; i < 16; i++) {
        if (rx[i] < 0x10)
            Serial.print("0");
        Serial.print(rx[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}
