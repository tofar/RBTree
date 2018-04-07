#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*定义结点的颜色*/
#define RED true
#define BLACK false

typedef struct Node
{
    struct Node *parent; //父节点
    struct Node *left;   //左子树
    struct Node *right;  //右子树
    int value;           //value
    bool color;          //节点颜色
} * RBTree;

/*获取祖父节点*/
RBTree getAncestor(RBTree node);
/*获取叔父结点*/
RBTree getUncle(RBTree node);
/*左旋*/
void rotateLeft(RBTree *root, RBTree node);
/*右旋*/
void rotateRight(RBTree *root, RBTree node);
/*查找结点*/
RBTree findNodeByValue(RBTree root, int value);
/*查找最小的结点*/
RBTree getMinNode(RBTree root);
/*查找最大的结点*/
RBTree getMaxNode(RBTree root);

/*通过value插入一个结点*/
void addNodeByValue(RBTree *root, int value);
/*插入修复*/
void fixInsert(RBTree *root, RBTree node);

/*通过value删除节点*/
void removeNodeByValue(RBTree *root, int value);
/*删除结点*/
void removeNode(RBTree *root, RBTree node);
/*真正删除节点*/
RBTree getRealRemoveNode(RBTree *root, RBTree node);
/*删除修复*/
void fixRemove(RBTree *root, RBTree node);

/*打印红黑树*/
void printRBTree(RBTree root);

/*定义节点空指针*/
RBTree nil = NULL;

/*获取祖父节点*/
RBTree getAncestor(RBTree node)
{
    if (node->parent != nil)
    {
        return node->parent->parent;
    }
    return NULL;
}
/*获取叔父结点*/
RBTree getUncle(RBTree node)
{
    RBTree ancestor = getAncestor(node);
    if (ancestor != nil)
    {
        if (ancestor->left == node->parent)
        {
            return ancestor->right;
        }
        return ancestor->left;
    }
}
/*左旋*/
void rotateLeft(RBTree *root, RBTree node)
{
    if (node->right != nil)
    {
        //当前节点的右节点
        RBTree right = node->right;
        node->right = right->left;
        if (right->left != nil)
        {
            right->left->parent = node;
        }
        right->parent = node->parent;

        if (node->parent == nil)
        {
            //node本来为根节点，此时变化为原来node的右节点
            *root = right;
        }
        else
        {
            if (node == node->parent->left)
            {
                node->parent->left = right;
            }
            else
            {
                node->parent->right = right;
            }
        }
        right->left = node;
        node->parent = right;
    }
}
/*右旋*/
void rotateRight(RBTree *root, RBTree node)
{
    if (node->left != nil)
    {
        //当前节点的左节点
        RBTree left = node->left;
        node->left = left->right;
        if (left->right != nil)
        {
            left->right->parent = node;
        }
        left->parent = node->parent;
        if (node->parent == nil)
        {
            //node本来为根节点，此时变化为原来node的左节点
            *root = left;
        }
        else
        {
            if (node == node->parent->left)
            {
                node->parent->left = left;
            }
            else
            {
                node->parent->right = left;
            }
        }
        left->right = node;
        node->parent = left;
    }
}

/*通过value插入一个结点*/
void addNodeByValue(RBTree *root, int value)
{
    if ((*root) == NULL)
    {
        *root = (RBTree)malloc(sizeof(struct Node));
        //初始化nil结点
        nil = (RBTree)malloc(sizeof(struct Node));
        nil->color = BLACK;
        //设置结点的指向
        (*root)->parent = nil;
        (*root)->left = nil;
        (*root)->right = nil;
        //设置结点属性,value 和color
        (*root)->value = value;
        (*root)->color = BLACK;
    }
    else
    {
        RBTree node = *root;
        /*插入节点的父节点*/
        RBTree p = nil;
        while (node != nil)
        {
            p = node;
            if (value > node->value)
                node = node->right;
            else if (value < node->value)
                node = node->left;
            else
                return;
        }
        node = (RBTree)malloc(sizeof(struct Node));
        node->parent = p;
        node->left = nil;
        node->right = nil;

        node->value = value;
        node->color = RED;
        if (value < p->value)
        {
            p->left = node;
        }
        else
        {
            p->right = node;
        }
        fixInsert(root, node);
    }
}

/*插入修复*/
void fixInsert(RBTree *root, RBTree node)
{
    RBTree uncle;
    while (node->parent->color == RED)
    {
        RBTree ancestor = getAncestor(node);
        if (node->parent == ancestor->left)
        {
            uncle = ancestor->right; //获取叔父结点
            if (uncle->color == RED)
            { //case 1  如果叔父结点为红色(此时祖父节点一定为黑),则把父节点和叔父结点着黑,祖父结点着红,node上移到祖父结点
                uncle->color = BLACK;
                node->parent->color = BLACK;
                ancestor->color = RED;
                node = ancestor;
            }
            else
            {
                if (node == node->parent->right)
                { //case 3  如果叔父结点为黑色, node为右结点, 循环变量node变为node的父亲结点,左旋转变化后的node结点, 变为case2的情况
                    node = node->parent;
                    rotateLeft(root, node);
                }
                //case 2 叔父结点为黑色,且node为左结点,node的父节点着黑,node的祖父着红,然后旋转node的祖父结点
                node->parent->color = BLACK;
                ancestor->color = RED;
                rotateRight(root, ancestor);
            }
        }
        else
        { //对称 修复同上
            uncle = ancestor->left;
            if (uncle->color == RED)
            {
                uncle->color = BLACK;
                node->parent->color = BLACK;
                ancestor->color = RED;
                node = ancestor;
            }
            else
            {
                if (node == node->parent->left)
                {
                    node = node->parent;
                    rotateRight(root, node);
                }
                node->parent->color = BLACK;
                ancestor->color = RED;
                rotateLeft(root, ancestor);
            }
        }
    }
    //case 1中可能会把node变为root，而node->color为RED，故需要此步确保正确
    (*root)->color = BLACK;
}

void removeNodeByValue(RBTree *root, int value)
{
    //root 为指向根节点的指针的指针， *root 为指向根节点的指针
    removeNode(root, findNodeByValue(*root, value));
}

void removeNode(RBTree *root, RBTree node)
{
    if (node == nil || node == NULL)
        return;
    RBTree y; //实际要删除的节点
    RBTree x; //实际要删除结点的子结点
    if (node->left == nil || node->right == nil)
    {
        y = node;
    }
    else
    {
        y = getRealRemoveNode(root, node);
    }

    if (y->left != nil)
        x = y->left;
    else
        x = y->right;

    //删除结点y
    x->parent = y->parent;
    if (y->parent == nil)
    {
        *root = x;
    }
    else
    {
        if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;
    }

    if (y != node)
    {
        //替换node和y的valu
        //由于本来要删除node，现在转换为删除node右节点的最小节点y，再把node的value换为y的value即可
        node->value = y->value;
    }
    //如果删除的结点是黑色,违法了性质5,要进行删除修复
    if (y->color == BLACK)
    {
        fixRemove(root, x);
    }
    free(y);
}

/*查找实际要删除的结点*/
RBTree getRealRemoveNode(RBTree *root, RBTree node)
{
    //查找右节点的最小节点
    RBTree p = node->right;
    while (p->left != nil)
    {
        p = p->left;
    }
    return p;
}

/*删除修复*/
void fixRemove(RBTree *root, RBTree node)
{
    while (node != (*root) && node->color == BLACK)
    {
        if (node == node->parent->left)
        {
            RBTree brother = node->parent->right; //brother为node的兄弟结点
            if (brother->color == RED)
            { //case 1 兄弟结点为红色
                brother->color = BLACK;
                node->parent->color = RED;
                rotateLeft(root, node->parent);
                brother = node->parent->right; //brother颜色一定为BLACK，因为node->parent->color = RED
            }

            //brother 颜色一定为BLACK
            if (brother == nil)
                break;
            if (brother->left->color == BLACK && brother->right->color == BLACK)
            { //case2 兄弟结点的两个子结点都为黑
                brother->color = RED;
                node = node->parent;
            }
            else if (brother->left->color == RED)
            { //case3 brother的左子树为红色,右子树为黑色
                brother->color = RED;
                brother->left->color = BLACK;
                rotateRight(root, brother);
                brother = node->parent->right;
            }
            else
            {
                //case 4 brotther的右子树为红色
                brother->color = node->parent->color;
                node->parent->color = BLACK;
                brother->right->color = BLACK;
                rotateLeft(root, node->parent);
                node = *root; //循环的出口
            }
        }
        else
        { //对称 修复同上
            RBTree brother = node->parent->left;
            if (brother->color == RED)
            { //case 1
                brother->color = BLACK;
                node->parent->color = RED;
                rotateRight(root, node->parent);
                brother = node->parent->left;
            }
            if (brother == nil)
                break;
            if (brother->left->color == BLACK && brother->right->color == BLACK)
            { //case 2
                brother->color = RED;
                node = node->parent;
            }
            else if (brother->left->color == BLACK)
            { //case 3
                brother->color = RED;
                brother->right->color = BLACK;
                rotateLeft(root, brother);
                brother = node->parent->left;
            }
            else
            { //case 4
                brother->color = node->parent->color;
                node->parent->color = BLACK;
                brother->left->color = BLACK;
                rotateRight(root, node->parent);
                node = *root; //循环的出口
            }
        }
    }
    //防止case 2中的父节点为红色的
    node->color = BLACK;
}

/*查找某个结点*/
RBTree findNodeByValue(RBTree root, int value)
{
    if (root != nil)
    {
        if (root->value < value)
            findNodeByValue(root->right, value);
        else if (root->value > value)
            findNodeByValue(root->left, value);
        else
            return root == nil ? NULL : root;
    }
    else
    {
        return nil;
    }
}

/*打印结点，中序遍历*/
void printRBTree(RBTree root)
{
    if (root != NULL && root != nil)
    {
        printRBTree(root->left);
        printf("%d(%s)\n", root->value, (root->color == RED) ? "红" : "黑");
        printRBTree(root->right);
    }
}
/*查找最小结点*/
RBTree getMinNode(RBTree root)
{
    RBTree node = root;
    RBTree p = nil;
    while (node != nil)
    {
        p = node;
        node = node->left;
    }
    return p == nil ? NULL : p;
}

/*查找最大结点*/
RBTree getMaxNode(RBTree root)
{
    RBTree node = root;
    RBTree p = nil;
    while (node != nil)
    {
        p = node;
        node = node->right;
    }
    return p == nil ? NULL : p;
}

int main(int argc, char **argv)
{
    RBTree root = NULL;
    addNodeByValue(&root, 12);
    addNodeByValue(&root, 1);
    addNodeByValue(&root, 9);
    addNodeByValue(&root, 2);
    addNodeByValue(&root, 0);
    addNodeByValue(&root, 11);
    addNodeByValue(&root, 7);
    addNodeByValue(&root, 19);
    addNodeByValue(&root, 4);
    addNodeByValue(&root, 15);
    addNodeByValue(&root, 18);
    addNodeByValue(&root, 5);
    addNodeByValue(&root, 14);
    addNodeByValue(&root, 13);
    addNodeByValue(&root, 10);
    addNodeByValue(&root, 16);
    addNodeByValue(&root, 6);
    addNodeByValue(&root, 3);
    addNodeByValue(&root, 8);
    addNodeByValue(&root, 17);

    removeNodeByValue(&root, 12);
    removeNodeByValue(&root, 1);
    removeNodeByValue(&root, 9);
    removeNodeByValue(&root, 2);
    // printRBTree(root);
    removeNodeByValue(&root, 0);
    removeNodeByValue(&root, 11);
    removeNodeByValue(&root, 7);
    removeNodeByValue(&root, 19);
    removeNodeByValue(&root, 4);
    removeNodeByValue(&root, 15);
    removeNodeByValue(&root, 18);
    removeNodeByValue(&root, 5);
    removeNodeByValue(&root, 14);
    removeNodeByValue(&root, 13);
    removeNodeByValue(&root, 10);
    removeNodeByValue(&root, 16);
    removeNodeByValue(&root, 6);
    removeNodeByValue(&root, 3);
    removeNodeByValue(&root, 8);
    removeNodeByValue(&root, 17);

    return 0;
}