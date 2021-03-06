/* <strm/bin/var_int.test.cc>

   Unit test for <strm/bin/var_int.h>.

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

#include <strm/bin/var_int.h>

#include <algorithm>
#include <cstring>
#include <functional>
#include <limits>
#include <utility>
#include <vector>

#include <base/bg_generator.h>
#include <test/kit.h>

using namespace std;
using namespace placeholders;
using namespace Base;
using namespace Strm::Bin;

using TItem = pair<uint64_t, size_t>;

/* Generates pairs of value-size, where the size is the number of bytes which
   should be required to encode the value. */
static bool ForEachItem(const function<bool (const TItem &)> &cb) {
  assert(cb);
  bool keep_going = cb(TItem(0, 1)) && cb(TItem(1, 1));
  for (int i = 0; keep_going && i < 9; ++i) {
    uint64_t val = static_cast<uint64_t>(1) << ((i + 1) * 7);
    keep_going = cb(TItem(val - 1, i + 1)) && cb(TItem(val, i + 2));
  }
  return keep_going && cb(TItem(numeric_limits<uint64_t>::max(), 10));
}

FIXTURE(RoundTrip) {
  TVarIntEncoder encoder;
  TVarIntDecoder decoder;
  ForEachItem(
    [&encoder, &decoder](const TItem &item) {
      assert(&item);
      /* Encode the value and make sure the encoded form is the expected
         number of bytes. */
      encoder.Encode(item.first);
      if (!EXPECT_EQ(encoder.GetSize(), item.second)) {
        return false;
      }
      /* Decode the encoded form and make sure we got the original value
         back. */
      decoder.Decode(encoder.GetStart(), encoder.GetLimit());
      const auto *val = *decoder;
      return EXPECT_TRUE(val) && EXPECT_EQ(*val, item.first);
    }
  );
}

using TRange = pair<const uint8_t *, const uint8_t *>;

/* Generates start-limit ranges of a given number of bytes from a given
   buffer.  We use this to break up the buffer into pieces of various
   sizes, to test our ability to decode partial encodings. */
static bool ForEachRange(
    const function<bool (const TRange &)> &cb,
    const uint8_t *start, const uint8_t *limit, size_t range_size) {
  assert(cb);
  while (start < limit) {
    const uint8_t *next = min(start + range_size, limit);
    if (!cb(TRange(start, next))) {
      return false;
    }
    start = next;
  }
  return true;
}

/* For a given sequence of ranges of bytes, generate all the values we can.
   We assume the bytes contain encoded var-ints and that we will decode
   full and partial images. */
static bool ForEachVal(
    const function<bool (uint64_t)> &cb, TBgGenerator<TRange> &range_gen) {
  assert(cb);
  /* Loop while we have ranges of bytes to decode. */
  TVarIntDecoder decoder;
  while (range_gen) {
    /* Pop the next range from the range generator. */
    const TRange &range = *range_gen;
    ++range_gen;
    const uint8_t
        *cursor = range.first,
        *limit = range.second;
    /* Consume bytes. */
    while (cursor < limit) {
      /* Try to decode the next var-int.  If we succeed, yield the value. */
      cursor = decoder.Decode(cursor, limit);
      auto *val = *decoder;
      if (val && !cb(*val)) {
        return false;
      }
    }
  }
  return true;
}

FIXTURE(Piecewise) {
  /* Add up the var-int-encoded sizes of all the values generated by
     ForEachItem. */
  size_t size = 0;
  ForEachItem(
    [&size](const TItem &item) {
      size += item.second;
      return true;
    }
  );
  /* Make a buffer large enough to hold all the encoded var-ints and
     copy them in. */
  vector<uint8_t> buf(size);
  uint8_t
      *start = &buf[0],
      *limit = start;
  ForEachItem(
    [&limit](const TItem &item) {
      TVarIntEncoder encoder(item.first);
      auto size = encoder.GetSize();
      memcpy(limit, encoder.GetStart(), size);
      limit += size;
      return true;
    }
  );
  /* We'll decode the buffer's contents in chunks of various sizes. */
  vector<size_t> range_sizes = {
    size, size - 1, size / 2, size / 3, size / 4, 17
  };
  for (size_t range_size: range_sizes) {
    /* Generate ranges of the encoded data. */
    TBgGenerator<TRange> range_gen(bind(
        ForEachRange, _1, start, limit, range_size));
    /* Generate values by consuming those ranges as needed. */
    TBgGenerator<uint64_t> actual_gen(bind(
        ForEachVal, _1, ref(range_gen)));
    /* Generate the expected values again so we can compare. */
    TBgGenerator<TItem> expected_gen(ForEachItem);
    /* Compare the values we decode with the expected values. */
    for (; actual_gen && expected_gen; ++actual_gen, ++expected_gen) {
      EXPECT_EQ(*actual_gen, expected_gen->first);
    }
    /* Make sure the actual and expected generators finished simultaneously. */
    EXPECT_FALSE(actual_gen);
    EXPECT_FALSE(expected_gen);
  }
}
