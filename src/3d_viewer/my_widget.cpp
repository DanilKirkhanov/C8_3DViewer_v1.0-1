#include "my_widget.h"

My_Widget::My_Widget(QWidget *parent) : QOpenGLWidget(parent) {}

void My_Widget::initializeGL() { glEnable(GL_DEPTH_TEST); }

void My_Widget::paintGL() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (projection_flag) {
    float fov = model.width / (2 * tan((60 * M_PI / 180) / 2));
    glFrustum(-model.width, model.width, -model.width, model.width, fov,
              100000);
    glTranslated(0, 0, -model.width * 3);
  } else
    glOrtho(-model.width, model.width, -model.width, model.width, -model.width,
            model.width);

  glPointSize(point_size);
  glLineWidth(line_width);
  glClearColor(r, g, b, 0);
  glLineStipple(3, 666);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (!flag) draw();
}

void My_Widget::draw() {
  if (point_flag) {
    if (point_flag == 2) glEnable(GL_POINT_SMOOTH);
    glColor3d(r_point, g_point, b_point);
    for (int i = 0; i < model.vtx_count; i++) {
      glBegin(GL_POINTS);
      glVertex3f(model.vet_arr[i * 3], model.vet_arr[i * 3 + 1],
                 model.vet_arr[i * 3 + 2]);

      glEnd();
    }
    if (point_flag == 2) glDisable(GL_POINT_SMOOTH);
  }

  if (line_flag) glEnable(GL_LINE_STIPPLE);
  glColor3d(r_line, g_line, b_line);
  for (int j = 0; j < model.fct_count; j++) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < model.fct_vtx[j]; i++) {
      glVertex3f(model.vet_arr[(model.polygons[j].vertexes[i] - 1) * 3],
                 model.vet_arr[(model.polygons[j].vertexes[i] - 1) * 3 + 1],
                 model.vet_arr[(model.polygons[j].vertexes[i] - 1) * 3 + 2]);
    }
    glEnd();
  }
  if (line_flag) glDisable(GL_LINE_STIPPLE);
}

My_Widget::~My_Widget() {}
