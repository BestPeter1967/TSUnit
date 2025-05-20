/* ==========================================================================
 * @(#)File: TSUnitTestAddOns.cpp
 * Created: 2023-10-22
 * --------------------------------------------------------------------------
 *  (c)1982-2023 Tangerine-Software
 *
 *       Hans-Peter Beständig
 *       Kühbachstr. 8
 *       81543 München
 *       GERMANY
 *
 *       mailto:hdusel@tangerine-soft.de
 *       http://hdusel.tangerine-soft.de
 * --------------------------------------------------------------------------
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * ========================================================================== */
#include "TSUnitTestAddOns.hpp"
#include <cstdlib>
#include <stdio.h>
#include <cassert>

// Some Useful addons for Unittest...

#if !defined(dimof)
#define dimof(x) (sizeof(x) / sizeof(*(x)))
#endif

namespace tsunit {

static uint32_t _rot32r(uint32_t inNum, unsigned int places)
{
    places &= 0x1f;
    return (inNum >> places) | (inNum << (32-places));
}

template<size_t FROM_POS, size_t TO_POS>
constexpr static uint32_t _swapBits(uint32_t inValue)
{
    return (FROM_POS < TO_POS) ?
        ((inValue & (1ul<<FROM_POS)) << (TO_POS-FROM_POS)) | ((inValue & (1ul<<  TO_POS)) >> (TO_POS-FROM_POS)) :
        ((inValue & (1ul<<TO_POS))   << (FROM_POS-TO_POS)) | ((inValue & (1ul<<FROM_POS)) >> (FROM_POS-TO_POS));
}

// =======================================================================================
static std::uint32_t _scramble0(std::uint32_t param)
{
    return _swapBits<23,16>(param)
         | _swapBits<25,13>(param)
         | _swapBits<15,10>(param)
         | _swapBits<6,19>(param)
         | _swapBits<3,2>(param)
         | _swapBits<11,14>(param)
         | _swapBits<4,24>(param)
         | _swapBits<20,18>(param)
         | _swapBits<7,30>(param)
         | _swapBits<0,29>(param)
         | _swapBits<9,28>(param)
         | _swapBits<1,17>(param)
         | _swapBits<12,27>(param)
         | _swapBits<26,21>(param)
         | _swapBits<31,8>(param)
         | _swapBits<22,5>(param)
         ;
}

static std::uint32_t _scramble1(std::uint32_t param)
{
    return _swapBits<7,19>(param)
         | _swapBits<30,6>(param)
         | _swapBits<20,15>(param)
         | _swapBits<21,8>(param)
         | _swapBits<24,12>(param)
         | _swapBits<9,14>(param)
         | _swapBits<0,25>(param)
         | _swapBits<1,17>(param)
         | _swapBits<26,4>(param)
         | _swapBits<31,23>(param)
         | _swapBits<27,29>(param)
         | _swapBits<18,10>(param)
         | _swapBits<2,16>(param)
         | _swapBits<28,13>(param)
         | _swapBits<11,22>(param)
         | _swapBits<5,3>(param)
         ;
}

static std::uint32_t _scramble2(std::uint32_t param)
{
    return _swapBits<28,30>(param)
         | _swapBits<10,31>(param)
         | _swapBits<4,26>(param)
         | _swapBits<19,8>(param)
         | _swapBits<5,0>(param)
         | _swapBits<18,11>(param)
         | _swapBits<15,22>(param)
         | _swapBits<14,1>(param)
         | _swapBits<17,23>(param)
         | _swapBits<16,29>(param)
         | _swapBits<27,7>(param)
         | _swapBits<9,12>(param)
         | _swapBits<2,25>(param)
         | _swapBits<20,24>(param)
         | _swapBits<21,13>(param)
         | _swapBits<3,6>(param)
         ;
}

static std::uint32_t _scramble3(std::uint32_t param)
{
    return _swapBits<17,22>(param)
         | _swapBits<20,16>(param)
         | _swapBits<10,12>(param)
         | _swapBits<24,23>(param)
         | _swapBits<21,5>(param)
         | _swapBits<19,27>(param)
         | _swapBits<18,15>(param)
         | _swapBits<14,8>(param)
         | _swapBits<0,28>(param)
         | _swapBits<2,4>(param)
         | _swapBits<1,25>(param)
         | _swapBits<29,6>(param)
         | _swapBits<26,31>(param)
         | _swapBits<13,30>(param)
         | _swapBits<9,11>(param)
         | _swapBits<3,7>(param)
         ;
}

typedef std::uint32_t(*ScrambleFunct)(uint32_t);

static uint32_t _noiseNumber(std::uint32_t inIndex)
{
    static const uint32_t kNoiseTable[32] = {
          0x347cf746ul, 0x7b840e02ul, 0x4b6e3c4eul, 0x489b06c6ul
        , 0x2ba14c6eul, 0x4572434aul, 0x04600530ul, 0x7f9acc78ul
        , 0x50a98955ul, 0x071b0827ul, 0x15690047ul, 0x6c68f552ul
        , 0x5fc52edful, 0x61ca273bul, 0x44e4c5f4ul, 0x6a8f5fc5ul
        , 0x01fc910aul, 0x29e9de3ful, 0x2788c41eul, 0x6b5c5ce4ul
        , 0x39d672deul, 0x6d7680bful, 0x511f385dul, 0x577fa18ful
        , 0x7aa4ca0dul, 0x671710edul, 0x127e0c78ul, 0x567cb335ul
        , 0x65f0a296ul, 0x3f0eaf85ul, 0x4e838e1ful, 0x1a6d99ddul
    };

    const unsigned int noiseIdx = (inIndex & 0x1f);

    static const ScrambleFunct scrambleFunct[] {
           _scramble0 , _scramble1 , _scramble2 , _scramble3
    };

    const unsigned int scrambleFuncIdx = (inIndex & (dimof(scrambleFunct) - 1));

    return scrambleFunct[scrambleFuncIdx](inIndex - kNoiseTable[noiseIdx]);
}

std::uint32_t hash(const void* inDataPtr, unsigned int inDataSize)
{
    const uint8_t* dataPtr = static_cast<const uint8_t*>(inDataPtr);
    uint32_t cs = 0xac3b843bul;

    for (;inDataSize;--inDataSize)
    {
        const uint32_t thisDataWord  = (*dataPtr++);

        cs += _noiseNumber((thisDataWord >> 4) + cs);
        cs += _noiseNumber(cs + thisDataWord * 238);
        cs ^= _noiseNumber(_rot32r(1057592071 + thisDataWord, 15));
    }
    return cs;
}

static std::uint32_t randomSeed = 0x3ac4821cul;

void pseudoRandomsetSeed(std::uint32_t inSeed)
{
    randomSeed = inSeed;
}

std::uint32_t pseudoRandom()
{
    randomSeed += hash(&randomSeed, sizeof(randomSeed));
    return randomSeed;
}

float pseudoRandomFloat(float minValue, float maxValue)
{
    return minValue + (maxValue - minValue) * (float(pseudoRandom()) / float(UINT32_MAX));
}
} // namespace tsunit
