#include "crypto.h"
#include <openssl/sha.h> // For SHA256 hashing

#if OPENSSL_VERSION_NUMBER >= 0x030000000 // 3.0.0
#include <openssl/evp.h>
#endif

namespace crypto {
std::vector<unsigned char> CalculateGeneralHash(const void *data, size_t len) {
    unsigned char hash[SHA256_DIGEST_LENGTH];

#if OPENSSL_VERSION_NUMBER >= 0x030000000 // 3.0.0
    EVP_Digest(data, len, hash, NULL, EVP_sha256(), NULL);
#else
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data, len);
    SHA256_Final(hash, &sha256);
#endif

    return std::vector<unsigned char>(hash, hash + SHA256_DIGEST_LENGTH);
}
} // namespace crypto