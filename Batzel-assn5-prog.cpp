//------------------------------------------------------------------------------
// CODE FILENAME: Batzel-assn5-prog.cpp
// DESCRIPTION:   The purpose of this program is to maintain a binary search tree(BST).
//                An input text file containing integers is supplied by the user
//                and through various BST functions, is inserted into the tree.
//                Once created, the user may add, delete, or view nodes contained
//                in the tree.
// CLASS/TERM:    CP372/14S8W2
// DESIGNER:      Andrew Batzel
// FUNCTIONS:     getFile - Prompts the user for an input file.
//                fileExists - Tests to see if the file exists.
//                isEmptyFile - Tests to see if there is any data in the file.
//                getData - Reads from file to generate search tree.
//                createTree - Allocates memory for the main tree structure.
//                isEmptyTree - Tests to see if the tree is NULL.
//                createNode - Allocates memory for nodes within the tree.
//                insertNode - Inserts a node into the correct location in the tree.
//                findNode - Searches for a target node in the tree.
//                displayMenu - Displays the actions available to the user.
//                actionController - Makes function calls based on the users chosen action.
//                deleteNode - Finds the location of a target node that will be deleted.
//                deleteFromTree - Removes a node from the search tree.
//                nodeCount - Accesses the count element of the tree structure.
//                inOrderDisplay - Traverses the tree in ascending order.
//                formatDisplay - Displays a number within the tree and ensures only 10 numbers are on each row.
//                freeNodes - Deallocates all nodes within the search tree.
//                destroyTree - Deallocates the main tree structure.
//------------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstddef>
#include <cctype>

using namespace std;

// global constants
const int MAX_COLUMNS = 10,
          INIT_COLUMN = 0;
const char EXIT_CHAR = 'E';

// abstract data types

struct treeNode {
                   int number;
                   treeNode *leftPtr;
                   treeNode *rightPtr;
                };

struct binarySearchTree {
                            int count;
                            treeNode *root;
                        };

// function prototypes
void getFile(ifstream& dataIn);
bool fileExists(ifstream& dataIn);
bool isEmptyFile(ifstream& dataIn);
void getData(ifstream& dataIn, binarySearchTree *&mainTree, bool& memoryFail);
binarySearchTree *createTree();
bool isEmptyTree(binarySearchTree *mainTree);
treeNode *createNode(int num);
void insertNode(binarySearchTree *&mainTree, treeNode *newNode);
treeNode *findNode(binarySearchTree *&mainTree, int num, bool& flag);
char displayMenu(binarySearchTree *&mainTree);
void actionController(binarySearchTree *&mainTree, char& treeAction);
void deleteNode(binarySearchTree *&mainTree, int num);
void deleteFromTree(treeNode *&nodeToRemove);
int nodeCount(binarySearchTree *mainTree);
void inOrderDisplay(treeNode *node, int& currentColumn);
void formatDisplay(int num, int& currentColumn);
void freeNodes(treeNode *&node);
void destroyTree(binarySearchTree *&mainTree);

//------------------------------------------------------------------------------
// FUNCTION:     main
// DESCRIPTION:  Declares some of the main variable used in the program and makes
//               key function calls to control the flow of the program.
// INPUT:        N/A
// OUTPUT:
//     Return Val:  Returns 0 upon successful execution of the program.
// CALLS TO:     createTree
//               getFile
//               isEmptyfile
//               getData
//               displayMenu
//               actionController
//               freeNodes
//               destroyTree
//------------------------------------------------------------------------------

int main()
{
    binarySearchTree *mainTree;
    ifstream dataIn;
    char treeAction;
    bool memoryFail = false;
    
    // create an empty binary search tree
    mainTree = createTree();
    
    if (mainTree)
    {
        // Prompt user for a file name & loop until the name of the file exists
        getFile(dataIn);
        
        // Read data if file is not empty
        if (!isEmptyFile(dataIn))
        {
            getData(dataIn, mainTree, memoryFail);
        }
    } // end if memory correctly allocated for mainTree
    
    if (!memoryFail)
    {
        treeAction = displayMenu(mainTree);
        
        // loop through menu and user selection until exit is selected
        while (treeAction != 'E')
        {
              actionController(mainTree, treeAction);
              if (treeAction != 'E')
              {
                  treeAction = displayMenu(mainTree);
              }// end if treeAction defults to 'E' because of memory allocation failure
        } // end while user doesn't exit program
        
        // deallocate all nodes from tree
    } // end if memory allocations were successful
    else
    {
        cout << "ERROR - A memory allocation failure has occurred." << endl;
        system("pause");
    } // end if memory allocation failed
    
    // deallocate nodes in tree structure
    freeNodes(mainTree->root);
    
    // deallocate main tree structure
    destroyTree(mainTree);

    return 0;
}

//------------------------------------------------------------------------------
// FUNCTION:     getFile
// DESCRIPTION:  Prompts the user for an input file and opens it for testing by
//               fileExists function.
// INPUT:
//     Parameters:  dataIn - Reading input stream variable.
// OUTPUT:
//     Parameters:  dataIn - Same as input, passed by reference.
// CALLS TO:     fileExists
//------------------------------------------------------------------------------


void getFile(ifstream& dataIn)
{
     string fileName;
     
     do
     {
         if (!fileExists(dataIn))
         {
             cout << "File does not exist, enter  a file that does exist." << endl;
             dataIn.clear();
         }
         cout << "Enter a file name for integer data: ";
         cin >> fileName;
         
         dataIn.open (fileName.c_str());
     } while (!fileExists(dataIn));
     
     return;
}

//------------------------------------------------------------------------------
// FUNCTION:     fileExists
// DESCRIPTION:  Tests that a file exists.
// INPUT:
//     Parameters:  dataIn - Reading input stream variable.
// OUTPUT:
//     Parameters:  dataIn - Same as input, passed by reference.
//     Return Val:  exists - Boolean value of whether or not the file exists.
// CALLS TO:     N/A
//------------------------------------------------------------------------------


bool fileExists(ifstream& dataIn)
{
     bool exists;
     
     if (dataIn)
     {
         exists = true;
     }
     else
     {
         exists = false;
     }
     
     return exists;
}

//------------------------------------------------------------------------------
// FUNCTION:     isEmptyFile
// DESCRIPTION:  Tests if there is any data in the input file.
// INPUT:
//     Parameters:  dataIn - Reading input stream variable.
// OUTPUT:
//     Parameters:  dataIn - Same as input, passed by reference.
//     Return Val:  empty - Boolean value of whether or not the file contains data.
// CALLS TO:     N/A
//------------------------------------------------------------------------------


bool isEmptyFile(ifstream& dataIn)
{
     bool empty;

     if (dataIn.peek() != EOF)
     {
         empty = false;
     }
     else
     {
         empty = true;
     }
     
     return empty;
}

//------------------------------------------------------------------------------
// FUNCTION:     getData
// DESCRIPTION:  Reads data from the input file into a BST.
// INPUT:
//     Parameters:  dataIn - Reading input stream variable.
//                  mainTree - A pointer to the BST structure.
//                  memoryFail - Boolean value of memory allocation success/fail.
// OUTPUT:
//     Parameters:  dataIn - Same as input, passed by reference.
//                  mainTree - Same as input, passed by reference.
//                  memoryFail - Same as input, passed by reference.
// CALLS TO:     findNode
//               createNode
//               insertNode
//------------------------------------------------------------------------------

void getData(ifstream& dataIn, binarySearchTree *&mainTree, bool& memoryFail)
{
     int number;
     bool flag;
     treeNode *newNode;

     dataIn >> number;
     
     while (dataIn && !memoryFail)
     {
         findNode(mainTree, number, flag);
         
         if (!flag)
         {
             newNode = createNode(number);
             
             if (newNode)
             {
                 insertNode(mainTree, newNode);
             } // end if memory allocated for new node
             else
             {
                 memoryFail = true;
             } // end memory not allocated
         } // end if number doesn't already exist in the tree
         else
         {
             cout << endl << number << " already exists in tree and will be ignored." << endl;
         }
         
         dataIn >> number;
     } // read data from input file until last number
     
     dataIn.close();
     
     return;
}

//------------------------------------------------------------------------------
// FUNCTION:     createTree
// DESCRIPTION:  Allocated memory for the main BST structure and initializes its
//               variables.
// INPUT:        N/A
// OUTPUT:
//     Return Val:  newTree - A pointer to main BST structure.
// CALLS TO:     N/A
//------------------------------------------------------------------------------


binarySearchTree *createTree()
{
    binarySearchTree *newTree;
    
    newTree = new (nothrow) binarySearchTree;
    
    if (newTree)
    {
        newTree->count = 0;
        newTree->root = NULL;
    }
    
    return newTree;
}

//------------------------------------------------------------------------------
// FUNCTION:     isEmptyTree
// DESCRIPTION:  Tests the root pointer of BST structure to see if its NULL.
// INPUT:
//     Parameters:  mainTree - A pointer to the BST structure.
// OUTPUT:
//     Return Val:  empty - Boolean value of whether the tree is empty or not
// CALLS TO:     N/A
//------------------------------------------------------------------------------


bool isEmptyTree(binarySearchTree *mainTree)
{
     bool empty;
     
     if (mainTree->root == NULL)
     {
         empty = true;
     }
     else
     {
         empty = false;
     }
     
     return empty;
}

//------------------------------------------------------------------------------
// FUNCTION:     createNode
// DESCRIPTION:  Allocates memory for a node within the BST and initializes the
//               variables in the nodes structure.
// INPUT:
//     Paramaters:  num - An integer that will be in the node.
// OUTPUT:
//     Return Val:  newNode - A pointer to the newly created node.
// CALLS TO:     N/A
//------------------------------------------------------------------------------

treeNode *createNode(int num)
{
   treeNode *newNode;
   
   newNode = new (nothrow) treeNode;
   
   if (newNode)
   {
       newNode->number = num;
       newNode->leftPtr = NULL;
       newNode->rightPtr = NULL;
   }
   
   return newNode;      
}

//------------------------------------------------------------------------------
// FUNCTION:     insertNode
// DESCRIPTION:  Adds a node into the BST in ascending order.
// INPUT:
//     Parameters:  mainTree - A pointer to the main BST structure.
//                  newNode - A pointer to the new node being inserted.
// OUTPUT:
//     Parameters:  mainTree - Same as input, passed by reference.
// CALLS TO:     isEmptyTree     
//------------------------------------------------------------------------------

void insertNode(binarySearchTree *&mainTree, treeNode *newNode)
{
     treeNode *current,
              *trailCurrent;
     bool flag;
     
     // make new node the root if the tree is empty
     if (isEmptyTree(mainTree))
     {
         mainTree->count++;
         mainTree->root = newNode;
     } // end if tree is empty
     else
     {
         mainTree->count++;
         current = mainTree->root;
         while (current != NULL)
         {
               trailCurrent = current;
               if (current->number > newNode->number)
               {
                   current = current->leftPtr;
               } // end if new number is less than current node number
               else
               {
                   current = current->rightPtr;
               }// end if new number is greater than current node number
         } // end while current points to a value
         
         if (trailCurrent->number > newNode->number)
         {
             trailCurrent->leftPtr = newNode;
         }
         else
         {
             trailCurrent->rightPtr = newNode;
         }
     }
     
     return;
}

//------------------------------------------------------------------------------
// FUNCTION:     findNode
// DESCRIPTION:  Traverses the BST until a target node is found, or all elements
//               have been inspected.
// INPUT:
//     Parameters:  mainTree - A pointer to the main BST structure.
//                  num - The integer that is the target value.
//                  flag - Boolean value of whether the value was found or not.
// OUTPUT:
//     Parameters:  mainTree - Same as input, passed by reference. 
//                  flag - Same as input, passed by reference.
//     Return Val:  testNode - A pointer to the node if found, NULL if not.
// CALLS TO:     isEmptyTree
//------------------------------------------------------------------------------

treeNode *findNode(binarySearchTree *&mainTree, int num, bool& flag)
{
    flag = false;
    treeNode *testNode;
    
    testNode = mainTree->root;
    
    if (isEmptyTree(mainTree))
    {
        flag = false;
    } // end if tree is empty
    else
    {
        while ((testNode != NULL) && !flag)
        {
              if (testNode->number == num)
              {
                  flag = true;
              } // end if target number is found
              else if (testNode->number > num)
              {
                  testNode = testNode->leftPtr;
              } // end if node number is greater than target number
              else
              {
                  testNode = testNode->rightPtr;
              } // end if node number is less than target number
        } // end while pointer contains a value and is not the number searched for
    } // end if tree is not empty
    
    return testNode;
}

//------------------------------------------------------------------------------
// FUNCTION:     displayMenu
// DESCRIPTION:  Displays the actions available to the user and loops users input
//               until a valid one is selected.
// INPUT:
//     Parameters:  mainTree - A pointer to the main BST structure.
// OUTPUT:
//     Parameters:  mainTree - Same as input, passed by reference.
//     Return Val:  menuChoice - A character of the users selected action.
// CALLS TO:     isEmptyTree
//               nodeCount
//------------------------------------------------------------------------------

char displayMenu(binarySearchTree *&mainTree)
{
     char menuChoice;
     
     if (!isEmptyTree(mainTree))
     {
         cout << "\nThere are " << nodeCount(mainTree) << " integers in the binary search tree." << endl << endl;
     }
     else
     {
         cout << "\nBinary search tree is empty." << endl << endl;
     }
     
     cout << "--------------------Menu Options--------------------" << endl
          << "S - Show all integerrs in the binary search tree." << endl
          << "A - Add an integer to the tree." << endl
          << "D - Delete an integer from the tree." << endl
          << "F - Find an integer and display its subtree." << endl
          << "E - Exit the program." << endl;
     do
     {
          cout << "Enter a choice from the options above: ";
          cin >> menuChoice;
          menuChoice = toupper(menuChoice);
          if ((menuChoice != 'S') && (menuChoice != 'A') && (menuChoice != 'D') && (menuChoice != 'F') && (menuChoice != 'E'))
          {
              cout << "ERROR - Invalid character selection." << endl;
          }
     }while ((menuChoice != 'S') && (menuChoice != 'A') && (menuChoice != 'D') && (menuChoice != 'F') && (menuChoice != 'E'));
     
     return menuChoice;
}

//------------------------------------------------------------------------------
// FUNCTION:     actionController
// DESCRIPTION:  Makes calls to the functions selected by the user.
// INPUT:
//     Parameters:  mainTree - A pointer to the main BST structure.
//                  treeAction - A character of what action will be taken.
// OUTPUT:
//     Parameters:  mainTree - Same as input, passed by reference.
//                  treeAction - Same as input, passed by reference.
// CALLS TO:     isEmptyTree
//               inOrderDisplay
//               findNode
//               createNode
//               insertNode
//               deleteNode
//------------------------------------------------------------------------------

void actionController(binarySearchTree *&mainTree, char& treeAction)
{
     treeNode *miscNode;
     int num,
         initColumn = INIT_COLUMN;
     bool flag;
     
     switch(treeAction)
     {
         case 'S':
              cout << "\nValues stored in entire binary search tree are:" << endl;
              if (!isEmptyTree(mainTree))
              {
                  inOrderDisplay(mainTree->root, initColumn);
                  initColumn = INIT_COLUMN;
                  cout << endl << endl;
              }
              break;
              
         case 'A':
              do
              {
                  cout << "Enter a number to add to the tree: ";
                  cin >> num;
              }while (num <= 0);
              miscNode = findNode(mainTree, num, flag);
              if (!flag)
              {
                  miscNode = createNode(num);
                  if (miscNode)
                  {
                      insertNode(mainTree, miscNode);
                      cout << num << " added to tree." << endl;
                  } // end if memory allocated for new node
                  else
                  {
                      cout << "ERROR - A memory allocation failure has occurred." << endl;
                      treeAction = EXIT_CHAR;
                  } // end memory not allocated
              }
              else
              {
                  cout << "Number already exists in tree and cannot be added." << endl;
              }
              system("pause");
              system("cls");
              break;
              
         case 'D':
              cout << "Enter a number to delete from the tree: ";
              cin >> num;
              miscNode = findNode(mainTree, num, flag);
              if (miscNode != NULL)
              {
                  deleteNode(mainTree, miscNode->number);
                  mainTree->count--;
                  cout << num << " deleted from tree." << endl;
              }
              else
              {
                  cout << num << " does not exist in the binary search tree." << endl;
              }
              system("pause");
              system("cls");
              break;
              
         case 'F':
              cout << "Enter a number to find: ";
              cin >> num;
              miscNode = findNode(mainTree, num, flag);
              if (miscNode != NULL)
              {
                  cout << "Values stored subtree with root " << num << " are:" << endl;
                  inOrderDisplay(miscNode, initColumn);
                  cout << endl;
                  initColumn = INIT_COLUMN;
              }
              else
              {
                 cout << num << " doen not exist in the binary search tree." << endl;
              }
              system("pause");
              system("cls");
              break;
     }
     
     return;
}

//------------------------------------------------------------------------------
// FUNCTION:     deleteNode
// DESCRIPTION:  Traverses through the BST finding a target node that will be deleted.
// INPUT:
//     Parameters:  mainTree - A pointer to the main BST structure.
//                  num - An integer of the target value to be deleted
// OUTPUT:
//     Parameters:  mainTree - Same as input, passed by reference.
// CALLS TO:     deleteFromTree
//------------------------------------------------------------------------------

void deleteNode(binarySearchTree *&mainTree, int num)
{
     treeNode *current,
              *trail;
     bool found = false;
     
     current = mainTree->root;
     trail = mainTree->root;
     
     while ((current != NULL) && !found)
     {
           if (current->number == num)
           {
               found = true;
           }
           else
           {
               trail = current;
               
               if (current->number > num)
               {
                   current = current->leftPtr;
               }
               else
               {
                   current = current->rightPtr;
               }
           }
     }
     
     if (current == mainTree->root)
     {
         deleteFromTree(mainTree->root);
     }
     else if (trail->number > num)
     {
          deleteFromTree(trail->leftPtr);
     }
     else
     {
         deleteFromTree(trail->rightPtr);
     }
     
     return;
}

//------------------------------------------------------------------------------
// FUNCTION:     deleteFromTree
// DESCRIPTION:  Deletes a node from the main BST structure.
// INPUT:
//     Parameters:  nodeToRemove - A pointer to the node that will be deleted.
// OUTPUT:
//     Parameters:  nodeToRemove - Same as input, passed by reference.
// CALLS TO:     N/A
//------------------------------------------------------------------------------

void deleteFromTree(treeNode *&nodeToRemove)
{
     treeNode *tempPtr,
              *current,
              *trail;
     
     if ((nodeToRemove->leftPtr == NULL) && (nodeToRemove->rightPtr == NULL))
     {
         tempPtr = nodeToRemove;
         nodeToRemove = NULL;
         delete tempPtr;
     }
     else if (nodeToRemove->leftPtr == NULL)
     {
          tempPtr = nodeToRemove;
          nodeToRemove = tempPtr->rightPtr;
          delete tempPtr;
     }
     else if (nodeToRemove->rightPtr == NULL)
     {
          tempPtr = nodeToRemove;
          nodeToRemove = tempPtr->leftPtr;
          delete tempPtr;
     }
     else
     {
         current = nodeToRemove->leftPtr;
         trail = NULL;
         
         while (current->rightPtr != NULL)
         {
               trail = current;
               current = current->rightPtr;
         }
         
         nodeToRemove->number = current->number;
         
         if (trail == NULL)
         {
             nodeToRemove->leftPtr = current->leftPtr;
         }
         else
         {
             trail->rightPtr = current->leftPtr;
         }
         delete current;
     }
     
     return;
}

//------------------------------------------------------------------------------
// FUNCTION:     nodeCount
// DESCRIPTION:  Accesses the count variable in the main BST structure.
// INPUT:
//     Parameters:  mainTree - A pointer to the main BST structure.
// OUTPUT:
//     Return Val:  num - An integer of the total nodes in the BST.
// CALLS TO:     N/A
//------------------------------------------------------------------------------

int nodeCount(binarySearchTree *mainTree)
{
    int num = 0;
    
    num = mainTree->count;
    
    return num;
}

//------------------------------------------------------------------------------
// FUNCTION:     inOrderDisplay
// DESCRIPTION:  Traverses the BST using inorder traversal to display all nodes
//               in ascending order.
// INPUT:
//     Parameters:  node - A pointer to a node within the BST.
//                  currentColumn - An integers of how many columns have been displayed.
// OUTPUT:
//     Parameters:  currentColumn - Same as input, passed by reference.
// CALLS TO:     inOrderDisplay
//               formatDisplay
//------------------------------------------------------------------------------

void inOrderDisplay(treeNode *node, int& currentColumn)
{    
     if (node != NULL)
     {
         inOrderDisplay(node->leftPtr, currentColumn);
         formatDisplay(node->number, currentColumn);     
         inOrderDisplay(node->rightPtr, currentColumn);
     }
     
     return;
}

//------------------------------------------------------------------------------
// FUNCTION:     formatDisplay
// DESCRIPTION:  Displays an integer ensuring that there are 10 per row.
// INPUT:
//     Parameters:  num - An integer from the BST to be displayed.
//                  currentColumn - An integer of how many columns have been displayed.
// OUTPUT:
//     Parameters:  currentColumn - Same as input, passed by reference.
// CALLS TO:     N/A
//------------------------------------------------------------------------------

void formatDisplay(int num, int& currentColumn)
{
     if (currentColumn >= MAX_COLUMNS)
     {
         currentColumn = INIT_COLUMN;
         cout << endl;
     }
     cout << right << setw(6) << num;
     currentColumn++;
     
     return;
}

//------------------------------------------------------------------------------
// FUNCTION:     freeNodes
// DESCRIPTION:  Deallocates nodes from BST using postorder traversal.
// INPUT:
//     Parameters:  node - A pointer to a node in the BST.
// OUTPUT:
//     Parameters:  node - Same as input, passed by reference.
// CALLS TO:     freeNodes
//               deleteFromTree
//------------------------------------------------------------------------------

void freeNodes(treeNode *&node)
{
     if (node != NULL)
     {
         freeNodes(node->leftPtr);
         freeNodes(node->rightPtr);
         deleteFromTree(node);
     }
     
     return;
}

//------------------------------------------------------------------------------
// FUNCTION:     destroyTree
// DESCRIPTION:  Deallocates main BST structure from memory.
// INPUT:
//     Parameters:  mainTree - A pointer to the main BST structure.
// OUTPUT:
//     Parameters:  mainTree - Same as input, passed by reference.
// CALLS TO:     N/A
//------------------------------------------------------------------------------

void destroyTree(binarySearchTree *&mainTree)
{
     delete mainTree;
     
     return;
}
