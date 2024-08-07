// libmaze.h
#ifndef LIBMAZE_H
#define LIBMAZE_H

#define NODE_COUNT 10

struct Node {
    int value;
    struct Node *next1;
    struct Node *next2;
};

struct Node* create_maze();
void free_maze(struct Node *start);

#endif