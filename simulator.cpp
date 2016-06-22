#include <iostream>
#include <unistd.h>

#include <opencv2/opencv.hpp>

#include "controller.h"

using namespace cv;

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void showHelpInfo(char *s);
int main(int argc, char ** argv) {
  // true for planets and false for potential
  bool mode = false;
  double gravity = 0;
  int layers = 10;
  int fps = 30;
  int number = 2;
  // 0 - no debug
  // 1 - some debug
  // 2 - go mad
  int debug = 0;

  if (argc == 1) {
    showHelpInfo(argv[0]);
    std::cout << "\n Now running with default settings" << std::endl;
  }
  char tmp;
  while((tmp=getopt(argc,argv,"hn:m:g::l:f:dx"))!=-1)
  {
    switch(tmp)
    {
      /*option h show the help infomation*/
      case 'h':
        showHelpInfo(argv[0]);
        return 0;
        break;
      /*option n set number of balls*/
      case 'n':
        number = atoi(optarg);
        break;
      /*option m set mode*/
      case 'm':
        if (strcmp(optarg, "potential") == 0) {
          mode = false;
        } else if (strcmp(optarg, "planets") == 0) {
          mode = true;
        } else {
          std::cout << ANSI_COLOR_RED << "Invalid argument: " << optarg << " for option -m " << ANSI_COLOR_RESET << std::endl;
          showHelpInfo(argv[0]);
          return -1;
        }
        break;
      /*option g set gravity to on*/
      case 'g':
        gravity = 1.0;
        if (optarg != NULL) {
          gravity = atof(optarg);
        }
        break;
      /*option l set layers*/
      case 'l':
        layers = atoi(optarg);
        break;
      /*option f set fps*/
      case 'f':
        fps = atoi(optarg);
        break;
      /*option d set debug mode*/
      case 'd':
        debug = 1;
        break;
      /*option x set extreme debug*/
      case 'x':
        debug = 2;
        break;
      case '?':
        showHelpInfo(argv[0]);
        return -1;
        break;
    }
  }
  if(debug) {
    std::cout << "Number of Balls: " << number << std::endl;
    std::cout << "Mode           : ";
    if (mode)
      std::cout << "Planets" << std::endl;
    else
      std::cout << "Potential" << std::endl;
    std::cout << "Gravity-Factor : " << gravity << std::endl;
    std::cout << "Layers         : " << layers << std::endl;
    std::cout << "Accuracy(FPS)  : " << fps << std::endl;
    std::cout << "Debug Level    : " << debug << std::endl;
  }

  Mat display;
  namedWindow("floatme", WINDOW_AUTOSIZE);
  Controller cc(number, debug);
  if (debug) {
    std::cout << "Controller Started" << std::endl;
  }

  while (true) {

    // Planet modes
    if (mode)
      display = Mat(480, 640, CV_8UC3, Scalar(255, 255, 255));
    else
      display = Mat(480, 640, CV_8UC1, Scalar(0));

    // Display outlying border
    rectangle(display, Point(20, 20), Point(620, 460), Scalar::all(0), 1, 8);

    // Update full game state
    cc.update(fps, gravity);

    // Draw new state
    cc.draw(display, layers);

    // Some blur then display
    GaussianBlur( display, display, Size( 3, 3 ), 0, 0 );
    imshow("floatme", display);

    // FPS = 60
    int c = waitKey(1000 / 60);
    if (c == 'q') {
      break;
    }
  }
}

void showHelpInfo(char *s) {
  std::cout << "Usage:   " << s << " [-option] [argument]" << std::endl;
  std::cout << "options: " << "-h                      show help information(this output)" << std::endl;
  std::cout << "         " << "-n number               number of planets" <<std::endl;
  std::cout << "         " << "-m (planet | potential) draw either planets or potential." <<std::endl;
  std::cout << "         " << "-g[gravity-factor=1]    enable gravity. You can set the factor too(optional) like -g1.2 - no spaces." << std::endl;
  std::cout << "         " << "-l layers               number of intensity layers. Not yet implemented" << std::endl;
  std::cout << "         " << "-f fps                  set fps. This is not actual fps but rather the supposed fps by the program." << std::endl;
  std::cout << "         " << "                          For example: If fps=1000 output is at 60fps assuming that output is 1000fps." << std::endl;
  std::cout << "         " << "                          Can be used for better calculations" << std::endl;
  std::cout << "         " << "-d                      debug mode - lots of output" << std::endl;
  std::cout << "         " << "-x                      extreme mode - continuous output" << std::endl;
  std::cout << std::endl;
  std::cout << "example: " << s << " -n 2 -d potential -g0 -l 10 -f 30" << std::endl;
  std::cout << "         " << " The above example is the default incase any one of the options are not specified" << std::endl;
}
