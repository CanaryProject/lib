/**
 * Canary Lib - Canary Project a free 2D game platform
 * Copyright (C) 2020  Lucas Grossi <lucas.ggrossi@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "xtea.hpp"

namespace CanaryLib {
  bool XTEA::decrypt(uint16_t length, uint8_t* buffer) const {
    #if defined(__AVX512F__)
    int32_t messageLength = static_cast<int32_t>(length) - 256;
    #elif defined(__AVX2__)
    int32_t messageLength = static_cast<int32_t>(length) - 128;
    #elif defined(__SSE2__)
    int32_t messageLength = static_cast<int32_t>(length) - 64;
    #else
    int32_t messageLength = static_cast<int32_t>(length);
    #endif
    int32_t readPos = 0;
    const uint32_t k[] = {key[0], key[1], key[2], key[3]};
    uint32_t precachedControlSum[32][2];
    uint32_t sum = 0xC6EF3720;
    for (int32_t i = 0; i < 32; ++i) {
      precachedControlSum[i][0] = (sum + k[(sum >> 11) & 3]);
      sum += XTEA_DELTA;
      precachedControlSum[i][1] = (sum + k[sum & 3]);
    }
    #if defined(__AVX512F__)
    while (readPos <= messageLength) {
      const __m512i data0 = _mm512_shuffle_epi32(_mm512_loadu_si512(reinterpret_cast<const void*>(buffer + readPos)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m512i data1 = _mm512_shuffle_epi32(_mm512_loadu_si512(reinterpret_cast<const void*>(buffer + readPos + 64)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m512i data2 = _mm512_shuffle_epi32(_mm512_loadu_si512(reinterpret_cast<const void*>(buffer + readPos + 128)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m512i data3 = _mm512_shuffle_epi32(_mm512_loadu_si512(reinterpret_cast<const void*>(buffer + readPos + 192)), _MM_SHUFFLE(3, 1, 2, 0));
      __m512i vdata0 = _mm512_unpacklo_epi64(data0, data1);
      __m512i vdata1 = _mm512_unpackhi_epi64(data0, data1);
      __m512i vdata2 = _mm512_unpacklo_epi64(data2, data3);
      __m512i vdata3 = _mm512_unpackhi_epi64(data2, data3);
      for (int32_t i = 0; i < 32; ++i) {
        vdata1 = _mm512_sub_epi32(vdata1, _mm512_xor_si512(_mm512_add_epi32(_mm512_xor_si512(_mm512_slli_epi32(vdata0, 4), _mm512_srli_epi32(vdata0, 5)), vdata0), _mm512_set1_epi32(precachedControlSum[i][0])));
        vdata0 = _mm512_sub_epi32(vdata0, _mm512_xor_si512(_mm512_add_epi32(_mm512_xor_si512(_mm512_slli_epi32(vdata1, 4), _mm512_srli_epi32(vdata1, 5)), vdata1), _mm512_set1_epi32(precachedControlSum[i][1])));
        vdata3 = _mm512_sub_epi32(vdata3, _mm512_xor_si512(_mm512_add_epi32(_mm512_xor_si512(_mm512_slli_epi32(vdata2, 4), _mm512_srli_epi32(vdata2, 5)), vdata2), _mm512_set1_epi32(precachedControlSum[i][0])));
        vdata2 = _mm512_sub_epi32(vdata2, _mm512_xor_si512(_mm512_add_epi32(_mm512_xor_si512(_mm512_slli_epi32(vdata3, 4), _mm512_srli_epi32(vdata3, 5)), vdata3), _mm512_set1_epi32(precachedControlSum[i][1])));
      }
      _mm512_storeu_si512(reinterpret_cast<void*>(buffer + readPos), _mm512_unpacklo_epi32(vdata0, vdata1));
      readPos += 64;
      _mm512_storeu_si512(reinterpret_cast<void*>(buffer + readPos), _mm512_unpackhi_epi32(vdata0, vdata1));
      readPos += 64;
      _mm512_storeu_si512(reinterpret_cast<void*>(buffer + readPos), _mm512_unpacklo_epi32(vdata2, vdata3));
      readPos += 64;
      _mm512_storeu_si512(reinterpret_cast<void*>(buffer + readPos), _mm512_unpackhi_epi32(vdata2, vdata3));
      readPos += 64;
    }
    messageLength += 128;
    if (readPos <= messageLength) {
      const __m512i data0 = _mm512_shuffle_epi32(_mm512_loadu_si512(reinterpret_cast<const void*>(buffer + readPos)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m512i data1 = _mm512_shuffle_epi32(_mm512_loadu_si512(reinterpret_cast<const void*>(buffer + readPos + 64)), _MM_SHUFFLE(3, 1, 2, 0));
      __m512i vdata0 = _mm512_unpacklo_epi64(data0, data1);
      __m512i vdata1 = _mm512_unpackhi_epi64(data0, data1);
      for (int32_t i = 0; i < 32; ++i) {
        vdata1 = _mm512_sub_epi32(vdata1, _mm512_xor_si512(_mm512_add_epi32(_mm512_xor_si512(_mm512_slli_epi32(vdata0, 4), _mm512_srli_epi32(vdata0, 5)), vdata0), _mm512_set1_epi32(precachedControlSum[i][0])));
        vdata0 = _mm512_sub_epi32(vdata0, _mm512_xor_si512(_mm512_add_epi32(_mm512_xor_si512(_mm512_slli_epi32(vdata1, 4), _mm512_srli_epi32(vdata1, 5)), vdata1), _mm512_set1_epi32(precachedControlSum[i][1])));
      }
      _mm512_storeu_si512(reinterpret_cast<void*>(buffer + readPos), _mm512_unpacklo_epi32(vdata0, vdata1));
      readPos += 64;
      _mm512_storeu_si512(reinterpret_cast<void*>(buffer + readPos), _mm512_unpackhi_epi32(vdata0, vdata1));
      readPos += 64;
    }
    messageLength += 64;
    #endif
    #if defined(__AVX2__)

    //AVX2 unroll - only if AVX512 disabled
    #if !defined(__AVX512F__)
    while (readPos <= messageLength) {
      const __m256i data0 = _mm256_shuffle_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(buffer + readPos)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m256i data1 = _mm256_shuffle_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(buffer + readPos + 32)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m256i data2 = _mm256_shuffle_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(buffer + readPos + 64)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m256i data3 = _mm256_shuffle_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(buffer + readPos + 96)), _MM_SHUFFLE(3, 1, 2, 0));
      __m256i vdata0 = _mm256_unpacklo_epi64(data0, data1);
      __m256i vdata1 = _mm256_unpackhi_epi64(data0, data1);
      __m256i vdata2 = _mm256_unpacklo_epi64(data2, data3);
      __m256i vdata3 = _mm256_unpackhi_epi64(data2, data3);
      for (int32_t i = 0; i < 32; ++i) {
        vdata1 = _mm256_sub_epi32(vdata1, _mm256_xor_si256(_mm256_add_epi32(_mm256_xor_si256(_mm256_slli_epi32(vdata0, 4), _mm256_srli_epi32(vdata0, 5)), vdata0), _mm256_set1_epi32(precachedControlSum[i][0])));
        vdata0 = _mm256_sub_epi32(vdata0, _mm256_xor_si256(_mm256_add_epi32(_mm256_xor_si256(_mm256_slli_epi32(vdata1, 4), _mm256_srli_epi32(vdata1, 5)), vdata1), _mm256_set1_epi32(precachedControlSum[i][1])));
        vdata3 = _mm256_sub_epi32(vdata3, _mm256_xor_si256(_mm256_add_epi32(_mm256_xor_si256(_mm256_slli_epi32(vdata2, 4), _mm256_srli_epi32(vdata2, 5)), vdata2), _mm256_set1_epi32(precachedControlSum[i][0])));
        vdata2 = _mm256_sub_epi32(vdata2, _mm256_xor_si256(_mm256_add_epi32(_mm256_xor_si256(_mm256_slli_epi32(vdata3, 4), _mm256_srli_epi32(vdata3, 5)), vdata3), _mm256_set1_epi32(precachedControlSum[i][1])));
      }
      _mm256_storeu_si256(reinterpret_cast<__m256i*>(buffer + readPos), _mm256_unpacklo_epi32(vdata0, vdata1));
      readPos += 32;
      _mm256_storeu_si256(reinterpret_cast<__m256i*>(buffer + readPos), _mm256_unpackhi_epi32(vdata0, vdata1));
      readPos += 32;
      _mm256_storeu_si256(reinterpret_cast<__m256i*>(buffer + readPos), _mm256_unpacklo_epi32(vdata2, vdata3));
      readPos += 32;
      _mm256_storeu_si256(reinterpret_cast<__m256i*>(buffer + readPos), _mm256_unpackhi_epi32(vdata2, vdata3));
      readPos += 32;
    }
    messageLength += 64;
    #endif

    if (readPos <= messageLength) {
      const __m256i data0 = _mm256_shuffle_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(buffer + readPos)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m256i data1 = _mm256_shuffle_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(buffer + readPos + 32)), _MM_SHUFFLE(3, 1, 2, 0));
      __m256i vdata0 = _mm256_unpacklo_epi64(data0, data1);
      __m256i vdata1 = _mm256_unpackhi_epi64(data0, data1);
      for (int32_t i = 0; i < 32; ++i) {
        vdata1 = _mm256_sub_epi32(vdata1, _mm256_xor_si256(_mm256_add_epi32(_mm256_xor_si256(_mm256_slli_epi32(vdata0, 4), _mm256_srli_epi32(vdata0, 5)), vdata0), _mm256_set1_epi32(precachedControlSum[i][0])));
        vdata0 = _mm256_sub_epi32(vdata0, _mm256_xor_si256(_mm256_add_epi32(_mm256_xor_si256(_mm256_slli_epi32(vdata1, 4), _mm256_srli_epi32(vdata1, 5)), vdata1), _mm256_set1_epi32(precachedControlSum[i][1])));
      }
      _mm256_storeu_si256(reinterpret_cast<__m256i*>(buffer + readPos), _mm256_unpacklo_epi32(vdata0, vdata1));
      readPos += 32;
      _mm256_storeu_si256(reinterpret_cast<__m256i*>(buffer + readPos), _mm256_unpackhi_epi32(vdata0, vdata1));
      readPos += 32;
    }
    messageLength += 32;
    #endif
    #if defined(__SSE2__)

    //SSE2 unroll - only if AVX2 disabled
    #if !defined(__AVX2__)
    while (readPos <= messageLength) {
      const __m128i data0 = _mm_shuffle_epi32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(buffer + readPos)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m128i data1 = _mm_shuffle_epi32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(buffer + readPos + 16)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m128i data2 = _mm_shuffle_epi32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(buffer + readPos + 32)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m128i data3 = _mm_shuffle_epi32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(buffer + readPos + 48)), _MM_SHUFFLE(3, 1, 2, 0));
      __m128i vdata0 = _mm_unpacklo_epi64(data0, data1);
      __m128i vdata1 = _mm_unpackhi_epi64(data0, data1);
      __m128i vdata2 = _mm_unpacklo_epi64(data2, data3);
      __m128i vdata3 = _mm_unpackhi_epi64(data2, data3);
      for (int32_t i = 0; i < 32; ++i) {
        vdata1 = _mm_sub_epi32(vdata1, _mm_xor_si128(_mm_add_epi32(_mm_xor_si128(_mm_slli_epi32(vdata0, 4), _mm_srli_epi32(vdata0, 5)), vdata0), _mm_set1_epi32(precachedControlSum[i][0])));
        vdata0 = _mm_sub_epi32(vdata0, _mm_xor_si128(_mm_add_epi32(_mm_xor_si128(_mm_slli_epi32(vdata1, 4), _mm_srli_epi32(vdata1, 5)), vdata1), _mm_set1_epi32(precachedControlSum[i][1])));
        vdata3 = _mm_sub_epi32(vdata3, _mm_xor_si128(_mm_add_epi32(_mm_xor_si128(_mm_slli_epi32(vdata2, 4), _mm_srli_epi32(vdata2, 5)), vdata2), _mm_set1_epi32(precachedControlSum[i][0])));
        vdata2 = _mm_sub_epi32(vdata2, _mm_xor_si128(_mm_add_epi32(_mm_xor_si128(_mm_slli_epi32(vdata3, 4), _mm_srli_epi32(vdata3, 5)), vdata3), _mm_set1_epi32(precachedControlSum[i][1])));
      }
      _mm_storeu_si128(reinterpret_cast<__m128i*>(buffer + readPos), _mm_unpacklo_epi32(vdata0, vdata1));
      readPos += 16;
      _mm_storeu_si128(reinterpret_cast<__m128i*>(buffer + readPos), _mm_unpackhi_epi32(vdata0, vdata1));
      readPos += 16;
      _mm_storeu_si128(reinterpret_cast<__m128i*>(buffer + readPos), _mm_unpacklo_epi32(vdata2, vdata3));
      readPos += 16;
      _mm_storeu_si128(reinterpret_cast<__m128i*>(buffer + readPos), _mm_unpackhi_epi32(vdata2, vdata3));
      readPos += 16;
    }
    messageLength += 32;
    #endif

    if (readPos <= messageLength) {
      const __m128i data0 = _mm_shuffle_epi32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(buffer + readPos)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m128i data1 = _mm_shuffle_epi32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(buffer + readPos + 16)), _MM_SHUFFLE(3, 1, 2, 0));
      __m128i vdata0 = _mm_unpacklo_epi64(data0, data1);
      __m128i vdata1 = _mm_unpackhi_epi64(data0, data1);
      for (int32_t i = 0; i < 32; ++i) {
        vdata1 = _mm_sub_epi32(vdata1, _mm_xor_si128(_mm_add_epi32(_mm_xor_si128(_mm_slli_epi32(vdata0, 4), _mm_srli_epi32(vdata0, 5)), vdata0), _mm_set1_epi32(precachedControlSum[i][0])));
        vdata0 = _mm_sub_epi32(vdata0, _mm_xor_si128(_mm_add_epi32(_mm_xor_si128(_mm_slli_epi32(vdata1, 4), _mm_srli_epi32(vdata1, 5)), vdata1), _mm_set1_epi32(precachedControlSum[i][1])));
      }
      _mm_storeu_si128(reinterpret_cast<__m128i*>(buffer + readPos), _mm_unpacklo_epi32(vdata0, vdata1));
      readPos += 16;
      _mm_storeu_si128(reinterpret_cast<__m128i*>(buffer + readPos), _mm_unpackhi_epi32(vdata0, vdata1));
      readPos += 16;
    }
    messageLength += 32;
    #endif
    while (readPos < messageLength) {
      uint32_t vData[2];
      memcpy(vData, buffer + readPos, 8);
      for (int32_t i = 0; i < 32; ++i) {
        vData[1] -= ((vData[0] << 4 ^ vData[0] >> 5) + vData[0]) ^ precachedControlSum[i][0];
        vData[0] -= ((vData[1] << 4 ^ vData[1] >> 5) + vData[1]) ^ precachedControlSum[i][1];
      }
      memcpy(buffer + readPos, vData, 8);
      readPos += 8;
    }
  }

  void XTEA::encrypt(uint16_t length, uint8_t* buffer) const {
    #if defined(__AVX512F__)
    int32_t messageLength = static_cast<int32_t>(length) - 256;
    #elif defined(__AVX2__)
    int32_t messageLength = static_cast<int32_t>(length) - 128;
    #elif defined(__SSE2__)
    int32_t messageLength = static_cast<int32_t>(length) - 64;
    #else
    int32_t messageLength = static_cast<int32_t>(length);
    #endif
    int32_t readPos = 0;
    const uint32_t k[] = {key[0], key[1], key[2], key[3]};
    uint32_t precachedControlSum[32][2];
    uint32_t sum = 0;
    for (int32_t i = 0; i < 32; ++i) {
      precachedControlSum[i][0] = (sum + k[sum & 3]);
      sum -= XTEA_DELTA;
      precachedControlSum[i][1] = (sum + k[(sum >> 11) & 3]);
    }
    #if defined(__AVX512F__)
    while (readPos <= messageLength) {
      const __m512i data0 = _mm512_shuffle_epi32(_mm512_loadu_si512(reinterpret_cast<const void*>(buffer + readPos)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m512i data1 = _mm512_shuffle_epi32(_mm512_loadu_si512(reinterpret_cast<const void*>(buffer + readPos + 64)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m512i data3 = _mm512_shuffle_epi32(_mm512_loadu_si512(reinterpret_cast<const void*>(buffer + readPos + 128)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m512i data4 = _mm512_shuffle_epi32(_mm512_loadu_si512(reinterpret_cast<const void*>(buffer + readPos + 192)), _MM_SHUFFLE(3, 1, 2, 0));
      __m512i vdata0 = _mm512_unpacklo_epi64(data0, data1);
      __m512i vdata1 = _mm512_unpackhi_epi64(data0, data1);
      __m512i vdata3 = _mm512_unpacklo_epi64(data3, data4);
      __m512i vdata4 = _mm512_unpackhi_epi64(data3, data4);
      for (int32_t i = 0; i < 32; ++i) {
        vdata0 = _mm512_add_epi32(vdata0, _mm512_xor_si512(_mm512_add_epi32(_mm512_xor_si512(_mm512_slli_epi32(vdata1, 4), _mm512_srli_epi32(vdata1, 5)), vdata1), _mm512_set1_epi32(precachedControlSum[i][0])));
        vdata1 = _mm512_add_epi32(vdata1, _mm512_xor_si512(_mm512_add_epi32(_mm512_xor_si512(_mm512_slli_epi32(vdata0, 4), _mm512_srli_epi32(vdata0, 5)), vdata0), _mm512_set1_epi32(precachedControlSum[i][1])));
        vdata3 = _mm512_add_epi32(vdata3, _mm512_xor_si512(_mm512_add_epi32(_mm512_xor_si512(_mm512_slli_epi32(vdata4, 4), _mm512_srli_epi32(vdata4, 5)), vdata4), _mm512_set1_epi32(precachedControlSum[i][0])));
        vdata4 = _mm512_add_epi32(vdata4, _mm512_xor_si512(_mm512_add_epi32(_mm512_xor_si512(_mm512_slli_epi32(vdata3, 4), _mm512_srli_epi32(vdata3, 5)), vdata3), _mm512_set1_epi32(precachedControlSum[i][1])));
      }
      _mm512_storeu_si512(reinterpret_cast<void*>(buffer + readPos), _mm512_unpacklo_epi32(vdata0, vdata1));
      readPos += 64;
      _mm512_storeu_si512(reinterpret_cast<void*>(buffer + readPos), _mm512_unpackhi_epi32(vdata0, vdata1));
      readPos += 64;
      _mm512_storeu_si512(reinterpret_cast<void*>(buffer + readPos), _mm512_unpacklo_epi32(vdata3, vdata4));
      readPos += 64;
      _mm512_storeu_si512(reinterpret_cast<void*>(buffer + readPos), _mm512_unpackhi_epi32(vdata3, vdata4));
      readPos += 64;
    }
    messageLength += 128;
    if (readPos <= messageLength) {
      const __m512i data0 = _mm512_shuffle_epi32(_mm512_loadu_si512(reinterpret_cast<const void*>(buffer + readPos)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m512i data1 = _mm512_shuffle_epi32(_mm512_loadu_si512(reinterpret_cast<const void*>(buffer + readPos + 64)), _MM_SHUFFLE(3, 1, 2, 0));
      __m512i vdata0 = _mm512_unpacklo_epi64(data0, data1);
      __m512i vdata1 = _mm512_unpackhi_epi64(data0, data1);
      for (int32_t i = 0; i < 32; ++i) {
        vdata0 = _mm512_add_epi32(vdata0, _mm512_xor_si512(_mm512_add_epi32(_mm512_xor_si512(_mm512_slli_epi32(vdata1, 4), _mm512_srli_epi32(vdata1, 5)), vdata1), _mm512_set1_epi32(precachedControlSum[i][0])));
        vdata1 = _mm512_add_epi32(vdata1, _mm512_xor_si512(_mm512_add_epi32(_mm512_xor_si512(_mm512_slli_epi32(vdata0, 4), _mm512_srli_epi32(vdata0, 5)), vdata0), _mm512_set1_epi32(precachedControlSum[i][1])));
      }
      _mm512_storeu_si512(reinterpret_cast<void*>(buffer + readPos), _mm512_unpacklo_epi32(vdata0, vdata1));
      readPos += 64;
      _mm512_storeu_si512(reinterpret_cast<void*>(buffer + readPos), _mm512_unpackhi_epi32(vdata0, vdata1));
      readPos += 64;
    }
    messageLength += 64;
    #endif
    #if defined(__AVX2__)

    //AVX2 unroll - only if AVX512 disabled
    #if !defined(__AVX512F__)
    while (readPos <= messageLength) {
      const __m256i data0 = _mm256_shuffle_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(buffer + readPos)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m256i data1 = _mm256_shuffle_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(buffer + readPos + 32)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m256i data2 = _mm256_shuffle_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(buffer + readPos + 64)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m256i data3 = _mm256_shuffle_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(buffer + readPos + 96)), _MM_SHUFFLE(3, 1, 2, 0));
      __m256i vdata0 = _mm256_unpacklo_epi64(data0, data1);
      __m256i vdata1 = _mm256_unpackhi_epi64(data0, data1);
      __m256i vdata2 = _mm256_unpacklo_epi64(data2, data3);
      __m256i vdata3 = _mm256_unpackhi_epi64(data2, data3);
      for (int32_t i = 0; i < 32; ++i) {
        vdata0 = _mm256_add_epi32(vdata0, _mm256_xor_si256(_mm256_add_epi32(_mm256_xor_si256(_mm256_slli_epi32(vdata1, 4), _mm256_srli_epi32(vdata1, 5)), vdata1), _mm256_set1_epi32(precachedControlSum[i][0])));
        vdata1 = _mm256_add_epi32(vdata1, _mm256_xor_si256(_mm256_add_epi32(_mm256_xor_si256(_mm256_slli_epi32(vdata0, 4), _mm256_srli_epi32(vdata0, 5)), vdata0), _mm256_set1_epi32(precachedControlSum[i][1])));
        vdata2 = _mm256_add_epi32(vdata2, _mm256_xor_si256(_mm256_add_epi32(_mm256_xor_si256(_mm256_slli_epi32(vdata3, 4), _mm256_srli_epi32(vdata3, 5)), vdata3), _mm256_set1_epi32(precachedControlSum[i][0])));
        vdata3 = _mm256_add_epi32(vdata3, _mm256_xor_si256(_mm256_add_epi32(_mm256_xor_si256(_mm256_slli_epi32(vdata2, 4), _mm256_srli_epi32(vdata2, 5)), vdata2), _mm256_set1_epi32(precachedControlSum[i][1])));
      }
      _mm256_storeu_si256(reinterpret_cast<__m256i*>(buffer + readPos), _mm256_unpacklo_epi32(vdata0, vdata1));
      readPos += 32;
      _mm256_storeu_si256(reinterpret_cast<__m256i*>(buffer + readPos), _mm256_unpackhi_epi32(vdata0, vdata1));
      readPos += 32;
      _mm256_storeu_si256(reinterpret_cast<__m256i*>(buffer + readPos), _mm256_unpacklo_epi32(vdata2, vdata3));
      readPos += 32;
      _mm256_storeu_si256(reinterpret_cast<__m256i*>(buffer + readPos), _mm256_unpackhi_epi32(vdata2, vdata3));
      readPos += 32;
    }
    messageLength += 64;
    #endif

    if (readPos <= messageLength) {
      const __m256i data0 = _mm256_shuffle_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(buffer + readPos)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m256i data1 = _mm256_shuffle_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(buffer + readPos + 32)), _MM_SHUFFLE(3, 1, 2, 0));
      __m256i vdata0 = _mm256_unpacklo_epi64(data0, data1);
      __m256i vdata1 = _mm256_unpackhi_epi64(data0, data1);
      for (int32_t i = 0; i < 32; ++i) {
        vdata0 = _mm256_add_epi32(vdata0, _mm256_xor_si256(_mm256_add_epi32(_mm256_xor_si256(_mm256_slli_epi32(vdata1, 4), _mm256_srli_epi32(vdata1, 5)), vdata1), _mm256_set1_epi32(precachedControlSum[i][0])));
        vdata1 = _mm256_add_epi32(vdata1, _mm256_xor_si256(_mm256_add_epi32(_mm256_xor_si256(_mm256_slli_epi32(vdata0, 4), _mm256_srli_epi32(vdata0, 5)), vdata0), _mm256_set1_epi32(precachedControlSum[i][1])));
      }
      _mm256_storeu_si256(reinterpret_cast<__m256i*>(buffer + readPos), _mm256_unpacklo_epi32(vdata0, vdata1));
      readPos += 32;
      _mm256_storeu_si256(reinterpret_cast<__m256i*>(buffer + readPos), _mm256_unpackhi_epi32(vdata0, vdata1));
      readPos += 32;
    }
    messageLength += 32;
    #endif
    #if defined(__SSE2__)

    //SSE2 unroll - only if AVX2 disabled
    #if !defined(__AVX2__)
    while (readPos <= messageLength) {
      const __m128i data0 = _mm_shuffle_epi32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(buffer + readPos)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m128i data1 = _mm_shuffle_epi32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(buffer + readPos + 16)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m128i data3 = _mm_shuffle_epi32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(buffer + readPos + 32)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m128i data4 = _mm_shuffle_epi32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(buffer + readPos + 48)), _MM_SHUFFLE(3, 1, 2, 0));
      __m128i vdata0 = _mm_unpacklo_epi64(data0, data1);
      __m128i vdata1 = _mm_unpackhi_epi64(data0, data1);
      __m128i vdata3 = _mm_unpacklo_epi64(data3, data4);
      __m128i vdata4 = _mm_unpackhi_epi64(data3, data4);
      for (int32_t i = 0; i < 32; ++i) {
        vdata0 = _mm_add_epi32(vdata0, _mm_xor_si128(_mm_add_epi32(_mm_xor_si128(_mm_slli_epi32(vdata1, 4), _mm_srli_epi32(vdata1, 5)), vdata1), _mm_set1_epi32(precachedControlSum[i][0])));
        vdata1 = _mm_add_epi32(vdata1, _mm_xor_si128(_mm_add_epi32(_mm_xor_si128(_mm_slli_epi32(vdata0, 4), _mm_srli_epi32(vdata0, 5)), vdata0), _mm_set1_epi32(precachedControlSum[i][1])));
        vdata3 = _mm_add_epi32(vdata3, _mm_xor_si128(_mm_add_epi32(_mm_xor_si128(_mm_slli_epi32(vdata4, 4), _mm_srli_epi32(vdata4, 5)), vdata4), _mm_set1_epi32(precachedControlSum[i][0])));
        vdata4 = _mm_add_epi32(vdata4, _mm_xor_si128(_mm_add_epi32(_mm_xor_si128(_mm_slli_epi32(vdata3, 4), _mm_srli_epi32(vdata3, 5)), vdata3), _mm_set1_epi32(precachedControlSum[i][1])));
      }
      _mm_storeu_si128(reinterpret_cast<__m128i*>(buffer + readPos), _mm_unpacklo_epi32(vdata0, vdata1));
      readPos += 16;
      _mm_storeu_si128(reinterpret_cast<__m128i*>(buffer + readPos), _mm_unpackhi_epi32(vdata0, vdata1));
      readPos += 16;
      _mm_storeu_si128(reinterpret_cast<__m128i*>(buffer + readPos), _mm_unpacklo_epi32(vdata3, vdata4));
      readPos += 16;
      _mm_storeu_si128(reinterpret_cast<__m128i*>(buffer + readPos), _mm_unpackhi_epi32(vdata3, vdata4));
      readPos += 16;
    }
    messageLength += 32;
    #endif

    if (readPos <= messageLength) {
      const __m128i data0 = _mm_shuffle_epi32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(buffer + readPos)), _MM_SHUFFLE(3, 1, 2, 0));
      const __m128i data1 = _mm_shuffle_epi32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(buffer + readPos + 16)), _MM_SHUFFLE(3, 1, 2, 0));
      __m128i vdata0 = _mm_unpacklo_epi64(data0, data1);
      __m128i vdata1 = _mm_unpackhi_epi64(data0, data1);
      for (int32_t i = 0; i < 32; ++i) {
        vdata0 = _mm_add_epi32(vdata0, _mm_xor_si128(_mm_add_epi32(_mm_xor_si128(_mm_slli_epi32(vdata1, 4), _mm_srli_epi32(vdata1, 5)), vdata1), _mm_set1_epi32(precachedControlSum[i][0])));
        vdata1 = _mm_add_epi32(vdata1, _mm_xor_si128(_mm_add_epi32(_mm_xor_si128(_mm_slli_epi32(vdata0, 4), _mm_srli_epi32(vdata0, 5)), vdata0), _mm_set1_epi32(precachedControlSum[i][1])));
      }
      _mm_storeu_si128(reinterpret_cast<__m128i*>(buffer + readPos), _mm_unpacklo_epi32(vdata0, vdata1));
      readPos += 16;
      _mm_storeu_si128(reinterpret_cast<__m128i*>(buffer + readPos), _mm_unpackhi_epi32(vdata0, vdata1));
      readPos += 16;
    }
    messageLength += 32;
    #endif
    while (readPos < messageLength) {
      uint32_t vData[2];
      memcpy(vData, buffer + readPos, 8);
      for (int32_t i = 0; i < 32; ++i) {
        vData[0] += ((vData[1] << 4 ^ vData[1] >> 5) + vData[1]) ^ precachedControlSum[i][0];
        vData[1] += ((vData[0] << 4 ^ vData[0] >> 5) + vData[0]) ^ precachedControlSum[i][1];
      }
      memcpy(buffer + readPos, vData, 8);
      readPos += 8;
    }
  }
}