#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * 红黑树的性质
 * 1.每个结点要么红色,要么黑色
 * 2.根结点是黑色的
 * 3.每个叶子结点都是黑色(nil结点)
 * 4.如果一个几点为红色,那么两个子结点都是黑色(不能有连续两个红色结点)
 * 5.任意一结点到叶子结点每条路径上黑色结点的个数都相等
 */

/**
 *  左旋转规则  假设当前结点为x
 *  1.结点变为右子树的左结点 
 *      x->parent = x->right;
 *  2.右子树上移变为父亲结点,根据结点的分布来确定,右树变为父亲后,相对于祖父结点的分布
 *      if(x==x->parent->parent->left){
 *          x->parent->parent->left = x->right;
 *      }else{
 *          x->parent->parent->right = x->right;
 *      }
 *      x->right->parent = x->parent;
 *  3.右子树的左结点作为当前结点的右结点
 *      x->right = x->right->left;
 *
 *         5                               7
 *          \         结点5左旋转         /  \
 *           7       -------------->      5   8
 *          / \                            \
 *          6  8                            6
 */

/**
 *  右旋转规则  假设当前结点为x
 *  1.结点变为左子树的右结点
 *      x->parent = x->left;
 *  2.左子树上移变为父亲结点,根据结点的分布来确定父亲相对于祖父结点的分布
 *      if(x==x->parent->left){
 *          x->parent->parent->left = x->left;
 *      }else{
 *          x->parent->parent->right = x->left;
 *      }
 *      x->left->parent = x->parent;
 *  3.左子树的右结点,作为当前结点的左结点
 *      x->left = x->left->right;
 *
 *      4                                 2
 *     /           节点4右旋转           / \
 *     2          ------------->         1  4
 *    / \                                   /
 *    1  3                                 3
 */

/**
 *   结点的插入规则
 *   1.节点在最底端插入,且节点是红色的 (假设插入节点x)
 *   2.插入后如果父节点是红色的,则要进行修复
 *     1.x的父节点是祖父节点的左子树
 *      case1 :叔父节点y是红色
 *              1.将y与x的父亲同时着为黑色
 *              2.然后把x的祖父变为红色
 *              3.把x上移为祖父节点
 *      case2:叔父节点是黑色,x是右节点
 *              1.将x变为x的parent
 *              2.左旋转x
 *      case3:叔父节点是黑色,x是左节点
 *              1.将x的parent变黑
 *              2.x的祖父变红
 *              3.右旋转x的祖父节点
 *     2.x的父节点是祖父节点的右子树(修复同上)
 *   3.如果上升为根节点,则将根节点变黑
 *              
*/

/**
 *   结点的删除规则  假设删除节点为z  实际删除节点y x删除y的唯一子节点(先左后后)
 *   1.如果有一个子树为空,直接删除  y=z   
 *   2.如果右子树不为空则,删除右子树中最左边的节点,如果右子树为空,则删除祖先节点中为左子树的祖先 y
 *   3. if(y->left!=NULL) x=y->left else x= y->right;
 *   4.将原来y的父母设置为x的父母因为y即将被删除  ,x在y父亲中的分布取决与y在父亲中的分布,如果y是左节点,则x也是左节点,如果y是右节点则x也是右节点
 *   5.如果y!=z则把y的值换给z
 *   6.如果y为黑色则,则要进行修复,因为在某条路径上少了一个黑色节点
 *
 *   删除修复规则如下:
 *      1.删除的修复是循环的,条件是 x不为根节点且x是一个黑色节点,如果x是一个黑色节点则,直接染红即可
 *      2.分两种情况
 *          x是左子树 假设x的兄弟节点为w
 *              case 1: w为红色
 *                      1.w着黑
 *                      2.x的parent变红
 *                      3.左旋转x的parent
 *                      4.重置兄弟节点 w = x->parent->right;
 *              case 2: w黑色 且两个子树都是黑色
 *                      1.w 着黑
 *                      2.x变为x的parent
 *              case 3: w黑色 w的左子树是红色 w的右子树是黑色
 *                      1.w着红
 *                      2.w的左子着黑
 *                      3.右旋转w
 *                      4.重置兄弟节点   w=x->parent->right;
 *              case 4: w的右子树是红色
 *                      1.w着色为x父亲的颜色
 *                      2.x的parent着黑
 *                      3.w的右子树着黑
 *                      4.左旋转x的parent
 *              修复完成 x=T  退出
 *          x是右子树 (过程同上)
 *      3.设置根节点的颜色,过程有可能改变了根节点的颜色
 *   
*/

/*定义结点的颜色*/
#define RED true
#define BLACK false

typedef struct Node
{
    struct Node *parent; //父节点
    struct Node *left;   //左子树
    struct Node *right;  //右子树
    int value;           //value
    /*data 域*/
    bool color; //节点颜色
} * RBTree;

/*通过value插入一个结点*/
void addNodeByValue(RBTree *T, int value);
/*查找结点*/
RBTree findNode(RBTree T, int value);
/*删除结点*/
void removeNode(RBTree *T, RBTree node);
/*查找最小的结点*/
RBTree getMinNode(RBTree T);
/*查找最大的结点*/
RBTree getMaxNode(RBTree T);
/*打印红黑树*/
void printRBTree(RBTree T);
/*插入修复*/
void fixInsert(RBTree *T, RBTree node);
/*删除修复*/
void fixRemove(RBTree *T, RBTree node);
/*获取祖父节点*/
RBTree getAncestor(RBTree node);
/*获取叔父结点*/
RBTree getUncle(RBTree node);
/*左旋*/
void rotateLeft(RBTree *T, RBTree node);
/*右旋*/
void rotateRight(RBTree *T, RBTree node);
/*真正删除节点*/
RBTree realRemoveNode(RBTree *T, RBTree node);

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
void rotateLeft(RBTree *T, RBTree node)
{
    if (node->right != nil)
    {
        RBTree y = node->right;
        node->right = y->left;
        if (y->left != nil)
        {
            y->left->parent = node;
        }
        y->parent = node->parent;

        if (node->parent == nil)
        {
            *T = y;
        }
        else
        {
            if (node == node->parent->left)
            {
                node->parent->left = y;
            }
            else
            {
                node->parent->right = y;
            }
        }
        y->left = node;
        node->parent = y;
    }
}
/*右旋*/
void rotateRight(RBTree *T, RBTree node)
{
    if (node->left != nil)
    {
        RBTree y = node->left;
        node->left = y->right;
        if (y->right != nil)
        {
            y->right->parent = node;
        }
        y->parent = node->parent;
        if (node->parent == nil)
        {
            *T = y;
        }
        else
        {
            if (node == node->parent->left)
            {
                node->parent->left = y;
            }
            else
            {
                node->parent->right = y;
            }
        }
        y->right = node;
        node->parent = y;
    }
}

/*查找实际要删除的结点*/
RBTree realRemoveNode(RBTree *T, RBTree node)
{
    RBTree p = node->right;
    while (p->left != nil)
    {
        p = p->left;
    }
    return p;
}
/*通过value插入一个结点*/
void addNodeByValue(RBTree *T, int value)
{
    if ((*T) == NULL)
    {
        *T = (RBTree)malloc(sizeof(struct Node));
        //初始化nil结点
        nil = (RBTree)malloc(sizeof(struct Node));
        nil->color = BLACK;
        //设置结点的指向
        (*T)->parent = nil;
        (*T)->left = nil;
        (*T)->right = nil;
        //设置结点属性,value 和color
        (*T)->value = value;
        (*T)->color = BLACK;
    }
    else
    {
        RBTree node = *T;
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
        fixInsert(T, node);
    }
}

/*插入修复*/
void fixInsert(RBTree *T, RBTree node)
{
    RBTree y;
    while (node->parent->color == RED)
    {
        RBTree ancestor = getAncestor(node);
        if (node->parent == ancestor->left)
        {
            y = ancestor->right; //获取叔父结点
            if (y->color == RED)
            { //case 1  如果叔父结点为红色,则把父节点和叔父结点着黑,祖父结点着红,z上移到祖父结点
                y->color = BLACK;
                node->parent->color = BLACK;
                ancestor->color = RED;
                node = ancestor;
            }
            else
            {
                if (node == node->parent->right)
                { //case 2  如果叔父结点为黑色,z右结点,z上移为父亲结点,左旋转z结点,此时变为case3的情况
                    node = node->parent;
                    rotateLeft(T, node);
                }
                node->parent->color = BLACK; //case 3 叔父结点为黑色,且z的左结点,z的父亲着着黑,z的祖父着红,然后旋转z的祖父结点
                ancestor->color = RED;
                rotateRight(T, ancestor);
            }
        }
        else
        { //对称 修复同上
            y = ancestor->left;
            if (y->color == RED)
            {
                y->color = BLACK;
                node->parent->color = BLACK;
                ancestor->color = RED;
                node = ancestor;
            }
            else
            {
                if (node == node->parent->left)
                {
                    node = node->parent;
                    rotateRight(T, node);
                }
                node->parent->color = BLACK;
                ancestor->color = RED;
                rotateLeft(T, ancestor);
            }
        }
    }
    (*T)->color = BLACK;
}

/**
 *   结点的删除规则  假设删除节点为z  实际删除节点y x删除y的唯一子节点(先左后后)
 *   1.如果有一个子树为空,直接删除  y=node   
 *   2.如果右子树不为空则,删除右子树中最左边的节点,如果右子树为空,则删除祖先节点中为左子树的祖先 y
 *   3. if(y->left!=NULL) node=y->left else node= y->right;
 *   4.将原来y的父母设置为x的父母因为y即将被删除  ,x在y父亲中的分布取决与y在父亲中的分布,如果y是左节点,则x也是左节点,如果y是右节点则x也是右节点
 *   5.如果y!=z则把y的值换给z
 *   6.如果y为黑色则,则要进行修复,因为在某条路径上少了一个黑色节点
 *
 */

void removeNode(RBTree *T, RBTree node)
{
    if (node == nil || node == NULL)
        return;
    RBTree y;
    RBTree x;
    if (node->left == nil || node->right == nil)
    {
        y = node;
    }
    else
    {
        y = realRemoveNode(T, node);
    }
    //x指向实际删除结点的子结点
    if (y->left != nil)
        x = y->left;
    else
        x = y->right;

    x->parent = y->parent; //删除结点y
    if (y->parent == nil)
    {
        *T = x;
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
        node->value = y->value;
    }
    //如果删除的结点是黑色,违法了性质5,要进行删除修复
    if (y->color == BLACK)
    {
        fixRemove(T, x);
    }
}

/*删除修复*/
void fixRemove(RBTree *T, RBTree node)
{
    while (node != (*T) && node->color == BLACK)
    {
        if (node == node->parent->left)
        {
            RBTree brother = node->parent->right; //brother 为x的兄弟结点
            if (brother->color == RED)
            { //case 1 兄弟结点为红色
                brother->color = BLACK;
                node->parent->color = RED;
                rotateLeft(T, node->parent);
                brother = node->parent->right;
            }
            if (brother == nil)
                break;
            if (brother->left->color == BLACK && brother->right->color == BLACK)
            { //case2 兄弟结点的两个子结点都为黑
                brother->color = RED;
                node = node->parent;
            }
            else if (brother->right->color == BLACK)
            { //case3 w的左子树为红色,右子树为黑色
                brother->color = RED;
                brother->left->color = BLACK;
                rotateRight(T, brother);
                brother = node->parent->right;
            }
            brother->color = node->parent->color; //case 4 w的右子树为红色
            node->parent->color = BLACK;
            brother->right->color = BLACK;
            rotateLeft(T, node->parent);
        }
        else
        { //对称 修复同上
            RBTree brother = node->parent->left;
            if (brother->color == RED)
            { //case 1
                brother->color = BLACK;
                node->parent->color = RED;
                rotateRight(T, node->parent);
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
                rotateLeft(T, brother);
                brother = node->parent->left;
            }
            brother->color = node->parent->color;
            node->parent->color = BLACK;
            brother->left->color = BLACK;
            rotateRight(T, node->parent);
        }
        node = *T;
    }
    node->color = BLACK;
}

/*查找某个结点*/
RBTree findNode(RBTree T, int value)
{
    if (T != nil)
    {
        if (T->value < value)
            findNode(T->right, value);
        else if (T->value > value)
            findNode(T->left, value);
        else
            return T == nil ? NULL : T;
    }
}

/*打印结点*/
void printRBTree(RBTree T)
{
    if (T != NULL && T != nil)
    {
        printRBTree(T->left);
        printf("%d(%s)\n", T->value, (T->color == RED) ? "红" : "黑");
        printRBTree(T->right);
    }
}
/*查找最小最小结点*/
RBTree getMinNode(RBTree T)
{
    RBTree node = T;
    RBTree p = nil;
    while (node != nil)
    {
        p = node;
        node = node->left;
    }
    return p == nil ? NULL : p;
}

/*查找最大结点*/
RBTree getMaxNode(RBTree T)
{
    RBTree node = T;
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
    RBTree T = NULL;
    RBTree node;
    int i = 0;
    for (i = 1; i <= 20; i++)
        addNodeByValue(&T, i);
    addNodeByValue(&T, 80);
    addNodeByValue(&T, 90);
    addNodeByValue(&T, 40);
    removeNode(&T, findNode(T, 130));
    removeNode(&T, findNode(T, 90));
    printRBTree(T);
    printf("max=%d\n", getMaxNode(T)->value);
    printf("min=%d\n", getMinNode(T)->value);
    printf("search=%d\n", findNode(T, 16)->value);
    return 0;
}