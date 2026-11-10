#include <iostream>
#include <stack>
using namespace std;

class Node{
    public:
    int val;
    Node* left;
    Node* right;

    Node(int value){
        val = value;
        left = NULL;
        right = NULL;
    }
};

class BST{
    public:
    Node* root;

    BST(){
        root = NULL;
    }

    ~BST(){
        destroy(root);
    }

    // frees the tree, children before parent
    void destroy(Node* x){
        if(x==NULL) return;
        destroy(x->left);
        destroy(x->right);
        delete x;
    }

    void Insert(int v){
        Node* newNode = new Node(v);
        if(root==NULL) root = newNode;
        else{
            Node* temp = root;
            while(temp!=NULL){
                if(temp->val==v){
                    delete newNode;
                    return;
                }
                else if(v<temp->val && temp->left!=NULL){
                    temp = temp->left;
                }
                else if(v>temp->val && temp->right!=NULL){
                    temp = temp->right;
                }
                else break;     // empty spot found, temp is the parent
            }
            if(v<temp->val) temp->left = newNode;
            else temp->right = newNode;
        }
    }

    void printInorder(Node* x){
        if(x==NULL) return;
        printInorder(x->left);
        cout << x->val << " ";
        printInorder(x->right);
    }

    // finds inorder predecessor and successor of key in one walk from the root
    void findPredSucc(int key){
        Node* pred = NULL;
        Node* succ = NULL;
        Node* temp = root;
        bool found = false;
        while(temp!=NULL){
            if(key<temp->val){
                succ = temp;        // last left turn is a successor candidate
                temp = temp->left;
            }
            else if(key>temp->val){
                pred = temp;        // last right turn is a predecessor candidate
                temp = temp->right;
            }
            else{
                found = true;
                // if the node has subtrees, they give closer answers than ancestors
                if(temp->left!=NULL){
                    Node* p = temp->left;
                    while(p->right!=NULL) p = p->right;
                    pred = p;
                }
                if(temp->right!=NULL){
                    Node* s = temp->right;
                    while(s->left!=NULL) s = s->left;
                    succ = s;
                }
                break;
            }
        }
        if(!found) cout << "Key " << key << " is not in the tree, showing nearest keys" << endl;
        cout << "Predecessor of " << key << ": ";
        if(pred==NULL) cout << "None" << endl;
        else cout << pred->val << endl;
        cout << "Successor of " << key << ": ";
        if(succ==NULL) cout << "None" << endl;
        else cout << succ->val << endl;
    }

    // prints keys in [low, high], skipping subtrees that cannot contain them
    // cmp counts every comparison of a key against low or high
    void rangeRecursive(Node* x, int low, int high, int &cmp){
        if(x==NULL) return;
        cmp++;
        if(x->val>low) rangeRecursive(x->left, low, high, cmp);
        cmp++;
        if(x->val>=low){
            cmp++;
            if(x->val<=high) cout << x->val << " ";
        }
        cmp++;
        if(x->val<high) rangeRecursive(x->right, low, high, cmp);
    }

    // same query using an explicit stack instead of recursion
    void rangeIterative(int low, int high, int &cmp){
        stack<Node*> st;
        Node* curr = root;
        while(curr!=NULL || !st.empty()){
            while(curr!=NULL){
                cmp++;
                if(curr->val>=low){
                    st.push(curr);
                    curr = curr->left;
                }
                else curr = curr->right;    // too small, skip it and its left subtree
            }
            if(st.empty()) break;
            curr = st.top();
            st.pop();
            cmp++;
            if(curr->val>high) break;       // everything after this is bigger
            cout << curr->val << " ";
            curr = curr->right;
        }
    }
};

int main(){
    BST tree;
    int arr[] = {50, 30, 70, 20, 40, 60, 80, 35, 45, 65, 85};
    int n = sizeof(arr)/sizeof(arr[0]);
    for(int i=0; i<n; i++){
        tree.Insert(arr[i]);
    }

    cout << "Inorder: ";
    tree.printInorder(tree.root);
    cout << endl << endl;

    tree.findPredSucc(40);
    cout << endl;

    int low = 30, high = 70;
    int recCmp = 0, itCmp = 0;

    cout << "Range [" << low << ", " << high << "] recursive: ";
    tree.rangeRecursive(tree.root, low, high, recCmp);
    cout << endl;

    cout << "Range [" << low << ", " << high << "] iterative: ";
    tree.rangeIterative(low, high, itCmp);
    cout << endl << endl;

    cout << "Comparisons (recursive): " << recCmp << endl;
    cout << "Comparisons (iterative): " << itCmp << endl;
}
