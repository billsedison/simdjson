#pragma once

#include "jsonparser/common_defs.h"
#include "jsonparser/simdjson_internal.h"

// structural chars here are
// they are { 0x7b } 0x7d : 0x3a [ 0x5b ] 0x5d , 0x2c
// we are also interested in the four whitespace characters
// space 0x20, linefeed 0x0a, horizontal tab 0x09 and carriage return 0x0d

// these are the chars that can follow a true/false/null or number atom
// and nothing else
const u32 structural_or_whitespace_negated[256] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,

    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1,

    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

// return non-zero if not a structural or whitespace char
// zero otherwise
really_inline u32 is_not_structural_or_whitespace(u8 c) {
  return structural_or_whitespace_negated[c];
}

const char digittoval[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0,  1,  2,  3,  4,  5,  6,  7,  8,
    9,  -1, -1, -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1};

// return true if we have a valid hex between 0000 and FFFF
/*inline bool hex_to_u32(const u8 *src, u32 *res) {
  u8 v1 = src[0];
  u8 v2 = src[1];
  u8 v3 = src[2];
  u8 v4 = src[3];
  *res = digittoval[v1] << 12 | digittoval[v2] << 8 | digittoval[v3] << 4 |
         digittoval[v4];
  return (int32_t)(*res) >= 0;
}*/

// returns a value with the highest bit set if it is not valud
uint32_t hex_to_u32_nocheck(const u8 *src) {
  u8 v1 = src[0];
  u8 v2 = src[1];
  u8 v3 = src[2];
  u8 v4 = src[3];
  return digittoval[v1] << 12 | digittoval[v2] << 8 | digittoval[v3] << 4 |
         digittoval[v4];
}

// given a code point cp, writes to c
// the utf-8 code, outputting the length in
// bytes, if the length is zero, the code point
// is invalid
//
// This can possibly be made faster using pdep
// and clz and table lookups, but JSON documents
// have few escaped code points, and the following
// function looks cheap.
// 
// Note: we assume that surrogates are treated separately
//
inline size_t codepoint_to_utf8(uint32_t cp, u8 *c) {
  if (cp <= 0x7F) {
    c[0] = cp;
    return 1; // ascii
  } else if (cp <= 0x7FF) {
    c[0] = (cp >> 6) + 192;
    c[1] = (cp & 63) + 128;
    return 2; // universal plane
  //  Surrogates are treated elsewhere...
  //} //else if (0xd800 <= cp && cp <= 0xdfff) {
  //  return 0; // surrogates // could put assert here
  } else if (cp <= 0xFFFF) {
    c[0] = (cp >> 12) + 224;
    c[1] = ((cp >> 6) & 63) + 128;
    c[2] = (cp & 63) + 128;
    return 3;
  } else if (cp <= 0x10FFFF) { // if you know you have a valid code point, this is not needed
    c[0] = (cp >> 18) + 240;
    c[1] = ((cp >> 12) & 63) + 128;
    c[2] = ((cp >> 6) & 63) + 128;
    c[3] = (cp & 63) + 128;
    return 4;
  }
  // will return 0 when the code point was too large.
  return 0; // bad r
}


