#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>

std::vector<uint8_t> createETH2Frame(const std::vector<uint8_t> &data,
                                     const uint8_t *sourceMac, const uint8_t *destinationMac,
                                     const uint8_t ttl, const uint8_t tos, const uint8_t protocol,
                                     const uint32_t sourceAddress, const uint32_t destinationAddress);

// get the minimum size of a ETH Frame that contains a IP payload
// 34 = 14 bytes for ETH2 + 20 byes IPv4 (minimum)
size_t getIPMinimumSize();

void getIPPayload(const size_t lengthOfFrame, const uint8_t *frame,
                  size_t &lengthOfPayload, const uint8_t *&payload, uint32_t &destination, uint8_t &protocol);
