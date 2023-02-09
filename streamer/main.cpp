#include <gst/gst.h>

#include <stdio.h>
#include <string.h>

#include <gstStreamUtil.h>
#include <gstDeviceUtil.h>

#include <crow_all.h>

#include <vector>

using namespace std;

// https://github.com/matthew1000/gstreamer-cheat-sheet


int main (int argc, char *argv[]) {
    gst_init(&argc, &argv);

    GList *devices = getDevices(FALSE);

    //int status = pushStreamToKVS("new_stream", 0, NULL, 320, 240, 10000000, 333333);
    //int status2 = displayCameraStream(0, 1920, 1080, 30, 1);
    
    char * streamNames[] = {"test1", "test2"};
    int cameraIndices[] = {0, 1};
    int resWidthArr[] = {1920, 1920};
    int resHeightArr[] = {1080, 1080};
    int fps_nums[] = {5000000, 10000000};
    int fps_dens[] = {83333, 333333};

    //displayCameraStream(1, "autovideosink", 1920, 1080, 5000000, 83333);
    displayCameraStream(1, "fpsdisplaysink", 640, 360, 10000000, 38461);
    /*
    vector<gstCameraDeviceConfigs> camDevices;

    for (int i = 0; i < g_list_length(devices); i++) {
        gstCameraDeviceConfigs camDevice;
        GstDevice *device = static_cast<GstDevice *>(g_list_nth_data(devices, i));
        char *deviceName = static_cast<char *>(gst_device_get_display_name(device));
        g_print("Name: %s\n", deviceName);

        camDevice.name = deviceName;

        GstCaps *caps = gst_device_get_caps(device);

        int width;
        int height;

        int numCaps = gst_caps_get_size(caps);

        for (int i = 0; i < numCaps; i++) {
            GstStructure *caps_struct = gst_caps_get_structure(caps, i);

            if(!gst_structure_get_int(caps_struct, "width", &width) || !gst_structure_get_int(caps_struct, "height", &height)) {
                g_print("Can't get width or height");
            } else {
                g_print("Width: %d, Height: %d\n", width, height);
            }

            Resolution res;
            res.width = width;
            res.height = height;

            camDevice.resolutions.push_back(res);

            const char* codec = static_cast<const char*>(gst_structure_get_name(caps_struct));

            // get highest framerate
            float frameRate = 0;
            int frameRateNum = 0;
            int frameRateDenom = 0;
            
            vector<FrameRate> framerates;

            if( gst_structure_get_fraction(caps_struct, "framerate", &frameRateNum, &frameRateDenom) ) {
                frameRate = float(frameRateNum) / float(frameRateDenom);
                FrameRate fps;
                fps.num = frameRateNum;
                fps.den = frameRateDenom;
                fps.fps = frameRate;

                framerates.push_back(fps);

            } else {
                // it's a list of framerates, pick the max
                GValueArray* frameRateList = NULL;

                if( gst_structure_get_list(caps_struct, "framerate", &frameRateList) && frameRateList->n_values > 0 )
                {
                    for( uint32_t n=0; n < frameRateList->n_values; n++ )
                    {
                        GValue* value = frameRateList->values + n;

                        FrameRate fps;

                        if( GST_VALUE_HOLDS_FRACTION(value) )
                        {
                            frameRateNum = gst_value_get_fraction_numerator(value);
                            frameRateDenom = gst_value_get_fraction_denominator(value);

                            

                            if( frameRateNum > 0 && frameRateDenom > 0 )
                            {
                                fps.num = frameRateNum;
                                fps.den = frameRateDenom;

                                const float rate = float(frameRateNum) / float(frameRateDenom);
                                fps.fps = rate;

                                framerates.push_back(fps);
                            }
                        }
                    }
                }
            }

            camDevice.frameRates.push_back(framerates);

            g_print("\t %d: %s\n\n", i+1, gst_structure_to_string(caps_struct));

            camDevices.push_back(camDevice);
        }

        vector<gstCameraConfig> camConfigs;

        for (gstCameraDeviceConfigs cam : camDevices) {
            //g_print("Width: %d, Height: %d\n", cam.resolutions[0].width, cam.resolutions[0].height);
        }

        
        //print_device(device, FALSE);
        //printf("device: %s\n", gst_device_get_display_name(device));
    }
    */

    //displayCameraStream(0, "fpsdisplaysink", 1920, 1080, 5000000, 83333);
    //displayCameraStream(1, "fpsdisplaysink", 1920, 1080, 5000000, 83333);
    //streamToFile(0, "test.mp4", 1920, 1080, 30, 1);

    //auto app = Gtk::Application::create("org.gtkmm.examples.base");
    //return app->make_window_and_run<MyWindow>(argc, argv);

    //int multipleStreams = pushStreamToKVSMultiple(2, streamNames, cameraIndices, resWidthArr, 
    //                                             resHeightArr, fps_nums, fps_dens);

    //pushStreamToKVS("shubhstream", 0, NULL, 1920, 1080, 30, 1);

    return 0;
}