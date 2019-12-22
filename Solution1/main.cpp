


#include <iostream>
#include "itemArray_v2.h"
#include "arrayItem_v2.h"
#include "generalArray_v2.h"

// for advanced part only
//#include "generalArray_v2.h"



void testItemFunctions_part1(basic_item* item_ptr)
{
	cout << "Function printing item type" << endl;
	item_ptr->printItemTypeName();
	cout << endl;

	cout << "Function requesting user to input item value" << endl;
	item_ptr->enterItemFromKeyboard();
	cout << endl;

	cout << "Function requesting user to input item value" << endl;
	item_ptr->enterItemFromKeyboard();
	cout << endl;
	
	cout << "Function printing item on Screen" << endl;
	item_ptr->printItemOnScreen();
	cout << endl;

	cout << "Function generating Random item " << endl;
	item_ptr->generateRandomItem();
	item_ptr->printItemOnScreen();
	cout << endl;
	cout << endl;
}


void testItemFunctions_part2(basic_item* item_ptr_1, basic_item* item_ptr_2, basic_sort_criteria* sort_criteria)
{
	cout << "Assigning Random values to first item" << endl;
	item_ptr_1->generateRandomItem();
	item_ptr_1->printItemOnScreen();
	cout << endl;

	cout << "Assigning Random values to second item" << endl;
	item_ptr_2->generateRandomItem();
	item_ptr_2->printItemOnScreen();
	cout << endl;

	cout << "Check compatibility between the items" << endl;
	bool comparison = item_ptr_1->compatibilityCheck(item_ptr_2);
	if (comparison)
		cout << "Items compatible" << endl;
	else
		cout << "Items NOT compatible" << endl;
	cout << endl;

	cout << "Function comparing two items" << endl;
	if (sort_criteria != NULL)
	{
		cout << "  Set the comparison criteria:" << endl; 
		sort_criteria->setOptionFromKeyboard();
		cout << endl;
	}
	comparison = item_ptr_1->IsLargerThan(item_ptr_2, sort_criteria);
	if (comparison)
		cout << "First item larger than the second" << endl;
	else
		cout << "First item smaller (or equal) than the second" << endl;

	cout << endl; 
	cout << endl;
}


void testItemFunctions_part3(basic_item* item_ptr_1)
{
	basic_item* item_ptr_2;
	cout << "Allocating empty item" << endl;
	item_ptr_2=item_ptr_1->allocateEmptyItem();
	cout << endl;

	cout << "Testing newly allocated item:" << endl;
	testItemFunctions_part1(item_ptr_2);
	cout << "Done." << endl;
	cout << endl;

	cout << "Deleting empty item" << endl;
	item_ptr_1->deallocateItem(item_ptr_2);

}


void test_Individualtem()
{
	// Try each one below (ucomment one line at a time)
	// When ready, repleace these with items of the type you have implemented 
	integer_itemWithLimits		testintWL1, testintWL1a;
	integer_item testitem1, testitem1a; basic_sort_criteria testsort_crit;
	//intmat_item testitem1, testitem1a; intmat_sort_criteria testsort_crit; //testsort_crit.setOption(intmat_sort_criteria::sort_determ);
	studentrecord_item testdateitem1, testdateitem1a;
	
	// 
	cout << "Test input funcitons:" << endl;
//	testItemFunctions_part1(&testitem1);
	//testitemfunctions_part1(&testintwl1);
	//testItemFunctions_part1(&testdateitem1);

	cout << endl;
	cout << "Now with locked item:" << endl;
//	testitem1.setLocked(true);
	//testintWL1.setLocked(true);
	//testdateitem1.setLocked(true);


//	testItemFunctions_part1(&testitem1);
	//testitemfunctions_part1(&testintwl1);
	//testItemFunctions_part1(&testdateitem1);


	//testitem1.setLocked(false);
	//testintWL1.setLocked(false);
	testdateitem1.setLocked(false);
	cout << endl;
	
	
	cout << "Test comparison of two items:" << endl;	
	// this uses the default comparison option:
	//testItemFunctions_part2(&testitem1, &testitem1a, NULL);
	//testItemFunctions_part2(&testintWL1, &testintWL1a, NULL);
	testItemFunctions_part2(&testdateitem1, &testdateitem1a, NULL);


	// this uses the full default comparison rule (via user input)
//	testItemFunctions_part2(&testitem1, &testitem1a, &testsort_crit);
	//testItemFunctions_part2(&testintWL1, &testintWL1a, &testsort_crit);
	testItemFunctions_part2(&testdateitem1, &testdateitem1a, &testsort_crit);
	cout << endl;
	
	
	cout << "Test allocate/deallocate funcitons:" << endl;
	//testItemFunctions_part3(&testitem1);
	//testItemFunctions_part3(&testintWL1);
	testItemFunctions_part3(&testdateitem1);
	cout << endl;
}

void test_integer_itemWithLimits()
{
	// Try each one below (ucomment one line at a time)
	// When ready, repleace these with items of the type you have implemented 

	//basic_item testitem1; // pure virtual: can't be allocated
	//integer_item testitem1, testitem1a; basic_sort_criteria testsort_crit;
	//intmat_item testitem1, testitem1a; intmat_sort_criteria testsort_crit; //testsort_crit.setOption(intmat_sort_criteria::sort_determ);
	//myitem testitem1, testitem1a; basic_sort_criteria testsort_crit;

	integer_itemWithLimits testitem1, testitem1a; basic_sort_criteria testsort_crit;

	// 
	cout << "Test input funcitons:" << endl;
	testItemFunctions_part1(&testitem1);
	cout << endl;
	cout << "Now with locked item:" << endl;
	testitem1.setLocked(true);
	testItemFunctions_part1(&testitem1);
	testitem1.setLocked(false);
	cout << endl;


	cout << "Test comparison of two items:" << endl;
	// this uses the default comparison option:
	//testItemFunctions_part2(&testitem1, &testitem1a, NULL);

	// this uses the full default comparison rule (via user input)
	testItemFunctions_part2(&testitem1, &testitem1a, &testsort_crit);
	cout << endl;


	cout << "Test allocate/deallocate funcitons:" << endl;
	testItemFunctions_part3(&testitem1);
	cout << endl;
}

void basic_test_integer_itemWithLimits()
{
	integer_itemWithLimits testitem1;  basic_sort_criteria testsort_crit;

	cout << "Testing user input functions" << endl;
	testitem1.inputRangeFromKeyboard();
	testitem1.printLimitsToScreen();
	testitem1.enterItemFromKeyboard();
	testitem1.printItemOnScreen();


	cout << endl << "Testing random item generation" << endl;
	integer_itemWithLimits testitem1a(-10, 10);
	testitem1a.generateRandomItem();
	testitem1a.printLimitsToScreen();
	testitem1a.printItemOnScreen();

	cout << endl << "Testing comparison items " << endl;
	bool result  = testitem1.IsLargerThan(&testitem1a, &testsort_crit);
	
	if (result == true)
	{
		cout << "testitem1 is larger than testitem1a" << endl;
	}
	else{ cout << endl << "testitem1 is smaller than testitem1a" << endl; }

	cout << endl << "End of test" << endl;
}

void testArrayFunctions_part1(item_array& testArray)
{
	int arraysize;
	cout << "Enter arraysize and hit enter: " << endl;
	cin >> arraysize;
	testArray.allocateArrayAndItems(arraysize);
	testArray.printArrayInfoOnScreen();
	testArray.printArrayOnScreen();
	// 
	cout << "Test input funciton:" << endl;
	testArray.enterArrayFromKeyboard();
	testArray.printArrayOnScreen();
	cout << "Test random input funciton:" << endl;
	testArray.fillRandomValueArray();
	testArray.printArrayOnScreen();
	cout << endl;
	cout << "Now with locked Array:" << endl;
	testArray.setArrayLocked(true);
	testArray.fillRandomValueArray();
	testArray.printArrayOnScreen();
	testArray.setArrayLocked(false);
	cout << endl;
}

void testArrayFunctions_part2(item_array& testArray, basic_sort_criteria& testsort_crit)
{
	cout << " Fill array randomly: " << endl;
	testArray.fillRandomValueArray();
	testArray.printArrayOnScreen();
	cout << "Done." << endl;

	cout << " Choose sort criterion: " << endl;
	testsort_crit.setOptionFromKeyboard();
	cout << "Done." << endl;

	cout << " Sort array and print the result: " << endl;
	testArray.quickSort(&testsort_crit);
	testArray.printArrayOnScreen();
	cout << "Done." << endl;
}

void test_EntireArray()
{
	// Try each one below (ucomment one line at a time)
	// When ready, repleace these with items of the type you have implemented 
	//integer_item testitem; basic_sort_criteria testsort_crit;

	studentrecord_item testitem; studentrecord_item_sort_criteria testsort_crit; //testsort_crit.setOption(intmat_sort_criteria::sort_determ);

	//integer_itemWithLimits testitem; basic_sort_criteria testsort_crit;
	//composite_item testitem; compositeItem_sort_criteria testsort_crit;

	general_item_array testArray;
	int arraysize;

	cout << "The type of item used for all entries in the array is:"; 
	testitem.printItemTypeName();	
	testArray.attachItemPrototype(&testitem);
	cout << endl << endl << endl;

	cout << "Testing Array allocation and data entry: " << endl;
	//testArrayFunctions_part1(testArray);
	cout << "Enter size of the array" << endl;
	cin >> arraysize;
	
	testArray.allocateArrayAndItems(arraysize);
	cout << "Done." << endl << endl;

	cout << "Testing Array sorting " << endl;
	testArrayFunctions_part2(testArray, testsort_crit);
	cout << "Done." << endl;
}

void test_CompositeItem() 
{
	// Try each one below (ucomment one line at a time)
	// When ready, repleace these with items of the type you have implemented 

	//basic_item testitem1; // pure virtual: can't be allocated
	//integer_item testitem1, testitem1a; basic_sort_criteria testsort_crit;
	//intmat_item testitem1, testitem1a; intmat_sort_criteria testsort_crit; //testsort_crit.setOption(intmat_sort_criteria::sort_determ);
	//myitem testitem1, testitem1a; basic_sort_criteria testsort_crit;

	// Composite Item test
	composite_item testitem1, testitem1a; compositeItem_sort_criteria testsort_crit;

	// 
	cout << "Test input funcitons:" << endl;
	testItemFunctions_part1(&testitem1);
	cout << endl;
	cout << "Now with locked item:" << endl;
	testitem1.setLocked(true);
	testItemFunctions_part1(&testitem1);
	testitem1.setLocked(false);
	cout << endl;


	cout << "Test comparison of two items:" << endl;
	// this uses the default comparison option:
	//testItemFunctions_part2(&testitem1, &testitem1a, NULL);

	// this uses the full default comparison rule (via user input)
	testItemFunctions_part2(&testitem1, &testitem1a, &testsort_crit);
	cout << endl;


	cout << "Test allocate/deallocate funcitons:" << endl;
	testItemFunctions_part3(&testitem1);
	cout << endl;
}

void test_DateItem()
{
	date_item test_date1;
	date_item test_date2;
	date_item test_date3;

	cout << "3 Random dates are generated, and printed to the screen." << endl;
	test_date1.generateRandomItem();
	test_date2.generateRandomItem();
	test_date3.generateRandomItem();

	test_date1.printItemOnScreen();
	test_date2.printItemOnScreen();
	test_date3.printItemOnScreen();

	studentrecord_item_sort_criteria sort_crit;
	sort_crit.setOptionFromKeyboard();



	test_date1.IsLargerThan(&test_date2, &sort_crit);
	test_date1.IsLargerThan(&test_date3, &sort_crit);
	

	//test_date1.IsLargerThan(test_date2);
}

void test_simple_search()
{}

void test_basic_string_item()
{
	vector<string> programs = { "EEE", "Mech", "Chem", "Civil", "Petr" };
	basic_string_itemWithLimits teststrings("degree programs", programs);
	teststrings.enterItemFromKeyboard();
	teststrings.printItemOnScreen();
}

int main()
{
	
	//
	//test_integer_itemWithLimits();
	test_EntireArray();

	//basic_test_integer_itemWithLimits();

	//test_simple_search();

	//test_CompositeItem();
	//test_EntireArray_compositeItem();
	//test_DateItem();

	





}

