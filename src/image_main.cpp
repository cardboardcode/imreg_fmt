#include <imreg_fmt/image_registration.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char **argv)
{
    cv::Mat im0, im1;

    if (argc < 3)
    {
        std::cout << "[ ERROR ]: Please specify two image files." << std::endl;
        std::cout << "Eg. ./image_main input.jpeg rotated_input.jpeg" << std::endl;
        return 1;
    }
    im0 = cv::imread(argv[1], cv::IMREAD_COLOR);
    im1 = cv::imread(argv[2], cv::IMREAD_COLOR);

    std::string third_string = "--not-save-result";

    if (!(argv[3] != NULL)) {
      third_string = "--not-save-result";
    } else {
      third_string = "--save-result";
    }
    
    bool isSaveMode = false;

    isSaveMode = (!third_string.compare("--save-result"));

    if (isSaveMode) {
      cv::imwrite("im0.png", im0);
      std::cout << "Written to file [ im0.png ]" << std::endl;
      cv::imwrite("im1.png", im1);
      std::cout << "Written to file [ im1.png ]" << std::endl;
    } else {
      cv::imshow("im0", im0);
      cv::imshow("im1", im1);
    }

    ImageRegistration image_registration(im0, isSaveMode);

    // x, y, rotation, scale
    std::vector<double> transform_params(4, 0.0);
    cv::Mat registered_image;
    image_registration.registerImage(im1, registered_image, transform_params, true);

    cv::Mat overlay_image;
    cv::addWeighted(image_registration.getCurrentImage(), 0.5, registered_image, 0.5, 0.0, overlay_image);

    if (isSaveMode) {
      cv::Mat output_image;
      overlay_image.convertTo(output_image, CV_8UC1, 255, 0);
      cv::imwrite("overlay_image.png", output_image);
      std::cout << "Written to file [ overlay_image.png ]" << std::endl;
    } else {
      cv::imshow("overlay_image", overlay_image);
    }

    std::cout << "x: " << transform_params[0] << ", y: "
              << transform_params[1] << ", rotation: " << transform_params[2]
              << ", scale: " << transform_params[3] << std::endl;

    cv::waitKey(0);
    return 0;
}
