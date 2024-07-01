#pragma once

#include "byte_arrays/byte_arrays.h"

#include <optional>
#include <string>

namespace blocks {

class BlockDetFields {
  public:
    BlockDetFields(uint32_t version, const std::string &prevHashString,
                   const byte_arrays::Ui256 &merkleRoot, uint32_t ts,
                   const byte_arrays::Ui256 &target);
    friend std::ostream &operator<<(std::ostream &os,
                                    const BlockDetFields &fields);
    const byte_arrays::Bytes &GetHeaderPart() const;
    const byte_arrays::Ui256 &GetTarget() const;
    uint32_t GetTs() const;

  private:
    uint32_t version_;
    std::string prevHashString_;
    byte_arrays::Ui256 merkleRoot_;
    uint32_t ts_;
    byte_arrays::Ui256 target_;
    byte_arrays::Bytes headerPart_;
};

class Block {
  public:
    Block(BlockDetFields detFields, uint32_t nonce,
          const byte_arrays::Ui256 &hash);
    friend std::ostream &operator<<(std::ostream &os, const Block &fields);
    std::string GetHashString() const;
    uint32_t GetTs() const;

  private:
    BlockDetFields detFields_;
    uint32_t nonce_;
    byte_arrays::Ui256 hash_;
};

std::optional<Block> MineBlock(const blocks::BlockDetFields &detFields,
                               uint32_t max_nonce);

class SimpleBlockchain {
  public:
    SimpleBlockchain(std::string &initial_target_string,
                     uint32_t blockchain_version, uint32_t max_nonce,
                     uint32_t initial_timestamp,
                     size_t min_attempts_to_find_ts);
    void MineNextBlock();

  private:
    std::optional<blocks::Block> TryMineBlocks(uint32_t timestamp,
                                               const std::string &prevHash,
                                               const std::string &transactions);

  private:
    byte_arrays::Ui256 target_;
    uint32_t blockchain_version_;
    uint32_t max_nonce_;
    size_t min_attempts_to_find_ts_;
    std::vector<blocks::Block> chain_;
};

} // namespace blocks