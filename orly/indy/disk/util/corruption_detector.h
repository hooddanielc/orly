/* <orly/indy/disk/util/corruption_detector.h>

   TODO

   Copyright 2010-2014 OrlyAtomics, Inc.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

#pragma once

#include <cassert>
#include <cstdint>

#include <unistd.h>

#include <base/error_utils.h>
#include <base/murmur.h>
#include <base/no_construction.h>

namespace Orly {

  namespace Indy {

    namespace Disk {

      namespace Util {

        /* TODO */
        class TCorruptionDetector {
          NO_CONSTRUCTION(TCorruptionDetector);
          public:

          /* TODO */
          static void inline WriteMurmur(size_t *buf, size_t buf_size, size_t key) {
            *reinterpret_cast<uint64_t *>(reinterpret_cast<uint8_t *>(buf) + buf_size - sizeof(size_t)) = Base::Murmur(buf, (buf_size / sizeof(size_t) - 1), key);
          }

          /* TODO */
          static bool inline TryReadMurmur(size_t *buf, size_t buf_size, size_t key) {
            uint64_t hash = *reinterpret_cast<uint64_t *>(reinterpret_cast<uint8_t *>(buf) + buf_size - sizeof(size_t));
            return hash == Base::Murmur(buf, (buf_size / sizeof(size_t) - 1), key);
          }

          /* TODO */
          static void inline ReadMurmur(size_t *buf, size_t buf_size, size_t key) {
            if (!TryReadMurmur(buf, buf_size, key)) {
              throw std::runtime_error("Caught Disk Corruption");
            }
          }

        };  // TCorruptionDetector

      }  // Util

    }  // Disk

  }  // Indy

}  // Orly