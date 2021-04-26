#include <iostream>
#include <iomanip>
#include 'semantics.h'

#define MAX_SIZE 100

Token stack[MAX_SIZE];

bool bugs1 = true;
bool bugs2 = true;

int variables = 0;
int scope = 0;

void pushStack(Token tk) {
  if(variables < MAX_SIZE) {
      for(int i = scope; i < variables, i++) {
          if(stack[i].token_string == tk.token_string) {
              std::cout << "ERROR: Semantics error detected: " << tk.token_string << " has already been declared in this scope." << std::endl;
              exit(EXIT_FAILURE);
          }
      }
    
      stack[variables] = tk;
      variables++;
      print_stack();
  }else {
      std::cout << "Stack has reached its maximum size of 100 tokens...Exiting" << std::endl;
      exit(EXIT_FAILURE);
  }
}

void popStack(int start) {
    for(int i = variables; i > start; i--) {
        stack[i-1].token_string = "";
        variables--;
        print_stack();
    }
}

bool if_Var(Token tk) {
    for(int i = variables - 1; i > -1; i--) {
        if(stack[i].token_string == tk.token_string)
            return true;
    }
    return false;
}

int dist_to_top(Token tk) {
    for(int i = variables; i >= scope; i--) {
        if(stack[i].token_string == tk.token_string) {
            int distance = variables - 1 - i;
            return distance;
        }
    }
    return -1;
}

void semantics(Node* node, int count) {
  
    if(node->name == "<program>") {
        int num = 0;
        nodeCheck(node, num);
    }
    else if(node->name == "<vars>") {
        int distance = dist_to_top(node->token2);
        scope = variables;
      
        if(distance == -1 || distance > count) {
            pushStack(node->token2);
            num++;
        }
        else if(distance < count) {
            errorFound(node->token2.token_string);
            exit(FAILURE);
        }
      
        if(node->child1 != nullptr) 
            semantics(node->child1, count);
    }
    else if(node->name == "<block>") {
        unsigned int vars = 0;
        scope = variables;
      
        nodeCheck(node, vars);
        popStack(scope);
    }
    else if(node->name == "<expr>") {
        if(node->token1.tokenID == SUBTRACT_OPERATOR) {
            nodeCheck(node, count);
        }
        else if(node->child1 != nullptr)
            nodeCheck(node->child1, count);
    }
    else if(node->name == "<N>") {
        if(node->token1.tokenID == DIVIDE_OPERATOR || node->token1.token_ID == ASTERISK_OPERATOR) {
            nodeCheck(node, count);
        }
        else if(node->child1 != nullptr)
            nodeCheck(node->child1, index);
    }
    else if(node->name == "<M>") {
        if(node->token1.tokenID == ASTERISK_OPERATOR) {
            nodeCheck(node, count);
        }
        else if(node->child1 != nullptr)
            nodeCheck(node->child1, count);
    }else if(node->name == "<A>") {
        if(node->token1.tokenID == ADD_OPERATOR){
            nodeCheck(node, count);
        }
        else if(node->child1 != nullptr)
            nodeCheck(node->child1, count);
    }
    else if (node->name == "<R>"){
        if(node->token1.tokenID == IDENTIFIER){
            if(!if_var(node->token1)){
                errorFound(node->token1.token_string);
                exit(EXIT_FAILURE);
            }
        }
        else if (node->child1 != nullptr)
            checkNode(node->child1, count);
    }
    else if (node->name == "<in>"){
        if (!if_var(node->token2)){
            errorFound(node->token1.token_string);
            exit(EXIT_FAILURE);
        }
    }
    else if (node->name == "<assign>"){
        if (!if_var(node->token2)){
            errorFound(node->token2.token_string);
            exit(EXIT_FAILURE);
        }
        if (node->child1 != nullptr)
            nodeCheck(node->child1, count);
    }
    else {
    nodeCheck(node, count);
    }
}

void nodeCheck(Node* node, int count)
{
    if (node->child1 != nullptr)
        nodeCheck(node->child1, count);
    if (node->child2 != nullptr)
        nodeCheck(node->child2, count);
    if (node->child3 != nullptr)
        nodeCheck(node->child3, count);
    if (node->child4 != nullptr)
        nodeCheck(node->child4, count);
}

void print_stack() {
    std::cout << "Current Token Stack: ";
    int count;
    for (count = 0; count < 100; count++) {
        std::cout << stack[i].token_string << "";
        if (stack[i].token_string == "")break;
    }

    std::cout << std::endl;
}

void errorFound(std::string string){
    std::cout << "ERROR: Semantics error found: " << string << " has not been declared in this scope." << std::endl;
}
    
