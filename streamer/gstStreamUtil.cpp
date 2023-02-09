#include <gst/gst.h>
#include <gtkmm.h>

#include <glib-unix.h>

#include <stdio.h>
#include <string.h>
#include <signal.h>

#include <gstDeviceUtil.h>

#include <vector>

using namespace std;

int signal_watch_intr_id;

char * ACCESS_KEY = "";
char * SECRET_KEY = "";
char * DEFAULT_REGION = "";

typedef struct _CustomData {
    GstElement *pipeline;
    GMainLoop *loop;
} CustomData;

static gboolean intr_handler (CustomData *data) {
  printf("handling interrupt.\n");
  //GstElement *pipeline = (GstElement *) user_data;
  gst_element_set_state (data->pipeline, GST_STATE_READY);
  g_main_loop_quit (data->loop);


  /* post an application specific message 
  gst_element_post_message (GST_ELEMENT (data->pipeline),
      gst_message_new_application (GST_OBJECT (data->pipeline),
          gst_structure_new ("GstLaunchInterrupt",
              "message", G_TYPE_STRING, "Pipeline interrupted", NULL)));*/

  /* remove signal handler */
  signal_watch_intr_id = 0;
  return G_SOURCE_REMOVE;
}

static void cb_message (GstBus *bus, GstMessage *msg, CustomData *data) {

  switch (GST_MESSAGE_TYPE (msg)) {
    case GST_MESSAGE_ERROR: {
      GError *err;
      gchar *debug;

      gst_message_parse_error (msg, &err, &debug);
      g_print ("Error: %s\n", err->message);
      g_error_free (err);
      g_free (debug);

      gst_element_set_state (data->pipeline, GST_STATE_READY);
      g_main_loop_quit (data->loop);
      break;
    }
    case GST_MESSAGE_EOS:
      /* end-of-stream */
      g_print("EOS");
      gst_element_set_state (data->pipeline, GST_STATE_READY);
      g_main_loop_quit (data->loop);
      break;
    case GST_MESSAGE_BUFFERING: {
      gint percent = 0;

      /* If the stream is live, we do not care about buffering. */
      //if (data->is_live) break;

      gst_message_parse_buffering (msg, &percent);
      g_print ("Buffering (%3d%%)\r", percent);
      /* Wait until buffering is complete before start/resume playing */
      if (percent < 100)
        gst_element_set_state (data->pipeline, GST_STATE_PAUSED);
      else
        gst_element_set_state (data->pipeline, GST_STATE_PLAYING);
      break;
    }
    case GST_MESSAGE_CLOCK_LOST:
      /* Get a new clock */
      gst_element_set_state (data->pipeline, GST_STATE_PAUSED);
      gst_element_set_state (data->pipeline, GST_STATE_PLAYING);
      break;
    default:
      /* Unhandled message */
      break;
    }
}

// Stream to file:
// gst-launch-1.0 avfvideosrc device-index=0 ! videoconvert ! video/x-raw,width=640,height=480,framerate=30/1 ! x264enc ! mp4mux ! filesink location="/Users/williamdas/Downloads/test.mp4" -e
// https://github.com/crearo/gstreamer-cookbook/blob/master/README.md

int streamToFile(int cameraIdx, char *filename, int resWidth, int resHeight, int fps_num, int fps_den) {
    GstElement *pipeline, *source, *convert, *encoder, *muxer, *sink, *queue, *capsfilter;
    CustomData data;

    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    GMainLoop *loop;

    GstCaps *caps;

    data.pipeline = gst_pipeline_new("camera-stream");

    source = gst_element_factory_make("avfvideosrc", "source");
    convert = gst_element_factory_make ("videoconvert", "convert");
    encoder = gst_element_factory_make("x264enc", "encoder");
    muxer = gst_element_factory_make("mp4mux", "mux");
    capsfilter = gst_element_factory_make("capsfilter", "capsfilter");
    queue = gst_element_factory_make ("queue", "queue");
    sink = gst_element_factory_make ("filesink", "sink");

    caps = gst_caps_new_simple ("video/x-raw",
          "width", G_TYPE_INT, resWidth,
          "height", G_TYPE_INT, resHeight,
          "framerate", GST_TYPE_FRACTION, fps_num, fps_den,
          NULL);
    
    g_assert(source);
    g_assert(convert);
    g_assert(encoder);
    g_assert(muxer);
    g_assert(capsfilter);
    g_assert(queue);
    g_assert(sink);

    /*
    if (!pipeline || !source || !convert || !queue || !capsfilter || !encoder || !sink) {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }*/

    g_object_set(G_OBJECT (source), "device-index", cameraIdx, NULL);
    g_object_set( G_OBJECT (capsfilter),  "caps",  caps, NULL );
    g_object_set( G_OBJECT (sink),  "location",  filename, NULL );

    gst_bin_add_many (GST_BIN (data.pipeline), source, convert, capsfilter, encoder, muxer, queue, sink, NULL);
    
    if (gst_element_link_many (source, convert, capsfilter, encoder, muxer, queue,  sink, NULL) != TRUE) {
        g_printerr ("Elements could not be linked.\n");
        gst_object_unref (pipeline);
        return -1;
    }

      /* Wait until error or EOS */
    bus = gst_element_get_bus (data.pipeline);

    ret = gst_element_set_state (data.pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        printf("Unable to set the pipeline to the playing state.");
        //cout << "Unable to set the pipeline to the playing state." << endl;
        gst_object_unref (data.pipeline);
        return -1;
    }

    data.loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(data.loop);

    gst_bus_add_signal_watch (bus);
    g_signal_connect (bus, "message", G_CALLBACK (cb_message), &data);
    signal_watch_intr_id = g_unix_signal_add (SIGINT, (GSourceFunc) intr_handler, &data);

    g_main_loop_unref(data.loop);
    gst_object_unref (bus);
    gst_element_set_state (data.pipeline, GST_STATE_NULL);
    gst_object_unref (data.pipeline);
    gst_caps_unref (caps);

    return 0;
}

// Display camera stream
int displayCameraStream(int cameraIdx, char *videosink, int resWidth, int resHeight, int fps_num, int fps_den) {
    GstElement *pipeline, *source, *convert, *sink, *queue, *capsfilter;
    
    GstBus *bus;
    GstStateChangeReturn ret;
    GMainLoop *loop;

    GstCaps *caps;

    pipeline = gst_pipeline_new("camera-stream");
    source = gst_element_factory_make("avfvideosrc", "source");
    convert = gst_element_factory_make ("videoconvert", "convert");
    capsfilter = gst_element_factory_make("capsfilter", "capsfilter");
    queue = gst_element_factory_make ("queue", "queue");
    sink = gst_element_factory_make (videosink, "sink");

    caps = gst_caps_new_simple ("video/x-raw",
          "width", G_TYPE_INT, resWidth,
          "height", G_TYPE_INT, resHeight,
          "framerate", GST_TYPE_FRACTION, fps_num, fps_den,
          NULL);
    
    g_assert(source);
    g_assert(convert);
    g_assert(capsfilter);
    g_assert(queue);
    g_assert(sink);

    /*
    if (!pipeline || !source || !convert || !queue || !capsfilter || !encoder || !sink) {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }*/

    g_object_set(G_OBJECT (source), "device-index", cameraIdx, NULL);
    g_object_set( G_OBJECT (capsfilter),  "caps",  caps, NULL );
    g_object_set(G_OBJECT (sink), "sync", FALSE, NULL);

    gst_bin_add_many (GST_BIN (pipeline), source, convert, capsfilter, queue, sink, NULL);
    
    if (gst_element_link_many (source, convert, capsfilter, queue,  sink, NULL) != TRUE) {
        g_printerr ("Elements could not be linked.\n");
        gst_object_unref (pipeline);
        return -1;
    }

    ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        printf("Unable to set the pipeline to the playing state.");
        //cout << "Unable to set the pipeline to the playing state." << endl;
        gst_object_unref (pipeline);
        return -1;
    }

    bus = gst_element_get_bus (pipeline);

    loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);

    g_main_loop_unref(loop);
    gst_object_unref (bus);
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (pipeline);
    gst_caps_unref (caps);

    return 0;
}

int pushStreamToKVSMultiple(int numStreams, char * streamNames[], int cameraIndices[], 
                            int resWidthArr[], int resHeightArr[], int fps_nums[], int fps_dens[]) {
    GMainLoop *mainLoop;
    GstStateChangeReturn ret;
    
    vector<GstElement *> pipelines;

    for (int i = 0; i < numStreams; i++) {
        GstElement *pipeline, *source, *convert, *encoder, *sink, 
                *queue, *autovideosink, *capsfilter, *secondQueue;
        GstBus *bus;
        GstStateChangeReturn ret;
        GstCaps *caps;

        pipeline = gst_pipeline_new ("kvs-streamer");
        source = gst_element_factory_make ("avfvideosrc", "source");
        convert = gst_element_factory_make ("videoconvert", "convert");
        capsfilter = gst_element_factory_make("capsfilter", "capsfilter");
        queue = gst_element_factory_make ("queue", "queue");
        encoder = gst_element_factory_make ("vtenc_h264_hw", "encoder");
        secondQueue = gst_element_factory_make ("queue", "second_queue");
        sink = gst_element_factory_make ("kvssink", "sink");

        caps = gst_caps_new_simple ("video/x-raw",
            "width", G_TYPE_INT, resWidthArr[i],
            "height", G_TYPE_INT, resHeightArr[i],
            "framerate", GST_TYPE_FRACTION, fps_nums[i], fps_dens[i],
            NULL);
        
        g_assert(source);
        g_assert(convert);
        g_assert(capsfilter);
        g_assert(queue);
        g_assert(encoder);
        g_assert(secondQueue);
        g_assert(sink);

        /*
        if (!pipeline || !source || !convert || !queue || !capsfilter || !encoder || !sink) {
            g_printerr ("Not all elements could be created.\n");
            return -1;
        }*/

        g_object_set(G_OBJECT (source), "device-index", cameraIndices[i], NULL);
        g_object_set(G_OBJECT (encoder), "allow-frame-reordering", FALSE, NULL);
        g_object_set(G_OBJECT (encoder), "realtime", TRUE, NULL);
        g_object_set(G_OBJECT (encoder), "max-keyframe-interval", 45, NULL);
        g_object_set( G_OBJECT (capsfilter),  "caps",  caps, NULL );
        g_object_set(G_OBJECT (sink), "stream-name", streamNames[i], NULL);
        g_object_set(G_OBJECT (sink), "storage-size", 128, NULL);
        g_object_set(G_OBJECT (sink), "access-key", ACCESS_KEY, NULL);
        g_object_set(G_OBJECT (sink), "secret-key", SECRET_KEY, NULL);
        g_object_set(G_OBJECT (sink), "aws-region", DEFAULT_REGION, NULL);

        gst_bin_add_many (GST_BIN (pipeline), source, convert, capsfilter, queue, encoder, secondQueue, sink, NULL);
        
        if (gst_element_link_many (source, convert, capsfilter, queue, encoder, secondQueue, sink, NULL) != TRUE) {
            g_printerr ("Elements could not be linked.\n");
            gst_object_unref (pipeline);
            return -1;
        }

        pipelines.push_back(pipeline);
    }

    for (GstElement *pipeline : pipelines) {
        ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
        if (ret == GST_STATE_CHANGE_FAILURE) {
            printf("Unable to set the pipeline to the playing state.");
            gst_object_unref (pipeline);
            return -1;
        }
    }

    mainLoop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(mainLoop);

    return 0;
}

int pushStreamToKVSFromDevice(char * streamName, GstDevice *device) {
    char *deviceName = static_cast<char *>(gst_device_get_display_name(device));
    char *deviceClass = static_cast<char *>(gst_device_get_device_class(device));
    GstCaps *caps = gst_device_get_caps(device);

    //pushStreamToKVS();
}

int pushStreamToKVS(char * streamName, int cameraIdx, char * cameraName, int resWidth, int resHeight, int fps_num, int fps_den) {
    /*
        https://medium.com/lifesjourneythroughalens/implementing-gstreamer-webcam-usb-internal-streaming-mac-c-clion-76de0fdb8b34
        https://github.com/awslabs/amazon-kinesis-video-streams-producer-sdk-cpp/blob/master/docs/macos.md

        Video + audio pipeline:
        gst-launch-1.0 -v avfvideosrc ! videoconvert ! vtenc_h264_hw allow-frame-reordering=FALSE realtime=TRUE max-keyframe-interval=45 ! kvssink name=sink stream-name="irbtest" access-key="" secret-key="" aws-region="us-east-1" osxaudiosrc ! audioconvert ! avenc_aac ! queue ! sink.

        Video pipeline (clear):
        gst-launch-1.0 avfvideosrc device-index=0 ! videoconvert ! vtenc_h264_hw allow-frame-reordering=FALSE realtime=TRUE max-keyframe-interval=45 ! kvssink stream-name="irbtest" storage-size=128 access-key="" secret-key="" aws-region="us-east-1"
        gst-launch-1.0 avfvideosrc device-index=0 ! videoconvert ! video/x-raw,width=1920,height=1080,framerate=30/1 ! vtenc_h264_hw allow-frame-reordering=FALSE realtime=TRUE max-keyframe-interval=45 ! kvssink stream-name="irbtest" storage-size=128 access-key="" secret-key="" aws-region="us-east-1"

        Stream two cameras simultaneously to KVS:
        gst-launch-1.0 avfvideosrc device-index=1 ! videoconvert ! video/x-raw,width=1920,height=1080,framerate=30/1 ! queue ! vtenc_h264_hw allow-frame-reordering=FALSE realtime=TRUE max-keyframe-interval=45 ! queue ! kvssink stream-name="irbtest" storage-size=128 access-key="" secret-key="" aws-region="us-east-1" avfvideosrc device-index=0 ! videoconvert ! video/x-raw,width=1920,height=1080,framerate=10000000/333333 ! queue ! vtenc_h264_hw allow-frame-reordering=FALSE realtime=TRUE max-keyframe-interval=45 ! queue ! kvssink stream-name="irbtest2" storage-size=128 access-key="" secret-key="" aws-region="us-east-1"
    */
    
    GstElement *pipeline, *source, *convert, *encoder, *sink, 
                *queue, *autovideosink, *capsfilter, *secondQueue;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    GMainLoop *loop;

    GstCaps *caps;

    if (cameraIdx == NULL)
        cameraIdx = 0;

    pipeline = gst_pipeline_new ("kvs-streamer");
    source = gst_element_factory_make ("avfvideosrc", "source");
    convert = gst_element_factory_make ("videoconvert", "convert");
    capsfilter = gst_element_factory_make("capsfilter", "capsfilter");
    queue = gst_element_factory_make ("queue", "queue");
    encoder = gst_element_factory_make ("vtenc_h264_hw", "encoder");
    secondQueue = gst_element_factory_make ("queue", "second_queue");
    sink = gst_element_factory_make ("kvssink", "sink");

    caps = gst_caps_new_simple ("video/x-raw",
          "width", G_TYPE_INT, resWidth,
          "height", G_TYPE_INT, resHeight,
          "framerate", GST_TYPE_FRACTION, fps_num, fps_den,
          NULL);
    
    g_assert(source);
    g_assert(convert);
    g_assert(capsfilter);
    g_assert(queue);
    g_assert(encoder);
    g_assert(secondQueue);
    g_assert(sink);

    /*
    if (!pipeline || !source || !convert || !queue || !capsfilter || !encoder || !sink) {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }*/

    // Source props
    g_object_set(G_OBJECT (source), "device-index", cameraIdx, NULL);

    // Encoder props
    g_object_set(G_OBJECT (encoder), "allow-frame-reordering", FALSE, NULL);
    g_object_set(G_OBJECT (encoder), "realtime", TRUE, NULL);
    g_object_set(G_OBJECT (encoder), "max-keyframe-interval", 45, NULL);

    // Capsfilter props
    g_object_set( G_OBJECT (capsfilter),  "caps",  caps, NULL );

    // kvssink props
    g_object_set(G_OBJECT (sink), "stream-name", streamName, NULL);
    g_object_set(G_OBJECT (sink), "storage-size", 128, NULL);
    g_object_set(G_OBJECT (sink), "access-key", ACCESS_KEY, NULL);
    g_object_set(G_OBJECT (sink), "secret-key", SECRET_KEY, NULL);
    g_object_set(G_OBJECT (sink), "aws-region", DEFAULT_REGION, NULL);

    gst_bin_add_many (GST_BIN (pipeline), source, convert, capsfilter, queue, encoder, secondQueue, sink, NULL);

    /*if (gst_element_link (source, sink) != TRUE) {
        g_printerr ("Elements could not be linked.\n");
        gst_object_unref (pipeline);
        return -1;
    }*/
    
    if (gst_element_link_many (source, convert, capsfilter, queue, encoder, secondQueue, sink, NULL) != TRUE) {
        g_printerr ("Elements could not be linked.\n");
        gst_object_unref (pipeline);
        return -1;
    }

    ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        printf("Unable to set the pipeline to the playing state.");
        gst_object_unref (pipeline);
        return -1;
    }

    bus = gst_element_get_bus (pipeline);

    loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);

    g_main_loop_unref(loop);
    gst_object_unref (bus);
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (pipeline);
    gst_caps_unref (caps);

    return 0;
}