#include "Map.h"

template <typename T1, typename T2>
Map<T1, T2>::Map() {
	head = nullptr;
	size = 0;
}

template <typename T1, typename T2>
Map<T1, T2>::~Map() {
	removeNode(head);
}

template <typename T1, typename T2>
void Map<T1, T2>::removeNode(Node<T1, T2>* temp) {
	if (temp == nullptr) return;
	removeNode(temp->left);
	removeNode(temp->right);
	if (temp->parent && temp->parent->left == temp) temp->parent->left = nullptr;
	else if (temp->parent) temp->parent->right = nullptr;
	delete temp;
	size--;
}

template <typename T1, typename T2>
void Map<T1, T2>::insert(T1 key, T2 value) {
	Node<T1, T2>* newNode = new Node<T1, T2>(key, value);
	Node<T1, T2>* i = head;
	bool flag = 0;

	if (head == nullptr) {
		newNode->color = 0;
		size++;
		head = newNode;
		keys.push_back(newNode->key);
		values.push_back(newNode->value);
		return;
	}

	while (!flag) {
		if (newNode->key == i->key) return;
		else if (newNode->key < i->key) {
			if (i->left == nullptr) {
				i->left = newNode;
				newNode->parent = i;
				flag = 1;
				size++;
				keys.push_back(newNode->key);
				values.push_back(newNode->value);
			}
			else i = i->left;
		}
		else {
			if (i->right == nullptr) {
				i->right = newNode;
				newNode->parent = i;
				flag = 1;
				size++;
				keys.push_back(newNode->key);
				values.push_back(newNode->value);
			}
			else i = i->right;
		}
	}
	balanceInsert(newNode);
	head->color = 0;
}

template <typename T1, typename T2>
void Map<T1, T2>::balanceInsert(Node<T1, T2>* node) {
	if (node->parent == nullptr) return;
	if (!node->parent->color) return;
	if (node->parent->parent == nullptr) {
		node->parent->color = 0;
		return;
	}

	Node<T1, T2>* father = node->parent;
	Node<T1, T2>* granny = father->parent;

	if (granny->right == father) {
		if (!granny->left || !granny->left->color) {
			if (father->right == node) {
				leftRotate(node->parent->parent);
				node->parent->color = 0;
				node->parent->left->color = 1;
			}
			else {
				rightRotate(node->parent);
				leftRotate(node->parent);
				node->color = 0;
				node->left->color = 1;
			}
		}
		else {
			father->color = 0;
			if (granny->left)granny->left->color = 0;
			granny->color = 1;
			balanceInsert(granny);
		}
	}
	else {
		if (!granny->right || !granny->right->color) {
			if (father->left == node) {
				rightRotate(node->parent->parent);
				node->parent->color = 0;
				node->parent->right->color = 1;
			}
			else {
				leftRotate(node->parent);
				rightRotate(node->parent);
				node->color = 0;
				node->right->color = 1;
			}
		}
		else {
			father->color = 0;
			if (granny->right)granny->right->color = 0;
			granny->color = 1;
			balanceInsert(granny);
		}
	}
}

template <typename T1, typename T2>
void Map<T1, T2>::remove(T1 key) {
	bool flag = 0;
	if (head->key == key && size == 1) {
		keys.remove(head->key);
		values.remove(head->value);
		delete head;
		head = nullptr;
		size--;
	}

	Node<T1, T2>* temp = findNode(key);
	if (!temp)return;

	keys.remove(temp->key);
	auto iter = values.end();
	if (values.size())iter--;
	if (iter != values.end() && *iter == temp->value) values.pop_back();
	else {
		for (auto iter = values.begin(); iter != values.end(); iter++)
		{
			if (*iter == temp->value) {
				iter = values.erase(iter);
				break;
			}
		}
	}

	int count = (temp->left != nullptr) + (temp->right != nullptr);

	if (count == 2) {
		Node<T1, T2>* buff = temp;
		buff = temp->right;
		while (buff->left)buff = buff->left;
		temp->value = buff->value;
		temp->key = buff->key;
		temp = buff;
		count = (temp->left != nullptr) + (temp->right != nullptr);
	}
	if (count == 1) {
		if (temp->left) {
			temp->key = temp->left->key;
			temp->value = temp->left->value;
			temp = temp->left;
		}
		else {
			temp->key = temp->right->key;
			temp->value = temp->right->value;
			temp = temp->right;
		}
		count = (temp->left != nullptr) + (temp->right != nullptr);
	}
	Node<T1, T2>* father = temp->parent;
	if (count == 0) {
		if (temp->color) {
			if (father->left == temp) father->left = nullptr;
			else father->right = nullptr;
			delete temp;
			size--;
		}
		else {
			bool side; // 0 - left, 1 - right
			if (father->left == temp) father->left = nullptr, side = 0;
			else father->right = nullptr, side = 1;
			delete temp;
			size--;
			balanceRemove(father, side);
		}
	}
}

template <typename T1, typename T2>
void Map<T1, T2>::balanceRemove(Node<T1, T2>* node, bool side) {
	Node<T1, T2>* son;
	if (side) {
		son = node->left;
		if (node->color && !son->color && (!son->left || !son->left->color) && (!son->right || !son->right->color))
		{
			node->color = 0;
			son->color = 1;
		}
		else if (node->color && !son->color && son->left != nullptr && son->left->color)
		{
			node->color = 0;
			son->color = 1;
			son->left->color = 0;
			rightRotate(node);
		}
		else if (!node->color && son->color && son->right != nullptr && !son->right->color && (!son->right->left || !son->right->left->color) && (!son->right->right || !son->right->right->color))
		{
			son->color = 0;
			son->right->color = 1;
			rightRotate(node);
		}
		else if (!node->color && son->color && son->right != nullptr && !son->right->color && son->right->left != nullptr && son->right->left->color)
		{
			son->right->left->color = 0;
			leftRotate(son);
			rightRotate(node);
		}
		else if (!node->color && !son->color && son->right != nullptr && son->right->color)
		{
			son->right->color = 0;
			leftRotate(son);
			rightRotate(node);
		}
		else if (!node->color && !son->color && (!son->right || !son->right->color) && (!son->left || !son->left->color))
		{
			son->color = 1;
			if (node->parent) {
				if (node->parent->left == node) balanceRemove(node->parent, 0);
				else balanceRemove(node->parent, 1);
			}
		}
	}
	else {
		son = node->right;
		if (node->color && !node->right->color && (!node->right->left || !node->right->left->color) && (!node->right->right || !node->right->right->color)) {
			node->color = 0;
			node->right->color = 1;
		}
		else if (node->color && !node->right->color && node->right->right != nullptr && node->right->right->color) {
			node->color = 0;
			node->right->color = 1;
			node->right->right->color = 0;
			leftRotate(node);
		}
		else if (!node->color && node->right->color && node->right->left != nullptr && !node->right->left && (!node->right->left->left || !node->right->left->left->color) && (!node->right->left->right || !node->right->left->right->color)) {
			node->right->color = 1;
			node->right->left->color = 0;
			leftRotate(node);
		}
		else if (!node->color && node->right->color && node->right->left != nullptr && !node->right->left->color && node->right->left->right != nullptr && node->right->left->right->color) {
			node->right->left->right->color = 0;
			rightRotate(node->right);
			leftRotate(node);
		}
		else if (!node->color && !node->right->color && node->right->left != nullptr && node->right->left->color) {
			node->right->left->color = 1;
			rightRotate(node->right);
			leftRotate(node);
		}
		else if (!node->color && !node->right->color && (!node->right->left || !node->right->left->color) && (!node->right->right || !node->right->right->color)) {
			node->right->color = 1;
			if (node->parent) {
				if (node->parent->left == node) balanceRemove(node->parent, 0);
				else balanceRemove(node->parent, 1);
			}
		}
	}
}

template <typename T1, typename T2>
void Map<T1, T2>::leftRotate(Node<T1, T2>* node) {
	if (node->right == nullptr) return;

	Node<T1, T2>* temp = node->right;

	if (node == head) head = temp;

	node->right = temp->left;
	if (temp->left)temp->left->parent = node;

	if (node->parent) {
		if (node->parent->left == node) node->parent->left = temp;
		else node->parent->right = temp;
	}
	temp->parent = node->parent;

	node->parent = temp;
	temp->left = node;

}

template <typename T1, typename T2>
void Map<T1, T2>::rightRotate(Node<T1, T2>* node) {
	if (node->left == nullptr) return;

	Node<T1, T2>* temp = node->left;

	if (node == head) head = temp;

	node->left = temp->right;
	if (temp->right)temp->right->parent = node;

	if (node->parent) {
		if (node->parent->left == node) node->parent->left = temp;
		else node->parent->right = temp;
	}
	temp->parent = node->parent;

	node->parent = temp;
	temp->right = node;

}

template <typename T1, typename T2>
int Map<T1, T2>::getSize() {
	return size;
}

template <typename T1, typename T2>
Map<T1, T2>::Node<T1, T2>* Map<T1, T2>::getHead() const {
	return this->head;
}

template <typename T1, typename T2>
void Map<T1, T2>::print(Node<T1, T2>* node) {
	if (node == NULL) return;
	std::string color;
	if (node->color) color = "RED";
	else color = "BLACK";
	if (node->parent == NULL)
		std::cout << node->key << " : " << node->value << '\n';
	else if (node->parent->left == node)
	{
		std::cout << node->key << " : " << node->value << '\n';
	}
	else
	{
		std::cout << node->key << " : " << node->value << '\n';
	}
	print(node->left);
	print(node->right);
}

template <typename T1, typename T2>
void Map<T1, T2>::print() {
	if (head == NULL)
		std::cout << "empty Map\n";
	else
		print(head);
}

template <typename T1, typename T2>
T2 Map<T1, T2>::find(T1 key) {
	Node<T1, T2>* temp = findNode(key);
	if (temp)return temp->value;
	else return 0;
}

template <typename T1, typename T2>
Map<T1, T2>::Node<T1, T2>* Map<T1, T2>::findNode(T1 key) const {
	Node<T1, T2>* temp = head;
	bool flag = 0;
	while (!flag && temp) {
		if (temp->key == key) break;
		else if (temp->key < key) {
			if (temp->right) temp = temp->right;
			else break;
		}
		else {
			if (temp->left) temp = temp->left;
			else break;
		}
	}
	if (temp && temp->key == key)return temp;
	else return nullptr;
}

template <typename T1, typename T2>
bool Map<T1, T2>::exist(T1 key) {
	Node<T1, T2>* temp = findNode(key);
	if (temp) return 1;
	else return 0;
}

template <typename T1, typename T2>
void Map<T1, T2>::clear() {
	removeNode(head);
	head = nullptr;
}

template <typename T1, typename T2>
std::list<T1> Map<T1, T2>::getKeys() {
	return this->keys;
}

template <typename T1, typename T2>
std::list<T2> Map<T1, T2>::getValues() {
	return this->values;
}

template <typename T1, typename T2>
T2& Map<T1, T2>::operator[](const T1 key) {
	Node<T1, T2>* temp = findNode(key);
	return temp->value;
}

template <typename T1, typename T2>
T1 Map<T1, T2>::getHeadKey() {
	if (head) return head->key;
}

template <typename T1, typename T2>
T2 Map<T1, T2>::getHeadValue() {
	if (head) return head->value;
}


