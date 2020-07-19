#include "../common/MessageBox.h"

#define NOMINMAX
#include <Windows.h>

void DisplayMessage(std::string titel, std::string message) {
    MessageBox(GetActiveWindow(), titel.c_str(), message.c_str(), MB_OK | MB_ICONSTOP);
}
