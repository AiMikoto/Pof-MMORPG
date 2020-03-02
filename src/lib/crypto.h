#ifndef LIB_CRYPTO_CPP
#define LIB_CRYPTO_CPP

#include <string>

class crypto
{
public:
  std::string key;
  std::string iv;
  std::string encrypt(std::string data);
  std::string decrypt(std::string data);
private:
  int block_size = 128;
};

#endif // LIB_CRYPTO_CPP
