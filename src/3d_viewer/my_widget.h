#ifndef MY_WIDGET_H
#define MY_WIDGET_H

#include <QOpenGLWidget>

#include "mainwindow.h"

class My_Widget : public QOpenGLWidget {
  Q_OBJECT

 public:
  My_Widget(QWidget *parent = nullptr);
  ~My_Widget();
  void initializeGL() override;
  void paintGL() override;
  obj3d model;
  void draw();
  int flag;
  float r, g, b;
  float r_line, g_line, b_line;
  float r_point, g_point, b_point;
  float line_width, point_size;
  int line_flag, point_flag;
  int projection_flag;
};

#endif  // MY_WIDGET_H
