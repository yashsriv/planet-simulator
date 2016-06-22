class Rect {

  private:
    double _top;
    double _left;
    double _width;
    double _height;
    void calcValues();
  public:
    Rect();
    Rect(double, double, double, double);
    Rect copy();
    void move(double[2]);
    bool contains(Rect r);
    void setTop(double);
    void setBottom(double);
    void setLeft(double);
    void setRight(double);
    double top;
    double bottom;
    double left;
    double right;
    double centerx;
    double centery;

};
