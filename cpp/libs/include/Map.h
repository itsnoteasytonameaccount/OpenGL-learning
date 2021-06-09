
#include <stdlib.h>
#include <stdio.h>
#include <str_utils.h>

typedef struct MAP_ITEM *MapItem, MI;
typedef struct MAP *Map, M;
struct MAP_ITEM
{
    /* data */
    char *key;
    char *value;
    MapItem next;
};

struct MAP
{
    /* data */
    int len;
    MapItem (*get)(Map map, char *key);
    int (*put)(Map map, char *key, char *val);
    MapItem data;
};

Map initMap(int len);
int freeMap(Map map);