#include <iostream>
#include "EmuSelect/EmuSelector.hpp"

#define DEBUG 1

int main (void) {

    int selected_emulator;

    try {

        EmuSelector *selector = new EmuSelector(DEBUG);

        selector->startup();

        selected_emulator = selector->get_confirmed_option();
    }
    catch (std::exception &e){
        std::cerr << RED << e.what() << RESET << std::endl;
    }

    if (selected_emulator == -1) {
        return (0);
    }

    switch (selected_emulator) {

        case 1:
            std::cout << YELLOW << "Launching game boy emulator..." << RESET << std::endl;
            break;
    }
}
