#pragma once

#include <unordered_map>
#include <vector>
#include <string>

using Code = short int;
using Symbol = short int;

struct Node {
	Symbol ch = 0;
	bool is_leaf = false;
	Node* left = nullptr;
	Node* right = nullptr;

	Node() = default;
};

struct CannonicalCode {
	Symbol symbol;
	size_t length;
};

void GetSymbolsCodes(const std::vector<CannonicalCode>& cannonical_pairs,
		std::unordered_map<Symbol, std::string>& symbols_code);

class Trie {
public:
	Trie(const std::unordered_map<short int, size_t>& symbol_count);
	Trie(const std::vector<CannonicalCode> cannonical_pairs);
	~Trie();

	std::vector<CannonicalCode> BuildCannonicalHuffman();

	void GoToRoot();
	void Descent(bool bit);
	bool IsLeaf();
	Symbol GetSymbol();


private:
	Node* root_ = nullptr;
	std::vector<CannonicalCode> cannonical_codes_;

	Node* current_node_;
	std::string current_path_;

	Node* BuildTree(const std::unordered_map<short int, size_t>& symbol_count);
	Node* BuildTreeBySymbolsCodes(std::unordered_map<Symbol, std::string>& symbols_code);
	void SetLengthToSymbols(Node* node, size_t code_length);
	void DeleteTree(Node* node);
};