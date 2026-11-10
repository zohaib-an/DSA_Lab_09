#include <iostream>
#include <string>
using namespace std;

class WordNode{
    public:
    string word;
    int count;
    WordNode* left;
    WordNode* right;

    WordNode(string w){
        word = w;
        count = 1;
        left = NULL;
        right = NULL;
    }
};

class WordBST{
    public:
    WordNode* root;
    int uniqueWords;
    int totalWords;

    WordBST(){
        root = NULL;
        uniqueWords = 0;
        totalWords = 0;
    }

    ~WordBST(){
        destroy(root);
    }

    void destroy(WordNode* x){
        if(x==NULL) return;
        destroy(x->left);
        destroy(x->right);
        delete x;
    }

    // inserts a word, or increments its count if it already exists
    void Insert(string w){
        totalWords++;
        if(root==NULL){
            root = new WordNode(w);
            uniqueWords++;
            return;
        }
        WordNode* temp = root;
        while(true){
            if(w==temp->word){
                temp->count++;
                return;
            }
            else if(w<temp->word){
                if(temp->left==NULL){
                    temp->left = new WordNode(w);
                    uniqueWords++;
                    return;
                }
                temp = temp->left;
            }
            else{
                if(temp->right==NULL){
                    temp->right = new WordNode(w);
                    uniqueWords++;
                    return;
                }
                temp = temp->right;
            }
        }
    }

    // splits the paragraph on anything that is not a letter and lowercases each word
    void buildFromText(string text){
        string current = "";
        for(int i=0; i<(int)text.length(); i++){
            char c = text[i];
            if((c>='A' && c<='Z') || (c>='a' && c<='z')){
                if(c>='A' && c<='Z') c = c - 'A' + 'a';
                current += c;
            }
            else if(current!=""){
                Insert(current);
                current = "";
            }
        }
        if(current!="") Insert(current);
    }

    void inorderPrint(WordNode* x){
        if(x==NULL) return;
        inorderPrint(x->left);
        cout << x->word << ": " << x->count << endl;
        inorderPrint(x->right);
    }

    void displayAlphabetical(){
        cout << "Words in alphabetical order:" << endl;
        inorderPrint(root);
    }

    // collects nodes into arr in alphabetical order
    void fillArray(WordNode* x, WordNode** arr, int &idx){
        if(x==NULL) return;
        fillArray(x->left, arr, idx);
        arr[idx++] = x;
        fillArray(x->right, arr, idx);
    }

    // insertion sort is stable, so equal counts stay in alphabetical order
    void displayByFrequency(){
        WordNode** arr = new WordNode*[uniqueWords];
        int idx = 0;
        fillArray(root, arr, idx);
        for(int i=1; i<uniqueWords; i++){
            WordNode* key = arr[i];
            int j = i-1;
            while(j>=0 && arr[j]->count<key->count){
                arr[j+1] = arr[j];
                j--;
            }
            arr[j+1] = key;
        }
        cout << "Words by frequency (highest to lowest):" << endl;
        for(int i=0; i<uniqueWords; i++){
            cout << arr[i]->word << ": " << arr[i]->count << endl;
        }
        delete[] arr;
    }

    int maxCount(WordNode* x){
        if(x==NULL) return 0;
        int m = x->count;
        int l = maxCount(x->left);
        int r = maxCount(x->right);
        if(l>m) m = l;
        if(r>m) m = r;
        return m;
    }

    void printWithCount(WordNode* x, int target){
        if(x==NULL) return;
        printWithCount(x->left, target);
        if(x->count==target) cout << x->word << " ";
        printWithCount(x->right, target);
    }

    void findMostFrequentWord(){
        int m = maxCount(root);
        cout << "Most frequent word(s) (" << m << " times): ";
        printWithCount(root, m);
        cout << endl;
    }

    void displaySummary(){
        cout << "Total unique words: " << uniqueWords << endl;
        cout << "Total word count: " << totalWords << endl;
    }
};

int main(){
    string text = "The quick brown fox jumps over the lazy dog. The fox is quick and smart.";
    WordBST tree;
    tree.buildFromText(text);

    tree.displayAlphabetical();
    cout << endl;
    tree.displayByFrequency();
    cout << endl;
    tree.findMostFrequentWord();
    cout << endl;
    tree.displaySummary();
}
