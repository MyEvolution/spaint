/**
 * itmx: RGBDFrameMessage.cpp
 * Copyright (c) Torr Vision Group, University of Oxford, 2017. All rights reserved.
 */

#include <iostream>

#include "remotemapping/RGBDFrameMessage.h"

namespace itmx {

//#################### CONSTRUCTORS ####################

RGBDFrameMessage::RGBDFrameMessage(const Vector2i& rgbImageSize, const Vector2i& depthImageSize)
: m_depthImageSize(depthImageSize), m_rgbImageSize(rgbImageSize)
{
  m_frameIndexSegment = std::make_pair(0, sizeof(int));
  m_poseSegment = std::make_pair(end_of(m_frameIndexSegment), bytes_for_pose());
  m_rgbImageSegment = std::make_pair(end_of(m_poseSegment), rgbImageSize.width * rgbImageSize.height * sizeof(Vector4u));
  m_depthImageSegment = std::make_pair(end_of(m_rgbImageSegment), depthImageSize.width * depthImageSize.height * sizeof(short));
  m_data.resize(end_of(m_depthImageSegment));
}

//#################### PUBLIC STATIC MEMBER FUNCTIONS ####################

RGBDFrameMessage_Ptr RGBDFrameMessage::make(const Vector2i& rgbImageSize, const Vector2i& depthImageSize)
{
  return RGBDFrameMessage_Ptr(new RGBDFrameMessage(rgbImageSize, depthImageSize));
}

//#################### PUBLIC MEMBER FUNCTIONS ####################

void RGBDFrameMessage::extract_depth_image(ORShortImage *depthImage) const
{
  if(depthImage->dataSize * sizeof(short) != m_depthImageSegment.second)
  {
    std::cerr << "Warning: The target image has a different size to that of the depth image in the message" << std::endl;
    depthImage->ChangeDims(m_depthImageSize);
  }

  memcpy(reinterpret_cast<char*>(depthImage->GetData(MEMORYDEVICE_CPU)), &m_data[m_depthImageSegment.first], m_depthImageSegment.second);
}

void RGBDFrameMessage::extract_rgb_image(ORUChar4Image *rgbImage) const
{
  if(rgbImage->dataSize * sizeof(Vector4u) != m_rgbImageSegment.second)
  {
    std::cerr << "Warning: The target image has a different size to that of the RGB image in the message" << std::endl;
    rgbImage->ChangeDims(m_rgbImageSize);
  }

  memcpy(reinterpret_cast<char*>(rgbImage->GetData(MEMORYDEVICE_CPU)), &m_data[m_rgbImageSegment.first], m_rgbImageSegment.second);
}

const Vector2i& RGBDFrameMessage::get_depth_image_size() const
{
  return m_depthImageSize;
}

const Vector2i& RGBDFrameMessage::get_rgb_image_size() const
{
  return m_rgbImageSize;
}

void RGBDFrameMessage::set_depth_image(const ORShortImage_CPtr& depthImage)
{
  memcpy(&m_data[m_depthImageSegment.first], reinterpret_cast<const char*>(depthImage->GetData(MEMORYDEVICE_CPU)), m_depthImageSegment.second);
}

void RGBDFrameMessage::set_rgb_image(const ORUChar4Image_CPtr& rgbImage)
{
  memcpy(&m_data[m_rgbImageSegment.first], reinterpret_cast<const char*>(rgbImage->GetData(MEMORYDEVICE_CPU)), m_rgbImageSegment.second);
}

}
