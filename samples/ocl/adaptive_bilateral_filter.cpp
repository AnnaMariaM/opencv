// This sample shows the difference of adaptive bilateral filter and bilateral filter.
#include "opencv2/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/ocl.hpp"

using namespace cv;
using namespace std;


int main( int argc, const char** argv )
{
    const char* keys =
        "{ i input   |          | specify input image }"
        "{ k ksize   |     5    | specify kernel size }"
        "{ h help    | false    | print help message }";

    CommandLineParser cmd(argc, argv, keys);
    if (cmd.has("help"))
    {
        cout << "Usage : adaptive_bilateral_filter [options]" << endl;
        cout << "Available options:" << endl;
        cmd.printMessage();
        return EXIT_SUCCESS;
    }

    string src_path = cmd.get<string>("i");
    int ks = cmd.get<int>("k");
    const char * winName[] = {"input", "adaptive bilateral CPU", "adaptive bilateral OpenCL", "bilateralFilter OpenCL"};

    Mat src = imread(src_path), abFilterCPU;
    if (src.empty())
    {
        cout << "error read image: " << src_path << endl;
        return EXIT_FAILURE;
    }

    ocl::oclMat dsrc(src), dABFilter, dBFilter;

    Size ksize(ks, ks);
    adaptiveBilateralFilter(src,abFilterCPU, ksize, 10);
    ocl::adaptiveBilateralFilter(dsrc, dABFilter, ksize, 10);
    ocl::bilateralFilter(dsrc, dBFilter, ks, 30, 9);

    Mat abFilter = dABFilter, bFilter = dBFilter;
    imshow(winName[0], src);
    imshow(winName[1], abFilterCPU);
    imshow(winName[2], abFilter);
    imshow(winName[3], bFilter);
    waitKey();

    return EXIT_SUCCESS;
}
