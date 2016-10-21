#include "bst.h"
#include "avlbst.h"
#include <string>
#include <functional>
#include <vector>

using namespace std;

int main(int argc, char const *argv[])
{
	AVLTree<string, int> avl;
	AVLTree<int, int> avlIntInt;
	vector<string> elems;
	vector<int> elemsInt;

	int num = 20;
	for(int i = 0; i < num; ++i)
	{
		int number;
		number = rand() % 12345678; // 1000, 1142344, 1234(10)
		elems.push_back("TheOneRing" + std::to_string(number));
		elemsInt.push_back(number);
		// number = i;
		cout << number << "\n ";
		// avl.insert(std::make_pair("TheOneRing" + std::to_string(number), number));
		avlIntInt.insert(std::make_pair(number, number));
		cout << endl;
	}

	avl.printAVL();
	static int count = 0;
	// for ( auto it = avl.begin(); it != avl.end(); ++it ) {
	// 	cout << it->first << " " << it->second << endl;
	// 	count++;
	// }
	for ( auto it = avlIntInt.begin(); it != avlIntInt.end(); ++it ) {
		cout << it->first << " " << it->second << endl;
		count++;
	}
	cout << "count : " << count << endl;
	cout << "elems: " << endl;
	for (int i = 0; i < num; i++)
		cout << elemsInt[i] << ", ";
	cout << endl << "------------------->removing:" << endl;

	/* remove multiple elements test */
	for (int i = 0; i < num; ++i) {
		cout << elemsInt[i] << " | ";
		// avl.remove(elems[i]);
		// avl.printAVL();
		avlIntInt.remove(elemsInt[i]);
		avlIntInt.printAVL();
		cout << endl;
	}

	cout << endl << "after all" << endl;
	// for ( auto it = avl.begin(); it != avl.end(); ++it ) {
	// 	cout << it->first << " " << it->second << endl;
	// }

	
	return 0;
}