#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

#define SAVE_FILE "events.dat"

typedef struct TreeNode
{
  int data;
  unsigned char height;
  struct TreeNode *left;
  struct TreeNode *right;
} TreeNode;

unsigned char height(TreeNode *p)
{
  return p ? p->height : 0;
}

int bfactor(TreeNode *p)
{
  return height(p->right) - height(p->left);
}

void fixheight(TreeNode *p)
{
  unsigned char hl = height(p->left);
  unsigned char hr = height(p->right);
  p->height = (hl > hr ? hl : hr) + 1;
}

TreeNode *rotateright(TreeNode *p) // правый поворот вокруг p
{
  TreeNode *q = p->left;
  p->left = q->right;
  q->right = p;
  fixheight(p);
  fixheight(q);
  return q;
}
TreeNode *rotateleft(TreeNode *q) // левый поворот вокруг q
{
  TreeNode *p = q->right;
  q->right = p->left;
  p->left = q;
  fixheight(q);
  fixheight(p);
  return p;
}
TreeNode *balance(TreeNode *p) // балансировка узла p
{
  fixheight(p);
  if (bfactor(p) == 2)
  {
    if (bfactor(p->right) < 0)
      p->right = rotateright(p->right);
    return rotateleft(p);
  }
  if (bfactor(p) == -2)
  {
    if (bfactor(p->left) > 0)
      p->left = rotateleft(p->left);
    return rotateright(p);
  }
  return p; // балансировка не нужна
}

TreeNode *findmin(TreeNode *p) // поиск узла с минимальным ключом в дереве p
{
  return p->left ? findmin(p->left) : p;
}
TreeNode *findmax(TreeNode *p) // поиск узла с максимальным ключом в дереве p
{
  return p->right ? findmax(p->right) : p;
}

TreeNode *removemin(TreeNode *p) // удаление узла с минимальным ключом из дерева p
{
  if (p->left == 0)
    return p->right;
  p->left = removemin(p->left);
  return balance(p);
}

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
    printf("Очередь пуста\n");
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
  root->height = 1;
  root->left = NULL;
  root->right = NULL;
  return root;
}

TreeNode *addElement(int data, TreeNode *root)
{
  if (root == NULL)
  {
    root = createNode(data);
  }
  else if (root->data == data)
  {
    printf("Такой элемент уже есть в дереве\n");
    return root;
  }
  else if (data < root->data)
  {
    if (root->left)
      root->left = addElement(data, root->left);
    else
      root->left = createNode(data);
  }
  else
  {
    if (root->right)
      root->right = addElement(data, root->right);
    else
      root->right = createNode(data);
  }
  return balance(root);
}

TreeNode *removeNode(TreeNode *p, int k) // удаление ключа k из дерева p
{
  if (!p)
  {
    printf("Узел %d не был найден!\n", k);
    return 0;
  }
  if (k < p->data)
    p->left = removeNode(p->left, k);
  else if (k > p->data)
    p->right = removeNode(p->right, k);
  else //  k == p->key
  {
    TreeNode *q = p->left;
    TreeNode *r = p->right;
    free(p);
    p = NULL;

    printf("Узел %d успешно удален!\n", k);

    if (!r)
      return q;
    TreeNode *min = findmin(r);
    min->right = removemin(r);
    min->left = q;

    return balance(min);
  }
  return balance(p);
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

void freeTree(TreeNode *root)
{
  if (root == NULL)
    return;
  freeTree(root->left);
  freeTree(root->right);
  free(root);
}
void serialize(struct TreeNode *root, FILE *fp)
{
  if (root == NULL)
  {
    int marker = -1;
    fwrite(&marker, sizeof(int), 1, fp);
    return;
  }
  fwrite(&(root->data), sizeof(int), 1, fp);
  serialize(root->left, fp);
  serialize(root->right, fp);
}

TreeNode *deserialize(FILE *fp)
{
  int val;

  if (fread(&val, sizeof(int), 1, fp) != 1 || val == -1)
  {
    return NULL;
  }
  TreeNode *root = createNode(val);

  root->left = deserialize(fp);
  root->right = deserialize(fp);
  fixheight(root);

  return root;
}

int main()
{
  TreeNode *root = NULL;

  setlocale(LC_ALL, "Rus");

  int choice;

  printf("=== AVL ===\n");

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
    printf("8. Удаление узла\n");
    printf("9. Максимальный узел дерева\n");
    printf("10. Минимальный узел дерева\n");
    printf("11. Высота дерева\n");
    printf("12. Сохранение дерева в файл\n");
    printf("13. Загрузка дерева из файла\n");
    printf("0. Выход\n");
    printf("Выбор > ");

    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
      printf("Введите число которое вы хотите добавить в дерево: ");
      int n;
      scanf("%d", &n);
      root = addElement(n, root);
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
      printf("Введите узел который вы хотите найти в дереве: ");
      int value;
      scanf("%d", &value);
      find(value, root);
      break;
    case 8:
      if (root == NULL)
      {
        printf("Дерево пустое!\n");
        break;
      }
      printf("Введите узел который вы хотите удалить в дереве: ");
      int node;
      scanf("%d", &node);
      root = removeNode(root, node);

      break;
    case 9:
    {
      if (root == NULL)
      {
        printf("Дерево пустое!\n");
        break;
      }
      int maxNode = findmax(root)->data;
      printf("Максимальный узел: %d\n", maxNode);
      break;
    }
    case 10:
    {
      if (root == NULL)
      {
        printf("Дерево пустое!\n");
        break;
      }
      int minNode = findmin(root)->data;
      printf("Минимальный узел: %d\n", minNode);
      break;
    }
    case 11:
      if (root == NULL)
      {
        printf("Дерево пустое!\n");
        break;
      }
      printf("Высота дерева равна %d\n", height(root));
      break;
    case 12:
    {
      FILE *fpw = fopen(SAVE_FILE, "wb");
      if (fpw)
      {
        serialize(root, fpw);
        fclose(fpw);
        printf("Дерево сохранено в файл %s\n", SAVE_FILE);
      }
      else
      {
        printf("Ошибка при открытии файла для записи\n");
      }
    }
    break;
    case 13:
    {
      FILE *fpr = fopen(SAVE_FILE, "rb");
      if (fpr)
      {
        freeTree(root);
        root = deserialize(fpr);
        fclose(fpr);
        printf("Дерево загружено из файла %s\n", SAVE_FILE);
      }
      else
      {
        printf("Ошибка при открытии файла для чтения\n");
      }
    }
    break;
    case 0:
      printf("Выход...\n");
      break;
    default:
      printf("Неверный пункт.\n");
    }

  } while (choice != 0);
  freeTree(root);
  return 0;
}
