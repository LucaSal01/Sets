#include <stdio.h>
#include <stdlib.h>
#define Node struct set_node
#define Mall(size) malloc(sizeof(size))
struct set_node
{
    const char *key;
    size_t key_len;
    struct set_node *next;
};
struct set_table
{
    struct set_node **nodes;
    size_t hashmap_size;
};
size_t djb33x_hash(const char *key, const size_t keylen)
{
    size_t hash = 5381;
    for (size_t i = 0; i < keylen; i++)
    {
        hash = ((hash << 5) + hash) ^ key[i];
    }
    return hash;
}
size_t get_index(struct set_table *table, const char *key, const size_t key_len)
{
    size_t hash = djb33x_hash(key, key_len);
    return hash % table->hashmap_size;
}
struct set_table *set_table_new(const size_t hashmap_size)
{
    struct set_table *table = malloc(sizeof(struct set_table));
    if (!table)
    {
        return NULL;
    }
    table->hashmap_size = hashmap_size;
    table->nodes = calloc(table->hashmap_size, sizeof(struct set_node *));
    if (!table->nodes)
    {
        free(table);
        return NULL;
    }
    return table;
}
struct set_node *set_insert(struct set_table *table, const char *key, const size_t key_len)
{
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    struct set_node *head = table->nodes[index];
    if (!head)
    {
        table->nodes[index] = malloc(sizeof(struct set_node));
        if (!table->nodes[index])
        {
            return NULL;
        }
        table->nodes[index]->key = key;
        table->nodes[index]->key_len = key_len;
        table->nodes[index]->next = NULL;
        return table->nodes[index];
    }
    struct set_node *new_item = malloc(sizeof(struct set_node));
    if (!new_item)
    {
        return NULL;
    }
    new_item->key = key;
    new_item->key_len = key_len;
    new_item->next = NULL;
    struct set_node *tail = head;
    while (head)
    {
        tail = head;
        head = head->next;
    }
    tail->next = new_item;
    return new_item;
}
struct set_node *set_search_key(struct set_table *table, const char *key, const size_t key_len)
{
    struct set_node *head = table->nodes[get_index(table, key, key_len)];
    if (!head)
        return NULL;
    while (head->key != key)
    {
        printf("%s", head->key);
        if (!head->next)
            return NULL; // end of cycle
        head = head->next;
    }
    printf("%s", head->key);
    return head;
}
int set_Remove(struct set_table *table, const char *key, const size_t key_len)
{
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    struct set_node *head_node = table->nodes[index];
    struct set_node *previous_node;

    while (head_node)
    {
        if (head_node->key == key)
        {
            if (!head_node->next)
            {
                printf("I've removed the last Key \"%s\" at index [%llu]\n", head_node->key, index);
                previous_node->next = head_node->next;
                free(head_node);
                return 0;
            }
            else if (!previous_node)
            {
                printf("I've removed the first Key \"%s\" at index [%llu]\n", head_node->key, index);
                previous_node = head_node->next;
                free(table->nodes[index]);
                table->nodes[index] = NULL;
                table->nodes[index] = previous_node;
                return 0;
            }
            else
            {
                printf("I've removed a middle Key \"%s\" at index [%llu]\n", head_node->key, index);
                previous_node->next = head_node->next;
                free(head_node);
                head_node = NULL;
                return 0;
            }
        }
        previous_node = head_node;
        head_node = head_node->next;
    }
    printf("No Key corresponded found\n");
    return -1;
}
int main()
{
    const int hashmap_size = 10;
    struct set_table *table = set_table_new(hashmap_size);
    set_insert(table, "Yoshi", hashmap_size);
    set_insert(table, "Mario", hashmap_size);
    set_insert(table, "Luigi", hashmap_size);
    set_insert(table, "Bowser", hashmap_size);
    set_insert(table, "Peach", hashmap_size);
    struct set_node **current_node = table->nodes;
    size_t index = 0;
    struct set_node *head_node;
    for (index = 0; index < table->hashmap_size; index++)
    {
        if ((head_node = current_node[index]))
        {
            printf("Index [%llu]:\n", index);
            struct set_node *curr_node = head_node;
            while (curr_node)
            {
                printf("\t%s\n", curr_node->key);
                curr_node = curr_node->next;
            }
        }
    }
    printf(" value is: %s\n", set_search_key(table, "Yoshi", hashmap_size)->key);
    set_Remove(table,"Peach",hashmap_size);
    return 0;
}