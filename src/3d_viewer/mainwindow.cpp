#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  ui->openGLWidget->flag = 1;
  ui->openGLWidget->point_size = 1;
  ui->openGLWidget->line_width = 1;
  ui->openGLWidget->r_line = 0;
  ui->openGLWidget->g_line = 0;
  ui->openGLWidget->b_line = 0;
  ui->openGLWidget->r = 0;
  ui->openGLWidget->g = 0;
  ui->openGLWidget->b = 0;
  ui->openGLWidget->r_point = 0;
  ui->openGLWidget->g_point = 0;
  ui->openGLWidget->b_point = 0;
  ui->openGLWidget->projection_flag = 0;

  load_settings();
  apply_settings();

  old_x = 0;
  old_y = 0;
  old_z = 0;

  connect(ui->pushButton_choiseFile, SIGNAL(clicked()), this,
          SLOT(choisefile()));
  connect(ui->fon_r, SIGNAL(valueChanged(int)), this, SLOT(rgb_changed()));
  connect(ui->fon_g, SIGNAL(valueChanged(int)), this, SLOT(rgb_changed()));
  connect(ui->fon_b, SIGNAL(valueChanged(int)), this, SLOT(rgb_changed()));

  connect(ui->line_r, SIGNAL(valueChanged(int)), this,
          SLOT(rgb_line_changed()));
  connect(ui->line_g, SIGNAL(valueChanged(int)), this,
          SLOT(rgb_line_changed()));
  connect(ui->line_b, SIGNAL(valueChanged(int)), this,
          SLOT(rgb_line_changed()));

  connect(ui->point_r, SIGNAL(valueChanged(int)), this,
          SLOT(rgb_point_changed()));
  connect(ui->point_g, SIGNAL(valueChanged(int)), this,
          SLOT(rgb_point_changed()));
  connect(ui->point_b, SIGNAL(valueChanged(int)), this,
          SLOT(rgb_point_changed()));

  connect(ui->checkBox, SIGNAL(clicked()), this, SLOT(line_type()));
  connect(ui->line_width, SIGNAL(valueChanged(double)), this,
          SLOT(value_changed()));
  connect(ui->point_size, SIGNAL(valueChanged(double)), this,
          SLOT(value_changed()));

  connect(ui->horizontalSlider_x, SIGNAL(valueChanged(int)), this,
          SLOT(angle_changed_x()));
  connect(ui->horizontalSlider_y, SIGNAL(valueChanged(int)), this,
          SLOT(angle_changed_y()));
  connect(ui->horizontalSlider_z, SIGNAL(valueChanged(int)), this,
          SLOT(angle_changed_z()));
  connect(ui->change_size, SIGNAL(clicked()), this, SLOT(size_change()));

  connect(ui->radioButton, SIGNAL(clicked()), this, SLOT(type_point()));
  connect(ui->radioButton_2, SIGNAL(clicked()), this, SLOT(type_point()));
  connect(ui->radioButton_3, SIGNAL(clicked()), this, SLOT(type_point()));

  connect(ui->radioButton_4, SIGNAL(clicked()), this, SLOT(type_projection()));
  connect(ui->radioButton_5, SIGNAL(clicked()), this, SLOT(type_projection()));

  connect(ui->make_screenshot, SIGNAL(clicked()), this,
          SLOT(make_screenshot()));
}

MainWindow::~MainWindow() {
  if (!ui->openGLWidget->flag) {
    free_obj3d(&ui->openGLWidget->model);
  }
  save_settings();
  delete ui;
}

void MainWindow::choisefile() {
  if (!ui->openGLWidget->flag) {
    free_obj3d(&ui->openGLWidget->model);
    ui->openGLWidget->flag = 1;
  }
  file = QFileDialog::getOpenFileName(this, tr("Open file"), ".",
                                      tr("Object files (*.obj)"));
  FILE *f;
  std::string str = file.toStdString();
  char *c_str = &str[0];
  ui->openGLWidget->flag = open_obj_file(c_str, &f);
  if (!ui->openGLWidget->flag) read_obj(f, &ui->openGLWidget->model);
  calc_centers(&ui->openGLWidget->model);
  fclose(f);
}

void MainWindow::rgb_changed() {
  double temp = ui->fon_r->value();
  ui->openGLWidget->r = temp / 100;
  temp = ui->fon_g->value();
  ui->openGLWidget->g = temp / 100;
  temp = ui->fon_b->value();
  ui->openGLWidget->b = temp / 100;
  ui->openGLWidget->update();
}

void MainWindow::rgb_line_changed() {
  double temp = ui->line_r->value();
  ui->openGLWidget->r_line = temp / 100;
  temp = ui->line_g->value();
  ui->openGLWidget->g_line = temp / 100;
  temp = ui->line_b->value();
  ui->openGLWidget->b_line = temp / 100;
  ui->openGLWidget->update();
}

void MainWindow::rgb_point_changed() {
  double temp = ui->point_r->value();
  ui->openGLWidget->r_point = temp / 100;
  temp = ui->point_g->value();
  ui->openGLWidget->g_point = temp / 100;
  temp = ui->point_b->value();
  ui->openGLWidget->b_point = temp / 100;
  ui->openGLWidget->update();
}

void MainWindow::angle_changed_x() {
  double angle = ui->horizontalSlider_x->value() - old_x;
  old_x = ui->horizontalSlider_x->value();
  double rad = angle * M_PI / 180.0;
  rotate_x(rad, &ui->openGLWidget->model);
  ui->openGLWidget->update();
}

void MainWindow::angle_changed_y() {
  double angle = ui->horizontalSlider_y->value() - old_y;
  old_y = ui->horizontalSlider_y->value();
  double rad = angle * M_PI / 180.0;
  rotate_y(rad, &ui->openGLWidget->model);
  ui->openGLWidget->update();
}

void MainWindow::angle_changed_z() {
  double angle = ui->horizontalSlider_z->value() - old_z;
  old_z = ui->horizontalSlider_z->value();
  double rad = angle * M_PI / 180.0;
  rotate_z(rad, &ui->openGLWidget->model);
  ui->openGLWidget->update();
}

void MainWindow::size_change() {
  if (!ui->openGLWidget->flag) {
    scale(ui->doubleSpinBox_size->value(), &ui->openGLWidget->model);
    ui->openGLWidget->update();
  }
}

void MainWindow::on_pushButton_move_x_clicked() {
  if (!ui->openGLWidget->flag) {
    move_x(ui->doubleSpinBox->value(), &ui->openGLWidget->model);
    ui->openGLWidget->update();
  }
}

void MainWindow::on_pushButton_move_y_clicked() {
  if (!ui->openGLWidget->flag) {
    move_y(ui->doubleSpinBox_2->value(), &ui->openGLWidget->model);
    ui->openGLWidget->update();
  }
}

void MainWindow::on_pushButton_move_z_clicked() {
  if (!ui->openGLWidget->flag) {
    move_z(ui->doubleSpinBox_3->value(), &ui->openGLWidget->model);
    ui->openGLWidget->update();
  }
}

void MainWindow::value_changed() {
  ui->openGLWidget->line_width = ui->line_width->value();
  ui->openGLWidget->point_size = ui->point_size->value();
  ui->openGLWidget->update();
}

void MainWindow::line_type() {
  ui->openGLWidget->line_flag = ui->checkBox->checkState();
  ui->openGLWidget->update();
}

void MainWindow::type_point() {
  if (ui->radioButton_3->isChecked()) ui->openGLWidget->point_flag = 0;
  if (ui->radioButton_2->isChecked()) ui->openGLWidget->point_flag = 1;
  if (ui->radioButton->isChecked()) ui->openGLWidget->point_flag = 2;
  ui->openGLWidget->update();
}

void MainWindow::type_projection() {
  if (ui->radioButton_4->isChecked()) ui->openGLWidget->projection_flag = 0;
  if (ui->radioButton_5->isChecked()) ui->openGLWidget->projection_flag = 1;
  ui->openGLWidget->update();
}

void MainWindow::load_settings() {
  QSettings settings("3d_settings", "set");
  ui->fon_r->setValue(settings.value("r_background", "0").toInt());
  ui->fon_g->setValue(settings.value("g_background", "0").toInt());
  ui->fon_b->setValue(settings.value("b_background", "0").toInt());

  ui->line_r->setValue(settings.value("r_line", "0").toInt());
  ui->line_g->setValue(settings.value("g_line", "100").toInt());
  ui->line_b->setValue(settings.value("b_line", "0").toInt());

  ui->point_r->setValue(settings.value("r_point", "100").toInt());
  ui->point_g->setValue(settings.value("g_point", "0").toInt());
  ui->point_b->setValue(settings.value("b_point", "0").toInt());

  ui->checkBox->setChecked(settings.value("line_type", "0").toBool());
  ui->radioButton->setChecked(settings.value("rb1", "1").toBool());
  ui->radioButton_2->setChecked(settings.value("rb2", "0").toBool());
  ui->radioButton_3->setChecked(settings.value("rb3", "0").toBool());

  ui->line_width->setValue(settings.value("line_width", "1").toDouble());
  ui->point_size->setValue(settings.value("point_size", "5").toDouble());

  ui->radioButton_4->setChecked(settings.value("ortho", "1").toBool());
  ui->radioButton_5->setChecked(settings.value("frustum", "0").toBool());
}

void MainWindow::apply_settings() {
  rgb_point_changed();
  rgb_line_changed();
  rgb_changed();
  line_type();
  type_point();
  value_changed();
  type_projection();
}

void MainWindow::save_settings() {
  QSettings settings("3d_settings", "set");
  settings.setValue("r_background", ui->fon_r->value());
  settings.setValue("b_background", ui->fon_b->value());
  settings.setValue("g_background", ui->fon_g->value());

  settings.setValue("r_line", ui->line_r->value());
  settings.setValue("b_line", ui->line_b->value());
  settings.setValue("g_line", ui->line_g->value());

  settings.setValue("r_point", ui->point_r->value());
  settings.setValue("b_point", ui->point_b->value());
  settings.setValue("g_point", ui->point_g->value());

  settings.setValue("line_type", ui->checkBox->checkState());
  settings.setValue("rb1", ui->radioButton->isChecked());
  settings.setValue("rb2", ui->radioButton_2->isChecked());
  settings.setValue("rb3", ui->radioButton_3->isChecked());

  settings.setValue("line_width", ui->line_width->value());
  settings.setValue("point_size", ui->point_size->value());

  settings.setValue("ortho", ui->radioButton_4->isChecked());
  settings.setValue("frustum", ui->radioButton_5->isChecked());
}

void MainWindow::make_screenshot() {
  QImage screen = ui->openGLWidget->grabFramebuffer();
  QString path = QFileDialog::getSaveFileName(this, tr("Save screenshot"),
                                              "/Users/", "Image (*.png)");
  screen.save(path, "PNG");
}
