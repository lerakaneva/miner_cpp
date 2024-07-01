#include "blocks/blocks.h"

#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {

    // Default values
    std::string target =
        "0000100000000000000000000000000000000000000000000000000000000000";
    uint32_t blockchain_version = 1;
    uint32_t max_nonce = 4294967295;
    uint32_t initial_timestamp = 0;
    size_t min_attempts_to_find_ts = 1;

    struct option long_options[] = {
        {"target", required_argument, nullptr, 't'},
        {"version", required_argument, nullptr, 'v'},
        {"max_nonce", required_argument, nullptr, 'n'},
        {"timestamp", required_argument, nullptr, 's'},
        {"attempts", required_argument, nullptr, 'a'},
        {nullptr, 0, nullptr, 0}};

    int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "t:v:n:s:a:", long_options,
                              &option_index)) != -1) {
        switch (opt) {
        case 't':
            target = optarg;
            break;
        case 'v':
            blockchain_version = std::strtoul(optarg, nullptr, 10);
            break;
        case 'n':
            max_nonce = std::strtoul(optarg, nullptr, 10);
            break;
        case 's':
            initial_timestamp = std::strtoul(optarg, nullptr, 10);
            break;
        case 'a':
            min_attempts_to_find_ts = std::strtoul(optarg, nullptr, 10);
            break;
        default:
            std::cerr << "Usage: " << argv[0] << " [options]\n"
                      << "Options:\n"
                      << "  --target <target>           Target\n"
                      << "  --version <version>         Blockchain version\n"
                      << "  --max_nonce <max_nonce>     Maximum nonce\n"
                      << "  --timestamp <timestamp>     Initial timestamp\n"
                      << "  --attempts <attempts>       Minimum attempts to "
                         "find timestamp\n";
            return 1;
        }
    }

    // Output the parsed values
    std::cout << "Target: " << target << std::endl;
    std::cout << "Blockchain Version: " << blockchain_version << std::endl;
    std::cout << "Max Nonce: " << max_nonce << std::endl;
    std::cout << "Initial Timestamp: " << initial_timestamp << std::endl;
    std::cout << "Minimum Attempts to Find Timestamp: "
              << min_attempts_to_find_ts << std::endl;

    blocks::SimpleBlockchain blockchain{target, blockchain_version, max_nonce,
                                initial_timestamp, min_attempts_to_find_ts};
    for (int i = 0; i < 10; ++i) {
        blockchain.MineNextBlock();
    }
    return 0;
}