#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <queue>
#include <iomanip>

using namespace std;


struct Node { // tree will be made out of these node structs

    char c; //character of the node
    unsigned frequency; //frequency of the character

    struct Node *left, *right; //left and right children of given node
};

// function to go through each node and assign codes, creates an unordered_map of chars and codes
void assignCodes(Node* root, string code, unordered_map<char, string> &codes)
{
	if (root == nullptr){  //check if you are off of the tree
		return;
	}

	if (root->left == nullptr && root->right == nullptr) {  //check if you are at a leaf node
		codes[root->c] = code; //assign the code to the given char
	}

    // recursively call assignCodes to assign codes for each char
	assignCodes(root->left, code + "0", codes);
	assignCodes(root->right, code + "1", codes);
}

// Function to initialize a new tree node
Node* createNode(char c, unsigned frequency, Node* left, Node* right)
{
	Node* node = new Node();
	node->c = c;
	node->frequency = frequency;
	node->left = left;
	node->right = right;
	return node;
}

//function that creates unordered map to hold each character along with its frequency
unordered_map<char, int> createCharFreqMap(string input){
  unordered_map<char, int> m;
  for(unsigned i = 0; i < input.size(); i++) {
      m[input[i]]++;
  }
  return m;
}

//function that builds the tree
unordered_map<char, string> getCodes(string input){

    unordered_map<char, int> m = createCharFreqMap(input);
    cout << "Total Characters : " << input.size() << endl;

    //each time we add to the queue we need to keep the queue sorted so we use a priority queue to hold nodes
    auto compare = [](Node* n1, Node* n2){ return n1->frequency > n2->frequency; };
    priority_queue<Node*, vector<Node*>, decltype(compare)> q(compare);

    //create nodes and add them into the priority_queue
    for(auto i : m){
        q.push(createNode(i.first, i.second, nullptr, nullptr));
    }

    cout << "Unique Characters : " << q.size() << endl;

    int nodeCount = q.size();

    while (q.size() > 1){
      //create pointers to left and right nodes of the soon to be created node (these are the lowest frequency nodes)
  		Node *left = q.top();
  		q.pop(); //remove from queue once used
  		Node *right = q.top();
  		q.pop();
  		int frequency = left->frequency + right->frequency;
  		q.push(createNode('~', frequency, left, right));
      nodeCount++;
    } //after this while, we are left with one node in the queue, this is the root node

  unordered_map<char, string> codes; //create unordered_map to hold the codes
	assignCodes(q.top(), "", codes);

  cout << "Node count : " << nodeCount << endl;

	return codes;
}

// function is purely to print out results
// this is not part of the algorithm and is not taken into account for time complexity
void printResults(unordered_map<char, string> codes, string input) {
	const char separator = ' ';
  const int colWidth = 20;
  int totalBitsSaved = 0;
	cout << left << setw(colWidth) << setfill(separator) << "Char";
  cout << left << setw(colWidth) << setfill(separator) << "Code";
  cout << left << setw(colWidth) << setfill(separator) << "Frequency";
  cout << left << setw(colWidth) << setfill(separator) << "Bits Saved" << endl;
	unordered_map<char, int> charFreq = createCharFreqMap(input);  //create map of input to get frequenct
  unordered_map<char, string>::iterator it = codes.begin(); //create iterator for map
  while(it != codes.end()) {//iterate through huffmanCode
      cout << left << setw(colWidth) << setfill(separator) << it->first;
      cout << left << setw(colWidth) << setfill(separator) << it->second;
      cout << left << setw(colWidth) << setfill(separator) << charFreq[it->first];
      int bitsSaved = charFreq[it->first] * 8 - charFreq[it->first] * it->second.size();  //compute bits saved by multiplying frequency by 8 (size of ascii code) then subtracting frequency times new code size
      cout << left << setw(colWidth) << setfill(separator) << bitsSaved << endl;
      totalBitsSaved = totalBitsSaved + bitsSaved;
      it++;
    }

    cout << '\n' << "Total bits saved by using Huffman Encoding on this input: " << totalBitsSaved << endl;

}

int main()
{
    string input;
    cout << "Enter a string: ";
    std::getline(std::cin, input);
    cout << "You entered \"" << input <<"\"" << endl;
    unordered_map<char, string> codes = getCodes(input);
    printResults(codes, input);
}
