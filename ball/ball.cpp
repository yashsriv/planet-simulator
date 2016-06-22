#include <chrono>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <random>

#include <opencv2/imgproc.hpp>

#include "ball.h"

Rect FIELD(20, 20, 600, 440);
//std::default_random_engine generator;
std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
std::uniform_int_distribution<int> colorDistribution(0, 255);
std::uniform_int_distribution<int> radiusDistribution(20, 35);
std::uniform_int_distribution<int> xdistribution(20, 620);
std::uniform_int_distribution<int> ydistribution(20, 460);
std::uniform_real_distribution<double> velocityDistribution(5, 10);

Ball::Ball(int debug) {
  for (int i = 0; i < 3; i++) {
    color[i] = colorDistribution(generator);
  }
  radius = radiusDistribution(generator);
  rect = Rect(xdistribution(generator), ydistribution(generator), 2 * radius, 2 * radius);
  for (int i = 0; i < 2; i++) {
    vel[i] = velocityDistribution(generator);
  }
  this -> d = debug;
  if(debug) {
    printf("[ %3.0lf, %3.0lf, %3.0lf ] [ %2.0lf %3.2lf %3.2lf]\n", color[0], color[1], color[2], radius, rect.centerx, rect.centery);
  }
}

void Ball::update(int fps) {
  double actualvel[2];
  actualvel[0] = vel[0] / fps;
  actualvel[1] = vel[1] / fps;
  rect.move(actualvel);
  if (!FIELD.contains(rect)) {
    if (rect.top < 20) {
      vel[0] = -vel[0];
      rect.setTop(20);
    }
    if (rect.bottom > 460) {
      vel[0] = -vel[0];
      rect.setBottom(460);
    }
    if (rect.left < 20) {
      vel[1] = -vel[1];
      rect.setLeft(20);
    }
    if (rect.right > 620) {
      vel[1] = -vel[1];
      rect.setRight(620);
    }
  }
}

double Ball::getPot(double i, double j) {
  double dx = (i - rect.centerx);
  double dy = (j - rect.centery);
  double distance = sqrt(dx * dx + dy * dy);
  if (distance < radius)
    return 3 * (radius * radius) - distance * distance;
  return 2 * (radius * radius * radius) / distance;
}

void Ball::draw(cv::Mat img) {
  if (img.channels() == 3) {
    circle(img, cv::Point(rect.centerx, rect.centery), radius, cv::Scalar(color[0], color[1], color[2]), -1, 8);
  }
  if (d == 2) {
    line(img, cv::Point(rect.centerx, rect.centery), cv::Point(rect.centerx + vel[1], rect.centery + vel[0]), cv::Scalar::all(0), 1, 8);
  }
  circle(img, cv::Point(rect.centerx, rect.centery), radius, cv::Scalar::all(0), 1, 8);
}

void Ball::debug() {
  printf(" [ %2.0lf, %3.2lf, %3.2lf ]\n", radius, rect.centerx, rect.centery);
}
