#include "bst.h"
#include "avlbst.h"
#include <string>
#include <functional>

using namespace std;

int main(int argc, char const *argv[])
{
	AVLTree<string, int> avl;


	for(int i = 0; i < 20; ++i)
	{
		int number;
		number = rand() % 1000;
		// number = i;
		cout << number << "\n ";
		avl.insert(std::make_pair("TheOneRing" + std::to_string(number), number));
		avl.print();
		cout << endl;
	}

	cout << endl;
	avl.print();
	cout << endl;

	for ( auto it = avl.begin(); it != avl.end(); ++it ) {
		cout << it->first << " " << it->second << endl;
	}

	
	return 0;
}