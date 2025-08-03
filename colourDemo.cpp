//
// Created by Michael Tomlinson on 2025/08/01.
//
#include <iostream>
#include "ANSI-color-codes.h"

int main() {
    // Regular colors
    std::cout << BLK << "Hello World (Black)" << reset << std::endl;
    std::cout << RED << "Hello World (Red)" << reset << std::endl;
    std::cout << GRN << "Hello World (Green)" << reset << std::endl;
    std::cout << YEL << "Hello World (Yellow)" << reset << std::endl;
    std::cout << BLU << "Hello World (Blue)" << reset << std::endl;
    std::cout << MAG << "Hello World (Magenta)" << reset << std::endl;
    std::cout << CYN << "Hello World (Cyan)" << reset << std::endl;
    std::cout << WHT << "Hello World (White)" << reset << std::endl;

    // Bold colors
    std::cout << BBLK << "Hello World (Bold Black)" << reset << std::endl;
    std::cout << BRED << "Hello World (Bold Red)" << reset << std::endl;
    std::cout << BGRN << "Hello World (Bold Green)" << reset << std::endl;
    std::cout << BYEL << "Hello World (Bold Yellow)" << reset << std::endl;
    std::cout << BBLU << "Hello World (Bold Blue)" << reset << std::endl;
    std::cout << BMAG << "Hello World (Bold Magenta)" << reset << std::endl;
    std::cout << BCYN << "Hello World (Bold Cyan)" << reset << std::endl;
    std::cout << BWHT << "Hello World (Bold White)" << reset << std::endl;

    // Underline colors
    std::cout << UBLK << "Hello World (Underline Black)" << reset << std::endl;
    std::cout << URED << "Hello World (Underline Red)" << reset << std::endl;
    std::cout << UGRN << "Hello World (Underline Green)" << reset << std::endl;
    std::cout << UYEL << "Hello World (Underline Yellow)" << reset << std::endl;
    std::cout << UBLU << "Hello World (Underline Blue)" << reset << std::endl;
    std::cout << UMAG << "Hello World (Underline Magenta)" << reset << std::endl;
    std::cout << UCYN << "Hello World (Underline Cyan)" << reset << std::endl;
    std::cout << UWHT << "Hello World (Underline White)" << reset << std::endl;

    // Regular background
    std::cout << BLKB << "Hello World (Black Background)" << reset << std::endl;
    std::cout << REDB << "Hello World (Red Background)" << reset << std::endl;
    std::cout << GRNB << "Hello World (Green Background)" << reset << std::endl;
    std::cout << YELB << "Hello World (Yellow Background)" << reset << std::endl;
    std::cout << BLUB << "Hello World (Blue Background)" << reset << std::endl;
    std::cout << MAGB << "Hello World (Magenta Background)" << reset << std::endl;
    std::cout << CYNB << "Hello World (Cyan Background)" << reset << std::endl;
    std::cout << WHTB << "Hello World (White Background)" << reset << std::endl;

    // High intensity background
    std::cout << BLKHB << "Hello World (High Intensity Black Background)" << reset << std::endl;
    std::cout << REDHB << "Hello World (High Intensity Red Background)" << reset << std::endl;
    std::cout << GRNHB << "Hello World (High Intensity Green Background)" << reset << std::endl;
    std::cout << YELHB << "Hello World (High Intensity Yellow Background)" << reset << std::endl;
    std::cout << BLUHB << "Hello World (High Intensity Blue Background)" << reset << std::endl;
    std::cout << MAGHB << "Hello World (High Intensity Magenta Background)" << reset << std::endl;
    std::cout << CYNHB << "Hello World (High Intensity Cyan Background)" << reset << std::endl;
    std::cout << WHTHB << "Hello World (High Intensity White Background)" << reset << std::endl;

    // High intensity text
    std::cout << HBLK << "Hello World (High Intensity Black)" << reset << std::endl;
    std::cout << HRED << "Hello World (High Intensity Red)" << reset << std::endl;
    std::cout << HGRN << "Hello World (High Intensity Green)" << reset << std::endl;
    std::cout << HYEL << "Hello World (High Intensity Yellow)" << reset << std::endl;
    std::cout << HBLU << "Hello World (High Intensity Blue)" << reset << std::endl;
    std::cout << HMAG << "Hello World (High Intensity Magenta)" << reset << std::endl;
    std::cout << HCYN << "Hello World (High Intensity Cyan)" << reset << std::endl;
    std::cout << HWHT << "Hello World (High Intensity White)" << reset << std::endl;

    // Bold high intensity text
    std::cout << BHBLK << "Hello World (Bold High Intensity Black)" << reset << std::endl;
    std::cout << BHRED << "Hello World (Bold High Intensity Red)" << reset << std::endl;
    std::cout << BHGRN << "Hello World (Bold High Intensity Green)" << reset << std::endl;
    std::cout << BHYEL << "Hello World (Bold High Intensity Yellow)" << reset << std::endl;
    std::cout << BHBLU << "Hello World (Bold High Intensity Blue)" << reset << std::endl;
    std::cout << BHMAG << "Hello World (Bold High Intensity Magenta)" << reset << std::endl;
    std::cout << BHCYN << "Hello World (Bold High Intensity Cyan)" << reset << std::endl;
    std::cout << BHWHT << "Hello World (Bold High Intensity White)" << reset << std::endl;

    // Resets
    std::cout << reset << "Hello World (reset)" << std::endl;
    std::cout << CRESET << "Hello World (CRESET)" << std::endl;
    std::cout << COLOR_RESET << "Hello World (COLOR_RESET)" << std::endl;

    return 0;
}