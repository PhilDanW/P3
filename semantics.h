/*********************************************************
* Author: Philip Wright                                  *
* Project 3: Semantics                                   *
* semantics.h contains function defintions for the .cpp  *
*********************************************************/
#ifndef SEMANTICS_H
#define SEMANTICS_H
#include "token.h"
#include "treeNode.h"

void push(Token tk);
void pop(int scope_begin);
void build_stack();
int find(Token tk);
bool var_exists(Token tk);
void print_stack();
void semantic_check(treeNode* node, int count);

void error_declared(std::string tokenString);


#endif
