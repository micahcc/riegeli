// Copyright 2017 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef RIEGELI_BYTES_LINE_READING_H_
#define RIEGELI_BYTES_LINE_READING_H_

#include <stddef.h>

#include <limits>
#include <string>
#include <utility>

#include "absl/strings/string_view.h"
#include "riegeli/bytes/reader.h"

namespace riegeli {

// Options for `ReadLine()`.
class ReadLineOptions {
 public:
  ReadLineOptions() noexcept {}

  // If `false`, recognized line terminator is LF ("\n").
  //
  // If `true`, recognized line terminators are LF, CR, or CRLF ("\n", "\r", or
  // "\r\n").
  //
  // Default: `false`
  ReadLineOptions& set_recognize_cr(bool recognize_cr) & {
    recognize_cr_ = recognize_cr;
    return *this;
  }
  ReadLineOptions&& set_recognize_cr(bool recognize_cr) && {
    return std::move(set_recognize_cr(recognize_cr));
  }
  bool recognize_cr() const { return recognize_cr_; }

  // If `false`, line terminators will be stripped.
  //
  // If `true`, each returned line will include its terminator if it was present
  // (it can be absent in the last line).
  //
  // Default: `false`
  ReadLineOptions& set_keep_newline(bool keep_newline) & {
    keep_newline_ = keep_newline;
    return *this;
  }
  ReadLineOptions&& set_keep_newline(bool keep_newline) && {
    return std::move(set_keep_newline(keep_newline));
  }
  bool keep_newline() const { return keep_newline_; }

  // Expected maximal line length.
  //
  // If this length is exceeded, reading fails with
  // `absl::ResourceExhaustedError()`.
  //
  // Default: `std::numeric_limits<size_t>::max()`
  ReadLineOptions& set_max_length(size_t max_length) & {
    max_length_ = max_length;
    return *this;
  }
  ReadLineOptions&& set_max_length(size_t max_length) && {
    return std::move(set_max_length(max_length));
  }
  size_t max_length() const { return max_length_; }

 private:
  bool recognize_cr_ = false;
  bool keep_newline_ = false;
  size_t max_length_ = std::numeric_limits<size_t>::max();
};

// Reads a line.
//
// Warning: if `options.recognize_cr()` is `true`, for lines terminated with CR
// `ReadLine()` reads ahead one character after the CR. If reading ahead only as
// much as needed is required, e.g. when communicating with another process,
// another implementation would be required (which would keep state between
// calls).
//
// Return values:
//  * `true`                           - success
//  * `false` (when `src->healthy()`)  - source ends (`dest->empty()`)
//  * `false` (when `!src->healthy()`) - failure (`dest->empty()`)
bool ReadLine(Reader* src, absl::string_view* dest,
              ReadLineOptions options = ReadLineOptions());
bool ReadLine(Reader* src, std::string* dest,
              ReadLineOptions options = ReadLineOptions());

}  // namespace riegeli

#endif  // RIEGELI_BYTES_LINE_READING_H_
