#include <gst/gst.h>
#include <stdio.h>
#include <string.h>

// https://github.com/matthew1000/gstreamer-cheat-sheet


char *ACCESS_KEY = "";
char *SECRET_KEY = "";
char *DEFAULT_REGION = ";

static gboolean my_bus_func (GstBus *bus, GstMessage *message, gpointer user_data)
   {
      GstDevice *device;
      gchar *name;

      switch (GST_MESSAGE_TYPE (message)) {
        case GST_MESSAGE_DEVICE_ADDED:
          gst_message_parse_device_added (message, &device);
          name = gst_device_get_display_name (device);
          g_print("Device added: %s\n", name);
          g_free (name);
          gst_object_unref (device);
          break;
        case GST_MESSAGE_DEVICE_REMOVED:
          gst_message_parse_device_removed (message, &device);
          name = gst_device_get_display_name (device);
          g_print("Device removed: %s\n", name);
          g_free (name);
          gst_object_unref (device);
          break;
        default:
          break;
      }

      return G_SOURCE_CONTINUE;
   }

GstDeviceMonitor *setup_raw_video_source_device_monitor (void) {
    GstDeviceMonitor *monitor;
    GstBus *bus;
    GstCaps *caps;

    monitor = gst_device_monitor_new();

    bus = gst_device_monitor_get_bus (monitor);
    gst_bus_add_watch (bus, my_bus_func, NULL);
    gst_object_unref (bus);

    caps = gst_caps_new_empty_simple ("video/x-raw");
    gst_device_monitor_add_filter (monitor, "Video/Source", caps);
    gst_caps_unref(caps);

    if (!gst_device_monitor_start(monitor))
        printf("NOOO");

    return monitor;
}

int displayCameraStream(int cameraIdx, int resWidth, int resHeight, int fps_num, int fps_den) {
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
    sink = gst_element_factory_make ("autovideosink", "sink");

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

int pushStreamToKVSMultiple(int numStreams, char *streamNames[], int cameraIndices[], 
                            int resWidthArr[], int resHeightArr[], int fps_nums[], int fps_dens[]) {
    GMainLoop *mainLoop;
    GstStateChangeReturn ret;
    
    GstElement *pipelines[numStreams];

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

        pipelines[i] = pipeline;
    }

    for (int i = 0; i < numStreams; i++) {
        GstElement *pipeline = pipelines[i];
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

int pushStreamToKVS(char *streamName, int cameraIdx, char *cameraName, int resWidth, int resHeight, int fps_num, int fps_den) {
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

    //gst_init(&argc, &argv);
    pipeline = gst_pipeline_new ("kvs-streamer");
    source = gst_element_factory_make ("avfvideosrc", "source");
    convert = gst_element_factory_make ("videoconvert", "convert");
    capsfilter = gst_element_factory_make("capsfilter", "capsfilter");
    queue = gst_element_factory_make ("queue", "queue");
    encoder = gst_element_factory_make ("vtenc_h264_hw", "encoder");
    secondQueue = gst_element_factory_make ("queue", "second_queue");
    sink = gst_element_factory_make ("kvssink", "sink");
    //autovideosink = gst_element_factory_make ("autovideosink", "sink");

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

    g_object_set(G_OBJECT (source), "device-index", cameraIdx, NULL);
    g_object_set(G_OBJECT (encoder), "allow-frame-reordering", FALSE, NULL);
    g_object_set(G_OBJECT (encoder), "realtime", TRUE, NULL);
    g_object_set(G_OBJECT (encoder), "max-keyframe-interval", 45, NULL);
    g_object_set( G_OBJECT (capsfilter),  "caps",  caps, NULL );
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

// Source: https://github.com/GStreamer/gst-plugins-base/blob/master/tools/gst-device-monitor.c
static gchar *get_launch_line (GstDevice * device) {
    static const char *const ignored_propnames[] = { "name", "parent", "direction", "template", "caps", NULL };
    GString *launch_line;
    GstElement *element;
    GstElement *pureelement;
    GParamSpec **properties, *property;
    GValue value = G_VALUE_INIT;
    GValue pvalue = G_VALUE_INIT;
    guint i, number_of_properties;
    GstElementFactory *factory;

    element = gst_device_create_element (device, NULL);

    if (!element)
    return NULL;

    factory = gst_element_get_factory (element);
    if (!factory) {
    gst_object_unref (element);
    return NULL;
    }

    if (!gst_plugin_feature_get_name (factory)) {
    gst_object_unref (element);
    return NULL;
    }

    launch_line = g_string_new (gst_plugin_feature_get_name (factory));

    pureelement = gst_element_factory_create (factory, NULL);

    /* get paramspecs and show non-default properties */
    properties =
        g_object_class_list_properties (G_OBJECT_GET_CLASS (element),
        &number_of_properties);
    if (properties) {
    for (i = 0; i < number_of_properties; i++) {
        gint j;
        gboolean ignore = FALSE;
        property = properties[i];

        /* skip some properties */
        if ((property->flags & G_PARAM_READWRITE) != G_PARAM_READWRITE)
        continue;

        for (j = 0; ignored_propnames[j]; j++)
        if (!g_strcmp0 (ignored_propnames[j], property->name))
            ignore = TRUE;

        if (ignore)
        continue;

        /* Can't use _param_value_defaults () because sub-classes modify the
        * values already.
        */

        g_value_init (&value, property->value_type);
        g_value_init (&pvalue, property->value_type);
        g_object_get_property (G_OBJECT (element), property->name, &value);
        g_object_get_property (G_OBJECT (pureelement), property->name, &pvalue);
        if (gst_value_compare (&value, &pvalue) != GST_VALUE_EQUAL) {
        gchar *valuestr = gst_value_serialize (&value);

        if (!valuestr) {
            GST_WARNING ("Could not serialize property %s:%s",
                GST_OBJECT_NAME (element), property->name);
            g_free (valuestr);
            goto next;
        }

        g_string_append_printf (launch_line, " %s=%s",
            property->name, valuestr);
        g_free (valuestr);

        }

    next:
        g_value_unset (&value);
        g_value_unset (&pvalue);
    }
    g_free (properties);
    }

    gst_object_unref (element);
    gst_object_unref (pureelement);

    return g_string_free (launch_line, FALSE);                                                
}

static gboolean print_structure_field (GQuark field_id, const GValue * value, gpointer user_data) {
    gchar *val;

    if (G_VALUE_HOLDS_UINT (value)) {
    val = g_strdup_printf ("%u (0x%08x)", g_value_get_uint (value),
        g_value_get_uint (value));
    } else {
    val = gst_value_serialize (value);
    }

    if (val != NULL)
    g_print ("\n\t\t%s = %s", g_quark_to_string (field_id), val);
    else
    g_print ("\n\t\t%s - could not serialise field of type %s",
        g_quark_to_string (field_id), G_VALUE_TYPE_NAME (value));

    g_free (val);

    return TRUE;
}

static gboolean print_field (GQuark field, const GValue * value, gpointer unused) {
    gchar *str = gst_value_serialize (value);

    g_print (", %s=%s", g_quark_to_string (field), str);
    g_free (str);
    return TRUE;
}

static void print_device(GstDevice * device, gboolean modified) {
    gchar *device_class, *str, *name;
    GstCaps *caps;
    GstStructure *props;
    guint i, size = 0;

    caps = gst_device_get_caps (device);
    if (caps != NULL)
    size = gst_caps_get_size (caps);

    name = gst_device_get_display_name (device);
    device_class = gst_device_get_device_class (device);
    props = gst_device_get_properties (device);

    g_print ("\nDevice %s:\n\n", modified ? "modified" : "found");
    g_print ("\tname  : %s\n", name);
    g_print ("\tclass : %s\n", device_class);
    for (i = 0; i < size; ++i) {
        GstStructure *s = gst_caps_get_structure (caps, i);
        GstCapsFeatures *features = gst_caps_get_features (caps, i);

        g_print ("\t%s %s", (i == 0) ? "caps  :" : "       ",
            gst_structure_get_name (s));
        if (features && (gst_caps_features_is_any (features) ||
                !gst_caps_features_is_equal (features,
                    GST_CAPS_FEATURES_MEMORY_SYSTEM_MEMORY))) {
            gchar *features_string = gst_caps_features_to_string (features);

            g_print ("(%s)", features_string);
            g_free (features_string);
    }
    gst_structure_foreach (s, print_field, NULL);
    g_print ("\n");
    }
    if (props) {
    g_print ("\tproperties:");
    gst_structure_foreach (props, print_structure_field, NULL);
    gst_structure_free (props);
    g_print ("\n");
    }
    str = get_launch_line (device);
    if (gst_device_has_classes (device, "Source"))
    g_print ("\tgst-launch-1.0 %s ! ...\n", str);
    else if (gst_device_has_classes (device, "Sink"))
    g_print ("\tgst-launch-1.0 ... ! %s\n", str);
    else if (gst_device_has_classes (device, "CameraSource")) {
    g_print ("\tgst-launch-1.0 %s.vfsrc name=camerasrc ! ... "
        "camerasrc.vidsrc ! [video/x-h264] ... \n", str);
    }

    g_free (str);
    g_print ("\n");

    g_free (name);
    g_free (device_class);

    if (caps != NULL)
    gst_caps_unref (caps);
}

GList *getDevices(gboolean verbose) {
    GstDeviceMonitor *monitor = gst_device_monitor_new();
    gst_device_monitor_set_show_all_devices (monitor, TRUE);

    GstCaps *caps;
    caps = gst_caps_new_empty_simple ("video/x-raw");
    gst_device_monitor_add_filter (monitor, "Video/Source", caps);
    gst_caps_unref(caps);

    if (!gst_device_monitor_start(monitor)) {
        g_printerr ("Failed to start device monitor!\n");
        return -1;
    }
    
    GList *devices = gst_device_monitor_get_devices(monitor);
    if (verbose) {
        for (int i = 0; i < g_list_length(devices); i++)
        {
            GstDevice *device = g_list_nth_data(devices, i);
            print_device(device, FALSE);
            //printf("device: %s\n", gst_device_get_display_name(device));
        }
    }

    gst_device_monitor_stop(monitor);
    gst_object_unref(monitor);

    return devices;
}

int main (int argc, char *argv[]) {
    gst_init(&argc, &argv);

    GList *devices = getDevices(TRUE);

    //int status = pushStreamToKVS("new_stream", 0, NULL, 320, 240, 10000000, 333333);
    //int status2 = displayCameraStream(0, 1920, 1080, 30, 1);
    char *streamNames[] = {"test1", "test2"};
    int cameraIndices[] = {0, 1};
    int resWidthArr[] = {1920, 1920};
    int resHeightArr[] = {1080, 1080};
    int fps_nums[] = {5000000, 10000000};
    int fps_dens[] = {83333, 333333};

    int multipleStreams = pushStreamToKVSMultiple(2, streamNames, cameraIndices, resWidthArr, 
                                                 resHeightArr, fps_nums, fps_dens);

    return 0;
}
