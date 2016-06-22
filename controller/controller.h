#include <opencv2/core.hpp>

#include "ball.h"

class Controller {

  private:
    int number;
    Ball * balls;
    int debug;

  public:
    Controller(int, int);
    ~Controller();
    void update(int, double);
    void draw(cv::Mat, int);
    void drawPotMap(cv::Mat, int);
    void checkCollisions();
    void accelerate(int, double);
};
