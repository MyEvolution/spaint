/**
 * itmx: MappingClientHandler.h
 * Copyright (c) Torr Vision Group, University of Oxford, 2018. All rights reserved.
 */

#ifndef H_ITMX_MAPPINGCLIENTHANDLER
#define H_ITMX_MAPPINGCLIENTHANDLER

#include <ITMLib/Objects/Camera/ITMRGBDCalib.h>

#include <tvgutil/containers/PooledQueue.h>
#include <tvgutil/net/ClientHandler.h>

#include "RGBDFrameCompressor.h"

namespace itmx {

/**
 * \brief An instance of this class can be used to manage the connection to a mapping client.
 */
class MappingClientHandler : public tvgutil::ClientHandler
{
  //#################### TYPEDEFS ####################
private:
  typedef tvgutil::PooledQueue<RGBDFrameMessage_Ptr> RGBDFrameMessageQueue;
  typedef boost::shared_ptr<RGBDFrameMessageQueue> RGBDFrameMessageQueue_Ptr;

  //#################### NESTED TYPES ####################
public:
  /**
   * \brief TODO
   */
  class RenderingImageHandler
  {
    //~~~~~~~~~~~~~~~~~~~~ PRIVATE VARIABLES ~~~~~~~~~~~~~~~~~~~~
  private:
    /** TODO */
    MappingClientHandler *m_clientHandler;

    /** TODO */
    boost::lock_guard<boost::mutex> m_lock;

    //~~~~~~~~~~~~~~~~~~~~ CONSTRUCTORS ~~~~~~~~~~~~~~~~~~~~
  public:
    /**
     * \brief TODO
     */
    RenderingImageHandler(MappingClientHandler *clientHandler)
    : m_clientHandler(clientHandler), m_lock(clientHandler->m_renderingImageMutex)
    {}

    //~~~~~~~~~~~~~~~~~~~~ COPY CONSTRUCTOR & ASSIGNMENT OPERATOR ~~~~~~~~~~~~~~~~~~~~
  private:
    // Deliberately private and unimplemented.
    RenderingImageHandler(const RenderingImageHandler&);
    RenderingImageHandler& operator=(const RenderingImageHandler&);

    //~~~~~~~~~~~~~~~~~~~~ PUBLIC MEMBER FUNCTIONS ~~~~~~~~~~~~~~~~~~~~
  public:
    ORUChar4Image_Ptr& get()
    {
      return m_clientHandler->m_renderingImage;
    }
  };

  typedef boost::shared_ptr<RenderingImageHandler> RenderingImageHandler_Ptr;

  //#################### PUBLIC VARIABLES ####################
public:
  /** The calibration parameters of the camera associated with the client. */
  ITMLib::ITMRGBDCalib m_calib;

  /** A dummy frame message to consume messages that cannot be pushed onto the queue. */
  RGBDFrameMessage_Ptr m_dummyFrameMessage;

  /** The frame compressor for the client. */
  RGBDFrameCompressor_Ptr m_frameCompressor;

  /** A place in which to store compressed RGB-D frame messages. */
  boost::shared_ptr<CompressedRGBDFrameMessage> m_frameMessage;

  /** A queue containing the RGB-D frame messages received from the client. */
  RGBDFrameMessageQueue_Ptr m_frameMessageQueue;

  /** A place in which to store compressed RGB-D frame header messages. */
  CompressedRGBDFrameHeaderMessage m_headerMessage;

  /** A flag indicating whether or not the images associated with the first message in the queue have already been read. */
  bool m_imagesDirty;

  /** A flag indicating whether or not the pose associated with the first message in the queue has already been read. */
  bool m_poseDirty;

  /** An optional image into which to render the scene for the client. */
  ORUChar4Image_Ptr m_renderingImage;

  /** The synchronisation mutex for the rendering image. */
  boost::mutex m_renderingImageMutex;

  /** An optional pose (in the client's coordinate system) from which the client wants the server to render the scene. */
  boost::optional<ORUtils::SE3Pose> m_renderingPose;

  /** The synchronisation mutex for the rendering pose. */
  boost::mutex m_renderingPoseMutex;

  //#################### CONSTRUCTORS ####################
public:
  /**
   * \brief Constructs a handler for a mapping client.
   *
   * \param clientID          The ID used by the server to refer to the client.
   * \param sock              The socket used to communicate with the client.
   * \param shouldTerminate   Whether or not the server should terminate.
   */
  MappingClientHandler(int clientID, const boost::shared_ptr<boost::asio::ip::tcp::socket>& sock, const boost::shared_ptr<const boost::atomic<bool> >& shouldTerminate);

  //#################### PUBLIC MEMBER FUNCTIONS ####################
public:
  /**
   * \brief Gets the size of the depth images produced by the client.
   *
   * \return  The size of the depth images produced by the client.
   */
  const Vector2i& get_depth_image_size() const;

  /**
   * \brief TODO
   */
  RenderingImageHandler_Ptr get_rendering_image();

  /**
   * \brief Gets the size of the colour images produced by the client.
   *
   * \return  The size of the colour images produced by the client.
   */
  const Vector2i& get_rgb_image_size() const;

  /** Override */
  virtual void run_iter();

  /** Override */
  virtual void run_post();

  /** Override */
  virtual void run_pre();
};

}

#endif