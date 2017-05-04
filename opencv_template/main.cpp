#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

cv::Mat src_image, hsv_image, hsv_image_mask;
int h_min = 0, s_min = 88, v_min = 2;
int h_max = 113, s_max = 255, v_max = 216;
 
void trackbarCallback(int, void*)
{
  cv::inRange(hsv_image, cv::Scalar(h_min, s_min, v_min), cv::Scalar(h_max, s_max, v_max), hsv_image_mask);
  cv::imshow("hsv_image_mask", hsv_image_mask);
  std::vector<std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchy;
  
  //opening
  cv::Mat opening_image;
  cv::morphologyEx(hsv_image_mask, opening_image, cv::MORPH_OPEN, cv::Mat(), cv::Point(-1, -1), 5);
  cv::imshow("opening_image", opening_image);
  
  cv::findContours(opening_image, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
  cv::Mat contour_image = src_image;
  for (int i = 0; i < contours.size(); i++) {
    cv::Moments contour_moments = cv::moments(contours.at(i), true);
    double x = contour_moments.m10 / contour_moments.m00;
    double y = contour_moments.m01 / contour_moments.m00;
    cv::circle(contour_image, cv::Point(x, y), 3, cv::Scalar(0, 0, 255), 5);
  }
  cv::drawContours(contour_image, contours, -1, cv::Scalar(0, 0, 255), 2);
  cv::imshow("contour", contour_image);
}

int main(int argc, char** argv)
{
  src_image = cv::imread(argv[1], 1);
  cv::namedWindow("src_image", cv::WINDOW_AUTOSIZE);
  cv::namedWindow("hsv_image_mask", cv::WINDOW_AUTOSIZE);
  cv::namedWindow("opening_image", cv::WINDOW_AUTOSIZE);
  cv::namedWindow("contour", cv::WINDOW_AUTOSIZE);
  cv::imshow("src_image", src_image);
  cv::createTrackbar("h_min", "src_image", &h_min, 255, trackbarCallback);
  cv::createTrackbar("h_max", "src_image", &h_max, 255, trackbarCallback);
  cv::createTrackbar("s_min", "src_image", &s_min, 255, trackbarCallback);
  cv::createTrackbar("s_max", "src_image", &s_max, 255, trackbarCallback);
  cv::createTrackbar("v_min", "src_image", &v_min, 255, trackbarCallback);
  cv::createTrackbar("v_max", "src_image", &v_max, 255, trackbarCallback);
  cv::cvtColor(src_image, hsv_image, CV_RGB2HSV);
  while(cv::waitKey(0));
  return 0;
}
