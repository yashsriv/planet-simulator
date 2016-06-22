#include <opencv2/core.hpp>

#include "rect.h"

class Ball {

  private:
    double color[3];
    int d;
  public:
    Rect rect;
    double radius;
    double vel[2];
    Ball(int);
    void update(int);
    void draw(cv::Mat);
    double getPot(double, double);
    void debug();

};

