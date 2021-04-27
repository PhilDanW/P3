/*********************************************************
* Author: Philip Wright                                  *
* Project 3: Semantics                                   *
* semantics.cpp processes the tree built in the parser   *
* and check for errors in semantics while building       *
* a stack of the variables that have been input from     * 
* the parse tree                                         *
*********************************************************/
#include "semantics.h"
#include <iostream>
#include <iomanip>

#define MAX_STACK 100

Token stack[MAX_STACK];

bool debug1 = false;
bool debugger2 = true;

int var_count = 0;
int scope = 0;


void build_stack(){
    for (int i = 0; i <= MAX_STACK; i++)
        stack[i].token_string = "";
}

void push(Token tk){
    if (var_count >= MAX_STACK)
    {
        std::cout << "Semantics error... Stack has reached size of 100 \nExiting..." << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        for (int i = scope; i < var_count; i++)
        {
            if (stack[i].token_string == tk.token_string)
            {
                std::cout << "Semantics error... \'" << tk.token_string << "\' has already been declared in this scope" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        
        stack[var_count] = tk; if (debugger2) { std::cout << "Adding \'" << tk.token_string << "\' to the stack\n"; }  // for debugging
        var_count++; if (debugger2) { std::cout << "VAR COUNT: " << var_count << "\n"; }  // for debugging
        print_stack();
    }
}

void pop(int scope_begin){
    for (int i = var_count; i > scope_begin; i--)
    {
        var_count--;
        stack[i].token_string == "";
    }
}

int find(Token tk){
    for (int i = var_count; i >= scope; i--)
    {
        if (stack[i].token_string == tk.token_string)
        {
            int tos_distance = var_count - 1 - i;
            return tos_distance;
        }
    }

    return -1;
}

bool var_exists(Token tk)
{
    for (int i = var_count - 1; i > -1; i--)
    {
        if (stack[i].token_string == tk.token_string)
            return true;
    }

    return false;
}

void semantic_check(treeNode* node, int count)
{
    if (node == nullptr)
        return;
    if (debug1) {
        if (node->label == "<vars>" || node->label == "<assign>") { std::cout << "Working on: " << std::setw(10) << std::left << node->label << "| Token: " << std::setw(9) << std::left << node->token2.token_string << "| "; }  // for debugging
        else { std::cout << "Working on: " << std::setw(10) << std::left << node->label << "| Token: " << std::setw(9) << std::left << node->token1.token_string << "| "; }  // for debugging
        print_stack();
    }
    if (node->label == "<program>")
    {
        int vars = 0;
        if (node->leaf1 != nullptr)
            semantic_check(node->leaf1, vars);
        if (node->leaf2 != nullptr)
            semantic_check(node->leaf2, vars);
        if (node->leaf3 != nullptr)
            semantic_check(node->leaf3, vars);
        if (node->leaf4 != nullptr)
            semantic_check(node->leaf4, vars);
    }
    else if (node->label == "<vars>")
    {
        int tos_distance = find(node->token2);
        scope = var_count;

        if (tos_distance == -1 || tos_distance > count)
        {
            
            push(node->token2);
            count++;
        }
        else if (tos_distance < count)
        {
            error_declared(node->token2.token_string);
            exit(EXIT_FAILURE);
        }

        if (node->leaf1 != nullptr)
            semantic_check(node->leaf1, count);
    }
    else if (node->label == "<block>")
    {
        unsigned int num_vars = 0;
        scope = var_count;

        if (node->leaf1 != nullptr)
            semantic_check(node->leaf1, num_vars);
        if (node->leaf2 != nullptr)
            semantic_check(node->leaf2, num_vars);
        if (node->leaf3 != nullptr)
            semantic_check(node->leaf3, num_vars);
        if (node->leaf4 != nullptr)
            semantic_check(node->leaf4, num_vars);

        pop(scope);
    }
    else if (node->label == "<expr>")
    {
        if (node->token1.tokenID == SUBTRACT_OPERATOR)
        {
            if (node->leaf1 != nullptr)
            semantic_check(node->leaf1, count);
            if (node->leaf2 != nullptr)
                semantic_check(node->leaf2, count);
            if (node->leaf3 != nullptr)
                semantic_check(node->leaf3, count);
            if (node->leaf4 != nullptr)
                semantic_check(node->leaf4, count);
        }
        else if (node->leaf1 != nullptr)
            semantic_check(node->leaf1, count);
    }
    else if (node->label == "<N>")
    {
        if (node->token1.tokenID == DIVIDE_OPERATOR || node->token1.tokenID == ASTERISK_OPERATOR)
        {
            if (node->leaf1 != nullptr)
            semantic_check(node->leaf1, count);
            if (node->leaf2 != nullptr)
                semantic_check(node->leaf2, count);
            if (node->leaf3 != nullptr)
                semantic_check(node->leaf3, count);
            if (node->leaf4 != nullptr)
                semantic_check(node->leaf4, count);
        }
        else if (node->leaf1 != nullptr)
            semantic_check(node->leaf1, count);
    }
    else if (node->label == "<M>")
    {
        if (node->token1.tokenID == ASTERISK_OPERATOR)
        {
            if (node->leaf1 != nullptr)
            semantic_check(node->leaf1, count);
            if (node->leaf2 != nullptr)
                semantic_check(node->leaf2, count);
            if (node->leaf3 != nullptr)
                semantic_check(node->leaf3, count);
            if (node->leaf4 != nullptr)
                semantic_check(node->leaf4, count);
        }
        else if (node->leaf1 != nullptr)
            semantic_check(node->leaf1, count);
    }

    else if (node->label == "<A>")
    {
        if (node->token1.tokenID == ADD_OPERATOR)
        {
            if (node->leaf1 != nullptr)
            semantic_check(node->leaf1, count);
            if (node->leaf2 != nullptr)
                semantic_check(node->leaf2, count);
            if (node->leaf3 != nullptr)
                semantic_check(node->leaf3, count);
            if (node->leaf4 != nullptr)
                semantic_check(node->leaf4, count);
        }
        else if (node->leaf1 != nullptr)
            semantic_check(node->leaf1, count);
    }

    else if (node->label == "<R>")
    {
        if (node->token1.tokenID == IDENTIFIER)
        {
            if (!var_exists(node->token1))
            {
                error_declared(node->token1.token_string);
                exit(EXIT_FAILURE);
            }
        }
        else if (node->leaf1 != nullptr)
            semantic_check(node->leaf1, count);
    }
    else if (node->label == "<in>")
    {
        if (!var_exists(node->token2))
        {
            error_declared(node->token1.token_string);
            exit(EXIT_FAILURE);
        }
    }
    else if (node->label == "<assign>")
    {
        if (!var_exists(node->token2))
        {
            error_declared(node->token2.token_string);
            exit(EXIT_FAILURE);
        }
        if (node->leaf1 != nullptr)
            semantic_check(node->leaf1, count);
    }
    else
    {
        if (node->leaf1 != nullptr)
            semantic_check(node->leaf1, count);
        if (node->leaf2 != nullptr)
            semantic_check(node->leaf2, count);
        if (node->leaf3 != nullptr)
            semantic_check(node->leaf3, count);
        if (node->leaf4 != nullptr)
            semantic_check(node->leaf4, count);
    }
}

void error_declared(std::string tokenString)
{
    std::cout << "Semantics error... \'" << tokenString << "\' has not been declared in this scope" << std::endl;
}


void print_stack() {
    std::cout << "Stack: ";
    int i;
    for (i = 0; i < 100; i++) {
        std::cout << stack[i].token_string << "";
        if (stack[i].token_string == "")break;
    }

    std::cout << std::endl;
}
    
