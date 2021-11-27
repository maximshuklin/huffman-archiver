#include "Trie.h"

#include <queue>
#include <iostream>
#include <exception>
#include <unordered_map>



struct NodeWithPriority
{
	size_t prior;
	Node* node_pointer;

	NodeWithPriority() = default;

	NodeWithPriority(size_t _prior, Node* _node_pointer) {
		prior = _prior;
		node_pointer = _node_pointer;
	}
};

bool operator<(const NodeWithPriority& lhs, const NodeWithPriority& rhs) {
	return lhs.prior > rhs.prior;
}

Node* Trie::BuildTree(const std::unordered_map<Symbol, size_t>& symbol_count) {
	if (symbol_count.empty()) {
		return new Node();
	}

	std::priority_queue<NodeWithPriority> sorted_nodes;
	for (auto& [ch, cnt] : symbol_count) {
		Node* leaf_node = new Node();
		leaf_node->ch = ch;
		leaf_node->is_leaf = true;
		leaf_node->left = nullptr;
		leaf_node->right = nullptr;
		sorted_nodes.push(NodeWithPriority(cnt, leaf_node));
	}

	while (sorted_nodes.size() > 1) {
		NodeWithPriority minimal_node = sorted_nodes.top();
		sorted_nodes.pop();
		NodeWithPriority second_minimal_node = sorted_nodes.top();
		sorted_nodes.pop();

		Node* new_node = new Node();
		new_node->left = minimal_node.node_pointer;
		new_node->right = second_minimal_node.node_pointer;
		new_node->is_leaf = false;

		size_t new_prior = minimal_node.prior + second_minimal_node.prior;

		sorted_nodes.push(NodeWithPriority(new_prior, new_node));
	}

	return sorted_nodes.top().node_pointer;
}

void Trie::SetLengthToSymbols(Node* node, size_t code_length = 0) {
	if (node->is_leaf) {
		cannonical_codes_.push_back({.symbol = node->ch, .length = code_length});
		return;
	}
	if (node->left != nullptr) {
		SetLengthToSymbols(node->left, code_length + 1);
	}
	if (node->right != nullptr) {
		SetLengthToSymbols(node->right, code_length + 1);
	}
}

std::vector<CannonicalCode> Trie::BuildCannonicalHuffman() {
	cannonical_codes_.clear();
	cannonical_codes_.shrink_to_fit();

	SetLengthToSymbols(root_);

	sort(cannonical_codes_.begin(), cannonical_codes_.end(), [](const CannonicalCode& lhs, const CannonicalCode& rhs) {
		return lhs.length < rhs.length || (lhs.length == rhs.length && lhs.symbol < rhs.symbol);
	});

	return cannonical_codes_;
}

void Trie::DeleteTree(Node* node) {
	if (node->left != nullptr) {
		DeleteTree(node->left);
	}
	if (node->right != nullptr) {
		DeleteTree(node->right);
	}
	delete node;
}

void Increase(std::string& str) {
	for (size_t i = 0; i < str.size(); ++i) {
		if (str[i] == '0') {
			str[i] = '1';
			return ;
		}
		str[i] = '0';
	}
	str += '1';
}

void GetSymbolsCodes(const std::vector<CannonicalCode>& cannonical_pairs,
	std::unordered_map<Symbol, std::string>& symbols_code) {

	if (cannonical_pairs.empty()) {
		return;
	}
	std::string first_code;
	for (size_t i = 0; i < cannonical_pairs[0].length; ++i) {
		first_code += '0';
	}
	symbols_code[cannonical_pairs[0].symbol] = first_code;

	for (size_t i = 1; i < cannonical_pairs.size(); ++i) {
		size_t current_length = cannonical_pairs[i].length;
		Symbol current_symbol = cannonical_pairs[i].symbol;
		Symbol prev_symbol = cannonical_pairs[i - 1].symbol;

		symbols_code[current_symbol] = symbols_code[prev_symbol];
		Increase(symbols_code[current_symbol]);

		while (symbols_code[current_symbol].length() < current_length) {
			symbols_code[current_symbol] = "0" + symbols_code[current_symbol];
		}
	}
	for (auto& [symbol, code] : symbols_code) {
		std::reverse(code.begin(), code.end());
	}
}

Node* Trie::BuildTreeBySymbolsCodes(std::unordered_map<Symbol, std::string>& symbols_code) {
	root_ = new Node();

	for (const auto& [symbol, code] : symbols_code) {
		Node* node = root_;
		for (const auto& ch : code) {
			if (ch == '0') {
				if (node->left == nullptr) {
					node->left = new Node();
				}
				node = node->left;
			} else {
				if (node->right == nullptr) {
					node->right = new Node();
				}
				node = node->right;
			}
		}
		node->is_leaf = true;
		node->ch = symbol;
	}
	return root_;
}

Trie::Trie(const std::unordered_map<Symbol, size_t>& symbol_count) {
	current_node_ = nullptr;
	current_path_ = "";
	root_ = BuildTree(symbol_count);
}

Trie::Trie(const std::vector<CannonicalCode> cannonical_pairs) {
	cannonical_codes_ = cannonical_pairs;


	std::unordered_map<Symbol, std::string> symbols_code;
	GetSymbolsCodes(cannonical_codes_, symbols_code);

	root_ = BuildTreeBySymbolsCodes(symbols_code);
}

Trie::~Trie() {
	DeleteTree(root_);
	cannonical_codes_.clear();
	cannonical_codes_.shrink_to_fit();
}



void Trie::GoToRoot() {
	current_node_ = root_;
	current_path_.clear();
}

void Trie::Descent(bool bit) {
	if (bit == 0) {
		if (current_node_->left != nullptr) {
			current_node_ = current_node_->left;
			current_path_ += '0';
		} else {
			throw std::runtime_error("Invalid operation. Probably your archive is not supported by this program");
		}
	} else {
		if (current_node_->right != nullptr) {
			current_node_ = current_node_->right;
			current_path_ += '1';
		} else {
			throw std::runtime_error("Invalid operation. Probably your archive is not supported by this program");
		}
	}
}

bool Trie::IsLeaf() {
	return current_node_->is_leaf;
}

Symbol Trie::GetSymbol() {
	return current_node_->ch;
}
