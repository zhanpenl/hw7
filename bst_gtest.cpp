#include "gtest/gtest.h"
#include <functional> 
#include <string>

// Intentionally left blank -- Include the student's BST header file here.
#include "bst.h"

typedef BinarySearchTree<std::string, int>::iterator StringIntIterator;
typedef BinarySearchTree<int, double>::iterator IntDoubleIterator;

class BSTTest : public testing::Test
{
	protected:
		BSTTest() { }
		~BSTTest() { }
		virtual void SetUp() { }
		virtual void TearDown() { }

		BinarySearchTree<std::string, int> mStringIntBST;
		BinarySearchTree<int, double> mIntDoubleBST;
};

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

TEST_F(BSTTest, InitiallyEmpty_01)
{
	EXPECT_EQ(mStringIntBST.begin(), mStringIntBST.end());

	EXPECT_EQ(mIntDoubleBST.begin(), mIntDoubleBST.end());
}

TEST_F(BSTTest, FindOnEmptyTree_02)
{
	StringIntIterator stringIntIT = mStringIntBST.find("Testing");
	EXPECT_EQ(stringIntIT, mStringIntBST.end());

	IntDoubleIterator intDoubleIT = mIntDoubleBST.find(0);
	EXPECT_EQ(intDoubleIT, mIntDoubleBST.end());
}

TEST_F(BSTTest, InsertSingleElementBeginNotEqualToEnd_03)
{
	mStringIntBST.insert(std::make_pair("Gandalf", 100));
	EXPECT_NE(mStringIntBST.begin(), mStringIntBST.end());

	mIntDoubleBST.insert(std::make_pair(10, 100.5));
	EXPECT_NE(mIntDoubleBST.begin(), mIntDoubleBST.end());
}

TEST_F(BSTTest, InsertSingleElementFindNotEqualToEnd_04)
{
	mStringIntBST.insert(std::make_pair("Gandalf", 100));
	StringIntIterator stringIntIT = mStringIntBST.find("Gandalf");
	EXPECT_NE(stringIntIT, mStringIntBST.end());

	mIntDoubleBST.insert(std::make_pair(10, 100.5));
	IntDoubleIterator intDoubleIT = mIntDoubleBST.find(10);
	EXPECT_NE(intDoubleIT, mIntDoubleBST.end());
}

TEST_F(BSTTest, InsertSingleElementDereferenceFirstSecond_05)
{
	mStringIntBST.insert(std::make_pair("Gandalf", 100));
	StringIntIterator stringIntIT = mStringIntBST.find("Gandalf");
	EXPECT_EQ(stringIntIT->first, "Gandalf");
	EXPECT_EQ(stringIntIT->second, 100);

	mIntDoubleBST.insert(std::make_pair(10, 100.5));
	IntDoubleIterator intDoubleIT = mIntDoubleBST.find(10);
	EXPECT_EQ(intDoubleIT->first, 10);
	EXPECT_EQ(intDoubleIT->second, 100.5);
}

TEST_F(BSTTest, InsertSingleElementAssignToPairByDereference_06)
{
	mStringIntBST.insert(std::make_pair("Gandalf", 100));
	StringIntIterator stringIntIT = mStringIntBST.find("Gandalf");
	std::pair<std::string, int> myStringIntPair = *stringIntIT;
	EXPECT_EQ(myStringIntPair.first, "Gandalf");
	EXPECT_EQ(myStringIntPair.second, 100);

	mIntDoubleBST.insert(std::make_pair(10, 100.5));
	IntDoubleIterator intDoubleIT = mIntDoubleBST.find(10);
	std::pair<int, double> myIntDoublePair = *intDoubleIT;
	EXPECT_EQ(myIntDoublePair.first, 10);
	EXPECT_EQ(myIntDoublePair.second, 100.5);
}

TEST_F(BSTTest, InsertSingleElementIncrementIterator_07)
{
	mStringIntBST.insert(std::make_pair("Gandalf", 100));
	StringIntIterator stringIntIT = mStringIntBST.find("Gandalf");
	++stringIntIT;
	EXPECT_EQ(stringIntIT, mStringIntBST.end());

	mIntDoubleBST.insert(std::make_pair(10, 100.5));
	IntDoubleIterator intDoubleIT = mIntDoubleBST.find(10);
	++intDoubleIT;
	EXPECT_EQ(intDoubleIT, mIntDoubleBST.end());
}

TEST_F(BSTTest, InsertSingleElementFindNonexistentElementEqualToEnd_08)
{
	mStringIntBST.insert(std::make_pair("Gandalf", 100));
	StringIntIterator stringIntIT = mStringIntBST.find("Sauramon");
	EXPECT_EQ(stringIntIT, mStringIntBST.end());

	mIntDoubleBST.insert(std::make_pair(10, 100.5));
	IntDoubleIterator intDoubleIT = mIntDoubleBST.find(100);
	EXPECT_EQ(intDoubleIT, mIntDoubleBST.end());
}

TEST_F(BSTTest, InsertTwoElementsFindOnBothNotEqualToEnd_09)
{
	mStringIntBST.insert(std::make_pair("Gandalf", 100));
	mStringIntBST.insert(std::make_pair("Sauramon", 200));
	StringIntIterator stringIntIT = mStringIntBST.find("Gandalf");
	EXPECT_NE(stringIntIT, mStringIntBST.end());
	stringIntIT = mStringIntBST.find("Sauramon");
	EXPECT_NE(stringIntIT, mStringIntBST.end());

	mIntDoubleBST.insert(std::make_pair(10, 100.5));
	mIntDoubleBST.insert(std::make_pair(20, 200.5));
	IntDoubleIterator intDoubleIT = mIntDoubleBST.find(10);
	EXPECT_NE(intDoubleIT, mIntDoubleBST.end());
	intDoubleIT = mIntDoubleBST.find(20);
	EXPECT_NE(intDoubleIT, mIntDoubleBST.end());
}

TEST_F(BSTTest, InsertTwoElementsDereferenceFirstAndSecond_10)
{
	mStringIntBST.insert(std::make_pair("Gandalf", 100));
	mStringIntBST.insert(std::make_pair("Sauramon", 200));
	StringIntIterator stringIntIT = mStringIntBST.find("Gandalf");
	EXPECT_EQ(stringIntIT->first, "Gandalf");
	EXPECT_EQ(stringIntIT->second, 100);
	stringIntIT = mStringIntBST.find("Sauramon");
	EXPECT_EQ(stringIntIT->first, "Sauramon");
	EXPECT_EQ(stringIntIT->second, 200);

	mIntDoubleBST.insert(std::make_pair(10, 100.5));
	mIntDoubleBST.insert(std::make_pair(20, 200.5));
	IntDoubleIterator intDoubleIT = mIntDoubleBST.find(10);
	EXPECT_EQ(intDoubleIT->first, 10);
	EXPECT_EQ(intDoubleIT->second, 100.5);
	intDoubleIT = mIntDoubleBST.find(20);
	EXPECT_EQ(intDoubleIT->first, 20);
	EXPECT_EQ(intDoubleIT->second, 200.5);
}

TEST_F(BSTTest, InsertTwoElementsIncrementIterator_11)
{
	mStringIntBST.insert(std::make_pair("Gandalf", 100));
	mStringIntBST.insert(std::make_pair("Sauramon", 200));
	StringIntIterator stringIntIT = mStringIntBST.find("Gandalf");
	EXPECT_EQ(stringIntIT->first, "Gandalf");
	EXPECT_EQ(stringIntIT->second, 100);
	++stringIntIT;
	EXPECT_EQ(stringIntIT->first, "Sauramon");
	EXPECT_EQ(stringIntIT->second, 200);
	++stringIntIT;
	EXPECT_EQ(stringIntIT, mStringIntBST.end());

	mIntDoubleBST.insert(std::make_pair(10, 100.5));
	mIntDoubleBST.insert(std::make_pair(20, 200.5));
	IntDoubleIterator intDoubleIT = mIntDoubleBST.find(10);
	EXPECT_EQ(intDoubleIT->first, 10);
	EXPECT_EQ(intDoubleIT->second, 100.5);
	++intDoubleIT;
	EXPECT_EQ(intDoubleIT->first, 20);
	EXPECT_EQ(intDoubleIT->second, 200.5);
	++intDoubleIT;
	EXPECT_EQ(intDoubleIT, mIntDoubleBST.end());
}

TEST_F(BSTTest, InsertTenElementsIncrementIteratorCheckOrder_12)
{
	const int NUMBER_OF_ELEMENTS = 10;

	for(int i = 0; i < NUMBER_OF_ELEMENTS; ++i)
	{
		mStringIntBST.insert(std::make_pair("TheOneRing" + std::to_string(i), i));
		mIntDoubleBST.insert(std::make_pair(i, i * 0.5));
	}

	int counter = 0;
	StringIntIterator stringIntIT = mStringIntBST.begin();
	while(stringIntIT != mStringIntBST.end())
	{
		EXPECT_EQ(stringIntIT->first, "TheOneRing" + std::to_string(counter));
		EXPECT_EQ(stringIntIT->second, counter);
		++counter;
		++stringIntIT;
	}
	EXPECT_EQ(counter, NUMBER_OF_ELEMENTS);

	counter = 0;
	IntDoubleIterator intDoubleIT = mIntDoubleBST.begin();
	while(intDoubleIT != mIntDoubleBST.end())
	{
		EXPECT_EQ(intDoubleIT->first, counter);
		EXPECT_EQ(intDoubleIT->second, counter * 0.5);
		++counter;
		++intDoubleIT;
	}
	EXPECT_EQ(counter, NUMBER_OF_ELEMENTS);
}

TEST_F(BSTTest, Insert10000RandomElementsIteratorCheckOrder_13)
{
	const int NUMBER_OF_ELEMENTS = 10000;

	srand(0);

	for(int i = 0; i < NUMBER_OF_ELEMENTS; ++i)
	{
		int number = rand();
		mStringIntBST.insert(std::make_pair("TheOneRing" + std::to_string(number), number));
		mIntDoubleBST.insert(std::make_pair(number, number * 0.5));
	}

	int counter = 0;
	StringIntIterator stringIntIT = mStringIntBST.begin();
	std::string previousString = stringIntIT->first;
	++stringIntIT;
	while(stringIntIT != mStringIntBST.end())
	{
		EXPECT_LE(previousString, stringIntIT->first);
		previousString = stringIntIT->first;
		++counter;
		++stringIntIT;
	}
	EXPECT_EQ(counter, NUMBER_OF_ELEMENTS - 1);

	counter = 0;
	IntDoubleIterator intDoubleIT = mIntDoubleBST.begin();
	int previousInt = intDoubleIT->first;
	++intDoubleIT;
	while(intDoubleIT != mIntDoubleBST.end())
	{
		EXPECT_LE(previousInt, intDoubleIT->first);
		previousInt = intDoubleIT->first;
		++counter;
		++intDoubleIT;
	}
	EXPECT_EQ(counter, NUMBER_OF_ELEMENTS - 1);
}

