#include <iostream>
#include "Map.h"
#include "Map.cpp"
#include "List.h"
#include "List.cpp"

using namespace std;

struct Elem {
	string code = "";
	char key;
	double p = 0;

	bool operator > (const Elem& b) {
		return (p > b.p);
	}

	bool operator < (const Elem& b) {
		return (p < b.p);
	}
};

Elem* elemArray;
double* segmentSum;
Map<char, string> codes;
Map<string, char> decodes;

int countProb(string input) {
	Map<char, double> ar;
	for (int i = 0; i < input.size(); i++) { 
		if (ar.exist(input[i])) ar[input[i]]++;
		else ar.insert(input[i], 1);
	}

	List<Elem> probabilities;

	while (ar.getSize()) { 
		Elem cur;
		cur.key = ar.getHeadKey();
		cur.p = ar.getHeadValue() / input.size();
		probabilities.sortedInsert(cur);
		ar.remove(ar.getHeadKey());
	}

	int size = probabilities.getSize();
	elemArray = new Elem[size];
	segmentSum = new double[size];

	for (int i = 0; i < size; i++) { 
		elemArray[i] = probabilities[0];
		if (i == 0)segmentSum[i] = elemArray[i].p;
		else segmentSum[i] = segmentSum[i - 1] + elemArray[i].p;
		probabilities.pop_front();
	}
	return size;
}

void code(int left, int right, bool side) { 
	for (int i = left; i <= right; i++)elemArray[i].code += char(48+side);
	int mid = left;
	if (left == right)return;
	while (segmentSum[mid + 1] - segmentSum[left] < segmentSum[right] - segmentSum[mid + 1]) mid++;
	code(left, mid, 0);
	code(mid + 1, right, 1);
}

void makeCodeTable(int size) {
	int mid = 0;

	while (segmentSum[mid + 1] - segmentSum[0] < segmentSum[size - 1] - segmentSum[mid + 1]) mid++; // O(a)

	code(0, mid, 0);
	code(mid + 1, size - 1, 1);

	for (int i = 0; i < size; i++) { 
		codes.insert(elemArray[i].key, elemArray[i].code); 
		decodes.insert(elemArray[i].code, elemArray[i].key);
	}
}

string code(string input) { 
	string coded = "";
	for (int i = 0; i < input.size(); i++) coded += codes[input[i]];
	return coded;
}

string decode(string input) { 
	string decoded = "";
	string buff = "";
	for (int i = 0; i < input.size(); i++) {
		buff += input[i];
		if (decodes.exist(buff)) {
			decoded += decodes[buff];
			buff = "";
		}
	}
	return decoded;
}

void answer(string coded, string decoded) {
	cout << " codes table" << '\n';
	codes.print();
	cout << " decodes table" << '\n';
	decodes.print();

	cout << '\n' << coded << '\n' << decoded << '\n';
	cout << " Coded Memory Size : " << coded.size() << '\n';
	cout << " Decoded Memory Size : " << decoded.size() * 8 << '\n';
	double ratio = decoded.size() * static_cast<double>(8) / coded.size();
	cout << " Copression ratio : " << ratio << '\n';
}

int main() {
	string input;
	cin >> input;
	int size = countProb(input);
	makeCodeTable(size);
	
	string coded = code(input);
	string decoded = decode(coded);

	answer(coded, decoded);

	delete[] segmentSum;
	delete[] elemArray;
}