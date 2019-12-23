#ifndef __ARRAYITEM_V2_H_
#define __ARRAYITEM_V2_H_

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <string>
#include <vector>

#include <iostream>
#include <random>
using namespace std;


////// this is needed to check compatibility between items (and to use a derived-class pointer that is passes as "base class")
template <class Base, class Derived> Derived* typecastItem(Base* basic_ptr, Derived* derivedItem_ptr)
{
	Derived* typecasted_ptr=dynamic_cast <Derived *>(basic_ptr);

	if(typecasted_ptr==NULL)				
		printf("\n Error typecasting item: type does not match the expected derived item\n");

	return typecasted_ptr;
}
//////


class basic_sort_criteria{
protected:
	bool ascending;	//	
public:
	basic_sort_criteria(){setAscending(true);}
	void setAscending(bool value){ascending=value;}
	bool getAscending(){return ascending;}
	virtual void setOptionFromKeyboard()
	{
		char sortascending;
		cout << "Sort option is ASCENDING? Type Y and press ENTER" << endl;
		cin >> sortascending;
		if( (sortascending=='y') || (sortascending == 'Y') )
			setAscending(true);
		else
			setAscending(false);
	}
	virtual void printOptionToScreen()
	{
		cout << "Sort option is ";
		if(getAscending())
			cout << "ASCENDING" << endl;
		else
			cout << "DISCENDING" << endl;		
	}
};

//
class array_manipulator;
//class item_array;

class basic_item{
protected:
	bool empty;
	string itemTypeName;	
	bool locked;
	string nameType;
	
	// Used (later on) by the general_item_array:
	int includedBy;
	void incrementincludedBy() { includedBy++; }
	void decrementincludedBy() { includedBy=( (includedBy>0) ? (includedBy-1) : 0 ); }
	// end test
public:
	basic_item() { empty = true; locked = false;  includedBy = 1; }
	~basic_item() { ; }
	bool isEmpty(){return empty;}
	bool isLocked() { return locked; }
	virtual void setLocked(bool lock_input)
	{
		if(!isEmpty())
			locked = lock_input;
	}
	int isIncludedBy() {return includedBy;}
	void printItemTypeName() { cout << endl << "Item type: " << itemTypeName << endl; }
	//These must be implemented by any derived item	
	virtual void printItemOnScreen()=0;
	virtual void enterItemFromKeyboard()=0;
	//virtual void loadItemFromFile(FILE* fin)=0;
	virtual void generateRandomItem()=0;	
	virtual bool IsLargerThan(basic_item* other_item, basic_sort_criteria* sort_criteria=NULL)=0;
	virtual bool IsEqualTo(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL) = 0;
	//virtual bool IsSmallerThan(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL) = 0;
	virtual basic_item* allocateEmptyItem()=0;
	virtual void deallocateItem(basic_item* itemToDestroy) = 0;
	//virtual basic_item* makeCopyofItem()=0;
	virtual bool compatibilityCheck(basic_item* other_item) = 0;		
	virtual string getNameType() = 0;
	//friend bool fromSourceToDestinationAarray(item_array& sourceArray, int startPosSour, int totElem, item_array& destArray, int startPosDest, bool removeFromSource);
	//friend void general_item_array::incrementIncludedBy(basic_item* item);
	//friend class array_manipulator;
	//friend class item_array;
	friend class general_item_array;
};

class basic_string_item : public basic_item{
protected:
	string item_value;
	string nameType;
public:
	basic_string_item(string name = "name") {
		itemTypeName = "basic_string_item"; 
		nameType = name;
		//generateRandomItem();
	}
	~basic_string_item() { cout << "basic_string_item destructor called" << endl; } // can remove the printout after testing

	string getItemVal() { return item_value; }

	virtual void setLocked(bool lock_input)
	{
		if (!isEmpty())
			locked = lock_input;
	}

	virtual void printItemOnScreen()
	{
		if (isEmpty())
			cout << "Item is empty." << endl;
		else
			cout << "Item value is  " << item_value << endl;
	}

	virtual void enterItemFromKeyboard()
	{
		if (isLocked())
			cout << "Error in enterItemFromKeyboard: Item is locked" << endl;
		else
		{
			cout << "Insert "<< nameType << " then hit enter." << endl;
			cin >> item_value;
			cout << endl;

			// item filled
			empty = false;
		}
	}

	virtual string getNameType() { return nameType; }

	virtual void generateRandomItem()
	{
		int max_val = 12, min_val = 3;
		const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
		int length;
		int max_rand_val = max_val;

		length = rand();
		length = length % (max_val - min_val + 1) + min_val;

		random_device random_device;
		mt19937 generator(random_device());
		uniform_int_distribution<> distribution(0, characters.size() - 1);

		std::string random_string;

		for (std::size_t i = 0; i < length; ++i)
		{
			random_string += characters[distribution(generator)];
		}

		item_value = random_string;
		empty = false;
	}



	//virtual void loadItemFromFile(FILE* fin);

	virtual bool IsLargerThan(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL)
	{
		bool result = false;

		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item == NULL)
			return false;


		// first typecast the other item to confimr it is the same as this;
		basic_string_item* typecasted_other_item = typecastItem(other_item, this);

		// check that it worked
		if (typecasted_other_item == NULL)
		{
			cout << endl << "Item type is: ";
			cout << itemTypeName << endl;
			return false;
			// items of the wrong type (or null pointers) will be pushed to the end of the list
		}

		for (int i = 0; i < item_value.size(); i++) {
			if (item_value[i] == typecasted_other_item->getItemVal()[i])
			{
				continue;
			}
			else if (item_value[i] > typecasted_other_item->getItemVal()[i])
			{
				result = true;
				break;
			}
			else
			{
				result = false;
				break;
			}
		}



		// chek if there are sorting options to apply 
		if (sort_criteria != NULL)
		{
			// if sorting is in descending order the result is reversed 
			if (!(sort_criteria->getAscending()))
				result = !result;
		}

		return result;
	}

	//virtual bool IsSmallerThan(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL)
	//{
	//	bool result = false;

	//	// if the other item is "empty" (non allocated) don't do any comparison
	//	if (other_item == NULL)
	//		return false;


	//	// first typecast the other item to confimr it is the same as this;
	//	basic_string_item* typecasted_other_item = typecastItem(other_item, this);

	//	// check that it worked
	//	if (typecasted_other_item == NULL)
	//	{
	//		cout << endl << "Item type is: ";
	//		cout << itemTypeName << endl;
	//		return false;
	//		// items of the wrong type (or null pointers) will be pushed to the end of the list
	//	}

	//	for (int i = 0; i < item_value.size(); i++) {
	//		if (item_value[i] == typecasted_other_item->getItemVal()[i])
	//		{
	//			continue;
	//		}
	//		// changed the operator to change the function
	//		else if (item_value[i] < typecasted_other_item->getItemVal()[i])
	//		{
	//			result = true;
	//			break;
	//		}
	//		else
	//		{
	//			result = false;
	//			break;
	//		}
	//	}



	//	// chek if there are sorting options to apply 
	//	if (sort_criteria != NULL)
	//	{
	//		// if sorting is in descending order the result is reversed 
	//		if (!(sort_criteria->getAscending()))
	//			result = !result;
	//	}

	//	return result;
	//}

	virtual bool IsEqualTo(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL)
	{
		{
			bool result = false;

			// if the other item is "empty" (non allocated) don't do any comparison
			if (other_item == NULL)
				return false;


			// first typecast the other item to confimr it is the same as this;
			basic_string_item* typecasted_other_item = typecastItem(other_item, this);

			// check that it worked
			if (typecasted_other_item == NULL)
			{
				cout << endl << "Item type is: ";
				cout << itemTypeName << endl;
				return false;
				// items of the wrong type (or null pointers) will be pushed to the end of the list
			}

			if (includes(other_item))
				result = true;
			else
				result = false;



			// chek if there are sorting options to apply 
			if (sort_criteria != NULL)
			{
				// if sorting is in descending order the result is reversed 
				if (!(sort_criteria->getAscending()))
					result = !result;
			}

			return result;
		}
	}

	virtual bool includes(basic_item* other_item)
	{
		bool result = false;

		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item == NULL)
			return false;


		// first typecast the other item to confimr it is the same as this;
		basic_string_item* typecasted_other_item = typecastItem(other_item, this);

		// check that it worked
		if (typecasted_other_item == NULL)
		{
			cout << endl << "Item type is: ";
			cout << itemTypeName << endl;
			return false;
			// items of the wrong type (or null pointers) will be pushed to the end of the list
		}


		string other_value = typecasted_other_item->getItemVal();

		if (item_value.find(other_value) != string::npos || other_value.find(item_value) != string::npos)
			result = true;
		else
			result = false;
		//for (int i = 0; i < item_value.size(); i++){
		//	for (int j = 0; j < other_value.size(); j++)
		//	{
		//		if (item_value[j] == other_value[j]) {
		//			result = true;
		//			continue;
		//		}
		//		else
		//			result = false;
		//			break;
		//	}

		//}
		return result;
	}

	virtual basic_item* allocateEmptyItem()
	{
		basic_item* result = new basic_string_item;
		if (result == NULL)
		{
			cout << endl << "Out of memeory allocating ";
			cout << itemTypeName << endl;
		}
		return result;
	}
	virtual void deallocateItem(basic_item* itemToDestroy)
	{
		if (itemToDestroy != NULL)
		{
			// first typecast the other item to confimr it is the same as this;
			basic_string_item* typecasted_other_item = typecastItem(itemToDestroy, this);
			delete typecasted_other_item;
		}
	}

	virtual bool compatibilityCheck(basic_item* other_item)
	{
		bool result = false;

		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item != NULL)
		{
			// typecast the other item to confirm it is the same as this;
			basic_string_item* typecasted_other_item = typecastItem(other_item, this);
			if (typecasted_other_item != NULL)
				result = true;
			else
			{
				cout << endl << "Check failed for Item type: ";
				cout << itemTypeName << endl;
			}
		}
		return result;
	}

};

class integer_item : public basic_item {
protected:
	int item_value;
public:
	integer_item() { itemTypeName = "integer_item"; }
	~integer_item() { cout << "integer_item destructor called" << endl; } // can remove the printout after testing

	int getItemVal() { return item_value; }

	virtual void printItemOnScreen()
	{
		if (isEmpty())
			cout << "Item is empty." << endl;
		else
			cout << "Item value is " << item_value << endl;
	}

	virtual void enterItemFromKeyboard()
	{
		if (isLocked())
			cout << "Error in enterItemFromKeyboard: Item is locked" << endl;
		else
		{
			cout << "Insert integer element then hit enter." << endl;
			cin >> item_value;
			cout << endl;

			// item filled
			empty = false;
		}
	}

	virtual void generateRandomItem()
	{
		if (isLocked())
			cout << "Error in generateRandomItem: Item is locked" << endl;
		else
		{
			int item;
			int max_rand_val = 1000;

			item = rand();
			item = item % (max_rand_val + 1);

			// turn to negative 30% of the time
			if ((rand() % 10) >= 7)
				item = (-1)*item;

			item_value = item;
			// item filled
			empty = false;
		}
	}

	virtual string getNameType(){return nameType;}

	void generateRandomItemWithinLimits(int min_val, int max_val)
	{
		if (isLocked())
			cout << "Error in generateRandomItem: Item is locked" << endl;
		else
		{
			int item;

			if (min_val > max_val)
			{
				int temp = min_val;
				min_val = max_val;
				min_val = temp;
			}

			int max_rand_val = max_val - min_val;

			item = rand();
			item = item % (max_rand_val);
			item = item + min_val;
			item_value = item;
			// item filled
			empty = false;
		}
	}

	//virtual void loadItemFromFile(FILE* fin);
	
	virtual bool IsLargerThan(basic_item* other_item, basic_sort_criteria* sort_criteria=NULL)
	{
		bool result=false;
		
		// if the other item is "empty" (non allocated) don't do any comparison
		if(other_item==NULL)
			return false;

		
		// first typecast the other item to confimr it is the same as this;
		integer_item* typecasted_other_item = typecastItem(other_item, this);
		
		// check that it worked
		if(typecasted_other_item==NULL)
		{
			cout << endl << "Item type is: ";
			cout << itemTypeName << endl;
			return false;
			// items of the wrong type (or null pointers) will be pushed to the end of the list
		}

		// now verify if the other item is larger than the curren
		if( getItemVal() > (typecasted_other_item->getItemVal()) )
			result=true;


		// chek if there are sorting options to apply 
		if(sort_criteria!=NULL)
		{
			// if sorting is in descending order the result is reversed 
			if( !( sort_criteria->getAscending() ) )
				result=!result;
		}

		return result;
	}

	//virtual bool IsSmallerThan(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL)
	//{
	//	bool result = false;

	//	// if the other item is "empty" (non allocated) don't do any comparison
	//	if (other_item == NULL)
	//		return false;


	//	// first typecast the other item to confimr it is the same as this;
	//	integer_item* typecasted_other_item = typecastItem(other_item, this);

	//	// check that it worked
	//	if (typecasted_other_item == NULL)
	//	{
	//		cout << endl << "Item type is: ";
	//		cout << itemTypeName << endl;
	//		return false;
	//		// items of the wrong type (or null pointers) will be pushed to the end of the list
	//	}

	//	// now verify if the other item is larger than the curren
	//	if (getItemVal() < (typecasted_other_item->getItemVal()))
	//		result = true;


	//	// chek if there are sorting options to apply 
	//	if (sort_criteria != NULL)
	//	{
	//		// if sorting is in descending order the result is reversed 
	//		if (!(sort_criteria->getAscending()))
	//			result = !result;
	//	}

	//	return result;
	//}

	virtual bool IsEqualTo(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL)
	{
		bool result = false;

		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item == NULL)
			return false;


		// first typecast the other item to confimr it is the same as this;
		integer_item* typecasted_other_item = typecastItem(other_item, this);

		// check that it worked
		if (typecasted_other_item == NULL)
		{
			cout << endl << "Item type is: ";
			cout << itemTypeName << endl;
			return false;
			// items of the wrong type (or null pointers) will be pushed to the end of the list
		}

		// now verify if the other item is larger than the curren
		if (getItemVal() == (typecasted_other_item->getItemVal()))
			result = true;


		// chek if there are sorting options to apply 
		if (sort_criteria != NULL)
		{
			// if sorting is in descending order the result is reversed 
			if (!(sort_criteria->getAscending()))
				result = !result;
		}

		return result;
	}

	virtual basic_item* allocateEmptyItem()
	{
		basic_item* result = new integer_item;
		if (result == NULL)
		{
			cout << endl << "Out of memeory allocating ";
			cout << itemTypeName << endl;
		}		return result;
	}
	virtual void deallocateItem(basic_item* itemToDestroy)
	{
		if (itemToDestroy != NULL)
		{
			// first typecast the other item to confimr it is the same as this;
			integer_item* typecasted_other_item = typecastItem(itemToDestroy, this);
			delete typecasted_other_item;
		}
	}
		
	virtual bool compatibilityCheck(basic_item* other_item)
	{
		bool result = false;

		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item != NULL)
		{
			// typecast the other item to confirm it is the same as this;
			integer_item* typecasted_other_item = typecastItem(other_item, this);
			if (typecasted_other_item != NULL)
				result = true;
			else
			{
				cout << endl << "Check failed for Item type: ";
				cout << itemTypeName << endl;
			}
		}
		return result;
	}
	
};

class studentrecord_item_sort_criteria : public basic_sort_criteria {
public:
	enum sortType { start = 0, firstName, secondName, fullDate, DayAndMonth, nationality, studedId, programme, level, CGS, bloodType, height, stop };
	sortType thesortoption;
private:

public:
	studentrecord_item_sort_criteria() { setOption(firstName); }
	void setOption(sortType value)
	{
		if ((value > start) && (value < stop))
			thesortoption = value;
		else
			thesortoption = firstName;
	}
	sortType getOption() { return thesortoption; }
	virtual void setOptionFromKeyboard()
	{
		char sortoption;
		cout << "Sort option: Type F for first name; Type S for second name; Type D for full date; Type M for day and month;" << endl;
		cout << "Sort option: Type N for nationality; Type I for studetId; Type P for programme; Type L for level;" << endl;
		cout << "Sort option: Type C for CGS; Type B for blood Type; then press ENTER" << endl;

		cin >> sortoption;
		switch (sortoption) {
		case 'F':
		case 'f':
			setOption(firstName);
			break;
		case 'S':
		case 's':
			setOption(secondName);
			break;
		case 'D':
		case 'd':
			setOption(fullDate);
			break;
		case 'M':
		case 'm':
			setOption(DayAndMonth);
			break;
		case 'N':
		case 'n':
			setOption(nationality);
			break;
		case 'I':
		case 'i':
			setOption(studedId);
			break;
		case 'P':
		case 'p':
			setOption(programme);
			break;
		case 'L':
		case 'l':
			setOption(level);
			break;
		case 'C':
		case 'c':
			setOption(CGS);
			break;
		case 'B':
		case 'b':
			setOption(bloodType);
			break;
		case 'H':
		case 'h':
			setOption(height);
			break;
		}
		basic_sort_criteria::setOptionFromKeyboard();
	}
	virtual void printOptionToScreen()
	{
		cout << "composite_item Sorting by ";
		switch (getOption()) {
		case firstName:
			cout << "first name " << endl;
			break;
		case secondName:
			cout << "second name " << endl;
			break;
		case fullDate:
			cout << "full date " << endl;
			break;
		case DayAndMonth:
			cout << "day and month" << endl;
			break;
		case nationality:
			cout << "nationality" << endl;
			break;
		case studedId:
			cout << "student id" << endl;
			break;
		case programme:
			cout << "degreee programme" << endl;
			break;
		case level:
			cout << "degreee level" << endl;
			break;
		case CGS:
			cout << "average CGS grade" << endl;
			break;
		case bloodType:
			cout << "bio data" << endl;
			break;
		case height:
			cout << "height" << endl;
			break;
		}
		basic_sort_criteria::printOptionToScreen();
	}
};

class compositeItem_sort_criteria : public basic_sort_criteria {
public:
	enum sortType { start, firstName, secondName, fullDate, DayAndMonth, day, month, year, stop };
private:
	sortType thesortoption;
public:
	compositeItem_sort_criteria() { setOption(firstName); }
	void setOption(sortType value)
	{
		if ((value > start) && (value < stop))
			thesortoption = value;
		else
			thesortoption = firstName;
	}
	sortType getOption() { return thesortoption; }
	virtual void setOptionFromKeyboard()
	{
		char sortoption;
		cout << "Sort option: Type F for first name; Type S for second name; Type D for full date; Type M for day and month; then press ENTER" << endl;
		cin >> sortoption;
		switch (sortoption) {
		case 'F':
		case 'f':
			setOption(firstName);
			break;
		case 'S':
		case 's':
			setOption(secondName);
			break;
		case 'D':
		case 'd':
			setOption(fullDate);
			break;
		case 'M':
		case 'm':
			setOption(DayAndMonth);
			break;
		}
		basic_sort_criteria::setOptionFromKeyboard();
	}
	virtual void printOptionToScreen()
	{
		cout << "composite_item Sorting by ";
		switch (getOption()) {
		case firstName:
			cout << "first name " << endl;
			break;
		case secondName:
			cout << "second name " << endl;
			break;
		case fullDate:
			cout << "full date " << endl;
			break;
		case DayAndMonth:
			cout << "day and month" << endl;
			break;
		}
		basic_sort_criteria::printOptionToScreen();
	}
};

class intmat_sort_criteria : public basic_sort_criteria{
public:
	enum matsortoptions{inf_val=0, sort_maxval, sort_determ, sup_val};
private:
	matsortoptions thesortoption;
public:	
	intmat_sort_criteria() {setOption(sort_maxval);}
	void setOption(matsortoptions value)
	{ 
		if( (value>inf_val) && (value < sup_val) )
			thesortoption = value;
		else
			thesortoption = sort_maxval;
	}
	matsortoptions getOption() { return thesortoption; }
	virtual void setOptionFromKeyboard()
	{
		char sortoption;
		cout << "Sort option: Type M for MAXVAL; Type D for DETERMINANT; then press ENTER" << endl;
		cin >> sortoption;
		switch (sortoption) {
		case 'M':
		case 'm':
			setOption(sort_maxval);
			break;
		case 'D':
		case 'd':
			setOption(sort_determ);
			break;
		}
		basic_sort_criteria::setOptionFromKeyboard();
	}
	virtual void printOptionToScreen()
	{
		cout << "intmat Sorting by ";
		switch (getOption()) {
		case sort_maxval:
			cout << "MAXVAL " << endl;
			break;
		case sort_determ:
			cout << "DETERMINANT " << endl;
			break;
		}
		basic_sort_criteria::printOptionToScreen();
	}
};

class integer_itemWithLimits : public integer_item {
protected:
	int item_value;
	int min_val, max_val;
	string nameType;

	void swap(int* xp, int* yp)
	{
		int temp = *xp;
		*xp = *yp;
		*yp = temp;
	}

public:
	integer_itemWithLimits( int min = -50, int max = 50, string name_val = "test values", bool manual_input = false) {
		min_val = min;
		max_val = max;
		itemTypeName = "integer_itemWithLimits"; 
		nameType = name_val;
		if (manual_input)
		{
			inputRangeFromKeyboard();
		}
	}

	~integer_itemWithLimits() { cout << "integer_itemWithLimits destructor called" << endl; } // can remove the printout after testing

	int getItemVal() { return item_value; }

	virtual void printItemOnScreen()
	{
		if (isEmpty())
			cout << "Item is empty." << endl;
		else
			cout << "Item value is " << item_value << endl;
	}

	virtual void printItemTypeName() { cout << endl << "Item type: " << itemTypeName << endl; }

	virtual string getNameType() { return nameType; }

	virtual void printItemAndTypeOnScreen()
	{
		if (isEmpty())
			cout << "Item is empty." << endl;
		else
			cout << "Item type is " << nameType << ". Item value is " << item_value << endl;
	}

	virtual void inputRangeFromKeyboard()
	{
		string default_val;
		cout << "input max range value " << endl;
		cin >> max_val;
		while (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "sorry please enter an integer" << endl;
			cout << "input max range value" << endl;
			cin >> max_val;
		}
		cout << endl;
		cout << "input min range value" << endl;
		cin >> min_val;
		while (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "sorry please enter an integer" << endl;
			cout << "input min range value" << endl;
			cin >> min_val;
		}
		if (min_val > max_val)
		{
			swap(&max_val, &min_val);
		}
		cout << endl;
	}

	virtual void enterItemFromKeyboard()
	{
		if (isLocked())
			cout << "Error in enterItemFromKeyboard: Item is locked" << endl;
		else
		{
			cout << "Insert integer "<< nameType << " element then hit enter. " << "range between " << min_val << " and " << max_val << endl;
			cin >> item_value;

			while (item_value > max_val || item_value < min_val)
			{
				cout << "Input was out of range. Please enter again." << endl;
				cin >> item_value;
			}
			cout << endl;

			// item filled
			empty = false;
		}
	}

	virtual void printLimitsToScreen()
	{
		cout << "Upper limit is :" << max_val << endl;
		cout << "Lower limit is " << min_val << endl << endl;
	}


	virtual void generateRandomItemWithinLimits(int Min_val, int Max_val)
	{
		if (isLocked())
			cout << "Error in generateRandomItem: Item is locked" << endl;
		if (Min_val < min_val || Max_val > max_val)
		{
			cout << "Error in generateRandomItemWithinLimits: inputs out of range" << endl;
		}
		else
		{
			int item;

			if (Min_val > Max_val)
			{
				swap(&max_val, &min_val);
			}

			int max_rand_val = Max_val - Min_val;

			item = rand();
			item = item % (max_rand_val);
			item = item + Min_val;
			item_value = item;
			// item filled
			empty = false;
		}
	}

	void generateRandomItem()
	{
		if (isLocked())
			cout << "Error in generateRandomItem: Item is locked" << endl;
		else
		{
			int item;


			int max_rand_val = max_val - min_val;

			item = rand();
			item = item % (max_rand_val);
			item = item + min_val;
			item_value = item;
			// item filled
			empty = false;
		}
	}

	//virtual void loadItemFromFile(FILE* fin);

	virtual bool IsLargerThan(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL)
	{
		bool result = false;

		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item == NULL)
			return false;


		// first typecast the other item to confimr it is the same as this;
		integer_itemWithLimits* typecasted_other_item = typecastItem(other_item, this);

		// check that it worked
		if (typecasted_other_item == NULL)
		{
			cout << endl << "Item type is: ";
			cout << itemTypeName << endl;
			return false;
			// items of the wrong type (or null pointers) will be pushed to the end of the list
		}
		int test = getItemVal();
		int otherTest = typecasted_other_item->getItemVal();
		// now verify if the other item is larger than the curren
		if (getItemVal() > (typecasted_other_item->getItemVal()))
			result = true;


		// chek if there are sorting options to apply 
		if (sort_criteria != NULL)
		{
			// if sorting is in descending order the result is reversed 
			if (!(sort_criteria->getAscending()))
				result = !result;
		}

		return result;
	}

	//virtual bool IsSmallerThan(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL)
	//{
	//	bool result = false;

	//	// if the other item is "empty" (non allocated) don't do any comparison
	//	if (other_item == NULL)
	//		return false;


	//	// first typecast the other item to confimr it is the same as this;
	//	integer_itemWithLimits* typecasted_other_item = typecastItem(other_item, this);

	//	// check that it worked
	//	if (typecasted_other_item == NULL)
	//	{
	//		cout << endl << "Item type is: ";
	//		cout << itemTypeName << endl;
	//		return false;
	//		// items of the wrong type (or null pointers) will be pushed to the end of the list
	//	}
	//	int test = getItemVal();
	//	int otherTest = typecasted_other_item->getItemVal();
	//	// now verify if the other item is larger than the curren
	//	if (getItemVal() < (typecasted_other_item->getItemVal()))
	//		result = true;


	//	// chek if there are sorting options to apply 
	//	if (sort_criteria != NULL)
	//	{
	//		// if sorting is in descending order the result is reversed 
	//		if (!(sort_criteria->getAscending()))
	//			result = !result;
	//	}

	//	return result;
	//}

	virtual bool IsEqualTo(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL)
	{
		bool result = false;

		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item == NULL)
			return false;


		// first typecast the other item to confimr it is the same as this;
		integer_itemWithLimits* typecasted_other_item = typecastItem(other_item, this);

		// check that it worked
		if (typecasted_other_item == NULL)
		{
			cout << endl << "Item type is: ";
			cout << itemTypeName << endl;
			return false;
			// items of the wrong type (or null pointers) will be pushed to the end of the list
		}
		int test = getItemVal();
		int otherTest = typecasted_other_item->getItemVal();
		// now verify if the other item is larger than the curren
		if (getItemVal() == (typecasted_other_item->getItemVal()))
			result = true;


		// chek if there are sorting options to apply 
		if (sort_criteria != NULL)
		{
			// if sorting is in descending order the result is reversed 
			if (!(sort_criteria->getAscending()))
				result = !result;
		}

		return result;
	}

	virtual basic_item* allocateEmptyItem()
	{
		basic_item* result = new integer_itemWithLimits;
		if (result == NULL)
		{
			cout << endl << "Out of memeory allocating ";
			cout << itemTypeName << endl;
		}
		return result;
	}
	virtual void deallocateItem(basic_item* itemToDestroy)
	{
		if (itemToDestroy != NULL)
		{
			// first typecast the other item to confimr it is the same as this;
			integer_item* typecasted_other_item = typecastItem(itemToDestroy, this);
			delete typecasted_other_item;
		}
	}

	virtual bool compatibilityCheck(basic_item* other_item)
	{
		bool result = false;

		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item != NULL)
		{
			// typecast the other item to confirm it is the same as this;
			integer_item* typecasted_other_item = typecastItem(other_item, this);
			if (typecasted_other_item != NULL)
				result = true;
			else
			{
				cout << endl << "Check failed for Item type: ";
				cout << itemTypeName << endl;
			}
		}
		return result;
	}

};

class date_item: public basic_item{
protected:
	bool dataSortType = true;

	unsigned int date_array[3]; // index 0 is day, 1 is month, 2 is year

	enum day_month_year{day, month, year, sup_val};
	day_month_year dmy;
	string name_type = "no name";
	bool date_set;

public:
	date_item() {
		date_set = false;
		itemTypeName = "date_item";
	}

	date_item(string name_val) {
		date_set = false;
		itemTypeName = "date_item";
		nameType = name_val;
	}

	~date_item() { cout << "Date item destructor called" << endl; }

	virtual void enterItemFromKeyboard()
	{
		int temp_day, temp_month, temp_year;
		bool valid = false;

		do {
			cout << "Enter day" << endl;
			cin >> temp_day;
			cout << "Enter month" << endl;
			cin >> temp_month;
			cout << "Enter year" << endl;
			cin >> temp_year;

			valid = true;

			if (temp_day < 1 || temp_day > 31 || temp_year < 1900 || temp_year > 3000)
			{
				cout << "Invalid date. Please enter again" << endl;
				valid = false;
			}
			else if (temp_month < 1 || temp_month > 12)
			{
				cout << "Invalid month. Please enter again" << endl;
				valid = false;
			}
			else if (temp_month == 2)
			{
				if (temp_day < 1 || temp_day > 28)
				{
					cout << "Invalid date. Please enter again " << endl;
					valid = false;
				}
			}
			else if (temp_month == 4 || temp_month == 6 || temp_month == 9 || temp_month == 11)
			{
				if (temp_day > 30)
				{
					cout << "Invalid date 2. Please enter again" << endl;
					valid = false;
				}
			}

		} while (valid == false);

		//date_array[0] = temp_day;
		date_array[day] = temp_day;
		date_array[month] = temp_month;
		date_array[year] = temp_year;
		
		date_set = true;
	}

	// functions to return day/month/year
	int getDay() { return date_array[day]; }
	//virtual bool IsLargerThan(basic_item* basicItem, basic_sort_criteria* sort_criteria_ptr = NULL)
	//{
	//	bool result = false;

	//	if (other_item == null)
	//		return false;

	int getMonth() { return date_array[month]; }
		
	int getYear() { return date_array[year]; }
		



	virtual void generateRandomItem()
	{
		int temp_day;
		int temp_month;
		int temp_year;

		bool valid = false;

		do {
			temp_day = rand() % 31 + 1;
			temp_month = rand() % 12 + 1;
			temp_year = 1900 + rand() % 100 + 1;

			valid = true;

			if (temp_day < 1 || temp_day > 31)
			{
				valid = false;
			}
			else if (temp_month < 1 || temp_month > 12)
			{
				valid = false;
			}
			else if (temp_month == 2)
			{
				if (temp_day < 1 || temp_day > 28)
				{
					valid = false;
				}
			}
			else if (temp_month == 4 || temp_month == 6 || temp_month == 9 || temp_month == 11)
			{
				if (temp_day > 30)
				{
					valid = false;
				}
			}
		} while (valid == false);

		date_array[0] = temp_day;
		date_array[1] = temp_month;
		date_array[2] = temp_year;

		date_set = true;
	}



	virtual void printItemOnScreen()
	{
		if (date_set == true)
		{
			cout << "DD = ";

			if (date_array[0] < 10)
			{
				cout << "0";
			}
			cout << date_array[0] << " ";

			cout << "MM = ";
			if (date_array[1] < 10) { cout << "0"; }
			cout << date_array[1] << " ";

			cout << "YYYY = " << date_array[2] << endl;
		}
		else
		{
			cout << "Date not set" << " ";
		}
	}

	virtual void printItemAndTypeOnScreen()
	{
		cout << "Item name is " << name_type << ". ";
		printItemOnScreen();
	}

	virtual string getNameType() { return nameType; }

	// ascending should bring up the oldest item first
	virtual bool IsLargerThan(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL)
	{
		bool result = false;
		compositeItem_sort_criteria CompositeSortOption;

		if (sort_criteria != NULL)
		{
			// first typecast the other item to confimr it is the same as this;
			compositeItem_sort_criteria* typecasted_sortoption = typecastItem(sort_criteria, &CompositeSortOption);
			if (typecasted_sortoption != NULL)
				CompositeSortOption.setOption(typecasted_sortoption->getOption());
		}


		auto test = CompositeSortOption.getOption();
		switch (CompositeSortOption.getOption()) {
			case(compositeItem_sort_criteria::DayAndMonth):
				dataSortType = false;
				break;
		}
		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item == NULL)
			return false;


		// first typecast the other item to confimr it is the same as this;
		date_item* typecasted_other_item = typecastItem(other_item, this);

		// check that it worked
		if (typecasted_other_item == NULL)
		{
			cout << endl << "Item type is: ";
			cout << itemTypeName << endl;
			return false;
			// items of the wrong type (or null pointers) will be pushed to the end of the list
		}

		// now verify if the other item is larger than the current
		// for the date item, is LARGER constitutes an OLDER DATE
		// ie YY = 1994 is considered larger than YY = 2000

		// if year is smaller (earlier)
		// if the year is the same, proceed to month
		/*int testval = getYear();
		int test_val = typecasted_other_item->getYear();*/
		if(dataSortType)
		{
			if (getYear() < (typecasted_other_item->getYear()))
				result = true;
			else if (getYear() == (typecasted_other_item->getYear()))
			{
				if (getMonth() < typecasted_other_item->getMonth())
					result = true;
				else if (getMonth() == typecasted_other_item->getMonth())
				{
					// what if two dates are the same, the item we are calling from is considered larger
					if (getDay() <= typecasted_other_item->getDay())
						result = true;
					else
						result = false;
				}
			}
		}
		else
		{
			if (getMonth() < typecasted_other_item->getMonth())
				result = true;
			else if (getMonth() == typecasted_other_item->getMonth())
			{
				// what if two dates are the same, the item we are calling from is considered larger
				if (getDay() <= typecasted_other_item->getDay())
					result = true;
				else
					result = false;
			}
		}



		// chek if there are sorting options to apply 
		if (sort_criteria != NULL)
		{
			// if sorting is in descending order the result is reversed 
			if (!(sort_criteria->getAscending()))
				result = !result;
		}

		return result;
	}

	// ascending should bring up the oldest item first
	//virtual bool IsSmallerThan(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL)
	//{
	//	bool result = false;
	//	compositeItem_sort_criteria CompositeSortOption;

	//	if (sort_criteria != NULL)
	//	{
	//		// first typecast the other item to confimr it is the same as this;
	//		compositeItem_sort_criteria* typecasted_sortoption = typecastItem(sort_criteria, &CompositeSortOption);
	//		if (typecasted_sortoption != NULL)
	//			CompositeSortOption.setOption(typecasted_sortoption->getOption());
	//	}


	//	auto test = CompositeSortOption.getOption();
	//	switch (CompositeSortOption.getOption()) {
	//	case(compositeItem_sort_criteria::DayAndMonth):
	//		dataSortType = false;
	//		break;
	//	}
	//	// if the other item is "empty" (non allocated) don't do any comparison
	//	if (other_item == NULL)
	//		return false;


	//	// first typecast the other item to confimr it is the same as this;
	//	date_item* typecasted_other_item = typecastItem(other_item, this);

	//	// check that it worked
	//	if (typecasted_other_item == NULL)
	//	{
	//		cout << endl << "Item type is: ";
	//		cout << itemTypeName << endl;
	//		return false;
	//		// items of the wrong type (or null pointers) will be pushed to the end of the list
	//	}

	//	// now verify if the other item is larger than the current
	//	// for the date item, is LARGER constitutes an OLDER DATE
	//	// ie YY = 1994 is considered larger than YY = 2000

	//	// if year is smaller (earlier)
	//	// if the year is the same, proceed to month
	//	/*int testval = getYear();
	//	int test_val = typecasted_other_item->getYear();*/

	//	// else statement added, there was no false before when year  was smaller
	//	if (dataSortType)
	//	{
	//		// HERE
	//		if (getYear() > (typecasted_other_item->getYear()))
	//			result = true;
	//		else if (getYear() == (typecasted_other_item->getYear()))
	//		{
	//			if (getMonth() > typecasted_other_item->getMonth())
	//				result = true;
	//			else if (getMonth() == typecasted_other_item->getMonth())
	//			{
	//				// what if two dates are the same, the item we are calling from is considered smaller
	//				if (getDay() >= typecasted_other_item->getDay())
	//					result = true;
	//				else
	//					result = false;
	//			}
	//			else
	//				result = false;
	//		}
	//		else
	//			result = false;
	//	}
	//	else
	//	{
	//		if (getMonth() > typecasted_other_item->getMonth())
	//			result = true;
	//		else if (getMonth() == typecasted_other_item->getMonth())
	//		{
	//			// what if two dates are the same, the item we are calling from is considered larger
	//			if (getDay() >= typecasted_other_item->getDay())
	//				result = true;
	//			else
	//				result = false;
	//		}
	//		else
	//			result = false;
	//	}



	//	// chek if there are sorting options to apply 
	//	if (sort_criteria != NULL)
	//	{
	//		// if sorting is in descending order the result is reversed 
	//		if (!(sort_criteria->getAscending()))
	//			result = !result;
	//	}

	//	return result;
	//}

	virtual bool IsEqualTo(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL)
	{
		bool result = false;
		compositeItem_sort_criteria CompositeSortOption;

		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item == NULL)
			return false;


		// first typecast the other item to confimr it is the same as this;
		date_item* typecasted_other_item = typecastItem(other_item, this);

		// check that it worked
		if (typecasted_other_item == NULL)
		{
			cout << endl << "Item type is: ";
			cout << itemTypeName << endl;
			return false;
			// items of the wrong type (or null pointers) will be pushed to the end of the list
		}

		// now verify if the other item is larger than the curren
		if (dataSortType)
		{
			if (getYear() == (typecasted_other_item->getYear()))
				if (getMonth() == typecasted_other_item->getMonth())
					if (getDay() == typecasted_other_item->getDay())
						result = true;
					else
					{
						if (getMonth() == typecasted_other_item->getMonth())
							if (getDay() <= typecasted_other_item->getDay())
								result = true;
					}

		}
		// chek if there are sorting options to apply 
		if (sort_criteria != NULL)
		{
			// if sorting is in descending order the result is reversed 
			if (!(sort_criteria->getAscending()))
				result = !result;
		}

		return result;
	}


	virtual basic_item* allocateEmptyItem()
	{
		basic_item* result = new date_item;
		if (result == NULL)
		{
			cout << endl << "Out of memeory allocating ";
			cout << itemTypeName << endl;
		}
		return result;
	}
	virtual void deallocateItem(basic_item* itemToDestroy)
	{
		if (itemToDestroy != NULL)
		{
			delete[] date_array;
			// first typecast the other item to confimr it is the same as this;
			basic_item* typecasted_other_item = typecastItem(itemToDestroy, this);
			delete typecasted_other_item;
		}
	}

	virtual bool compatibilityCheck(basic_item* other_item)
	{
		bool result = false;

		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item != NULL)
		{
			// typecast the other item to confirm it is the same as this;
			date_item* typecasted_other_item = typecastItem(other_item, this);
			if (typecasted_other_item != NULL)
				result = true;
			else
			{
				cout << endl << "Check failed for Item type: ";
				cout << itemTypeName << endl;
			}
		}
		return result;
	}


};

class composite_item: public basic_item{
protected:
	// string to hold first name, second name

	vector<basic_item*> composite_item_vector;


public:
	composite_item() {
		itemTypeName = "composite_item";
		composite_item_vector.push_back(new basic_string_item("first name"));
		composite_item_vector.push_back(new basic_string_item("second name"));
		composite_item_vector.push_back(new date_item("DOB"));
	}
	~composite_item() { cout << "composite_item destructor call" << endl; }

	basic_item* getCompsite_item(int item)
	{
		return composite_item_vector[item];
	}
	virtual void enterItemFromKeyboard()
	{
		cout << "first name:"<< endl;
		composite_item_vector[0]->enterItemFromKeyboard();
		cout << "second name:" << endl;
		composite_item_vector[1]->enterItemFromKeyboard();
		cout << "date" << endl;
		composite_item_vector[2]->enterItemFromKeyboard();
	}

	//These must be implemented by any derived item	
	virtual void printItemOnScreen()
	{
		cout << "first name: ";
		composite_item_vector[0]->printItemOnScreen();
		cout << "second name: ";
		composite_item_vector[1]->printItemOnScreen();
		cout << "date: ";
		composite_item_vector[2]->printItemOnScreen();
	}
	virtual string getNameType() { return nameType; }
	//virtual void loadItemFromFile(FILE* fin)=0;
	virtual void generateRandomItem()
	{
		composite_item_vector[0]->generateRandomItem();
		composite_item_vector[1]->generateRandomItem();
		composite_item_vector[2]->generateRandomItem();
	}

	virtual bool IsLargerThan(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL)
	{

		bool result = false;
		compositeItem_sort_criteria CompositeSortOption;

		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item == NULL)
			return false;
		

		// first typecast the other item to confimr it is the same as this;
		composite_item* typecasted_other_item = typecastItem(other_item, this);


		//check that it worked
		if (typecasted_other_item == NULL)
		{
			cout << "Other item is not of type intmat2x2_item." << endl;
			return false;
			// items of the wrong type (or null pointers) will be pushed to the end of the list
		}

		// check if the sort_option is specific for the int_mat
		if (sort_criteria != NULL)
		{
			// first typecast the other item to confimr it is the same as this;
			compositeItem_sort_criteria* typecasted_sortoption = typecastItem(sort_criteria, &CompositeSortOption);
			if (typecasted_sortoption != NULL)
				CompositeSortOption.setOption(typecasted_sortoption->getOption());
		}

		// now verify if the other item is larger than the current
		switch (CompositeSortOption.getOption()) {
		case(compositeItem_sort_criteria::firstName):
			result = composite_item_vector[0]->IsLargerThan(typecasted_other_item->getCompsite_item(0), sort_criteria);
			break;
		case(compositeItem_sort_criteria::secondName):	
			result = composite_item_vector[1]->IsLargerThan(typecasted_other_item->getCompsite_item(1), sort_criteria);
			break;
		case(compositeItem_sort_criteria::fullDate):
			result = composite_item_vector[2]->IsLargerThan(typecasted_other_item->getCompsite_item(2), sort_criteria);
			break;
		case(compositeItem_sort_criteria::DayAndMonth):
			result = composite_item_vector[2]->IsLargerThan(typecasted_other_item->getCompsite_item(2), sort_criteria);
			break;

		}

		// chek if ascending/decenting sorting applies 
		if (sort_criteria != NULL)
		{
			// if sorting is in descending order the result is reversed 
			if (!(sort_criteria->getAscending()))
				result = !result;
		}

		return result;
	}

	virtual bool IsEqualTo(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL)
	{

		bool result = false;
		compositeItem_sort_criteria CompositeSortOption;

		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item == NULL)
			return false;


		// first typecast the other item to confimr it is the same as this;
		composite_item* typecasted_other_item = typecastItem(other_item, this);

		// check that it worked
		if (typecasted_other_item == NULL)
		{
			cout << endl << "Item type is: ";
			cout << itemTypeName << endl;
			return false;
			// items of the wrong type (or null pointers) will be pushed to the end of the list
		}

		switch (CompositeSortOption.getOption()) {
		case(compositeItem_sort_criteria::firstName):
			result = composite_item_vector[0]->IsEqualTo(typecasted_other_item->getCompsite_item(0), sort_criteria);
			break;
		case(compositeItem_sort_criteria::secondName):
			result = composite_item_vector[1]->IsEqualTo(typecasted_other_item->getCompsite_item(1), sort_criteria);
			break;
		case(compositeItem_sort_criteria::fullDate):
			result = composite_item_vector[2]->IsEqualTo(typecasted_other_item->getCompsite_item(2), sort_criteria);
			break;
		case(compositeItem_sort_criteria::DayAndMonth):
			result = composite_item_vector[2]->IsEqualTo(typecasted_other_item->getCompsite_item(2), sort_criteria);
			break;


			// chek if there are sorting options to apply 
			if (sort_criteria != NULL)
			{
				// if sorting is in descending order the result is reversed 
				if (!(sort_criteria->getAscending()))
					result = !result;
			}
	
		}
		return result;
	}

	virtual basic_item* allocateEmptyItem()
	{
		basic_item* result = new composite_item;
		if (result == NULL)
			{
				cout << endl << "Out of memory allocating ";
				cout << itemTypeName << endl;
			}
		return result;
	}
	virtual void deallocateItem(basic_item * itemToDestroy)
		{

			if (itemToDestroy != NULL)
			{
				// first typecast the other item to confirm it is the same as this;
				composite_item* typecasted_other_item = typecastItem(itemToDestroy, this);
				delete typecasted_other_item;
			}
		}

	virtual bool compatibilityCheck(basic_item * other_item)
		{
			bool result = false;

			// if the other item is "empty" (non allocated) don't do any comparison
			if (other_item != NULL)
			{
				// typecast the other item to confirm it is the same as this;
				composite_item* typecasted_other_item = typecastItem(other_item, this);
				if (typecasted_other_item != NULL)
					result = true;
				else
				{
					cout << endl << "Check failed for Item type: ";
					cout << itemTypeName << endl;
				}
			}


			return result;
		}
};

class intmat_item: public basic_item{
protected:
	static const int matsize=2;
	int matrix[matsize][matsize];

	int getMatVal(int i, int j)
	{
		if( (i>=0 && i<matsize) && (j>=0 && j<matsize) && (!isEmpty()) )
		{
			return matrix[i][j];
		}
		else
			return 0;
	}

	void setMatVal(int i, int j, int val)
	{
		if( (i>=0 && i<matsize) && (j>=0 && j<matsize) && (isEmpty()) )
		{
			matrix[i][j] = val;
		}
	}

public:
	intmat_item(){itemTypeName = "intmat_item";}
	~intmat_item() { cout << "intmat_item destructor called" << endl; } // can remove the printout after testing
	
	//find and report the largest value in the matrix
	int getMaxVal()
	{
		int max=getMatVal(0, 0);
		for(int i=0; i<matsize; i++)
			for(int j=0; j<matsize; j++)
				if( max < getMatVal(i,j) )
					max=getMatVal(i,j);
		return max;
	}
	
	//compute and report matrix determinant
	int getDet()
	{	
		if (matsize == 2)
		{
			int maindiag = getMatVal(0, 0) * getMatVal(1, 1);
			int secdiag = getMatVal(0, 1) * getMatVal(1, 0);
			return (maindiag * ((-1)*secdiag));
		}
		cout << endl << " Determinant not ready for general case " << endl;
		return 0;
	}

	virtual void printItemOnScreen()
	{
		if(isEmpty())
			cout << "Item is empty." << endl;
		else
		{
			for(int i=0; i<matsize; i++)
				for(int j=0; j<matsize; j++)
					cout << "Item pos "<< i << " , " << j << " value is " << getMatVal(i,j) << " . " << endl;
			cout << endl;
		}
	}

	virtual string getNameType() { return nameType; }

	virtual void enterItemFromKeyboard()
	{
		if (isLocked())
			cout << "Error in enterItemFromKeyboard: Item is locked" << endl;
		else
		{
			int val;
			for (int i = 0; i < matsize; i++)
				for (int j = 0; j < matsize; j++)
				{
					cout << "Enter integer value for";
					cout << "Item pos " << i << " , " << j << " :" << endl;
					cin >> val;
					setMatVal(i, j, val);
				}
			cout << "Matrix filled" << endl;
		}
	}

	virtual void generateRandomItem()
	{
		if (isLocked())
			cout << "Error in generateRandomItem: Item is locked" << endl;
		else
		{
			int item;
			int max_rand_val = 100;

			for (int i = 0; i < matsize; i++)
				for (int j = 0; j < matsize; j++)
				{

					item = rand();
					item = item % (max_rand_val + 1);

					// turn to negative 30% of the time
					if ((rand() % 10) >= 7)
						item = (-1)*item;

					setMatVal(i, j, item);
				}
			// item filled
			empty = false;
		}
	}
	
	//virtual void loadItemFromFile(FILE* fin);
	
	virtual bool IsLargerThan(basic_item* other_item, basic_sort_criteria* sort_criteria=NULL)
	{
		bool result=false;
		intmat_sort_criteria matSortOption;
		
		// if the other item is "empty" (non allocated) don't do any comparison
		if(other_item==NULL)
			return false;

		
		// first typecast the other item to confimr it is the same as this;
		intmat_item* typecasted_other_item = typecastItem(other_item, this);
		
		// check that it worked
		if(typecasted_other_item==NULL)
		{
			cout << "Other item is not of type intmat2x2_item." << endl;
			return false;
			// items of the wrong type (or null pointers) will be pushed to the end of the list
		}

		// check if the sort_option is specific for the int_mat
		if (sort_criteria != NULL)
		{
			// first typecast the other item to confimr it is the same as this;
			intmat_sort_criteria* typecasted_sortoption = typecastItem(sort_criteria, &matSortOption);
			if (typecasted_sortoption != NULL)
				matSortOption.setOption(typecasted_sortoption->getOption());
		}

		// now verify if the other item is larger than the curren
		switch (matSortOption.getOption()) {
		case(intmat_sort_criteria::sort_maxval):
			if (getMaxVal() > (typecasted_other_item->getMaxVal()))
				result = true;
			break;
		case(intmat_sort_criteria::sort_determ):
			if( getDet() > (typecasted_other_item->getDet()) )
				result = true;
			break;
		}
		
		// chek if ascending/decenting sorting applies 
		if(sort_criteria!=NULL)
		{
			// if sorting is in descending order the result is reversed 
			if( !( sort_criteria->getAscending() ) )
				result=!result;
		}

		return result;
	}

	virtual bool IsEqualTo(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL)
	{
		bool result = false;
		intmat_sort_criteria matSortOption;

		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item == NULL)
			return false;


		// first typecast the other item to confimr it is the same as this;
		intmat_item* typecasted_other_item = typecastItem(other_item, this);

		// check that it worked
		if (typecasted_other_item == NULL)
		{
			cout << endl << "Item type is: ";
			cout << itemTypeName << endl;
			return false;
			// items of the wrong type (or null pointers) will be pushed to the end of the list
		}

		// now verify if the other item is larger than the curren
		switch (matSortOption.getOption()) {
		case(intmat_sort_criteria::sort_maxval):
			if (getMaxVal() > (typecasted_other_item->getMaxVal()))
				result = true;
			break;
		case(intmat_sort_criteria::sort_determ):
			if (getDet() > (typecasted_other_item->getDet()))
				result = true;
			break;
		}
		// chek if there are sorting options to apply 
		if (sort_criteria != NULL)
		{
			// if sorting is in descending order the result is reversed 
			if (!(sort_criteria->getAscending()))
				result = !result;
		}

		return result;
	}

	virtual basic_item* allocateEmptyItem()
	{
		basic_item* result = new intmat_item;
		if (result == NULL)
		{
			cout << endl << "Out of memory allocating ";
			cout << itemTypeName << endl;
		}
		return result;
	}
	virtual void deallocateItem(basic_item* itemToDestroy)
	{
		if (itemToDestroy != NULL)
		{
			// first typecast the other item to confirm it is the same as this;
			intmat_item* typecasted_other_item = typecastItem(itemToDestroy, this);
			delete typecasted_other_item;
		}
	}
	virtual bool compatibilityCheck(basic_item* other_item)
	{
		bool result = false;

		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item != NULL)
		{
			// typecast the other item to confirm it is the same as this;
			intmat_item* typecasted_other_item = typecastItem(other_item, this);
			if (typecasted_other_item != NULL)
				result = true;
			else
			{
				cout << endl << "Check failed for Item type: ";
				cout << itemTypeName << endl;
			}
		}
		return result;
	}

};

class basic_string_itemWithLimits : public basic_string_item
{
protected:
	vector<string> allowed_strings_vector;


public:
	basic_string_itemWithLimits() {
		vector<string> tempVect = {"test_val", "test_val2"};
		allowed_strings_vector = tempVect;
		itemTypeName = "composite_item";
	}
	basic_string_itemWithLimits(string name, vector<string> allowed_strings) {
		
		nameType = name;
		allowed_strings_vector = allowed_strings;
	}
	~basic_string_itemWithLimits() { cout << "basic_string_itemWithLimits destructor call" << endl; }

	string getAllowed_strings_item(int item)
	{
		return allowed_strings_vector[item];
	}

	virtual void setLocked(bool lock_input)
	{
		if (!isEmpty())
			locked = lock_input;
	}

	virtual void printItemOnScreen()
	{
		if (isEmpty())
			cout << "Item is empty." << endl;
		else
			cout << "Item value is " << item_value << " . " << endl;
	}

	virtual void printItemAndTypeOnScreen()
	{
		if (isEmpty())
			cout << "Item is empty." << endl;
		else
			cout << "Item value is " << item_value << " . " << endl;
	}

	virtual string getNameType() { return nameType; }

	virtual void enterItemFromKeyboard()
	{
		string tempVal;
		if (isLocked())
			cout << "Error in enterItemFromKeyboard: Item is locked" << endl;
		else
		{
			cout << "Please enter a string. These are the available options:" << endl;
			for (std::vector<string>::const_iterator i = allowed_strings_vector.begin(); i != allowed_strings_vector.end(); ++i)
				cout << *i << ' ';
			cout << endl;
			cout << "Insert " << nameType << " then hit enter." << endl;
			cin >> tempVal;
			
			while (!(find(allowed_strings_vector.begin(), allowed_strings_vector.end(), tempVal) != allowed_strings_vector.end()))
			{
				cout << "Invalid options. Please try again." << endl;

				cout << "these are the available options" << endl;
				for (std::vector<string>::const_iterator i = allowed_strings_vector.begin(); i != allowed_strings_vector.end(); ++i)
					cout << *i << ' ';
				cout << endl;
				cout << "Insert " << nameType << " then hit enter." << endl;
				cin >> tempVal;
				cout << endl;
			}

			item_value = tempVal;
			empty = false;
			
		}
	}

	virtual void generateRandomItem()
	{

		int index = rand();
		index = index % allowed_strings_vector.size();

		item_value = allowed_strings_vector[index];
		empty = false;
	}



	//virtual void loadItemFromFile(FILE* fin);

	virtual bool IsLargerThan(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL)
	{
		bool result = false;

		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item == NULL)
			return false;


		// first typecast the other item to confimr it is the same as this;
		basic_string_item* typecasted_other_item = typecastItem(other_item, this);

		// check that it worked
		if (typecasted_other_item == NULL)
		{
			cout << endl << "Item type is: ";
			cout << itemTypeName << endl;
			return false;
			// items of the wrong type (or null pointers) will be pushed to the end of the list
		}

		for (int i = 0; i < item_value.size(); i++) {
			if (item_value[i] == typecasted_other_item->getItemVal()[i])
			{
				continue;
			}
			else if (item_value[i] > typecasted_other_item->getItemVal()[i])
			{
				result = true;
				break;
			}
			else
			{
				result = false;
				break;
			}
		}



		// chek if there are sorting options to apply 
		if (sort_criteria != NULL)
		{
			// if sorting is in descending order the result is reversed 
			if (!(sort_criteria->getAscending()))
				result = !result;
		}

		return result;
	}

	//virtual bool IsSmallerThan(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL)
	//{
	//	bool result = false;

	//	// if the other item is "empty" (non allocated) don't do any comparison
	//	if (other_item == NULL)
	//		return false;


	//	// first typecast the other item to confimr it is the same as this;
	//	basic_string_item* typecasted_other_item = typecastItem(other_item, this);

	//	// check that it worked
	//	if (typecasted_other_item == NULL)
	//	{
	//		cout << endl << "Item type is: ";
	//		cout << itemTypeName << endl;
	//		return false;
	//		// items of the wrong type (or null pointers) will be pushed to the end of the list
	//	}

	//	for (int i = 0; i < item_value.size(); i++) {
	//		if (item_value[i] == typecasted_other_item->getItemVal()[i])
	//		{
	//			continue;
	//		}
	//		// changed the operator to change the function
	//		else if (item_value[i] < typecasted_other_item->getItemVal()[i])
	//		{
	//			result = true;
	//			break;
	//		}
	//		else
	//		{
	//			result = false;
	//			break;
	//		}
	//	}



	//	// chek if there are sorting options to apply 
	//	if (sort_criteria != NULL)
	//	{
	//		// if sorting is in descending order the result is reversed 
	//		if (!(sort_criteria->getAscending()))
	//			result = !result;
	//	}

	//	return result;
	//}

	virtual bool IsEqualTo(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL)
	{
		{
			bool result = false;

			// if the other item is "empty" (non allocated) don't do any comparison
			if (other_item == NULL)
				return false;


			// first typecast the other item to confimr it is the same as this;
			basic_string_item* typecasted_other_item = typecastItem(other_item, this);

			// check that it worked
			if (typecasted_other_item == NULL)
			{
				cout << endl << "Item type is: ";
				cout << itemTypeName << endl;
				return false;
				// items of the wrong type (or null pointers) will be pushed to the end of the list
			}

			if (includes(other_item))
				result = true;
			else
				result = false;



			// chek if there are sorting options to apply 
			if (sort_criteria != NULL)
			{
				// if sorting is in descending order the result is reversed 
				if (!(sort_criteria->getAscending()))
					result = !result;
			}

			return result;
		}
	}

	virtual bool includes(basic_item* other_item)
	{
		bool result = false;

		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item == NULL)
			return false;


		// first typecast the other item to confimr it is the same as this;
		basic_string_item* typecasted_other_item = typecastItem(other_item, this);

		// check that it worked
		if (typecasted_other_item == NULL)
		{
			cout << endl << "Item type is: ";
			cout << itemTypeName << endl;
			return false;
			// items of the wrong type (or null pointers) will be pushed to the end of the list
		}

		string other_value = typecasted_other_item->getItemVal();

		if (item_value.find(other_value) != string::npos || other_value.find(item_value) != string::npos)
			result = true;
		else
			result = false;


		//for (int i = 0; i < item_value.size(); i++) {
		//	for (int j = 0; j < other_value.size(); j++)
		//	{
		//		if (item_value[j] == other_value[j]) {
		//			result = true;
		//			continue;
		//		}
		//		else
		//			result = false;
		//		break;
		//	}

		//}
		return result;
	}

	virtual basic_item* allocateEmptyItem()
	{
		basic_item* result = new basic_string_itemWithLimits;
		if (result == NULL)
		{
			cout << endl << "Out of memeory allocating ";
			cout << itemTypeName << endl;
		}
		return result;
	}
	virtual void deallocateItem(basic_item* itemToDestroy)
	{
		if (itemToDestroy != NULL)
		{
			// first typecast the other item to confimr it is the same as this;
			basic_string_itemWithLimits* typecasted_other_item = typecastItem(itemToDestroy, this);
			delete typecasted_other_item;
		}
	}

	virtual bool compatibilityCheck(basic_item* other_item)
	{
		bool result = false;

		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item != NULL)
		{
			// typecast the other item to confirm it is the same as this;
			basic_string_itemWithLimits* typecasted_other_item = typecastItem(other_item, this);
			if (typecasted_other_item != NULL)
				result = true;
			else
			{
				cout << endl << "Check failed for Item type: ";
				cout << itemTypeName << endl;
			}
		}
		return result;
	}

};

class studentrecord_item : public basic_item {
protected:
	// string to hold first name, second name, nationality

	vector<basic_item*> studentrecord_item_vector;
	vector<string> allowed_blood_types;
	vector<string> allowed_levels;
	vector<string> allowed_programs;


public:
	studentrecord_item() {

		allowed_blood_types = { "O-", "O+", "A-", "A+", "B-", "B+", "AB-", "AB+" };
		allowed_levels = { "MEng", "BEng" };
		allowed_programs = { "EEE", "Mech", "Chem", "Civil", "Petr" };
		itemTypeName = "studentrecord_item";

		studentrecord_item_vector.push_back(new basic_string_item("first name"));
		studentrecord_item_vector.push_back(new basic_string_item("second name"));
		studentrecord_item_vector.push_back(new basic_string_item("nationality"));
		studentrecord_item_vector.push_back(new basic_string_itemWithLimits("degree program", allowed_programs));
		studentrecord_item_vector.push_back(new integer_itemWithLimits(51000000, 52099999, "student id", false));
		studentrecord_item_vector.push_back(new basic_string_itemWithLimits("level", allowed_levels));
		studentrecord_item_vector.push_back(new integer_itemWithLimits(0, 22, "ave. cgs mark"));
		studentrecord_item_vector.push_back(new date_item("DOB"));
		studentrecord_item_vector.push_back(new basic_string_itemWithLimits("blood type", allowed_blood_types)); // can make a blod_type item?
		studentrecord_item_vector.push_back(new integer_itemWithLimits(100, 250, "height"));

	}
	~studentrecord_item() { cout << "studentrecord_item destructor call" << endl; }

	basic_item* getStudentrecord_item(int item)
	{
		return studentrecord_item_vector[item];
	}

	virtual void printItemOnScreen() {
		for (int i = 0; i < studentrecord_item_vector.size(); i++)
		{
			cout << "Item type is: " << studentrecord_item_vector[i]->getNameType()  << ". ";
			studentrecord_item_vector[i]->printItemOnScreen();
		}
	}

	virtual string getNameType() { return nameType; }

	virtual void printItemAndNameOnScreen() {
		for (int i = 0; i < studentrecord_item_vector.size(); i++)
		{
			cout << "Item type is : " << studentrecord_item_vector[i]->getNameType();
			studentrecord_item_vector[i]->printItemOnScreen();
		}
	}

	virtual void enterItemFromKeyboard()
	{
		for (int i = 0; i < studentrecord_item_vector.size(); i++)
		{
			studentrecord_item_vector[i]->enterItemFromKeyboard();
		}
	}

	virtual void generateRandomItem()
	{
		for (int i = 0; i < studentrecord_item_vector.size(); i++)
		{
			studentrecord_item_vector[i]->generateRandomItem();
		}
	}

	virtual bool IsLargerThan(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL)
	{

		bool result = false;
		studentrecord_item_sort_criteria RecordSortOption;

		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item == NULL)
			return false;


		// first typecast the other item to confimr it is the same as this;
		studentrecord_item* typecasted_other_item = typecastItem(other_item, this);


		//check that it worked
		if (typecasted_other_item == NULL)
		{
			cout << "Other item is not of type intmat2x2_item." << endl;
			return false;
			// items of the wrong type (or null pointers) will be pushed to the end of the list
		}

		// check if the sort_option is specific for the studentrecord_item_sort_criteria
		if (sort_criteria != NULL)
		{
			// first typecast the other item to confimr it is the same as this;
			studentrecord_item_sort_criteria* typecasted_sortoption = typecastItem(sort_criteria, &RecordSortOption);
			if (typecasted_sortoption != NULL)
				RecordSortOption.setOption(typecasted_sortoption->getOption());

		}
		
		// now verify if the other item is larger than the curren
		//for (int i = (int)(studentrecord_item_sort_criteria::start); i < (int)(studentrecord_item_sort_criteria::stop); i++)
		//{

		//}


		switch (RecordSortOption.getOption()) {
		case(studentrecord_item_sort_criteria::firstName):
			result = studentrecord_item_vector[0]->IsLargerThan(typecasted_other_item->getStudentrecord_item(0), sort_criteria);
			break;
		case(studentrecord_item_sort_criteria::secondName):
			result = studentrecord_item_vector[1]->IsLargerThan(typecasted_other_item->getStudentrecord_item(1), sort_criteria);
			break;
		case(studentrecord_item_sort_criteria::nationality):
			result = studentrecord_item_vector[2]->IsLargerThan(typecasted_other_item->getStudentrecord_item(2), sort_criteria);
			break;
		case(studentrecord_item_sort_criteria::programme):
			result = studentrecord_item_vector[3]->IsLargerThan(typecasted_other_item->getStudentrecord_item(3), sort_criteria);
			break;
		case(studentrecord_item_sort_criteria::studedId):
			result = studentrecord_item_vector[4]->IsLargerThan(typecasted_other_item->getStudentrecord_item(4), sort_criteria);
			break;
		case(studentrecord_item_sort_criteria::level):
			result = studentrecord_item_vector[5]->IsLargerThan(typecasted_other_item->getStudentrecord_item(5), sort_criteria);
			break;
		case(studentrecord_item_sort_criteria::CGS):
			result = studentrecord_item_vector[6]->IsLargerThan(typecasted_other_item->getStudentrecord_item(6), sort_criteria);
			break;
		case(studentrecord_item_sort_criteria::DayAndMonth):
			result = studentrecord_item_vector[7]->IsLargerThan(typecasted_other_item->getStudentrecord_item(7), sort_criteria);
			break;
		case(studentrecord_item_sort_criteria::fullDate):
			result = studentrecord_item_vector[7]->IsLargerThan(typecasted_other_item->getStudentrecord_item(7), sort_criteria);
			break;
		case(studentrecord_item_sort_criteria::bloodType):
			result = studentrecord_item_vector[8]->IsLargerThan(typecasted_other_item->getStudentrecord_item(8), sort_criteria);
			break;
		case(studentrecord_item_sort_criteria::height):
			result = studentrecord_item_vector[9]->IsLargerThan(typecasted_other_item->getStudentrecord_item(9), sort_criteria);
			break;
		}
	


		// chek if ascending/decenting sorting applies 
		if (sort_criteria != NULL)
		{
			// if sorting is in descending order the result is reversed 
			if (!(sort_criteria->getAscending()))
				result = !result;
		}

		return result;
	}

	virtual bool IsEqualTo(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL)
	{

		bool result = false;
		studentrecord_item_sort_criteria RecordSortOption;

		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item == NULL)
			return false;


		// first typecast the other item to confimr it is the same as this;
		studentrecord_item* typecasted_other_item = typecastItem(other_item, this);


		//check that it worked
		if (typecasted_other_item == NULL)
		{
			cout << "Other item is not of type intmat2x2_item." << endl;
			return false;
			// items of the wrong type (or null pointers) will be pushed to the end of the list
		}

		// check if the sort_option is specific for the studentrecord_item_sort_criteria
		if (sort_criteria != NULL)
		{
			// first typecast the other item to confimr it is the same as this;
			studentrecord_item_sort_criteria* typecasted_sortoption = typecastItem(sort_criteria, &RecordSortOption);
			if (typecasted_sortoption != NULL)
				RecordSortOption.setOption(typecasted_sortoption->getOption());

		}

		switch (RecordSortOption.getOption()) {
		case(studentrecord_item_sort_criteria::firstName):
			result = studentrecord_item_vector[0]->IsEqualTo(typecasted_other_item->getStudentrecord_item(0), sort_criteria);
			break;
		case(studentrecord_item_sort_criteria::secondName):
			result = studentrecord_item_vector[1]->IsEqualTo(typecasted_other_item->getStudentrecord_item(1), sort_criteria);
			break;
		case(studentrecord_item_sort_criteria::nationality):
			result = studentrecord_item_vector[2]->IsEqualTo(typecasted_other_item->getStudentrecord_item(2), sort_criteria);
			break;
		case(studentrecord_item_sort_criteria::programme):
			result = studentrecord_item_vector[3]->IsEqualTo(typecasted_other_item->getStudentrecord_item(3), sort_criteria);
			break;
		case(studentrecord_item_sort_criteria::studedId):
			result = studentrecord_item_vector[4]->IsEqualTo(typecasted_other_item->getStudentrecord_item(4), sort_criteria);
			break;
		case(studentrecord_item_sort_criteria::level):
			result = studentrecord_item_vector[5]->IsEqualTo(typecasted_other_item->getStudentrecord_item(5), sort_criteria);
			break;
		case(studentrecord_item_sort_criteria::CGS):
			result = studentrecord_item_vector[6]->IsEqualTo(typecasted_other_item->getStudentrecord_item(6), sort_criteria);
			break;
		case(studentrecord_item_sort_criteria::DayAndMonth):
			result = studentrecord_item_vector[7]->IsEqualTo(typecasted_other_item->getStudentrecord_item(7), sort_criteria);
			break;
		case(studentrecord_item_sort_criteria::bloodType):
			result = studentrecord_item_vector[8]->IsEqualTo(typecasted_other_item->getStudentrecord_item(8), sort_criteria);
			break;
		case(studentrecord_item_sort_criteria::height):
			result = studentrecord_item_vector[9]->IsEqualTo(typecasted_other_item->getStudentrecord_item(9), sort_criteria);
			break;
		}

		// chek if ascending/decenting sorting applies 
		if (sort_criteria != NULL)
		{
			// if sorting is in descending order the result is reversed 
			if (!(sort_criteria->getAscending()))
				result = !result;
		}
		return result;
	}

	virtual basic_item* allocateEmptyItem()
	{
		basic_item* result = new studentrecord_item;
		if (result == NULL)
		{
			cout << endl << "Out of memory allocating ";
			cout << itemTypeName << endl;
		}
		return result;
	}

	virtual void deallocateItem(basic_item* itemToDestroy)
	{
		if (itemToDestroy != NULL)
		{
			// first typecast the other item to confirm it is the same as this;
			studentrecord_item* typecasted_other_item = typecastItem(itemToDestroy, this);
			delete typecasted_other_item;
		}
	}

	virtual bool compatibilityCheck(basic_item* other_item)
	{
		bool result = false;

		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item != NULL)
		{
			// typecast the other item to confirm it is the same as this;
			studentrecord_item* typecasted_other_item = typecastItem(other_item, this);
			if (typecasted_other_item != NULL)
				result = true;
			else
			{
				cout << endl << "Check failed for Item type: ";
				cout << itemTypeName << endl;
			}
		}


		return result;
	}

};
//#include "generalArray_v2.h"

#endif