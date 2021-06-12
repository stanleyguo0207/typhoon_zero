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
// but WITHOUT ANY WARRANTY{} without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "random_hub.h"

#include <random>
#include <thread>
#include <memory>

#include <sfmt/sfmt.h>

#include "debug_hub.h"

namespace tpn {

struct RandomGeneratorImpl : public CRandomSFMT {
  RandomGeneratorImpl(int seed) : CRandomSFMT(seed, 1) {}
};

namespace {

static int s_seed = 0;

static thread_local std::unique_ptr<RandomGeneratorImpl> s_sfmt_generator;

}  // namespace

static RandomGeneratorImpl *GetRandomGenerator() {
  RandomGeneratorImpl *gen = s_sfmt_generator.get();
  if (!gen) {
    gen = new RandomGeneratorImpl(
        static_cast<int>(std::time(nullptr) + (++s_seed)));
    s_sfmt_generator.reset(gen);
  }
  return gen;
}

uint32_t Rand32() { return GetRandomGenerator()->BRandom(); }

int32_t RandI32(int32_t min, int32_t max) {
  TPN_ASSERT(max >= min, "max:{} min:{}", max, min);
  return static_cast<int32_t>(GetRandomGenerator()->IRandom(min, max));
}

uint32_t RandU32(uint32_t min, uint32_t max) {
  TPN_ASSERT(max >= min, "max:{} min:{}", max, min);
  return GetRandomGenerator()->URandom(min, max);
}

float RandFloat(float min, float max) {
  TPN_ASSERT(max >= min, "max:{} min:{}", max, min);
  return static_cast<float>(GetRandomGenerator()->Random()) * (max - min) + min;
}

double RandDouble(double min, double max) {
  TPN_ASSERT(max >= min, "max:{} min:{}", max, min);
  return GetRandomGenerator()->Random() * (max - min) + min;
}

uint32_t RandMS(uint32_t min, uint32_t max) {
  TPN_ASSERT(max >= min, "max:{} min:{}", max, min);
  TPN_ASSERT(((std::numeric_limits<uint32_t>::max() /
               MilliSeconds::period::den)) >= max,
             "max:{} min:{}", max, min);
  return GetRandomGenerator()->URandom(min * MilliSeconds::period::den,
                                       max * MilliSeconds::period::den);
}

MilliSeconds RandTime(const MilliSeconds &min, const MilliSeconds &max) {
  long long diff = max.count() - min.count();
  TPN_ASSERT(diff >= 0, "max:{} min:{}", max.count(), min.count());
  TPN_ASSERT(diff <= 0xFFFFFFFF, "max:{} min:{}", max.count(), min.count());
  return min + MilliSeconds(RandU32(0, static_cast<uint32_t>(diff)));
}

double RandNorm() { return GetRandomGenerator()->Random(); }

double RandChance() { return RandNorm() * 100.0; }

uint32_t RandU32Weighted(const double *weights, size_t count) {
  std::discrete_distribution<uint32_t> dd{weights, weights + count};
  return dd(*(RandomEngine::Instance()));
}

TPN_SINGLETON_IMPL(RandomEngine)
}  // namespace tpn
