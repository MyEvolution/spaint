/**
 * spaint: RGBDPatchFeatureCalculator.cpp
 * Copyright (c) Torr Vision Group, University of Oxford, 2016. All rights reserved.
 */

#include "features/interface/RGBDPatchFeatureCalculator.h"

#include <random>
#include "util/MemoryBlockFactory.h"

namespace spaint
{
RGBDPatchFeatureCalculator::RGBDPatchFeatureCalculator()
{
  const MemoryBlockFactory &mbf = MemoryBlockFactory::instance();
// Setup the features in the same way as Julian's code
  m_normalizeRgb = true;
  m_offsetsRgb = mbf.make_block<Vector4i>(RGBDPatchFeature::RGB_FEATURE_COUNT);
  m_channelsRgb = mbf.make_block<uchar>(RGBDPatchFeature::RGB_FEATURE_COUNT);

  m_normalizeDepth = true;
  m_offsetsDepth = mbf.make_block<Vector4i>(RGBDPatchFeature::DEPTH_FEATURE_COUNT);

  // Setup colour features
  {
    std::mt19937 eng;

    const int channelMin = 0;
    const int channelMax = 2;
    const int radiusMin = 2;
    const int radiusMax = 130;

    std::uniform_int_distribution<size_t> channel_generator(channelMin, channelMax);
    std::uniform_int_distribution<size_t> offset_generator(radiusMin, radiusMax);
    std::uniform_int_distribution<size_t> sign_generator(0, 1);

    Vector4i *offsets = m_offsetsRgb->GetData(MEMORYDEVICE_CPU);
    uchar *channels = m_channelsRgb->GetData(MEMORYDEVICE_CPU);

    for(int i = 0; i < RGBDPatchFeature::RGB_FEATURE_COUNT; ++i)
    {
      // Might be different from the order used in scoreforests (there the random calls are inside a constructor)
      // TODO check that
      offsets[i][0] = offset_generator(eng) * (static_cast<float>(sign_generator(eng)) * 2 - 1);
      offsets[i][1] = offset_generator(eng) * (static_cast<float>(sign_generator(eng)) * 2 - 1);
      offsets[i][2] = offset_generator(eng) * (static_cast<float>(sign_generator(eng)) * 2 - 1);
      offsets[i][3] = offset_generator(eng) * (static_cast<float>(sign_generator(eng)) * 2 - 1);

      channels[i] = 2 - channel_generator(eng); // RGB2BGR
    }

    // Setup depth features
    {
      std::mt19937 eng;

      const int radiusMin = 2 / 2;
      const int radiusMax = 130 / 2;

      std::uniform_int_distribution<size_t> offset_generator(radiusMin, radiusMax);
      std::uniform_int_distribution<size_t> sign_generator(0, 1);

      Vector4i *offsets = m_offsetsDepth->GetData(MEMORYDEVICE_CPU);

      for(int i = 0; i < RGBDPatchFeature::DEPTH_FEATURE_COUNT; ++i)
      {
        offsets[i][0] = offset_generator(eng) * (static_cast<float>(sign_generator(eng)) * 2 - 1);
        offsets[i][1] = offset_generator(eng) * (static_cast<float>(sign_generator(eng)) * 2 - 1);
        offsets[i][2] = offset_generator(eng) * (static_cast<float>(sign_generator(eng)) * 2 - 1);
        offsets[i][3] = offset_generator(eng) * (static_cast<float>(sign_generator(eng)) * 2 - 1);
      }
    }
  }
}

RGBDPatchFeatureCalculator::~RGBDPatchFeatureCalculator() {}
}
