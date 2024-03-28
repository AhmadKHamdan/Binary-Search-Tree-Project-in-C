// Ahmad Hamdan - 1210241 - Section 4
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node *readFromFile(struct node *root);
struct node *newNode(int s_ID, char *s_Name, char *c_Name, int c_ID, char *e_date);
void printInorder(struct node *root, FILE *file);
struct node *insert(struct node *node, int s_ID, char *s_Name, char *c_Name, int c_ID, char *e_date);
struct node *search(struct node *root, int ID);
struct node *minValueNode(struct node *node);
struct node *deleteNode(struct node *root, int s_ID);
int countNodes(struct node *root);
struct node **treeToArray(struct node *root, struct node **arr, int *index);
struct node **sortArray(struct node **arr, int size);
struct node *insertStudent(struct node *root);
struct node *findStudent(struct node *root);
void printStudents(struct node **arr, int operation, int size);
struct node *deleteStudent(struct node *root);
void saveToFile(struct node *root);

// Struct for the nodes of the tree
struct node
{
    int s_ID;                  // Student ID
    char s_Name[20];           // Student Name
    char c_Name[20];           // City Name
    int c_ID;                  // Class ID
    char e_date[20];           // Enrollment Date
    struct node *left, *right; // Pointers to the children
};

int main()
{
    struct node *root = NULL;  // Creating the root of the BST tree
    root = readFromFile(root); // Read from the file at the start of the program
    struct node **arr;         // Creating the array of binary trees

    // Creating the main menu of the program
    int input = 0, index = 0;
    while (input != 8)
    {
        printf("\n\n1- Insert a new student\n");
        printf("2- Find a given student and update the info\n");
        printf("3- Print all students in lexicographic order\n");
        printf("4- Print all students from a given city\n");
        printf("5- Print all students by their class\n");
        printf("6- Delete a given student\n");
        printf("7- Save all students in file\n");
        printf("8- Exit\n");
        printf(".......................\n");
        printf("Please choose an operation: ");
        scanf("%d", &input);

        if (input == 1)
            root = insertStudent(root); // Inserting a new student
        if (input == 2)
            root = findStudent(root); // Find student and update info
        if (input == 3 || input == 4 || input == 5)
        {
            index = 0;
            arr = (struct node **)malloc(sizeof(struct node *) * countNodes(root));
            treeToArray(root, arr, &index);
            arr = sortArray(arr, index);

            if (input == 3)
                printStudents(arr, 3, index);
            if (input == 4)
                printStudents(arr, 4, index);
            if (input == 5)
                printStudents(arr, 5, index);
            free(arr);
        }
        if (input == 6)
            deleteStudent(root);
        if (input == 7)
            saveToFile(root);
        if (input == 8)
            printf("\nGood Bye..\n\n");
    }

    return 0;
}

// Function to read from a file
struct node *readFromFile(struct node *root)
{
    FILE *file;

    file = fopen("students.data.txt", "r");

    int s_ID_temp;
    char s_Name_temp[20];
    char c_Name_temp[20];
    int c_ID_temp;
    char e_date_temp[20];

    while (fscanf(file, "%d %s %s %d %s", &s_ID_temp, s_Name_temp, c_Name_temp, &c_ID_temp, e_date_temp) != EOF)
    {
        root = insert(root, s_ID_temp, s_Name_temp, c_Name_temp, c_ID_temp, e_date_temp);
    }

    fclose(file);
    return root;
}

// Function to create a new node0
struct node *newNode(int s_ID, char *s_Name, char *c_Name, int c_ID, char *e_date)
{
    struct node *temp = (struct node *)malloc(sizeof(struct node));
    temp->s_ID = s_ID;
    strcpy(temp->s_Name, s_Name);
    strcpy(temp->c_Name, c_Name);
    temp->c_ID = c_ID;
    strcpy(temp->e_date, e_date);
    temp->left = temp->right = NULL;
    return temp;
}

// Function to print the tree inorder traversal
void printInorder(struct node *root, FILE *file)
{
    if (root != NULL)
    {
        // Traverse left
        printInorder(root->left, file);

        // Traverse root
        fprintf(file, "\n %d \t %s \t %s \t %d \t %s", root->s_ID, root->s_Name, root->c_Name, root->c_ID, root->e_date);

        // Traverse right
        printInorder(root->right, file);
    }
}

// Function to insert a node into the tree
struct node *insert(struct node *node, int s_ID, char *s_Name, char *c_Name, int c_ID, char *e_date)
{
    // Return a new node if the tree is empty
    if (node == NULL)
    {
        return newNode(s_ID, s_Name, c_Name, c_ID, e_date);
    }

    // Traverse to the right place and insert the node
    if (s_ID < node->s_ID)
        node->left = insert(node->left, s_ID, s_Name, c_Name, c_ID, e_date);
    else
        node->right = insert(node->right, s_ID, s_Name, c_Name, c_ID, e_date);

    return node;
}

// Function to search for a node
struct node *search(struct node *root, int ID)
{
    if (root == NULL || root->s_ID == ID)
    {
        // If the root is NULL or the ID is found at the root, return the root
        return root;
    }

    if (ID < root->s_ID)
    {
        // If the ID is smaller than the root's ID, recursively search in the left subtree
        return search(root->left, ID);
    }
    else
    {
        // If the ID is greater than the root's ID, recursively search in the right subtree
        return search(root->right, ID);
    }
}

// Function to find the last node
struct node *minValueNode(struct node *node)
{
    struct node *current = node;

    // Find the leftmost leaf
    while (current && current->left != NULL)
        current = current->left;

    return current;
}

// Function to delete a node in a tree
struct node *deleteNode(struct node *root, int s_ID)
{
    // Return if the tree is empty
    if (root == NULL)
    {
        printf("\nStudent ID was not found.");
        return root;
    }

    // Find the node to be deleted
    if (s_ID < root->s_ID)
        root->left = deleteNode(root->left, s_ID);
    else if (s_ID > root->s_ID)
        root->right = deleteNode(root->right, s_ID);

    else
    {
        printf("\nStudent has been deleted");
        // If the node is with only one child or no child
        if (root->left == NULL)
        {
            struct node *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            struct node *temp = root->left;
            free(root);
            return temp;
        }

        // If the node has two children
        struct node *temp = minValueNode(root->right);

        // Place the inorder successor in position of the node to be deleted
        root->s_ID = temp->s_ID;
        strcpy(root->s_Name, temp->s_Name);
        strcpy(root->c_Name, temp->c_Name);
        root->c_ID = temp->c_ID;
        strcpy(root->e_date, temp->e_date);

        // Delete the inorder successor
        root->right = deleteNode(root->right, temp->s_ID);
    }
    return root;
}

// Function to count the number of nodes in a tree
int countNodes(struct node *root)
{
    if (root == NULL)
        return 0;
    else
        return 1 + countNodes(root->left) + countNodes(root->right);
}

// Function to convert the BST by ID to array
struct node **treeToArray(struct node *root, struct node **arr, int *index)
{
    if (root != NULL)
    {
        treeToArray(root->left, arr, index);
        arr[*index] = root;
        (*index)++;
        treeToArray(root->right, arr, index);
    }
}

// Function to sort the array lexicographically
struct node **sortArray(struct node **arr, int size)
{
    int i, j;
    struct node *temp = (struct node *)malloc(sizeof(struct node));
    for (i = 0; i < size - 1; i++)
    {
        for (j = 0; j < size - i - 1; j++)
        {
            if (strcmp(arr[j]->s_Name, arr[j + 1]->s_Name) > 0)
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    return arr;
}

// Function to insert a new student (OPTION ONE)
struct node *insertStudent(struct node *root)
{
    int s_ID_temp;
    char s_Name_temp[20];
    char c_Name_temp[20];
    int c_ID_temp;
    char e_date_temp[20];

    printf("\nEnter the information of the new student:\n");
    printf("Student ID, student name, home address, class ID and date of enrollment\n\n");

    scanf("%d %s %s %d %s", &s_ID_temp, s_Name_temp, c_Name_temp, &c_ID_temp, e_date_temp);
    root = insert(root, s_ID_temp, s_Name_temp, c_Name_temp, c_ID_temp, e_date_temp);

    printf("Success. Student have been added.\n");

    return root;
}

// Function to find a student and update its info (OPTION TWO)
struct node *findStudent(struct node *root)
{
    int ID, choice = 0;
    char s_Name_temp[20];
    char c_Name_temp[20];
    int c_ID_temp;
    char e_date_temp[20];

    printf("\nEnter the student's ID: ");
    scanf("%d", &ID);

    struct node *student = search(root, ID);

    if (student == NULL)
    {
        printf("\nStudent ID was not found.");
        return root;
    }

    printf("\nThe student's info:\n");
    printf("\n %d   %s %s   %d %s\n", student->s_ID, student->s_Name, student->c_Name, student->c_ID, student->e_date);

    printf("\nDo you want to edit the student's information?\n");
    printf("Enter 0 for no or 1 for yes: ");
    scanf("%d", &choice);
    if (choice)
    {
        printf("\nEnter the new information of the student:\n");
        printf("Student name, home address, class ID and date of enrollment:\n\n");
        scanf("%s %s %d %s", s_Name_temp, c_Name_temp, &c_ID_temp, e_date_temp);
        strcpy(student->s_Name, s_Name_temp);
        strcpy(student->c_Name, c_Name_temp);
        student->c_ID = c_ID_temp;
        strcpy(student->e_date, e_date_temp);
        printf("\nStudent's information has been updated.");
        return root;
    }
    else
    {
        return root;
    }
}

// Function to print students depending on the user's request (OPTIONS THREE - FIVE)
void printStudents(struct node **arr, int operation, int size)
{
    if (operation == 3)
        for (int i = 0; i < size; i++)
        {
            printf("\n %d \t %s \t %s \t %d \t %s", arr[i]->s_ID, arr[i]->s_Name, arr[i]->c_Name, arr[i]->c_ID, arr[i]->e_date);
        }
    else if (operation == 4)
    {
        char name[20];
        int empty = 1;
        printf("\nEnter the name of the city: ");
        scanf("%s", &name);
        for (int i = 0; i < size; i++)
        {
            if (strcmp(arr[i]->c_Name, name) == 0)
            {
                printf("\n %d \t %s \t %s \t %d \t %s", arr[i]->s_ID, arr[i]->s_Name, arr[i]->c_Name, arr[i]->c_ID, arr[i]->e_date);
                empty = 0;
            }
        }
        if (empty)
            printf("\nNo students were found.");
    }
    else if (operation == 5)
    {
        int class, empty = 1;
        printf("\nEnter the ID of the class: ");
        scanf("%d", &class);
        for (int i = 0; i < size; i++)
        {
            if (arr[i]->c_ID == class)
            {
                printf("\n %d \t %s \t %s \t %d \t %s", arr[i]->s_ID, arr[i]->s_Name, arr[i]->c_Name, arr[i]->c_ID, arr[i]->e_date);
                empty = 0;
            }
        }
        if (empty)
            printf("\nNo students were found.");
    }
}

// Function to delete a student (OPTION SIX)
struct node *deleteStudent(struct node *root)
{
    int ID;
    printf("\nEnter the student's ID: ");
    scanf("%d", &ID);

    root = deleteNode(root, ID);
    return root;
}

// Function to save the tree to the file (OPTION SEVEN)
void saveToFile(struct node *root)
{
    FILE *file = fopen("students.data.txt", "w");
    printInorder(root, file);
    printf("\nAll students have been Saved.");
    fclose(file);
}
