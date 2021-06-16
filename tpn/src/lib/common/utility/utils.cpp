//
//           ┌┬┐┬ ┬┌─┐┬ ┬┌─┐┌─┐┌┐┌
//            │ └┬┘├─┘├─┤│ ││ ││││
//            ┴  ┴ ┴  ┴ ┴└─┘└─┘┘└┘
//
// This file is part of the typhoon Project.
// Copyright (C) 2021 stanley0207@163.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "utils.h"

#include <cstring>

#include <utfcpp/utf8.h>

namespace tpn {

bool Utf8ToWstr(std::string_view utf8strv, std::wstring &wstr) {
  try {
    if (size_t len = utf8::distance(utf8strv.data(),
                                    utf8strv.data() + utf8strv.size())) {
      wstr.resize(len);
      utf8::utf8to16(utf8strv.data(), utf8strv.data() + utf8strv.size(),
                     &wstr[0]);
    }
  } catch (const std::exception &) {
    wstr.clear();
    return false;
  }
  return true;
}

bool Utf8ToWstr(const char *utf8str, size_t utf8size, wchar_t *wstr,
                size_t &wsize) {
  try {
    size_t len = utf8::distance(utf8str, utf8str + utf8size);
    if (len > wsize) {
      if (wsize > 0) {
        wstr[0] = L'\0';
      }
      wsize = 0;
      return false;
    }

    wsize = len;
    utf8::utf8to16(utf8str, utf8str + utf8size, wstr);
    wstr[len] = L'\0';
  } catch (const std::exception &) {
    if (wsize > 0) {
      wstr[0] = L'\0';
    }
    wsize = 0;
    return false;
  }
  return true;
}

bool Utf8ToWstr(std::string_view utf8strv, wchar_t *wstr, size_t &wsize) {
  return Utf8ToWstr(utf8strv.data(), utf8strv.size(), wstr, wsize);
}

bool WstrToUtf8(std::wstring_view wstrv, std::string &utf8str) {
  try {
    std::string tmp;
    tmp.resize(wstrv.size() * 4);
    if (wstrv.size()) {
      char *oend =
          utf8::utf16to8(wstrv.data(), wstrv.data() + wstrv.size(), &tmp[0]);
      tmp.resize(oend - (&tmp[0]));
    }
    utf8str = tmp;
  } catch (const std::exception &) {
    utf8str.clear();
    return false;
  }
  return true;
}

bool WstrToUtf8(wchar_t *wstr, size_t wsize, std::string &utf8str) {
  try {
    std::string tmp;
    tmp.resize(wsize * 4);
    if (wsize) {
      char *oend = utf8::utf16to8(wstr, wstr + wsize, &tmp[0]);
      tmp.resize(oend - (&tmp[0]));
    }
    utf8str = tmp;
  } catch (const std::exception &) {
    utf8str.clear();
    return false;
  }
  return true;
}

size_t Utf8Length(std::string &utf8str) {
  try {
    return utf8::distance(utf8str.c_str(), utf8str.c_str() + utf8str.size());
  } catch (const std::exception &) {
    utf8str.clear();
    return 0;
  }
  return true;
}

void Utf8Truncate(std::string &utf8str, size_t len) {
  try {
    size_t len_old =
        utf8::distance(utf8str.c_str(), utf8str.c_str() + utf8str.size());
    if (len_old <= len) {
      return;
    }

    std::wstring wstr;
    wstr.resize(len_old);
    utf8::utf8to16(utf8str.c_str(), utf8str.c_str() + utf8str.size(), &wstr[0]);
    wstr.resize(len);
    char *oend =
        utf8::utf16to8(wstr.c_str(), wstr.c_str() + wstr.size(), &utf8str[0]);
    utf8str.resize(oend - (&utf8str[0]));
  } catch (const std::exception &) {
    utf8str.clear();
  }
}

std::string &TrimLeft(std::string &src) {
  size_t pos = src.find_first_not_of(" \r\n\t");
  if (pos != std::string::npos) {
    src.erase(0, pos);
  }
  return src;
}

std::string &TrimRight(std::string &src) {
  size_t pos = src.find_last_not_of(" \r\n\t");
  if (pos != std::string::npos) {
    src.erase(pos + 1);
  }
  return src;
}

std::string &Trim(std::string &src) {
  if (src.empty()) {
    return src;
  }

  return TrimRight(TrimLeft(src));
}

std::vector<std::string> Tokenize(std::string_view input,
                                  std::string_view delimiters) {
  std::vector<std::string> tokens;
  size_t pos, last_pos = 0;

  while (true) {
    bool done = false;
    pos       = input.find_first_of(delimiters, last_pos);
    if (pos == std::string::npos) {
      done = true;
      pos  = input.length();
    }

    tokens.push_back(std::string{input.data() + last_pos, pos - last_pos});
    if (done) {
      return std::move(tokens);
    }
    last_pos = pos + 1;
  }
}

Tokenizer::Tokenizer(std::string_view src, char delimiter,
                     uint32_t reserve /* = 0 */,
                     bool keep_empty_strings /* = true */) {
  str_ = new char[src.length() + 1];
  std::memcpy(str_, src.data(), src.length() + 1);

  if (reserve) {
    storage_.reserve(reserve);
  }

  char *pos_old = str_;
  char *pos_new = str_;

  while (true) {
    if (*pos_new == delimiter) {  // 分隔符
      if (keep_empty_strings || pos_old != pos_new) {
        storage_.emplace_back(pos_old);
      }
      pos_old  = pos_new + 1;
      *pos_new = '\0';
    } else if ('\0' == *pos_new) {  // 结束符
      if (pos_old != pos_new) {
        storage_.emplace_back(pos_old);
      }
      break;
    }

    ++pos_new;
  }
}

}  // namespace tpn
