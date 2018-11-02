#ifndef AGORA_MEDIA_ENGINE_H
#define AGORA_MEDIA_ENGINE_H
#if defined _WIN32 || defined __CYGWIN__
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif

namespace agora
{
namespace media
{
/** Type of audio device.
*/
enum MEDIA_SOURCE_TYPE {
  /** Audio playback device.
  */
    AUDIO_PLAYOUT_SOURCE = 0,
    /** Microphone.
    */
    AUDIO_RECORDING_SOURCE = 1,
};

class IAudioFrameObserver
{
public:
  enum AUDIO_FRAME_TYPE {
    FRAME_TYPE_PCM16 = 0,  //PCM 16bit little endian
  };
  /** Definition of AudioFrame
  */
  struct AudioFrame {
    AUDIO_FRAME_TYPE type;
    /** Number of samples in the audio frame: samples = (int)samplesPerCall = (int)(sampleRate &times; sampleInterval)
    */
    int samples;  //number of samples in this frame
    /** Number of bytes per audio sample. For example, each PCM audio sample usually takes up 16 bits (2 bytes).
    */
    int bytesPerSample;  //number of bytes per sample: 2 for PCM16
    /** Number of audio channels.

     If the channel uses stereo sound, the data is interleaved.

     - 1: Mono
     - 2: Two channels
     */
    int channels;  //number of channels (data are interleaved if stereo)
    /** Sampling rate per second for the audio frame.
    */
    int samplesPerSec;  //sampling rate
    /** Audio frame data buffer. The valid data length is: samples &times; channels &times; bytesPerSample
    */
    void* buffer;  //data buffer
    /** Timestamp to render the audio stream.
    */
    int64_t renderTimeMs;
    int avsync_type;
  };
public:
    
  /** Retrieves the recorded audio frame.

   @param audioFrame See AudioFrame for details.
   @return
   - true: Valid buffer in AudioFrame and the recorded audio frame will be sent out.
   - false: Invalid buffer in AudioFrame and the recorded audio frame will be discarded.
   */
  virtual bool onRecordAudioFrame(AudioFrame& audioFrame) = 0;
  /** Retrieves the audio playback frame.

   This callback function returns after agora::rtc::RtcEngineParameters::playEffect is successfully called.

   @param audioFrame See AudioFrame for details.
   @return
   - true: Valid buffer in AudioFrame and the audio playback frame will be sent out.
   - false: Invalid buffer in AudioFrame and the audio playback frame will be discarded.
   */
  virtual bool onPlaybackAudioFrame(AudioFrame& audioFrame) = 0;
  /** Retrieves the audio playback frame of a specified user.

   @param uid User ID
   @param audioFrame See AudioFrame for details.
   @return
   - true: Valid buffer in AudioFrame and the audio playback frame of the specified user will be sent out.
   - false: Invalid buffer in AudioFrame and the audio playback frame of the specified user will be discarded.
   */
  virtual bool onPlaybackAudioFrameBeforeMixing(unsigned int uid, AudioFrame& audioFrame) = 0;
  /** Retrieves the mixed recorded and playback audio frame.

   @note This method only returns the single-channel data.

   @param audioFrame See AudioFrame for details.
   @return
   - true: Valid buffer in AudioFrame and the mixed recorded and playback audio frame will be sent out.
   - false: Invalid buffer in AudioFrame and the mixed recorded and playback audio frame will be discarded.
   */
  virtual bool onMixedAudioFrame(AudioFrame& audioFrame) = 0;

};

class IVideoFrameObserver
{
public:
  enum VIDEO_FRAME_TYPE {
    FRAME_TYPE_YUV420 = 0,  //YUV 420 format
  };
  /** Video frame information. The video data format is YUV420. The buffer provides a pointer to a pointer. The interface cannot modify the pointer of the buffer, but can modify the content of the buffer only.

  */
  struct VideoFrame {
    VIDEO_FRAME_TYPE type;
    /** Video pixel width.
    */
    int width;  //width of video frame
    /** Video pixel height.
    */
    int height;  //height of video frame
    /** Line span of the Y buffer within the YUV data.
    */
    int yStride;  //stride of Y data buffer
    /** Line span of the U buffer within the YUV data.
    */
    int uStride;  //stride of U data buffer
    /** Line span of the V buffer within the YUV data.
    */
    int vStride;  //stride of V data buffer
    /** Pointer to the Y buffer pointer within the YUV data.
     */
    void* yBuffer;  //Y data buffer
    /** Pointer to the U buffer pointer within the YUV data.
    */
    void* uBuffer;  //U data buffer
    /** Pointer to the V buffer pointer within the YUV data.
    */
    void* vBuffer;  //V data buffer
    /** Set the rotation of this frame before rendering the video. Supports 0, 90, 180, 270 degrees, clockwise.
    */
    int rotation; // rotation of this frame (0, 90, 180, 270)
    /** Timestamp (ms) for the video stream render. Use this timestamp to synchronize the video stream render while rendering the video streams.

     @note This timestamp is for rendering the video stream, and not for capturing the video stream.
     */
    int64_t renderTimeMs;
    int avsync_type;
  };
public:
  /** Retrieves the camera captured image.
   
   @param videoFrame VideoFrame
   */
  virtual bool onCaptureVideoFrame(VideoFrame& videoFrame) = 0;
    
  /** Processes the received image of the specified user (post-processing).

   @param uid User ID of the specified user sending the image.
   @param videoFrame VideoFrame
   */
  virtual bool onRenderVideoFrame(unsigned int uid, VideoFrame& videoFrame) = 0;
};

class IVideoFrame
{
public:
  enum PLANE_TYPE {
    Y_PLANE = 0,
    U_PLANE = 1,
    V_PLANE = 2,
    NUM_OF_PLANES = 3
  };
  enum VIDEO_TYPE {
    VIDEO_TYPE_UNKNOWN = 0,
    VIDEO_TYPE_I420 = 1,
    VIDEO_TYPE_IYUV = 2,
    VIDEO_TYPE_RGB24 = 3,
    VIDEO_TYPE_ABGR = 4,
    VIDEO_TYPE_ARGB = 5,
    VIDEO_TYPE_ARGB4444 = 6,
    VIDEO_TYPE_RGB565 = 7,
    VIDEO_TYPE_ARGB1555 = 8,
    VIDEO_TYPE_YUY2 = 9,
    VIDEO_TYPE_YV12 = 10,
    VIDEO_TYPE_UYVY = 11,
    VIDEO_TYPE_MJPG = 12,
    VIDEO_TYPE_NV21 = 13,
    VIDEO_TYPE_NV12 = 14,
    VIDEO_TYPE_BGRA = 15,
    VIDEO_TYPE_RGBA = 16,
  };
  virtual void release() = 0;
  virtual const unsigned char* buffer(PLANE_TYPE type) const = 0;

    /** Copy frame.

     If the required size is larger than the allocated one, new buffers of the adequate size will be allocated.

     @param dest_frame Address of the returned destination frame. See IVideoFrame.
     @return
     - 0: Success.
     - -1: Failure.
     */
  virtual int copyFrame(IVideoFrame** dest_frame) const = 0;
    /** Convert frame.

     @note It is assumed that source and destination have equal heights.

     @param dst_video_type Type of output video.
     @param dst_sample_size Required only for the parsing of MJPG.
     @param dst_frame Pointer to a destination frame. See IVideoFrame.
     @return
     - 0: Success.
     - < 0: Failure.
     */
  virtual int convertFrame(VIDEO_TYPE dst_video_type, int dst_sample_size, unsigned char* dst_frame) const = 0;

    /** Retrieves the specified component in the YUV space.

     @param type Component type: #PLANE_TYPE
     */
  virtual int allocated_size(PLANE_TYPE type) const = 0;
    /** Retrieves the stride of the specified component in the YUV space.

     @param type Component type: #PLANE_TYPE
     */
  virtual int stride(PLANE_TYPE type) const = 0;
    /** Retrieves the width of the frame.
     */
  virtual int width() const = 0;
    /** Retrieves the height of the frame.
     */
  virtual int height() const = 0;
    /** Retrieves the timestamp (90 ms) of the frame.
     */
  virtual unsigned int timestamp() const = 0;
    /** Retrieves the render time (ms).
     */
  virtual int64_t render_time_ms() const = 0;
    /** Checks if a plane is of zero size.
     
     @return
     - true: The plane is of zero size.
     - false: The plane is not of zero size.
     */
  virtual bool IsZeroSize() const = 0;
};

class IExternalVideoRenderCallback
{
public:
  virtual void onViewSizeChanged(int width, int height) = 0;
  virtual void onViewDestroyed() = 0;
};
/** Definition of ExternalVideoRenerContext.
 */
struct ExternalVideoRenerContext
{
  IExternalVideoRenderCallback* renderCallback;
    /** Video display window.
     */
  void* view;
    /** Video display mode: rtc::RENDER_MODE_TYPE */
  int renderMode;
    /** The image layer location.

     - 0: (Default) the image is at the bottom of the stack
     - 100: the image is at the top of the stack.

     @note If the value is set to below 0 or above 100, the #ERR_INVALID_ARGUMENT error occurs.
     */
  int zOrder;
    /** Video layout distance from the left.
     */
  float left;
    /** Video layout distance from the top.
     */
  float top;
    /** Video layout distance from the right.
     */
  float right;
    /** Video layout distance from the bottom.
     */
  float bottom;
};

class IExternalVideoRender
{
public:
  virtual void release() = 0;
  virtual int initialize() = 0;
  virtual int deliverFrame(const IVideoFrame& videoFrame, int rotation, bool mirrored) = 0;
};

class IExternalVideoRenderFactory
{
public:
  virtual IExternalVideoRender* createRenderInstance(const ExternalVideoRenerContext& context) = 0;
};

class IMediaEngine
{
public:
  virtual void release() = 0;
    
  /** Registers an audio frame observer object.
   
   This method is used to register an audio frame observer object (register a callback function). This method is required to register callback functions when the engine is required to provide an IAudioFrameObserver::onRecordAudioFrame or IAudioFrameObserver::onPlaybackAudioFrame callback function.
   
   @param observer Audio frame observer object instance. If NULL is passed in, the registration is canceled.
   @return
   - 0: Success.
   - < 0: Failure.
   */
  virtual int registerAudioFrameObserver(IAudioFrameObserver* observer) = 0;
    
  /** Registers a video frame observer object.
   
   This method is required to register callback functions when the engine is required to provide an IVideoFrameObserver::onCaptureVideoFrame or IVideoFrameObserver::onRenderVideoFrame callback function.
   
   @param observer Video frame observer object instance. If NULL is passed in, the registration is canceled.
   @return
   - 0: Success.
   - < 0: Failure.
   */
  virtual int registerVideoFrameObserver(IVideoFrameObserver* observer) = 0;
    
  virtual int registerVideoRenderFactory(IExternalVideoRenderFactory* factory) = 0;
    
  /** Pushes the external audio frame.
   
   @param type Type of audio capture device: #MEDIA_SOURCE_TYPE.
   @param frame Audio frame pointer: IAudioFrameObserver::AudioFrame.
   @param wrap Whether to use the placeholder. Agora recommends that users use the default values.
   - true: Used.
   - false: (Default) Not used.
   
   @return
   - 0: Success.
   - < 0: Failure.
   */
  virtual int pushAudioFrame(MEDIA_SOURCE_TYPE type, IAudioFrameObserver::AudioFrame *frame, bool wrap = false){ return -1; }

};

} //media

} //agora

#endif //AGORA_MEDIA_ENGINE_H
