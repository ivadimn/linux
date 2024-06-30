#ifndef _DICT_H_
#define _DICT_H_

#include <stddef.h>

#define CELL_CMP_EQ(a, b)  (strcmp((char*)(a), (char*)(b)) == 0)
typedef unsigned char alpha;

typedef struct cell_t {
    alpha *key;
    alpha* value;
} cell_t;

typedef struct dict_t {
    cell_t **data;          //данные
    size_t count;           //сколько уже заполнено
    size_t size;            //всего элементов массива
    size_t limit;           //если элементов больше чем лимит пересоздаём dict
    float factor;           //степень заполнения массива по
                            //достижении которого пересоздётся dict
    float mult;             //во столько раз увеличится размер dict
    
} dict_t;


dict_t* create_dict(size_t size, float factor, float mult);
void destroy_dict(dict_t* dict);

dict_t* recreate_dict(dict_t ** , cell_t*);
void put(dict_t**, alpha*, alpha*);
void* get(dict_t *dict, alpha* word);
void print_dict_st(dict_t*);
void print_dict(dict_t* );

unsigned long long hash_code(alpha* word);

#endif
