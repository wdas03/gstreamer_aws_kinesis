#include <utility>
#include <vector>

using namespace std;

#ifndef GST_DEVICE_UTIL_H
#define GST_DEVICE_UTIL_H

/*
#include <gst/gst.h>
#include <stdio.h>
#include <string.h>
*/

typedef struct _Resolution {
    int width;
    int height;
} Resolution;

typedef struct _frameRate {
    int num;
    int den;
    float fps;
} FrameRate;

typedef struct _gstCameraDeviceConfigs {
    char *name;
    vector<Resolution> resolutions;
    vector<vector<FrameRate> > frameRates;
    vector<char *> codecs;
} gstCameraDeviceConfigs;

typedef struct _gstCameraConfig {
    char *name;
    Resolution res;
    FrameRate fps;
    char *codec;
} gstCameraConfig;

// Source: https://github.com/GStreamer/gst-plugins-base/blob/master/tools/gst-device-monitor.c
static gchar *get_launch_line (GstDevice * device);

static gboolean print_structure_field (GQuark field_id, const GValue * value, gpointer user_data);

static gboolean print_field (GQuark field, const GValue * value, gpointer unused);

static void print_device(GstDevice *device, gboolean modified);

GList *getDevices(gboolean verbose);

static gboolean my_bus_func (GstBus *bus, GstMessage *message, gpointer user_data);

GstDeviceMonitor *setup_raw_video_source_device_monitor (void);

#endif