#include "./parser.h"

int open_obj_file(char *f_name, FILE **f) {
  *f = fopen(f_name, "r");
  if (!*f)
    fprintf(stderr, "s21_3dviewer: %s: файл не найден\n", f_name);
  else
    printf("Файл: %s открыт, указатель: %p\n", f_name, (void *)(*f));
  return (*f) ? SUCCESS : FAIL;
}

void initObj3d(obj3d *res) {
  // Количество вершин (vertex count)
  res->vtx_count = 0;
  // Количество граней (facet count)
  res->fct_count = 0;
  // Общее количество индексов
  res->total_indices = 0;
  // Границы
  res->bounds.x_max = 0;
  res->bounds.x_min = 0;
  res->bounds.y_max = 0;
  res->bounds.y_min = 0;
  res->bounds.z_max = 0;
  res->bounds.z_min = 0;
  // Указатели на массив вершин, массив индексов граней и структуру с данными о
  // гранях
  res->vet_arr = NULL;
  res->fct_vtx = NULL;
  res->polygons = NULL;
}

int read_obj(FILE *f, obj3d *res) {
  initObj3d(res);
  int status = FAIL;
  char buffer[BUFF_SIZE] = {0};
  while (fgets(buffer, BUFF_SIZE, f))
    if (buffer[0] == 'v' && buffer[1] == ' ')
      ++res->vtx_count;  // считаем вершины
    else if (buffer[0] == 'f' && buffer[1] == ' ')
      ++res->fct_count;  // считаем грани
  if (res->fct_count && res->vtx_count) status = s21_alloc_mem(res);
  if (!status) {
    status = fill_obj3d(f, res);
  }
  return (!status) ? SUCCESS : FAIL;
}

int fill_obj3d(FILE *f, obj3d *res) {
  char buffer[BUFF_SIZE] = {0};
  fseek(f, 0, SEEK_SET);
  unsigned int i = 0, j = 0,
               status = FAIL;  // Индексы, статус выполнения операции
  double *arr = res->vet_arr;  // Указатель на массив вершин в структуре obj3d
  while (fgets(buffer, BUFF_SIZE, f)) {
    // Если строка содержит вершину (начинается с 'v' и пробела)
    if (buffer[0] == 'v' && buffer[1] == ' ') {
      // Считываем координаты вершины и записываем их в массив вершин
      sscanf(buffer + 2, "%lf%lf%lf", &arr[i], &arr[i + 1], &arr[i + 2]);
      max_min(res, i);
      // Вызываем функцию для обновления границ модели (min и max)
      i += 3;  // Увеличиваем индекс для следующей вершины
    }
    // Если строка содержит грань (начинается с 'f' и пробела)
    else if (buffer[0] == 'f' && buffer[1] == ' ') {
      // Вызываем функцию для заполнения данных о грани в структуре obj3d
      status = fill_facets(buffer, res, j++);
    }
  }
  return (!status) ? SUCCESS : FAIL;
}

void max_min(obj3d *res, int i) {
  if (i == 0) {
    res->bounds.x_max = res->vet_arr[i];
    res->bounds.y_max = res->vet_arr[i + 1];
    res->bounds.z_max = res->vet_arr[i + 2];
    res->bounds.x_min = res->vet_arr[i];
    res->bounds.y_min = res->vet_arr[i + 1];
    res->bounds.z_min = res->vet_arr[i + 2];
  }
  if (res->vet_arr[i] > res->bounds.x_max) res->bounds.x_max = res->vet_arr[i];
  if (res->vet_arr[i + 1] > res->bounds.y_max)
    res->bounds.y_max = res->vet_arr[i + 1];
  if (res->vet_arr[i + 2] > res->bounds.z_max)
    res->bounds.z_max = res->vet_arr[i + 2];

  if (res->vet_arr[i] < res->bounds.x_max) res->bounds.x_min = res->vet_arr[i];
  if (res->vet_arr[i + 1] < res->bounds.y_max)
    res->bounds.y_min = res->vet_arr[i + 1];
  if (res->vet_arr[i + 2] < res->bounds.z_max)
    res->bounds.z_min = res->vet_arr[i + 2];
}

int fill_facets(char *buffer, obj3d *obj, unsigned int index) {
  unsigned int count =
      count_vtx_of_facet(buffer);  // Вычисляем количество вершин в грани
  obj->total_indices += count;  // Обновляем общее количество индексов
  obj->fct_vtx[index] =
      count;  // Записываем количество вершин для текущей грани
  // Выделяем память под массив вершин для текущей грани
  if (count > 0) {
    obj->polygons[index].vertexes =
        (unsigned int *)calloc(count, sizeof(unsigned int));
  }
  // Если выделение памяти прошло успешно, вызываем функцию для заполнения
  // массива вершин
  if (obj->polygons[index].vertexes) {
    parse_vertices(buffer, index, obj);
  }
  return (obj->polygons[index].vertexes) ? SUCCESS : FAIL;
}

unsigned int count_vtx_of_facet(char *buffer) {
  char str[255] = {0};  // Буфер для временного хранения слова
  unsigned int count = 0;  // Счетчик вершин
  buffer += 2; /* skip first f_ */  // Пропускаем первые два символа 'f_'
  // Пока не достигнут конец строки и удается считать слово
  while (*buffer && sscanf(buffer, "%s", str)) {
    unsigned int length = 0;
    length = strlen(str) + 1;
    buffer += length;
    if (length > 1) {
      ++count;
    }
    strcpy(str, "");
  }
  return count;
}

unsigned int parse_vertices(char *buffer, unsigned int index, obj3d *obj) {
  char str[255] = {0};  // Буфер для временного хранения слова
  unsigned int count = 0;  // Счетчик вершин
  buffer += 2;
  while (*buffer) {
    unsigned int length = 0;
    sscanf(buffer, "%s", str);
    length = strlen(str) + 1;
    buffer += length;
    // Если длина слова больше 1 (не пробел), увеличиваем счетчик вершин
    if (length > 1) {
      sscanf(str, "%u", &obj->polygons[index].vertexes[count]);
      ++count;
    }
    strcpy(str, "");
  }
  return count;
}

int s21_alloc_mem(obj3d *res) {
  res->vet_arr = (double *)calloc(res->vtx_count * DIMEN, sizeof(double));
  if (res->vet_arr)
    res->fct_vtx = (unsigned int *)calloc(res->fct_count, sizeof(unsigned int));
  if (res->fct_vtx)
    res->polygons = (polygon_t *)calloc(res->fct_count, sizeof(polygon_t));
  int status = (res->polygons) ? SUCCESS : FAIL;
  if (status) free_obj3d(res);
  return status;
}

void free_obj3d(obj3d *res) {
  if (res->polygons)
    for (unsigned int i = 0; i < res->fct_count; ++i)
      if (res->polygons[i].vertexes) free(res->polygons[i].vertexes);
  if (res->vet_arr) {
    free(res->vet_arr);
    res->vet_arr = NULL;
  }
  if (res->fct_vtx) {
    free(res->fct_vtx);
    res->fct_vtx = NULL;
  }
  if (res->polygons) {
    free(res->polygons);
    res->polygons = NULL;
  }
}

void scale(double a, obj3d *res) {
  for (unsigned int i = 0; i < res->vtx_count; i++) {
    res->vet_arr[i * 3] *= a;
    res->vet_arr[i * 3 + 1] *= a;
    res->vet_arr[i * 3 + 2] *= a;
  }
}

void rotate_x(double a, obj3d *res) {
  for (unsigned int i = 0; i < res->vtx_count; i++) {
    double y = res->vet_arr[i * 3 + 1];
    double z = res->vet_arr[i * 3 + 2];

    res->vet_arr[i * 3 + 1] = cos(a) * y - sin(a) * z;
    res->vet_arr[i * 3 + 2] = sin(a) * y + cos(a) * z;
  }
}

void rotate_y(double a, obj3d *res) {
  for (unsigned int i = 0; i < res->vtx_count; i++) {
    double x = res->vet_arr[i * 3];
    double z = res->vet_arr[i * 3 + 2];

    res->vet_arr[i * 3] = cos(a) * x + sin(a) * z;
    res->vet_arr[i * 3 + 2] = -sin(a) * x + cos(a) * z;
  }
}

void rotate_z(double a, obj3d *res) {
  for (unsigned int i = 0; i < res->vtx_count; i++) {
    double x = res->vet_arr[i * 3];
    double y = res->vet_arr[i * 3 + 1];

    res->vet_arr[i * 3] = cos(a) * x - sin(a) * y;
    res->vet_arr[i * 3 + 1] = sin(a) * x + cos(a) * y;
  }
}

void move_x(double a, obj3d *res) {
  for (unsigned int i = 0; i < res->vtx_count; i++) res->vet_arr[i * 3] += a;
}

void move_y(double a, obj3d *res) {
  for (unsigned int i = 0; i < res->vtx_count; i++)
    res->vet_arr[i * 3 + 1] += a;
}

void move_z(double a, obj3d *res) {
  for (unsigned int i = 0; i < res->vtx_count; i++)
    res->vet_arr[i * 3 + 2] += a;
}

void calc_centers(obj3d *res) {
  double x_width = (res->bounds.x_max + res->bounds.x_min);
  double y_width = (res->bounds.y_max + res->bounds.y_min);
  double z_width = (res->bounds.z_max + res->bounds.z_min);
  for (unsigned int i = 0; i < res->vtx_count; i++) {
    res->vet_arr[i * 3] = res->vet_arr[i * 3] - x_width / 2;
    res->vet_arr[i * 3 + 1] = res->vet_arr[i * 3 + 1] - y_width / 2;
    res->vet_arr[i * 3 + 2] = res->vet_arr[i * 3 + 2] - z_width / 2;
  }
  if (x_width >= y_width && x_width >= z_width) res->width = x_width;
  if (y_width >= x_width && y_width >= z_width) res->width = y_width;
  if (z_width >= y_width && z_width >= x_width) res->width = z_width;

  res->width *= 1.5;
}
