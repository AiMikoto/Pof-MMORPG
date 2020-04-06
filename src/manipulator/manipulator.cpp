#include "manipulator/crypto.h"
#include "manipulator/manipulator.h"

manipulator::manipulator(std::string hostname, int port, std::string tok):rcon(hostname, port, tok, g_rsa, g_aes)
{
}

unsigned long long manipulator::spawn()
{
}
