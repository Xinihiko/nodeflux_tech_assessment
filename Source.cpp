#include <crow.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "base64.h"

using namespace cv;
using namespace std;

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/image_resize").methods("POST"_method)([](const crow::request& req){
        auto x = crow::json::load(req.body);
        ostringstream os;
        crow::json::wvalue res;
        
        string base64_string = x["input_jpeg"].s();
        string dec_jpg = base64_decode(base64_string);
        vector<uchar> data(dec_jpg.begin(), dec_jpg.end());
        Mat image = imdecode(data, IMREAD_UNCHANGED);

        int new_width = int(x["desired_width"]);
        int new_height = int(x["desired_height"]);

        resize(image, image, Size(new_width, new_height));

        vector<uchar> buf;
        imencode(".jpg", image, buf);
        string vtostr(buf.begin(), buf.end());
        string encoded = base64_encode(vtostr);

        res["code"] = 200;
        res["message"] = "success";
        res["output_jpeg"] = encoded;
        os << crow::json::dump(res);
        return crow::response{ os.str()};
    });

    app.port(8080).multithreaded().run();
}