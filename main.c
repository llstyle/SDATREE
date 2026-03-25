#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 100

typedef struct TreeNode
{
  int data;
  struct TreeNode *left;
  struct TreeNode *right;
} TreeNode;

typedef struct QueueNode
{
  struct TreeNode *root;
  struct QueueNode *next;
} QueueNode;

QueueNode *front = NULL;
QueueNode *rear = NULL;

void enqueue(TreeNode *value)
{
  if (!value)
    return;
  if (front == NULL && front == rear)
  {
    front = malloc(sizeof(QueueNode));
    front->root = value;
    front->next = NULL;
    rear = front;
    return;
  }
  rear->next = malloc(sizeof(QueueNode));
  rear = rear->next;
  rear->next = NULL;
  rear->root = value;
}

void dequeue()
{
  if (front == NULL && front == rear)
  {
    printf("Queue is empty\n");
    return;
  }
  QueueNode *tmp = front;
  if (!tmp->next)
  {
    free(front);
    front = NULL;
    rear = NULL;
    return;
  }
  front = tmp->next;
  if (!front)
    rear = NULL;
  free(tmp);
}

void dfs(TreeNode *root)
{
  if (!root)
    return;
  dfs(root->left);
  printf("%d ", root->data);
  dfs(root->right);
}
void bfs(TreeNode *root)
{
  if (!root)
    return;
  enqueue(root);
  while (front != NULL)
  {
    TreeNode *tmp = front->root;
    printf("%d ", tmp->data);
    enqueue(tmp->left);
    enqueue(tmp->right);
    dequeue();
  }
}
void preorder(TreeNode *root)
{
  if (!root)
    return;
  printf("%d ", root->data);
  preorder(root->left);
  preorder(root->right);
}
void inorder(TreeNode *root)
{
  if (!root)
    return;
  inorder(root->left);
  printf("%d ", root->data);
  inorder(root->right);
}
void postorder(TreeNode *root)
{
  if (!root)
    return;
  postorder(root->left);
  postorder(root->right);
  printf("%d ", root->data);
}
TreeNode *createNode(int data)
{
  TreeNode *root = malloc(sizeof(TreeNode));
  root->data = data;
  root->left = NULL;
  root->right = NULL;
  return root;
}
void addElement(int data, TreeNode **root)
{
  if (*root == NULL)
  {
    *root = createNode(data);
  }
  else if ((*root)->data == data)
  {
    printf("Такой элемент уже есть в дереве\n");
  }
  else if (data < (*root)->data)
  {
    if ((*root)->left)
      addElement(data, &((*root)->left));
    else
      (*root)->left = createNode(data);
  }
  else
  {
    if ((*root)->right)
      addElement(data, &((*root)->right));
    else
      (*root)->right = createNode(data);
  }
}

void find(int value, TreeNode *root)
{
  if (!root)
  {
    printf("Число не найдено!");
    return;
  }
  if (root->data == value)
  {
    printf("%d\n", root->data);
    return;
  }
  printf("%d -> ", root->data);
  if (value > root->data)
    find(value, root->right);
  else
    find(value, root->left);
}

int main()
{
  TreeNode *root = NULL;

  setlocale(LC_ALL, "Rus");

  int choice;

  printf("=== BST ===\n");

  do
  {
    printf("\n=== МЕНЮ ===\n");
    printf("1. Добавить новый элемент в дерево\n");
    printf("2. Прямой обход\n");
    printf("3. Центрированный обход\n");
    printf("4. Обратный обход\n");
    printf("5. Обход в глубину\n");
    printf("6. Обход в ширину\n");
    printf("7. Поиск\n");
    printf("0. Выход\n");
    printf("Выбор > ");

    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
      printf("Введите число которое вы хотите добавить в дерево: ");
      int n;
      scanf("%d", &n);
      addElement(n, &root);
      break;
    case 2:
      preorder(root);
      break;
    case 3:
      inorder(root);
      break;
    case 4:
      postorder(root);
      break;
    case 5:
      dfs(root);
      break;
    case 6:
      bfs(root);
      break;
    case 7:
      printf("Введите число которое вы хотите найти в дереве: ");
      int value;
      scanf("%d", &value);
      find(value, root);
      break;
    case 0:
      printf("Выход...\n");
      break;
    default:
      printf("Неверный пункт.\n");
    }

  } while (choice != 0);

  return 0;
}
