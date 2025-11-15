// Syed Faruque
// Net-ID: stfaruque
// SBU-ID: 116340094


#include "hw7.h"

// helper function to find the precendence of an operator char
int find_precedence(char operator) {
    if (operator == '\'') return 3;
    else if (operator == '*') return 2;
    else return 1;
}


bst_sf* insert_bst_sf(matrix_sf *mat, bst_sf *root) {
    // case where this is the first node
    if (root == NULL) {
        // allocate memeory for the new node and initialize it's properties
        bst_sf *new_node = malloc(sizeof(bst_sf));
        new_node -> mat = mat;
        new_node -> left_child = NULL;
        new_node -> right_child = NULL;
        root = new_node;
    }
    // case where a root node exists
    else {
        char root_name = root -> mat -> name;
        char new_node_name = mat -> name;
        // below is a recursive process to add the new node while maintaining the property
        if (root_name < new_node_name) {
            root -> right_child = insert_bst_sf(mat, root->right_child);
        }
        else {
            root -> left_child = insert_bst_sf(mat, root->left_child);
        }
    }
    return root;
}

matrix_sf* find_bst_sf(char name, bst_sf *root) {
    // base case
    if (root == NULL) {
        return NULL;
    }
    char root_name = root->mat->name;
    // base case
    if (name == root_name) {
        return root->mat;
    }
    // recursive algorithm to search down the tree levels
    else if (name < root_name) {
        return find_bst_sf(name, root->left_child);
    }
    else {
        return find_bst_sf(name, root->right_child);
    }
}

void free_bst_sf(bst_sf *root) {
    // base case
    if (root == NULL) {
        return;
    }
    // free the left subtree
    free_bst_sf(root->left_child);
    // free the right subtree
    free_bst_sf(root->right_child);
    // free the current node
    free(root->mat);
    free(root);
}


matrix_sf* add_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    // extract important values
    unsigned int sum_rows = mat1->num_rows;
    unsigned int sum_cols = mat1->num_cols;
    int sum = 0;

    // initialize the new sum matrix
    matrix_sf *sum_matrix = malloc(sizeof(matrix_sf) + sum_rows * sum_cols * sizeof(int));
    sum_matrix -> name = '?'; 
    sum_matrix -> num_rows = sum_rows;
    sum_matrix -> num_cols = sum_cols;

    // fill out the new sum matrix
    for (unsigned int i = 0; i < sum_rows * sum_cols; i++) {
        sum_matrix -> values[i] = mat1->values[i] + mat2->values[i];
    }

    // return the sum matrix
    return sum_matrix;
}

matrix_sf* mult_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    // extract important values
    unsigned int m = mat1->num_rows;
    unsigned int n = mat1->num_cols;
    unsigned int p = mat2->num_cols;
    
    // initialize the new mult matrix
    matrix_sf *mult_matrix = malloc(sizeof(matrix_sf) + m * p * sizeof(int));
    mult_matrix->name = '?';
    mult_matrix->num_rows = m;
    mult_matrix->num_cols = p;
    
    // fill out the new mult matrix
    for (unsigned int i = 0; i < m; i++) {
        for (unsigned int j = 0; j < p; j++) {
            // this takes the dot product of the ith row in mat1 and the jth col in mat2
            int sum = 0;
            for (unsigned int k = 0; k < n; k++) {
                sum += mat1->values[i * n + k] * mat2->values[k * p + j];
            }
            mult_matrix->values[i * p + j] = sum;
        }
    }
    
    // return the mult matrix
    return mult_matrix;
}

matrix_sf* transpose_mat_sf(const matrix_sf *mat) {
    // extract important values
    unsigned int rows = mat->num_rows;
    unsigned int cols = mat->num_cols;

    // initialize the new transpose matrix
    matrix_sf *trans_matrix = malloc(sizeof(matrix_sf) + rows * cols * sizeof(int));
    trans_matrix->name = '?';
    trans_matrix->num_rows = cols;
    trans_matrix->num_cols = rows;

    // fill out the new transpose matrix
    // we want the value at row i col j in the mat to be in row j col i in the transpose
    for (unsigned int i = 0; i < rows; i++) {
        for (unsigned int j = 0; j < cols; j++) {
            trans_matrix->values[rows*j+i] = mat->values[cols*i+j];
        }
    }

    // return the transpose matrix
    return trans_matrix;
}

matrix_sf* create_matrix_sf(char name, const char *expr) {
    // extract important values by parsing the expression
    unsigned int num_rows, num_cols;
    int offset = 0;
    sscanf(expr, "%u %u%n", &num_rows, &num_cols, &offset);
    const char *ptr = expr + offset;

    // allocate the new matrix and initialize its name and dimension properties
    matrix_sf *mat = malloc(sizeof(matrix_sf) + num_rows * num_cols * sizeof(int));
    mat->name = name;
    mat->num_rows = num_rows;
    mat->num_cols = num_cols;

    // to initialize the values propertu we need to parse the rest of the expression

    // move a pointer to the point after the opening bracket
    while (*ptr != '[') ptr++;
    ptr++;

    // parse through the remaining expression and fill out the values property
    unsigned int index = 0;
    while (*ptr != ']' && index < num_rows * num_cols) {
        int val;
        int num_chars;
        // Skip spaces and semicolons by simply advancing the pointer when they are reached
        while (*ptr == ' ' || *ptr == ';') ptr++;
        // break out of the loop when the ending bracket is reached
        if (*ptr == ']') break;
        if (sscanf(ptr, "%d%n", &val, &num_chars) == 1) {
            mat->values[index++] = val;
            ptr += num_chars;
        } 
        else {
            break;
        }
    }


    // return the resulting matrix
    return mat;
}

char* infix2postfix_sf(char *infix) {

    // allocate memory for the postfix string and a stack
    unsigned int length = strlen(infix);
    char *postfix = malloc(length*2+1);
    char *stack = malloc(length + 1);

    // variables to keep track of the stack top and postfix index
    int stack_top = -1;
    int postfix_index = 0;

    // iterate through the string once and build up the postfix expression using the stack method
    for (int i = 0; infix[i] != '\0'; i++) {
        // extract the current char in the iteration
        char c = infix[i];
        // skip if the current char is whitespace
        if (c == ' ') continue;
        // if the current char is an uppercase alphabet we can add it to the postfix string
        if (isupper(c)) {
            postfix[postfix_index++] = c;
        }
        // if the current char is a opening parenthesis, add it to the stack
        else if (c == '(') {
            stack[++stack_top] = c;
        }
        // if the current char is a closing parenthesis, we want to remove everything from the stack and add it to the postfix string
        else if (c == ')') {
            while (stack_top >= 0 && stack[stack_top] != '(') {
                postfix[postfix_index++] = stack[stack_top--];
            }
            if (stack_top >= 0) stack_top--;
        }
        // if the current char is an operator, we will use the methodology given by the stack method to handle it
        else if (c == '\'' || c == '*' || c == '+') {
            int current_precendence = find_precedence(c);
            while (stack_top >= 0 && stack[stack_top] != '(') {
                char top = stack[stack_top];
                int top_precendence = find_precedence(top);
                if (top_precendence >= current_precendence) {
                    postfix[postfix_index++] = stack[stack_top--];
                }
                else {
                    break;
                }
            }
            stack[++stack_top] = c;
        }
    }

    while (stack_top >= 0) {
        postfix[postfix_index++] = stack[stack_top--];
    }
    
    postfix[postfix_index] = '\0';
    free(stack);
    return postfix;
}

matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root) {
    // get the postfix version of the given expression
    char *postfix = infix2postfix_sf(expr);

    // intitialize a stack, which will be used to evaluate the expression
    matrix_sf **stack = malloc(strlen(postfix) * sizeof(matrix_sf*));
    int stack_top = -1;

    // iterate through the postfix expression and use the stack to evaluate the expression
    for (int i = 0; postfix[i] != '\0'; i++) {
        // keep hold of the current char
        char c = postfix[i];

        // if the current char is an upper case letter we add it to the stack
        if (isupper(c)) {
            matrix_sf *mat = find_bst_sf(c, root);
            stack[++stack_top] = mat;
        }
        
        // if the current char is the unary operator we want to pop the last matrix off the stack and transpose it and readd
        else if (c == '\'') {
            matrix_sf *mat = stack[stack_top--];
            matrix_sf *result = transpose_mat_sf(mat);
            if (!isupper(mat->name)) free(mat);
            stack[++stack_top] = result;
        }

        // if the current char is either multiplication or addition operator we want to pop the 
        // last two matrices off the stack and perform the operation on them then readded the result
        else if (c == '*') {
            matrix_sf *mat2 = stack[stack_top--];
            matrix_sf *mat1 = stack[stack_top--];
            matrix_sf *mult_matrix = mult_mats_sf(mat1, mat2);
            stack[++stack_top] = mult_matrix;
        }
        else if (c == '+') {
            matrix_sf *mat2 = stack[stack_top--];
            matrix_sf *mat1 = stack[stack_top--];
            matrix_sf *sum_matrix = add_mats_sf(mat1, mat2);
            stack[++stack_top] = sum_matrix;
        }
    }

    // the last item in the stack should be the result matrix
    matrix_sf *result = stack[stack_top];

    // fix the name property
    result->name = name;

    // free space and return the result
    free(postfix);
    free(stack);
    return result;
}

matrix_sf *execute_script_sf(char *filename) {
    // open the file
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;
    
    // variables to store the root matrix and the last matrix
    bst_sf *root = NULL;
    matrix_sf *last_matrix = NULL;

    // iterate line by line through the file
    char *line = NULL;
    size_t max_line_size = MAX_LINE_LEN;
    while (getline(&line, &max_line_size, file) != -1) {
        char *ptr = line;
        // skip through any whitespace
        while (*ptr == ' ') ptr++;
        
        // extract the name of the matrix and increment the pointer
        char name = *ptr++;

        // skip any white spaces
        while (*ptr == ' ') ptr++;

        // skip the = sign
        ptr++;

        // skip any white spaces
        while (*ptr == ' ') ptr++;
        
        // if the next thing is an integer, that means we are in a matrix definition
        if (isdigit(*ptr)) {
            last_matrix = create_matrix_sf(name, ptr);
        } 

        // if the next thing is not an integer, that means we are at a formula
        else {
            char *expr_end = line + strlen(line) - 1;
            while (expr_end > ptr && (*expr_end == '\n' || *expr_end == ' ')) {
                *expr_end = '\0';
                expr_end--;
            }
            last_matrix = evaluate_expr_sf(name, ptr, root);
        }
        
        root = insert_bst_sf(last_matrix, root);
    }

    // free up any allocated memory and close the file and return the last matrix
    free(line);
    fclose(file);
    return last_matrix;
}

// This is a utility function used during testing. Feel free to adapt the code to implement some of
// the assignment. Feel equally free to ignore it.
matrix_sf *copy_matrix(unsigned int num_rows, unsigned int num_cols, int values[]) {
    matrix_sf *m = malloc(sizeof(matrix_sf)+num_rows*num_cols*sizeof(int));
    m->name = '?';
    m->num_rows = num_rows;
    m->num_cols = num_cols;
    memcpy(m->values, values, num_rows*num_cols*sizeof(int));
    return m;
}

// Don't touch this function. It's used by the testing framework.
// It's been left here in case it helps you debug and test your code.
void print_matrix_sf(matrix_sf *mat) {
    assert(mat != NULL);
    assert(mat->num_rows <= 1000);
    assert(mat->num_cols <= 1000);
    printf("%d %d ", mat->num_rows, mat->num_cols);
    for (unsigned int i = 0; i < mat->num_rows*mat->num_cols; i++) {
        printf("%d", mat->values[i]);
        if (i < mat->num_rows*mat->num_cols-1)
            printf(" ");
    }
    printf("\n");
}
