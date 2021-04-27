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

void semantic_check(Node* node, int count)
{
    if (node == nullptr)
        return;
    if (debug1) {
        if (node->name == "<vars>" || node->name == "<assign>") { std::cout << "Working on: " << std::setw(10) << std::left << node->name << "| Token: " << std::setw(9) << std::left << node->token_2.token_string << "| "; }  // for debugging
        else { std::cout << "Working on: " << std::setw(10) << std::left << node->name << "| Token: " << std::setw(9) << std::left << node->token_1.token_string << "| "; }  // for debugging
        print_stack();
    }
    if (node->name == "<program>")
    {
        int vars = 0;
        if (node->child_1 != nullptr)
            semantic_check(node->child_1, vars);
        if (node->child_2 != nullptr)
            semantic_check(node->child_2, vars);
        if (node->child_3 != nullptr)
            semantic_check(node->child_3, vars);
        if (node->child_4 != nullptr)
            semantic_check(node->child_4, vars);
    }
    else if (node->name == "<vars>")
    {
        int tos_distance = find(node->token_2);
        scope = var_count;

        if (tos_distance == -1 || tos_distance > count)
        {
            
            push(node->token_2);
            count++;
        }
        else if (tos_distance < count)
        {
            error_declared(node->token_2.token_string);
            exit(EXIT_FAILURE);
        }

        if (node->child_1 != nullptr)
            semantic_check(node->child_1, count);
    }
    else if (node->name == "<block>")
    {
        unsigned int num_vars = 0;
        scope = var_count;

        if (node->child_1 != nullptr)
            semantic_check(node->child_1, num_vars);
        if (node->child_2 != nullptr)
            semantic_check(node->child_2, num_vars);
        if (node->child_3 != nullptr)
            semantic_check(node->child_3, num_vars);
        if (node->child_4 != nullptr)
            semantic_check(node->child_4, num_vars);

        pop(scope);
    }
    else if (node->name == "<expr>")
    {
        if (node->token_1.token_ID == MINUS_TK)
        {
            if (node->child_1 != nullptr)
                semantic_check(node->child_1, count);
            if (node->child_2 != nullptr)
                semantic_check(node->child_2, count);
            if (node->child_3 != nullptr)
                semantic_check(node->child_3, count);
            if (node->child_4 != nullptr)
                semantic_check(node->child_4, count);
        }
        else if (node->child_1 != nullptr)
            semantic_check(node->child_1, count);
    }
    else if (node->name == "<N>")
    {
        if (node->token_1.token_ID == SLASH_TK || node->token_1.token_ID == ASTERISK_TK)
        {
            if (node->child_1 != nullptr)
                semantic_check(node->child_1, count);
            if (node->child_2 != nullptr)
                semantic_check(node->child_2, count);
            if (node->child_3 != nullptr)
                semantic_check(node->child_3, count);
            if (node->child_4 != nullptr)
                semantic_check(node->child_4, count);
        }
        else if (node->child_1 != nullptr)
            semantic_check(node->child_1, count);
    }
    else if (node->name == "<M>")
    {
        if (node->token_1.token_ID == ASTERISK_TK)
        {
            if (node->child_1 != nullptr)
                semantic_check(node->child_1, count);
            if (node->child_2 != nullptr)
                semantic_check(node->child_2, count);
            if (node->child_3 != nullptr)
                semantic_check(node->child_3, count);
            if (node->child_4 != nullptr)
                semantic_check(node->child_4, count);
        }
        else if (node->child_1 != nullptr)
            semantic_check(node->child_1, count);
    }



    else if (node->name == "<A>")
    {
        if (node->token_1.token_ID == PLUS_TK)
        {
            if (node->child_1 != nullptr)
                semantic_check(node->child_1, count);
            if (node->child_2 != nullptr)
                semantic_check(node->child_2, count);
            if (node->child_3 != nullptr)
                semantic_check(node->child_3, count);
            if (node->child_4 != nullptr)
                semantic_check(node->child_4, count);
        }
        else if (node->child_1 != nullptr)
            semantic_check(node->child_1, count);
    }

    else if (node->name == "<R>")
    {
        if (node->token_1.token_ID == ID_TK)
        {
            if (!var_exists(node->token_1))
            {
                error_declared(node->token_1.token_string);
                exit(EXIT_FAILURE);
            }
        }
        else if (node->child_1 != nullptr)
            semantic_check(node->child_1, count);
    }
    else if (node->name == "<in>")
    {
        if (!var_exists(node->token_2))
        {
            error_declared(node->token_1.token_string);
            exit(EXIT_FAILURE);
        }
    }
    else if (node->name == "<assign>")
    {
        if (!var_exists(node->token_2))
        {
            error_declared(node->token_2.token_string);
            exit(EXIT_FAILURE);
        }
        if (node->child_1 != nullptr)
            semantic_check(node->child_1, count);
    }
    else
    {
        if (node->child_1 != nullptr)
            semantic_check(node->child_1, count);
        if (node->child_2 != nullptr)
            semantic_check(node->child_2, count);
        if (node->child_3 != nullptr)
            semantic_check(node->child_3, count);
        if (node->child_4 != nullptr)
            semantic_check(node->child_4, count);
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
    
