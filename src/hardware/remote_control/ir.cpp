#include <iostream>
#include <wiringPi.h>
#include <vector>
#include <chrono>
#include <thread>
#include <bitset>

using namespace std;

#define IR_PIN 3 // IR receiver in wiringPi (physical pin 12)
#define BOUNCE_TIME 150 // Bounce time in milliseconds
#define MAX_BITS 32 // Maximum number of bits to capture

std::vector<int> binary_acquire(int pin, double duration) {
    std::vector<int> results;
    auto t0 = std::chrono::steady_clock::now();

    while (std::chrono::steady_clock::now() - t0 < std::chrono::milliseconds(static_cast<int>(duration * 2000))) {
        results.push_back(digitalRead(pin));
    }

    return results;
}

unsigned on_ir_receive() {
    std::vector<int> data = binary_acquire(IR_PIN, static_cast<double>(BOUNCE_TIME) / 1000);
    unsigned datasize = data.size();
    double rate = datasize * 1000 / static_cast<double>(BOUNCE_TIME);
    std::vector<int> dat{};
    std::vector<int> gap{};
    int i_break = 0;

    for (unsigned i = 1; i < datasize; i++) {
        if (data.at(i) != data.at(i - 1) || i == datasize - 1) {
            dat.push_back(data.at(i - 1));
            gap.push_back(static_cast<int>((i - i_break) / rate * 1000000));
            i_break = i;
        }
    }

    if (dat.size() >= 2) {
        std::vector<int> validData;
        for (unsigned i = 1; i < dat.size(); i++) {
            if (dat.at(i) != 1) {
                continue;
            }

            if (gap.at(i) <= 500) {
                validData.push_back(0);
            }
            else if (gap.at(i) > 600 && gap.at(i) <= 1400) {
                validData.push_back(1);
            }
        }

        if (validData.size() >= 8) {
            unsigned result = 0;
            for (unsigned i = 0; i < validData.size() && i < MAX_BITS; i++) {
                result = (result << 1) | validData[i];
            }
            return result;
        }
    }
    return 0;
}

int main() {
    if (wiringPiSetup() == -1) {
        cerr << "Setup wiringPi Failed!" << endl;
        return 1;
    }

    pinMode(IR_PIN, INPUT);

    cout << "Waiting for IR data..." << endl;

    while (true) {
        unsigned irCode = on_ir_receive();
        if (irCode != 0) {
            cout << "IR Code: 0x" << hex << irCode << endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Sleep to avoid busy-waiting
    }

    return 0;
}
