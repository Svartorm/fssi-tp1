#include "robdd.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/*
Parser using:
. : AND
+ : OR
! : NOT
* : XOR
Using AND(node_t a, node_t b) and OR(node_t a, node_t b) and NOT(node_t a) and XOR(node_t a, node_t b)
*/

// AND operation on two ROBDDs
node_t* AND(node_t *a, node_t *b) {
    // base case : if either a or b is false, return false
    if (a->label == '0' || b->label == '0') {
        node_t *result = (node_t*)malloc(sizeof(node_t));
        result->label = '0';
        result->left = NULL;
        result->right = NULL;
        return result;  
    }
    // base case : if either a or b is true, return the other
    if (a->label == '1') {
        return b;  
    }
    if (b->label == '1') {
        return a;  
    }
    // recursive case : create a new node with the same label as a and b
    node_t *result = (node_t*)malloc(sizeof(node_t));
    // smallest index 
    if (a->label < b->label) {
        result->label = a->label;
        // recursively compute the left and right children
        result->left = (node_t*)malloc(sizeof(node_t));
        result->left = AND(a->left, b);
        result->right = (node_t*)malloc(sizeof(node_t)); 
        result->right = AND(a->right, b);
    } else if (a->label > b->label) {
        result->label = b->label;
        // recursively compute the left and right children
        result->left = (node_t*)malloc(sizeof(node_t));
        result->left = AND(a, b->left);
        result->right = (node_t*)malloc(sizeof(node_t));
        result->right = AND(a, b->right);
    } else {
        result->label = a->label; 
        // recursively compute the left and right children
        result->left = (node_t*)malloc(sizeof(node_t));
        result->left = AND(a->left, b->left);
        result->right = (node_t*)malloc(sizeof(node_t));
        result->right = AND(a->right, b->right);
    }
    return result;
    
}

//TODO replace OR by the other one and fix other operations to feat this new implementation
node_t *OR(node_t *a, node_t *b) {
    // base case : if either a or b is true, return true
    if (a->label == '1' || b->label == '1') {
        node_t *result = (node_t*)malloc(sizeof(node_t));
        result->label = '1';
        result->left = NULL;
        result->right = NULL;
        return result;  
    }
    // base case : if either a or b is false, return the other
    if (a->label == '0') {
        return b;  
    }
    if (b->label == '0') {
        return a;  
    }
    // recursive case : create a new node with the same label as a and b
    node_t *result = (node_t*)malloc(sizeof(node_t));
    // smallest index 
    if (a->label < b->label) {
        result->label = a->label;
        // recursively compute the left and right children
        result->left = (node_t*)malloc(sizeof(node_t));
        result->left = OR(a->left, b);
        result->right = (node_t*)malloc(sizeof(node_t)); 
        result->right = OR(a->right, b);
    } else if (a->label > b->label) {
        result->label = b->label;
        // recursively compute the left and right children
        result->left = (node_t*)malloc(sizeof(node_t));
        result->left = OR(a, b->left);
        result->right = (node_t*)malloc(sizeof(node_t));
        result->right = OR(a, b->right);
    } else {
        result->label = a->label; 
        // recursively compute the left and right children
        result->left = (node_t*)malloc(sizeof(node_t));
        result->left = OR(a->left, b->left);
        result->right = (node_t*)malloc(sizeof(node_t));
        result->right = OR(a->right, b->right);
    }
    return result;
};
node_t *NOT(node_t *a) {
    // base case : if a is false, return true
    if (a->label == '0') {
        node_t* result = (node_t*)malloc(sizeof(node_t));
        result->label = '1';
        result->left = NULL;
        result->right = NULL;
        return result;  
    }
    // base case : if a is true, return false
    if (a->label == '1') {
        node_t* result = (node_t*)malloc(sizeof(node_t));
        result->label = '0';
        result->left = NULL;
        result->right = NULL;
        return result;  
    }
    // recursive case : create a new node with the same label as a
    node_t* result = (node_t*)malloc(sizeof(node_t));
    result->label = a->label; 
    // recursively compute the left and right children
    result->left = (node_t*)malloc(sizeof(node_t));
    result->left = NOT(a->left);
    result->right = (node_t*)malloc(sizeof(node_t));
    result->right = NOT(a->right);
    return result;
};
/*node_t* XOR(node_t *a, node_t *b) {
    // XOR can be implemented using AND, OR and NOT
    return OR(AND(a, NOT(b)), AND(NOT(a), b));
};*/



node_t* create_node(char label, node_t* left, node_t* right) {
    node_t* result = (node_t*)malloc(sizeof(node_t));
    result->label = label;
    result->left = left;
    result->right = right;
    return result;
}

node_t* parse(char* expression) {
    // base case : if the expression is a variable, return a node with the variable as label
    if (expression[0] != '(') {
        node_t* result = (node_t*)malloc(sizeof(node_t));
        result->label = expression[0];
        result->left = create_node('0', NULL, NULL); // false
        result->right = create_node('1', NULL, NULL); // true
        return result;  
    }
    // recursive case : parse the left and right subexpressions
    int i = 1; // skip the first '('
    int count = 0; // count the number of parentheses
    while (i < strlen(expression)) {
        if (expression[i] == '(') {
            count++;
        } else if (expression[i] == ')') {
            count--;
        } else if ((expression[i] == '.' || expression[i] == '+' || expression[i] == '!' || expression[i] == '*') && count == 0) {
            break; // found the operator at the top level
        }
        i++;
    }
    char operator = expression[i];
    char* left_expr = strndup(expression + 1, i - 1); // left subexpression
    char* right_expr = strndup(expression + i + 1, strlen(expression) - i - 2); // right subexpression
    node_t* left_node = parse(left_expr);
    node_t* right_node = parse(right_expr);
    free(left_expr);
    free(right_expr);
    
    node_t* result = (node_t*)malloc(sizeof(node_t));
    if (operator == '.') {
        *result = *AND(left_node, right_node);
    } else if (operator == '+') {
        *result = *OR(left_node, right_node);
    } else if (operator == '!') {
        result = NOT(left_node);
    } else if (operator == '*') {
        result = XOR(left_node, right_node);
    }
    return result;
}
void print_tree(node_t* node, int level) { // TODO: use lib to display the tree
    if (node == NULL) {
        return;
    }
    print_tree(node->right, level + 1);
    for (int i = 0; i < level; i++) {
        printf("   ");
    }
    printf("%c\n", node->label);
    print_tree(node->left, level + 1);
}
// TODO: implemant backtrace to optimize label ordering and reduce the size of the ROBDD
int main() {
    char* expression = "(a.b)"; // example expression
    node_t* result = parse(expression);
    // print the result
    print_tree(result, 0);  
    return 0;
}