#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE (16 * 4096)
#define DIMEN 3

enum st {
  SUCCESS = 0,
  FAIL,
};

// структура грани
typedef struct {
  unsigned int *vertexes;  // массив вершин, необходимых для соединения
} polygon_t;

// границы обьекта
typedef struct {
  double x_min;
  double x_max;
  double y_min;
  double y_max;
  double z_min;
  double z_max;
} axis_t;

// основная структура обьекта
typedef struct {
  unsigned int vtx_count;  // количество вершин
  unsigned int fct_count;  // количество граней
  unsigned int *fct_vtx;  // связанные вершины для полигона
  unsigned int total_indices;  // общее количество индексов всех граней
  polygon_t *polygons;  // массив граней
  double *vet_arr;      // массив вершин x1,y1,z1,x2..
  double width;
  axis_t bounds;
} obj3d;

int open_obj_file(char *f_name, FILE **f);
void initObj3d(obj3d *res);
int read_obj(FILE *f, obj3d *res);
int fill_obj3d(FILE *f, obj3d *res);
int fill_facets(char *buffer, obj3d *obj, unsigned int index);
unsigned int count_vtx_of_facet(char *buffer);
unsigned int parse_vertices(char *buffer, unsigned int index, obj3d *obj);
int s21_alloc_mem(obj3d *res);
void free_obj3d(obj3d *res);
void scale(double a, obj3d *res);
void rotate_x(double a, obj3d *res);
void rotate_y(double a, obj3d *res);
void rotate_z(double a, obj3d *res);
void move_x(double a, obj3d *res);
void move_y(double a, obj3d *res);
void move_z(double a, obj3d *res);
void max_min(obj3d *res, int i);
void calc_centers(obj3d *res);