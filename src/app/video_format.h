#ifndef VIDEO_FORMAT_H
#define VIDEO_FORMAT_H

# include <string>

// reference: https://gstreamer.freedesktop.org/documentation/video/video-format.html?gi-language=c#GstVideoFormat
const std::unordered_map<std::string, int> VideoFormat = {
  { "I420", 2 },  // planar 4:2:0 YUV
  { "YV12", 3 },  // planar 4:2:0 YVU (like I420 but UV planes swapped)
  { "YUY2", 4 },  // packed 4:2:2 YUV (Y0-U0-Y1-V0 Y2-U2-Y3-V2 Y4 ...)
  { "UYVY", 5 },  // packed 4:2:2 YUV (U0-Y0-V0-Y1 U2-Y2-V2-Y3 U4 ...)
  { "AYUV", 6 },  // packed 4:4:4 YUV with alpha channel (A0-Y0-U0-V0 ...)
  { "Y42B", 18 }, // planar 4:2:2 YUV
  { "YVYU", 19 }, // packed 4:2:2 YUV (Y0-V0-Y1-U0 Y2-V2-Y3-U2 Y4 ...)
  { "Y444", 20 }, // planar 4:4:4 YUV
  { "v210", 21 }, // packed 4:2:2 10-bit YUV, complex format
  { "v216", 22 }, // packed 4:2:2 16-bit YUV, Y0-U0-Y1-V1 order
  { "NV12", 23 }, // planar 4:2:0 YUV with interleaved UV plane
  { "NV21", 24 }, // planar 4:2:0 YUV with interleaved VU plane
  { "NV16", 51 }, // planar 4:2:2 YUV with interleaved UV plane
  { "NV61", 60 } // planar 4:2:2 YUV with interleaved VU plane (Since: 1.6)
};

#endif  // VIDEO_FORMAT_H
