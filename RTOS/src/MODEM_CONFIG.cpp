#include "../includes/MODEM_CONFIG.hpp"

TinyGsm modem(SerialAT);

TinyGsmClient client(modem);
