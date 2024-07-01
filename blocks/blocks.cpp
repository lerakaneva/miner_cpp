#include "blocks.h"
#include "crypto/crypto.h"

#include <cassert>

#include <ostream>

#include <iostream>

namespace {

byte_arrays::Bytes
CalculateHeaderWithoutNonce(uint32_t version, const std::string &prevHashString,
                            const byte_arrays::Ui256 &merkleRoot, uint32_t ts,
                            const byte_arrays::Ui256 &target) {
    return byte_arrays::Ui32(version) + byte_arrays::Ui256(prevHashString) +
           merkleRoot + byte_arrays::Ui32(ts) + target.GetCompact();
}

std::vector<unsigned char>
CalculateHash(const std::vector<unsigned char> &input) {
    return crypto::CalculateGeneralHash(input.data(), input.size());
}

} // namespace

namespace blocks {

BlockDetFields::BlockDetFields(uint32_t version,
                               const std::string &prevHashString,
                               const byte_arrays::Ui256 &merkleRoot,
                               uint32_t ts, const byte_arrays::Ui256 &target)
    : version_(version), prevHashString_(prevHashString),
      merkleRoot_(merkleRoot), ts_(ts), target_(target),
      headerPart_(CalculateHeaderWithoutNonce(version_, prevHashString_,
                                              merkleRoot_, ts_, target_)) {}

std::ostream &operator<<(std::ostream &os, const BlockDetFields &fields) {
    os << fields.version_ << '\t' << fields.prevHashString_ << '\t'
       << fields.merkleRoot_.GetHexString() << '\t' << fields.ts_ << '\t'
       << fields.target_.GetHexString();
    return os;
}

const byte_arrays::Bytes &BlockDetFields::GetHeaderPart() const {
    return headerPart_;
}

const byte_arrays::Ui256 &BlockDetFields::GetTarget() const { return target_; }

uint32_t BlockDetFields::GetTs() const { return ts_; }

Block::Block(BlockDetFields detFields, uint32_t nonce,
             const byte_arrays::Ui256 &hash)
    : detFields_(detFields), nonce_(nonce), hash_(hash) {}

std::ostream &operator<<(std::ostream &os, const Block &block) {
    os << block.detFields_ << "\t" << block.nonce_ << "\t"
       << block.hash_.GetHexString();
    return os;
}

std::string Block::GetHashString() const { return hash_.GetHexString(); }

uint32_t Block::GetTs() const { return detFields_.GetTs(); }

std::optional<Block> MineBlock(const blocks::BlockDetFields &detFields,
                               uint32_t max_nonce) {
    for (uint32_t nonce = 0; nonce < max_nonce; ++nonce) {
        byte_arrays::Bytes header =
            detFields.GetHeaderPart() + byte_arrays::Ui32(nonce);
        const byte_arrays::Ui256 hash{
            CalculateHash(CalculateHash(header.GetDataLittleEndian()))};
        if (hash < detFields.GetTarget()) {
            return Block(detFields, nonce, hash);
        }
    }
    return {};
}

SimpleBlockchain::SimpleBlockchain(std::string &initial_target_string,
                                   uint32_t blockchain_version,
                                   uint32_t max_nonce,
                                   uint32_t initial_timestamp,
                                   size_t min_attempts_to_find_ts)
    : target_(byte_arrays::Ui256{initial_target_string}),
      blockchain_version_(blockchain_version), max_nonce_(max_nonce),
      min_attempts_to_find_ts_(min_attempts_to_find_ts) {
    const auto block = TryMineBlocks(
        initial_timestamp,
        "0000000000000000000000000000000000000000000000000000000000000000",
        "Genesis Block");
    assert(block);

    chain_.push_back(block.value());
    std::cout << "Mined genesis block" << std::endl;
}

void SimpleBlockchain::MineNextBlock() {
    assert(chain_.size() > 0);
    const auto block =
        TryMineBlocks(chain_.back().GetTs() + 1, chain_.back().GetHashString(),
                      "Block_" + std::to_string(chain_.size() - 1));
    assert(block);
    chain_.push_back(block.value());
}

std::optional<blocks::Block>
SimpleBlockchain::TryMineBlocks(uint32_t timestamp, const std::string &prevHash,
                                const std::string &transactions) {
    for (size_t attempt = 0; attempt < min_attempts_to_find_ts_; ++attempt) {
        const auto blockDetFields =
            blocks::BlockDetFields(blockchain_version_, prevHash,
                                   byte_arrays::EncodeString(transactions),
                                   timestamp + attempt, target_);
        const auto block = MineBlock(blockDetFields, max_nonce_);
        if (block) {
            std::cout << block.value() << std::endl;
            return block;
        }
    }
    return {};
}
} // namespace blocks