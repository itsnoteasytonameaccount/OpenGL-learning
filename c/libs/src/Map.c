#include <Map.h>
int hash(Map map, char *str)
{
    int i = 0;
    char *ch = str;
    do
    {
        i += *ch;
    } while (*++ch);
    return i % map->len;
}
int put(Map map, char *key, char *var)
{
    int index = hash(map, key);
    MapItem list = map->data + index, item = list;
    while (item->next != NULL && strcmp(key, item->next->key))
    {
        // printf("compare!\n");
        item = item->next;
    }
    // printf("success!\n");
    if (item->next == NULL)
    {
        // printf("create key:%s val:%s!\n", key, var);
        item->next = (MapItem)malloc(sizeof(MI));
        item->next->key = (char *)malloc(strlen(key) + 1);
        item->next->next = NULL;
        copy_str_until(item->next->key, key, 0, 0);
        // printf("created!\n");
    }
    else
    {
        // printf("set before key:%s val:%s!\n", item->next->key, item->next->value);
        free(item->next->value);
        // printf("setted!\n");
    }
    item->next->value = (char *)malloc(strlen(var) + 1);
    copy_str_until(item->next->value, var, 0, 0);
    return 0;
}
MapItem get(Map map, char *key)
{
    int index = hash(map, key);
    MapItem list = map->data + index, item = list;
    while (item->next != NULL && strcmp(key, item->next->key))
    {
        item = item->next;
    }
    return item->next;
}
Map initMap(int len)
{
    int i, j;
    Map map = (Map)malloc(sizeof(M));
    map->data = (MapItem)malloc(sizeof(MI) * len);
    map->len = len;
    map->get = get;
    map->put = put;
    for (i = 0; i < len; i++)
    {
        map->data[i].next = NULL;
    }
    return map;
}
int freeMap(Map map)
{
    int i;
    MapItem mi = NULL;
    for (i = 0; i < map->len; i++)
    {
        while (map->data[i].next != NULL)
        {
            mi = map->data[i].next;
            map->data[i].next = map->data[i].next->next;
            free(mi->key);
            free(mi->value);
            free(mi);
        }
    }
    free(map->data);
    free(map);
    return 0;
}

/* int main()
{
    Map map = initMap(40);
    printf("ceshi\n");
    map->put(map, "ceshi", "ceshishuju1");
    map->put(map, "ceshi", "ceshishuju2");
    map->put(map, "ceshi", "ceshishuju3");
    map->put(map, "ceshi", "ceshishuju4");
    map->put(map, "ceshi", "ceshishuju5");
    map->put(map, "ceshi2", "ceshishuju6");
    printf("ceshi,%s,%s\n", map->get(map, "ceshi")->value, map->get(map, "ceshi2")->value);
    freeMap(map);
    return 0;
} */