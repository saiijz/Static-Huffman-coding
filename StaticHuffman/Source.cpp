#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#define MAX_SIZE 25600

struct Node {
	char key;
	Node *pLeft;
	Node *pRight;
	int freq;
	Node() {
		key = '\0';
		freq = -1;
		pLeft = NULL;
		pRight = NULL;
	}
	Node(char k, Node *pL, Node *pR, int f) {
		key = k;
		pLeft = pL;
		pRight = pR;
		freq = f;
	}
};

struct Bit {
	char c;
	std::string bit;
};

bool IsLeaf(Node *pRoot) {
	if (!pRoot->pLeft && !pRoot->pRight) {
		return true;
	}
	return false;
}

void SwapByte(Node *&x, Node *&y, Node *&pRoot) {
	int temp;
	temp = x->freq;
	x->freq = y->freq;
	y->freq = temp;

	char tempc;
	tempc = x->key;
	x->key = y->key;
	y->key = tempc;

	Node * pTemp{ new Node };
	if (y->pLeft || y->pRight) {
		pTemp->pLeft = x->pLeft;
		pTemp->pRight = x->pRight;
		x->pLeft = y->pLeft;
		x->pRight = y->pRight;
		y->pLeft = pTemp->pLeft;
		y->pRight = pTemp->pRight;
		pRoot = x;
		delete pTemp;
	}
}

void InterchangeSort(std::vector<Node*> NodeTable, Node *&pRoot) {
	for (int i{ 0 }; i < NodeTable.size()-1; i++) {
		for (int j{ i + 1 }; j < NodeTable.size(); j++) {
			if (NodeTable[i]->freq < NodeTable[j]->freq) {
				SwapByte(NodeTable[i], NodeTable[j],pRoot);
			}
			if (NodeTable[i]->freq == NodeTable[j]->freq && NodeTable[i]->key < NodeTable[j]->key) {
				SwapByte(NodeTable[i], NodeTable[j], pRoot);
			}
		}	
	}
}

int CreateNode(Node *&pRoot, char k, int f) {
	pRoot = new Node;
	if (pRoot == NULL) {
		return -1;
	}
	pRoot->key = k;
	pRoot->freq = f;
	return 0;
}

unsigned __int64 GerenateTree(Node*& pRoot, std::string s) {
	if (s.empty()) {
		return NULL;
	}

	std::vector<Node*> NodeTable;

	for (int i{ 0 }; i < s.length(); i++) {
		int check{ 0 };
		for (int j{ 0 }; j < NodeTable.size(); j++) {
			if (s[i] == NodeTable[j]->key) {
				NodeTable[j]->freq++;
				check = 1;
				break;
			}
		}
		if (check == 0) {
			CreateNode(pRoot, s[i], 1);
			NodeTable.push_back(pRoot);
		}
	}
	pRoot = NULL;
	InterchangeSort(NodeTable,pRoot);
	int mass{ static_cast<int>(NodeTable.size()) };
	while (true) {
		int last;
		last = NodeTable.size() - 1;
		CreateNode(pRoot, '*', NodeTable[last]->freq + NodeTable[last - 1]->freq);
		if (!NodeTable[last]->pLeft && !NodeTable[last]->pRight) {
			CreateNode(pRoot->pLeft, NodeTable[last]->key, NodeTable[last]->freq);
		}
		else {
			pRoot->pLeft = new Node(NodeTable[last]->key, NodeTable[last]->pLeft, NodeTable[last]->pRight, NodeTable[last]->freq);
		}
		if (!NodeTable[last-1]->pLeft && !NodeTable[last-1]->pRight) {
			CreateNode(pRoot->pRight, NodeTable[last - 1]->key, NodeTable[last - 1]->freq);
		}
		else {
			pRoot->pRight = new Node(NodeTable[last-1]->key, NodeTable[last-1]->pLeft, NodeTable[last-1]->pRight, NodeTable[last-1]->freq);
		}
		delete NodeTable[last];
		delete NodeTable[last - 1];
		NodeTable.pop_back();
		NodeTable.pop_back();
		NodeTable.push_back(pRoot);
		last--;
		if (last == 0) {
			return mass;
		}
		if (NodeTable[last - 1]->freq < NodeTable[last]->freq && IsLeaf(NodeTable[last-1]) && IsLeaf(NodeTable[last])) {
			InterchangeSort(NodeTable,pRoot);
		}
	}
}

int CreateMappingTable(Node *pRoot, std::vector<Bit> &hashedTable, std::string eb) {
	if (IsLeaf(pRoot)) {
		Bit temp;
		temp.bit = eb;
		temp.c = pRoot->key;
		hashedTable.push_back(temp);
		return 0;
	}
	CreateMappingTable(pRoot->pLeft, hashedTable, eb + '0');
	CreateMappingTable(pRoot->pRight, hashedTable, eb + '1');
}

void staticHuffmanEncoding(std::string& input,std::vector<Bit> &hashedTable) {
	Node *pRoot{ NULL };
	__int64 mass;
	mass = GerenateTree(pRoot, input);
	std::string eb{ "" };
	CreateMappingTable(pRoot, hashedTable, eb);
	for (int i{ 0 }; i < input.size(); i++) {
		for (int j{ 0 }; j < hashedTable.size(); j++) {
			if (input[i] == hashedTable[j].c) {
				eb += hashedTable[j].bit;
				continue;
			}
		}
	}
	input = eb;
}

void staticHuffmanDecoding(std::string &code, std::vector<Bit> hashedTable) {
	std::string eb{ "" };
	std::string temp{ "" };
	for (int i{ 0 }; i < code.size() ; i++) {
		temp += code[i];
		for (int j{ 0 }; j < hashedTable.size(); j++) {
			if (temp == hashedTable[j].bit) {
				eb += hashedTable[j].c;
				temp = "";
			}
		}
	}
	code = eb;
}

std::string readDataFromFile(std::string file) {
	std::ifstream fileInput(file);
	if (fileInput.fail()) {
		std::cout << "Failed to open this file!" << std::endl;
		return "";
	}
	std::string temp;
	while (!fileInput.eof()) {
		char line[25000];
		fileInput.getline(line, 25000);
		temp += line;
	}
	return temp;
}

void writeDataToFile(std::string file, std::string data, std::vector<Bit> hashedTable, int mode) {
	std::ofstream fileOutput(file);
	if (fileOutput.fail()) {
		std::cout << "Cannot open file at " << file << std::endl;
		return;
	}
	if (mode == 1) {
		for (int i{ 0 }; i < hashedTable.size(); i++) {
			fileOutput << hashedTable[i].c;
		}
		fileOutput << static_cast<char>(255);
		fileOutput << std::endl;
		for (int i{ 0 }; i < hashedTable.size(); i++) {
			fileOutput << hashedTable[i].bit << "*";
		}
		fileOutput << '#';
		fileOutput << std::endl;
		fileOutput << data;
	}
	if (mode == 2) {
		fileOutput << data << std::endl;
	}
}

void CompressFile() {
	std::cout << "File Input Address: ";
	std::string address;
	std::getline(std::cin, address);
	std::string n{ readDataFromFile(address) };
	if (n == "") {
		std::cout << "Cannot compress this file!" << std::endl;
		return;
	}
	std::string addressFileOutput;
	std::cout << "File Output Address: ";
	std::getline(std::cin, addressFileOutput);
	std::vector<Bit> hashedTable;
	staticHuffmanEncoding(n, hashedTable);
	writeDataToFile(addressFileOutput, n, hashedTable,1);
}

void ExtractFile() {
	std::cout << "Address Input File: ";
	std::string address;
	std::getline(std::cin, address);
	std::string n{ readDataFromFile(address) };
	if (n == "") {
		std::cout << "Cannot extract this file!" << std::endl;
		return;
	}
	std::string addressFileOutput;
	std::cout << "Address Output File: ";
	std::getline(std::cin, addressFileOutput);
	std::vector<Bit> hashedTable;
	int i{ 0 };
	Bit temp;
	while (true) {
		if (n[i] == static_cast<char>(255)) {
			break;
		}
		temp.c = n[i];
		hashedTable.push_back(temp);
		i++;
	}
	i = 0;
	for (unsigned __int64 j{ hashedTable.size() + 1 }; j < n.size(); j++) {
		if (n[j] == '#'){
			i = ++j;
			break;
		}
		if (n[j] == '*') {
			i++;
			continue;
		}
		hashedTable[i].bit += n[j];
	}
	std::string code;
	for (i; i < n.size(); i++) {
		code += n[i];
	}
	staticHuffmanDecoding(code, hashedTable);
	writeDataToFile(addressFileOutput, code, hashedTable, 2);
}

int main() {
	CompressFile();
	ExtractFile();
	system("pause");
}