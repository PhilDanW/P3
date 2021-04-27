#ifndef SEMANTICS_H
#define SEMANTICS_H
#include "token.h"
#include "treeNode.h"

void build();
void pushStack(Token tk);
void popStack(int start);
bool if_var(Token tk);
int dist_to_top(Token tk);
void semantics(treeNode* node, int count);
void print_stack();
void errorFound(std::string string);

#endif
