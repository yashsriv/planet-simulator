#include <iostream>
#include <math.h>

#include "controller.h"

Controller::Controller(int n, int debug) {
  number = n;
  balls = (Ball *)malloc(n * sizeof(Ball));
  for (int i = 0; i < n; i++) {
    if(debug) {
      std::cout << (i + 1) << ".";
    }
    balls[i] = Ball(debug);
  }
  this->debug = debug;
}

Controller::~Controller() {
  free(balls);
}

void Controller::update(int fps, double G) {
  for (int i = 0; i < number; i++) {
    balls[i].update(fps);
  }
  checkCollisions();
  accelerate(fps, G);
  if (debug == 2) {
    for (int i = 0; i < number; i++) {
      std::cout << (i + 1) << ".";
      switch(i % 6) {
        case 0:
          std::cout << "\x1b[36m";
          break;
        case 1:
          std::cout << "\x1b[35m";
          break;
        case 2:
          std::cout << "\x1b[34m";
          break;
        case 3:
          std::cout << "\x1b[33m";
          break;
        case 4:
          std::cout << "\x1b[32m";
          break;
        case 5:
          std::cout << "\x1b[31m";
          break;
      }
      balls[i].debug();
      std::cout << "\x1b[0m";
    }
  }
}

void Controller::draw(cv::Mat img, int layers) {
  for (int i = 0; i < number; i++) {
    balls[i].draw(img);
  }
  drawPotMap(img, layers);
}

struct BW {
  uchar intensity;
};

void Controller::drawPotMap(cv::Mat img, int layers) {
  if (img.channels() == 1) {
    double maxPot = 0.0;
    for (int i = 20; i <= 620; i++) {
      for (int j = 20; j <= 460; j++) {
        double pot = 0;
        for (int l = 0; l < number; l++) {
          pot += balls[l].getPot(i, j);
        }
        if (pot > maxPot) {
          maxPot = pot;
        }
      }
    }
    for (int i = 20; i <= 620; i++) {
      for (int j = 20; j <= 460; j++) {
        BW& bw = img.ptr<BW>(j)[i];
        double pot = 0;
        for (int l = 0; l < number; l++) {
          pot += balls[l].getPot(i, j);
        }
        bw.intensity = round(pot / maxPot * layers) / layers * 255;
      }
    }
  }
}

void Controller:: checkCollisions(void) {
  for (int i = 0; i < number; i++) {
    for (int j = i + 1; j < number; j++) {
      double dx = balls[i].rect.centerx - balls[j].rect.centerx;
      double dy = balls[i].rect.centery - balls[j].rect.centery;
      double distance = sqrt(dx * dx + dy * dy);
      if (distance < (balls[i].radius + balls[j].radius)) {
        double nx = -dx;
        double ny = -dy;
        double unx = nx / distance;
        double uny = ny / distance;
        double utx = -uny;
        double uty = unx;
        double v1n = unx * balls[i].vel[0] + uny * balls[i].vel[1];
        double v1t = utx * balls[i].vel[0] + uty * balls[i].vel[1];
        double v2n = unx * balls[j].vel[0] + uny * balls[j].vel[1];
        double v2t = utx * balls[j].vel[0] + uty * balls[j].vel[1];

        double v1tf = v1t;
        double v2tf = v2t;
        double v1nf = (v1n * (pow(balls[i].radius, 3) - pow(balls[j].radius, 3)) + (2 * pow(balls[j].radius, 3) * v2n)) / (pow(balls[i].radius, 3) + pow(balls[j].radius, 3));
        double v2nf = (v2n * (pow(balls[j].radius, 3) - pow(balls[i].radius, 3)) + (2 * pow(balls[i].radius, 3) * v1n)) / (pow(balls[i].radius, 3) + pow(balls[j].radius, 3));

        double v1nxf = v1nf * unx;
        double v1nyf = v1nf * uny;
        double v2nxf = v2nf * unx;
        double v2nyf = v2nf * uny;

        double v1txf = v1tf * utx;
        double v1tyf = v1tf * uty;
        double v2txf = v2tf * utx;
        double v2tyf = v2tf * uty;

        balls[i].vel[0] = v1nxf + v1txf;
        balls[i].vel[1] = v1nyf + v1tyf;
        balls[j].vel[0] = v2nxf + v2txf;
        balls[j].vel[1] = v2nyf + v2tyf;


        double extra = 1.01 * (balls[i].radius + balls[j].radius) - distance;
        double move1[2];
        double move2[2];
        move1[0] = -uny * extra;
        move1[1] = -unx * extra;
        balls[i].rect.move(move1);
        move2[0] = uny * extra;
        move2[1] = unx * extra;
        balls[j].rect.move(move2);
      }
    }
  }
}

void Controller::accelerate(int fps, double G) {
  for (int i = 0; i < number; i++) {
    for (int j = i + 1; j < number; j++) {
      double dx = balls[j].rect.centerx - balls[i].rect.centerx;
      double dy = balls[j].rect.centery - balls[i].rect.centery;
      double distance = sqrt(dx * dx + dy * dy);
      double force1 = G * pow(balls[j].radius, 3) / (dx * dx + dy * dy);
      double force2 = G * pow(balls[i].radius, 3) / (dx * dx + dy * dy);
      //std::cout << force1 << " " << force2 << std::endl;
      balls[i].vel[0] += force1 * (dy / distance) / fps ;
      balls[i].vel[1] += force1 * (dx / distance) / fps ;
      balls[j].vel[0] -= force2 * (dy / distance) / fps ;
      balls[j].vel[1] -= force2 * (dx / distance) / fps ;
    }
  }
}
