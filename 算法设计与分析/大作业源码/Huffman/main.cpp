#include <iostream>
#include <queue>
#include <unordered_map>

using namespace std;
struct HuffmanNode {
    char data;
    unsigned frequency;
    HuffmanNode *left, *right;
    HuffmanNode(char data, unsigned frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->frequency > b->frequency;
    }
};

void generateCodes(HuffmanNode* root, string code, unordered_map<char, string>& huffmanCodes) {
    if (root) {
        if (!root->left && !root->right) {
            huffmanCodes[root->data] = code;
        }
        generateCodes(root->left, code + "0", huffmanCodes);
        generateCodes(root->right, code + "1", huffmanCodes);
    }
}

void buildHuffmanTree(const string& text) {
    unordered_map<char, unsigned> frequencyMap;
    for (char ch : text) {
        frequencyMap[ch]++;
    }
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> minHeap;

    for (auto& pair : frequencyMap) {
        minHeap.push(new HuffmanNode(pair.first, pair.second));
    }

    while (minHeap.size() > 1) {
        HuffmanNode* left = minHeap.top();
        minHeap.pop();
        HuffmanNode* right = minHeap.top();
        minHeap.pop();
        HuffmanNode* newNode = new HuffmanNode('$', left->frequency + right->frequency);
        newNode->left = left;
        newNode->right = right;
        minHeap.push(newNode);
    }

    HuffmanNode* root = minHeap.top();
    unordered_map<char, string> huffmanCodes;
    generateCodes(root, "", huffmanCodes);
    cout << "Huffman Codes:\n";
    for (auto& pair : huffmanCodes) {
        cout << pair.first << " : " << pair.second << endl;
    }
}

int main() {
    string text = "stay hungry stay foolish!";
    buildHuffmanTree(text);
    return 0;
}


//Huffman Codes:
//f : 11110
//i : 11011
//a : 1110
//r : 11010
//g : 11001
//n : 11000
//l : 10111
//! : 10110
//t : 001
//u : 11111
//: 011
//o : 000
//s : 010
//h : 1010
//y : 100
