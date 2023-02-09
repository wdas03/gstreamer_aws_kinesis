#include <utility>
#include <vector>

#ifndef GST_STREAM_UTIL_H
#define GST_STREAM_UTIL_H

/*
#include <gst/gst.h>
#include <stdio.h>
#include <string.h>
*/

using namespace std;

int displayCameraStream(int cameraIdx, char *videosink, int resWidth, int resHeight, int fps_num, int fps_den);

int streamToFile(int cameraIdx, char *filename, int resWidth, int resHeight, int fps_num, int fps_den);

int pushStreamToKVSMultiple(int numStreams, char * streamNames[], int cameraIndices[], 
                            int resWidthArr[], int resHeightArr[], int fps_nums[], int fps_dens[]);

int pushStreamToKVSFromDevice(char * streamName, GstDevice *device);

int pushStreamToKVS(char * streamName, int cameraIdx, char * cameraName, int resWidth, int resHeight, int fps_num, int fps_den);

#endif