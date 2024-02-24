#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

extern "C" {
#include "../parser.h"
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  QString file;
  int old_x, old_y, old_z;

 private slots:
  void choisefile();

  void rgb_changed();
  void rgb_line_changed();
  void rgb_point_changed();

  void value_changed();
  void type_point();
  void line_type();

  void angle_changed_x();
  void angle_changed_y();
  void angle_changed_z();

  void on_pushButton_move_x_clicked();
  void on_pushButton_move_y_clicked();
  void on_pushButton_move_z_clicked();

  void size_change();
  void type_projection();

  void load_settings();
  void apply_settings();
  void save_settings();

  void make_screenshot();
  //    void make_gif();

 private:
  Ui::MainWindow *ui;
};
#endif  // MAINWINDOW_H
