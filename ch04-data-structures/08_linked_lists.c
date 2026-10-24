/*
 * Chapter 4.8 - Linked lists: a chain of nodes, each pointing to the next
 * A train: each carriage is coupled to the next one.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char         song[40];
    struct Node *next;          /* the coupling to the next carriage */
} Node;

/* add to the front: O(1), no shifting */
Node *push_front(Node *head, const char *song)
{
    Node *n = malloc(sizeof *n);
    if (n == NULL)
        return head;
    snprintf(n->song, sizeof n->song, "%s", song);
    n->next = head;
    return n;                   /* the new node is the new head */
}

/* add to the end: walk to the last node */
Node *push_back(Node *head, const char *song)
{
    Node *n = malloc(sizeof *n);
    if (n == NULL)
        return head;
    snprintf(n->song, sizeof n->song, "%s", song);
    n->next = NULL;
    if (head == NULL)
        return n;
    Node *cur = head;
    while (cur->next != NULL)
        cur = cur->next;
    cur->next = n;
    return head;
}

/* remove the first node whose song matches */
Node *remove_song(Node *head, const char *song)
{
    Node **link = &head;        /* points at the pointer we may need to change */
    while (*link != NULL) {
        if (strcmp((*link)->song, song) == 0) {
            Node *gone = *link;
            *link = gone->next; /* unhook it */
            free(gone);
            break;
        }
        link = &(*link)->next;
    }
    return head;
}

void print_playlist(const Node *head)
{
    int i = 1;
    for (const Node *cur = head; cur != NULL; cur = cur->next)
        printf("  %d. %s\n", i++, cur->song);
}

void free_all(Node *head)
{
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

int main(void)
{
    Node *playlist = NULL;      /* empty list */
    playlist = push_back(playlist, "Bohemian Rhapsody");
    playlist = push_back(playlist, "Imagine");
    playlist = push_back(playlist, "Hotel California");
    playlist = push_front(playlist, "Intro Track");
    printf("Playlist:\n");
    print_playlist(playlist);

    playlist = remove_song(playlist, "Imagine");
    printf("After removing Imagine:\n");
    print_playlist(playlist);

    free_all(playlist);
    return 0;
}
