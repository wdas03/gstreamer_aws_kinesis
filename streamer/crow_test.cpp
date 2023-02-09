#include <gst/gst.h>

#include <stdio.h>
#include <string.h>

#include <gstStreamUtil.h>
#include <gstDeviceUtil.h>

#include <crow_all.h>

#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
    gst_init(&argc, &argv);
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        GList *devices = getDevices(TRUE);
        return "Hello world";
    });

    app.port(18080).multithreaded().run();

    return 0;
}