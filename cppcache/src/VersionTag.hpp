/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#ifndef GEODE_VERSIONTAG_H_
#define GEODE_VERSIONTAG_H_

#include <geode/Serializable.hpp>

#include "MemberListForVersionStamp.hpp"

namespace apache {
namespace geode {
namespace client {

using internal::DSFid;

class RegionInternal;
class CacheImpl;

class VersionTag : public DataSerializableFixedId {
 protected:
  uint16_t m_bits;
  int32_t m_entryVersion;
  int16_t m_regionVersionHighBytes;
  int32_t m_regionVersionLowBytes;
  uint16_t m_internalMemId;
  uint16_t m_previousMemId;
  int64_t m_timeStamp;
  MemberListForVersionStamp& m_memberListForVersionStamp;

  static const uint8_t HAS_MEMBER_ID = 0x01;
  static const uint8_t HAS_PREVIOUS_MEMBER_ID = 0x02;
  static const uint8_t VERSION_TWO_BYTES = 0x04;
  static const uint8_t DUPLICATE_MEMBER_IDS = 0x08;
  static const uint8_t HAS_RVV_HIGH_BYTE = 0x10;

  static const uint8_t BITS_POSDUP = 0x01;
  static const uint8_t BITS_RECORDED = 0x02;  // has the rvv recorded this?
  static const uint8_t BITS_HAS_PREVIOUS_ID = 0x03;
  virtual void readMembers(uint16_t flags, DataInput& input);

 public:
  explicit VersionTag(MemberListForVersionStamp& memberListForVersionStamp);

  ~VersionTag() override = default;

  void toData(DataOutput& output) const override;

  void fromData(DataInput& input) override;

  DSFid getDSFID() const override { return DSFid::VersionTag; }

  static std::shared_ptr<Serializable> createDeserializable(
      MemberListForVersionStamp& memberListForVersionStamp);

  int32_t getEntryVersion() const { return m_entryVersion; }
  int16_t getRegionVersionHighBytes() const { return m_regionVersionHighBytes; }
  int32_t getRegionVersionLowBytes() const { return m_regionVersionLowBytes; }
  uint16_t getInternalMemID() const { return m_internalMemId; }
  uint16_t getPreviousMemID() const { return m_previousMemId; }
  void replaceNullMemberId(uint16_t memId);
  void setInternalMemID(uint16_t internalMemId) {
    m_internalMemId = internalMemId;
  }

  /**
   * for internal testing
   */
  VersionTag(int32_t entryVersion, int16_t regionVersionHighBytes,
             int32_t regionVersionLowBytes, uint16_t internalMemId,
             uint16_t previousMemId,
             MemberListForVersionStamp& memberListForVersionStamp);
};

}  // namespace client
}  // namespace geode
}  // namespace apache

#endif  // GEODE_VERSIONTAG_H_
