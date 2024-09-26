//this project based on /std:c++ 20
#define NOMINMAX 
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <tuple>
#include <cmath>
#include <list>
#include <thread>
#include <windows.h>
#include <algorithm>
#include <sstream>
#include <queue>
#include <stack>
#include <numeric>
#include <time.h>
#include <format>
#include <variant>//supported by /std:c++ 17
using namespace std;
using namespace chrono;
using namespace this_thread;

vector<vector<variant<int, string>>>gifts;//2-D vector which can contain (int / string) , it cannot output by cout>> 
vector<vector<variant<int, string>>>customers;//2-D vector which can contain (int / string) , it cannot output by cout>>
vector<vector<string>>redeem_gifts_transaction;
vector<vector<string>>modify_transaction;
vector<vector<string>>add_cc_points_transaction;
vector<vector<string>>gift_redeemed;
vector<vector<string>>summary_report;
vector<vector<string>>point_change_record;

int counter = 0, counter_1 = 0, counter_2 = 0, counter_3 = 0, counter_4 = 0, counter_5 = 0;
int length_str = 0;
int length_int = 0;
int required_pt = 0;
int ori_pt = 0;
int final_pt = 0;
int before_change = 0;
bool data_loaded = false;
bool found_name = false;
bool found_name_1 = false;
bool content_unchange = false;
bool check = false;
bool record = false;
char option_of_menu;
char option_of_customer_menu;
string category;
string input_ID;
string input_ID_cus;

//decoration variables for dyanmic output
char welcome[9] = "Welcome";
char exclamation_mark[2] = "!";
char menu_system[18] = "*** Main Menu ***";
char option[6][50] = { "[1] Load Starting Data" , "[2] Show Records" , "[3] Edit Customers" , "[4] Enter Customer View" , "[5] Show Transaction History" , "[6] Credits and Exit" };
char line[20] = "*****************";
char warn[100] = "Data did not loaded, please return to main menu and choose option 1.";
char back[5] = "back";
char return_1[50] = "Returning to the main menu";
char return_2[10] = ".....";

class functions_list {
public:
	void color_text(int color, string output)//chage to text output color and change back to default color automatically after output
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
		cout << output;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	}
	void global_color_on(int color) {//change all output's color during the switch is on
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	}
	void global_color_off() {//switch off color change
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	}
	void dynamic_output(char len[], int sec, bool endl_or_not) {// dynaimc_output for char array
		for (counter_1 = 0; counter_1 < strlen(len); counter_1++) {
			cout << len[counter_1];
			sleep_for(milliseconds(sec));//disrupt the output stream 
		}
		if (endl_or_not) {
			cout << endl;
		}
	}
	void menu_1() {											//main menu
		cout << "Welcome";
		if (data_loaded) {
			cout << " back";
		}
		cout << "!" << endl;
		cout << "*** Main Menu ***" << endl;
		for (counter = 0; counter < 6; counter++) {
			cout << option[counter] << endl;
		}
		cout << "*****************" << endl;
		cout << "Option (1 - 6) : ";
	}
	void menu_cus() {									//The cutomer menu
		char cus_op[4][50] = { "[1] Earn CC Points" , "[2] Redeem Gifts" , "[3] Modify CC Points Balance" , "[4] Return to Main Menu" };
		cout << "*****Customer View Menu *****" << endl;
		for (counter = 0; counter < 4; counter++) {
			cout << cus_op[counter] << endl;
		}
		cout << "Option (1 - 4) : ";
	}
	void int_ban_string_input(int& check_string) {//transmit the value which needed to do the detection into check_string(integer ver.)
		while (!(cin >> check_string))/*when the value in input stream doesn't conform the declare type of variable*/ {
			color_text(12, "Invalid input. Please input again: ");
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max()/*set the ignore limits to maximum*/, '\n'/*or user press the "Enter" key , because "Enter" also represent switch to next line*/);
		}
	}
	void double_ban_string_input(double& check_string) {//transmit the value which needed to do the detection into check_string(double ver.)
		while (!(cin >> check_string))/*when the value in input stream doesn't conform the declare type of variable*/ {
			color_text(12, "Invalid input. Please input again: ");
			cin.clear();//to ensure the cin.ignore can be done
			cin.ignore(numeric_limits<streamsize>::max()/*set the ignore limits to maximum*/, '\n'/*or user press the "Enter" key , because "Enter" also represent switch to next line*/);
		}
	}
	void int_ban_negative_num(int& check_negative) {//logically, some variables are not able to be negative value
		while (check_negative < 0) {
			color_text(12, "Please do not enter negative number, input again: ");
			int_ban_string_input(check_negative);
		}
	}
	void double_ban_negative_num(double& check_negative) {//logically, some variables are not able to be negative value
		while (check_negative < 0) {
			color_text(12, "Please do not enter negative number, input again: ");
			double_ban_string_input(check_negative);
		}
	}
};

struct VariantPrinter {//output vector content for the function: visit() 
	void operator()(const int& num) const {
		cout << num;
		length_int = to_string(num).length();
	}

	void operator()(const string& content) const {
		cout << content;
		length_str = content.length();
	}
};

struct find_target {//find vector content for the function: visit() 
	void operator()(const int& num) const {

	}
	void operator()(const string& content) const {
		if (content == input_ID) {
			found_name = true;
		}
	}
};

struct find_gift_category {//find specific column of vector content for the function: visit() 
	void operator()(const int& num) const {

	}
	void operator()(const string& content) const {
		size_t gift_type = content.find(category);
		if (gift_type == 0) {
			found_name_1 = true;
		}
	}
};

struct actual_g {//output the gift's required point of gold member
	void operator()(const int& num) const {
		cout << round(num * 0.9);
		length_int = to_string(round(num * 0.9)).length();
	}

	void operator()(const string& content) const {
	}
};

struct actual_s {// output the gift's required point of silver member
	void operator()(const int& num) const {
		cout << round(num * 0.95);
		length_int = to_string(round(num * 0.95)).length();
	}

	void operator()(const string& content) const {
	}
};

struct find_target_cus {//find vector content for the function: visit() 
	void operator()(const int& num) const {

	}
	void operator()(const string& content) const {
		if (content == input_ID_cus) {
			found_name_1 = true;
		}
	}
};

void select();//build prototype

void Load_Starting_Data() {
	functions_list fun;
	if (!content_unchange) {
		data_loaded = true;//record the data has been loaded
		//insert the original value into the 2-D variant vector
		gifts.push_back({ variant<int, string>("A01"), variant<int, string>("LG Internet TV"), variant<int , string>(3900), variant<int, string>(19000) });
		gifts.push_back({ variant<int, string>("A02"), variant<int, string>("Pioneer Hifi Set"), variant<int , string>(2400), variant<int, string>(11500) });
		gifts.push_back({ variant<int, string>("A03"), variant<int, string>("Sony DVD Player"), variant<int , string>(400), variant<int, string>(2000) });
		gifts.push_back({ variant<int, string>("B01"), variant<int, string>("Healthy Air Fryer"), variant<int , string>(1500), variant<int, string>(7300) });
		gifts.push_back({ variant<int, string>("B02"), variant<int, string>("Tefal Microwave Oven"), variant<int , string>(480), variant<int, string>(2400) });
		gifts.push_back({ variant<int, string>("B03"), variant<int, string>("Famous Coffee Maker"), variant<int , string>(1800), variant<int, string>(8800) });
		gifts.push_back({ variant<int, string>("B04"), variant<int, string>("Smart Rice Cooker"), variant<int , string>(600), variant<int, string>(2900) });
		gifts.push_back({ variant<int, string>("B05"), variant<int, string>("TechCook Toaster Oven"), variant<int , string>(450), variant<int, string>(2250) });
		gifts.push_back({ variant<int, string>("C01"), variant<int, string>("Wellcome $50 Coupon"), variant<int , string>(50), variant<int, string>(250) });
		gifts.push_back({ variant<int, string>("C02"), variant<int, string>("Mannings $50 Coupon"), variant<int , string>(50), variant<int, string>(250) });
		gifts.push_back({ variant<int, string>("C03"), variant<int, string>("Carol Restaurant $100 Coupon"), variant<int , string>(100), variant<int, string>(500) });
		gifts.push_back({ variant<int, string>("C04"), variant<int, string>("Shell $200 Coupon"), variant<int , string>(100), variant<int, string>(960) });
		gifts.push_back({ variant<int, string>("D01"), variant<int, string>("Clever Headset"), variant<int , string>(350), variant<int, string>(1750) });
		gifts.push_back({ variant<int, string>("D02"), variant<int, string>("HP Optic Mouse"), variant<int , string>(250), variant<int, string>(1250) });
		gifts.push_back({ variant<int, string>("D03"), variant<int, string>("Stylish Bluetooth Speaker"), variant<int , string>(800), variant<int, string>(3900) });
		customers.push_back({ variant<int, string>("Tommy2015"), variant<int, string>("B"), variant<int, string>(8500) });
		customers.push_back({ variant<int, string>("DavidChan"), variant<int, string>("B"), variant<int, string>(22800) });
		customers.push_back({ variant<int, string>("Luna123"), variant<int, string>("B"), variant<int, string>(650) });
		customers.push_back({ variant<int, string>("TigerMan"), variant<int, string>("B"), variant<int, string>(14000) });
		customers.push_back({ variant<int, string>("Max5678"), variant<int, string>("S"), variant<int, string>(2580) });
		customers.push_back({ variant<int, string>("Neo2000"), variant<int, string>("S"), variant<int, string>(8000) });
		customers.push_back({ variant<int, string>("CCTang"), variant<int, string>("S"), variant<int, string>(33554) });
		customers.push_back({ variant<int, string>("EchoWong"), variant<int, string>("G"), variant<int, string>(8650) });
		customers.push_back({ variant<int, string>("Rubychow"), variant<int, string>("G"), variant<int, string>(28000) });
		customers.push_back({ variant<int, string>("Ivy2023"), variant<int, string>("G"), variant<int, string>(12340) });
		//record the data has been loaded and no need to load again
		content_unchange = true;
	}
	else {
		fun.color_text(12, "Warning , the data is re-read is not allowed.");//it will cause data redundancy
		cout << endl;
	}
	fun.menu_1();//call the main menu
	select();
}

void check_data_loaded() {//only after data loaded can do the option 2-5
	functions_list fun;
	if (!data_loaded) {
		fun.color_text(12, "Warning!");
		cout << endl;
		fun.global_color_on(12);
		fun.dynamic_output(warn, 30, true);//output the warning info with 0.3 sec gap and auto endl
		fun.global_color_off();
		cout << endl;
		fun.menu_1();
		select();
	}
}
void Show_Records() {
	functions_list fun;
	check_data_loaded();
	VariantPrinter visitor;
	fun.global_color_on(240);
	const int column = 0;
	sort(gifts.begin(), gifts.end(), [&column](const vector<variant<int, string>>& item1, const vector<variant<int, string>>& item2) { //since it is variant type , it need build two variant vector to store the value which need to be compared.
		return item1[column] < item2[column];
		});
	//auto sort by gift ID and sorted in alphabetical order
	cout << "---------------------------------------------------------------------------------\n";
	cout << " Gift ID  |     Gift Description             | Price (HKD)    | Points Required |\n";
	cout << "---------------------------------------------------------------------------------\n";
	counter = 0;
	counter_1 = 0;
	// visitor
	for (const auto& row : gifts) {
		for (const auto& elem : row) {
			switch (counter) {
			case 0:
				cout << "  ";
				break;
			case 1:
				cout << "     ";
				break;
			case 2:
				cout << "  ";
				break;
			case 3:
				cout << "  ";
				break;
			}
			visit(visitor, elem);//cout element 
			switch (counter) {
			case 0:
				cout << "     |";
				break;
			case 1:
				cout << setw(30 - length_str) << "|";
				break;
			case 2:
				cout << setw(15 - length_int) << "|";
				break;
			case 3:
				cout << setw(16 - length_int) << "|";
				break;
			}
			counter++;
		}
		cout << endl;
		counter = 0;
		counter_1++;
	}
	cout << "---------------------------------------------------------------------------------\n";
	cout << "----------------------------------------------------------------\n";
	cout << "   Customer ID               |   Rank    |  Points Balance     | \n";
	cout << "----------------------------------------------------------------\n";
	const int column_1 = 0;
	//sort by customerID
	sort(customers.begin(), customers.end(), [&column_1](const vector<variant<int, string>>& item1, const vector<variant<int, string>>& item2) {
		return item1[column_1] < item2[column_1];
		});
	counter = 0;
	counter_1 = 0;
	for (const auto& row : customers) {
		for (const auto& elem : row) {
			switch (counter) {
			case 0:
				cout << "  ";
				break;
			case 1:
				cout << "    ";
				break;
			case 2:
				cout << "  ";
				break;
			}
			visit(visitor, elem);//cout customers content
			switch (counter) {
			case 0:
				cout << setw(28 - length_str) << "|";
				break;
			case 1:
				cout << setw(8 - length_str) << "|";
				break;
			case 2:
				cout << setw(20 - length_int) << "|";
				break;
			}
			counter++;
		}
		cout << endl;
		counter = 0;
		counter_1++;
	}
	cout << "----------------------------------------------------------------\n";
	fun.global_color_off();
	cout << endl;
	fun.menu_1();
	select();
}


void Edit_Customers() {
	functions_list fun;
	find_target visitor;
	VariantPrinter visitor_1;
	check_data_loaded();
	bool common = false;
	bool confirm = true;
	string rank;
	int cur_day;
	int cur_mon;
	int cur_yr;
	int point;
	cout << "Please enter a Customer ID: ";
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');//ignore the maxmium size of input stream
	getline(cin, input_ID);//ensure input_ID can be sign value since it is important for visitor to find target
	found_name = false;
	for (const auto& row : customers) {
		if (!row.empty()) {
			visit(visitor, row[0]);//visitor specific column for avoid rename
			if (found_name) {
				break;//stop comparing
			}
		}
	}
	if (!found_name) {//enter add customer view
		fun.color_text(14, "Add Customer");
		cout << endl;
		counter = 0;
		while (common == false) {
			if (counter >= 3) {
				fun.color_text(12, "Error input, return to main menu\n");
				fun.menu_1();
				select();
				break;
			}
			cout << "The date of customer became a member (in the format DD/MM/YYYY):";
			string date;
			cin >> date;
			cout << "The points balance value:";
			cin >> point;
			int day;
			int month;
			int year;
			sscanf_s(date.c_str(), "%d/%d/%d", &day, &month, &year); //get the date value and convert into int form so that can compare with actual time
			//using chrono and time.h lib.
			auto now = system_clock::now();
			time_t now_time = system_clock::to_time_t(now);
			tm* now_tm = localtime(&now_time);
			//assign the value from tm template into the int form
			cur_day = now_tm->tm_mday;
			cur_mon = now_tm->tm_mon + 1;
			cur_yr = now_tm->tm_year + 1900;
			if ((date.length() != 10) || (year > cur_yr || (year == cur_yr && month > cur_mon) || (year == cur_yr && month == cur_mon && day > cur_day)) || (year <= cur_yr - 100) || month > 12 || day > 31 || day < 0 || month < 0 || point < 0 || (((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) && month == 2 && day > 29) || (!((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) && month == 2 && day > 28))
			{
				if (date.length() != 10) {
					fun.color_text(12, "Invalid Input of date, please refer the format DD/MM/YYYY\n");
					counter++;
				}
				if (year > cur_yr || (year == cur_yr && month > cur_mon) || (year == cur_yr && month == cur_mon && day > cur_day)) {
					fun.color_text(12, "Invalid Input of date, please don't enter future date\n");
					if (counter - counter_1 != 1) {
						counter++;
					}
				}
				if (year < cur_yr - 100 || (year == cur_yr - 100 && month > cur_mon) || (year == cur_yr - 100 && month == cur_mon && day > cur_day) || (month > 12 || month <= 0) || day>31 || day <= 0) {
					fun.color_text(12, "Invalid Input of date, please don't enter a impossible date\n");
					if (counter - counter_1 != 1) {
						counter++;
					}
				}
				if (((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) && month == 2 && day > 29) {
					fun.color_text(12, "It is leap year, but the day of Feb beyond 29\n");
					if (counter - counter_1 != 1) {
						counter++;
					}
				}
				if (!((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) && month == 2 && day > 28) {
					fun.color_text(12, "It isn't leap year, but the day of Feb beyond 28\n");
					if (counter - counter_1 != 1) {
						counter++;
					}
				}
				if (point < 0) {
					fun.color_text(12, "Invalid Input of point value, please enter a correct point\n");
					counter++;
				}
				counter_1 = counter;
				continue;
			}
			common = true;
			// Add the new customer to the system
			if (cur_yr - year >= 1) {
				if (((cur_yr - year - 1) * 12 + (12 - month) + cur_mon) >= 12)
					rank = "G";
				else if ((((cur_yr - year - 1) * 12 + (12 - month) + cur_mon) >= 6))
					rank = "S";
				else
					rank = "B";
			}
			else if (cur_mon - month >= 6)
				rank = "S";
			else
				rank = "B";

		}
		if (rank == "G")
		{
			vector<variant<int, string>> newCustomer = { variant<int, string>(input_ID), variant<int, string>(rank), variant<int, string>(point) };
			customers.insert(customers.end(), newCustomer);

		}
		else if (rank == "B") {
			vector<variant<int, string>> newCustomer = { variant<int, string>(input_ID), variant<int, string>(rank), variant<int, string>(point) };
			customers.insert(customers.begin(), newCustomer);

		}
		else
		{
			vector<variant<int, string>> newCustomer = { variant<int, string>(input_ID), variant<int, string>(rank), variant<int, string>(point) };
			customers.insert(customers.begin() + 5, newCustomer);

		}
		fun.color_text(14, "The customer ");
		fun.color_text(14, input_ID);
		fun.color_text(14, " has been successfully added");
		cout << endl;
	}
	else {
		fun.color_text(8, "Delete Customer");
		cout << endl;
		string confirmation;
		cout << "Are you sure you want to delete the customer record? (Yes/No): ";
		cin >> confirmation;
		counter = 0;
		found_name = false;

		while (confirm == true) {									// this while loop is used for wrong input, make sure that if user input wrong put, they can try again
			if (confirmation == "Yes") {
				for (const auto& row : customers) {
					if (!row.empty()) {
						visit(visitor, row[0]);
						if (found_name) {
							cout << "Below record will be deleted : ";
							cout << endl;
							fun.global_color_on(14);
							visit(visitor_1, row[0]);
							cout << " | ";
							visit(visitor_1, row[1]);
							cout << " | ";
							visit(visitor_1, row[2]);
							cout << " | ";
							fun.global_color_off();
							cout << endl;
							break;
						}
						counter++;
					}
				}
				customers.erase(customers.begin() + counter);
				break;
			}
			else if (confirmation == "No") {
				cout << endl;
				cout << "The record will be remained." << endl;
				break;
			}
			else {
				cout << endl;
				cout << "No such option , please refer the input format carefully.\n" << endl;
				cout << "Are you sure you want to delete the customer record? (Yes/No): ";
				cin >> confirmation;
			}
		}
	}
	fun.menu_1();
	select();
}

void select2(); //build prototype

void Enter_Customer_View() {
	functions_list fun;
	find_target_cus visitor;
	check_data_loaded();
	cout << endl;
	cout << "Action for Customer ID : ";
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, input_ID_cus);
	found_name_1 = false;
	counter_2 = 0;
	for (const auto& row : customers) {
		if (!row.empty()) {
			visit(visitor, row[0]);
			if (found_name_1) {
				ori_pt = get<int>(customers[counter_2][2]);
				fun.menu_cus();
				select2();
				break;
			}
			counter_2++;
		}
	}
	fun.menu_1();
	select();
}

void cus_view1() {
	functions_list fun;
	find_target_cus visitor;
	VariantPrinter visitor_1;
	double money = 0;
	int CC_pt = 0;
	cout << "Money spent ? ";
	fun.double_ban_string_input(money);
	fun.double_ban_negative_num(money);
	CC_pt = floor(money / 250);
	if (CC_pt < 1) {
		fun.color_text(14, "The earned CC point is zero , returning to the customer menu ... ");
		fun.menu_cus();
		select2();
	}
	add_cc_points_transaction.push_back({ get<string>(customers[counter_2][0]) , to_string(CC_pt)  , to_string(money) });
	point_change_record.push_back({ get<string>(customers[counter_2][0])  , to_string(get<int>(customers[counter_2][2])) + " -> " + to_string(CC_pt + get<int>(customers[counter_2][2])) });
	cout << "The CC points earned : ";
	fun.color_text(14, to_string(CC_pt));
	cout << endl;
	found_name = false;
	counter = 0;
	for (const auto& row : customers) {
		if (!row.empty()) {
			visit(visitor, row[0]);
			if (found_name) {
				cout << "The customer ";
				fun.global_color_on(14);
				visit(visitor_1, row[0]);
				fun.global_color_off();
				cout << " has been added ";
				fun.color_text(14, to_string(CC_pt));
				cout << " CC point." << endl;
				fun.global_color_on(14);
				visit(visitor_1, row[2]);
				cout << " -> ";
				get<int>(customers[counter][2]) += CC_pt;
				visit(visitor_1, row[2]);
				fun.global_color_off();
				cout << endl;
				break;
			}
			counter++;
		}
	}
	record = true;
	fun.menu_cus();
	select2();
}

void cus_view2() {
	functions_list fun;
	find_target visitor;
	VariantPrinter visitor_1;
	find_gift_category visitor_2;
	actual_g visitor_3;
	actual_s visitor_4;
	int CC_point;
	cout << "Please enter Gift Category(A-D) : ";
	cin >> category;
	fun.global_color_on(14);
	cout << "---------------------------------------------------------------------------------------------------------------------\n";
	cout << " Gift ID  |     Gift Description             | Price (HKD)    | Actual Points Required | Enough CC points to redeem |\n";
	cout << "---------------------------------------------------------------------------------------------------------------------\n";
	const int column = 3;
	sort(gifts.begin(), gifts.end(), [&column](const vector<variant<int, string>>& item1, const vector<variant<int, string>>& item2) {
		return item1[column] < item2[column];
		});
	found_name_1 = false;
	counter = 0;
	counter_1 = 0;
	float percentage_off = 1;
	for (const auto& row : gifts) {
		if (!row.empty()) {
			found_name_1 = false;
			visit(visitor_2, row[0]);
			if (found_name_1) {
				cout << "  ";
				visit(visitor_1, row[0]);
				cout << "     |";
				cout << "     ";
				visit(visitor_1, row[1]);
				cout << setw(30 - length_str) << "|";
				cout << "  ";
				visit(visitor_1, row[2]);
				cout << setw(15 - length_int) << "|";
				cout << "  ";
				if (get<string>(customers[counter_2][1]) == "G") {
					visit(visitor_3, row[3]);
					percentage_off = 0.9;
				}
				else if (get<string>(customers[counter_2][1]) == "S") {
					visit(visitor_4, row[3]);
					percentage_off = 0.95;
				}
				else {
					visit(visitor_1, row[3]);
				}
				cout << setw(30 - length_int) << "|";
				cout << " ";
				if (customers[counter_2][2] >= gifts[counter][3]) {
					cout << "Yes";
				}
				else {
					fun.color_text(12, "No");
					fun.global_color_on(14);
					cout << " ";
				}
				cout << "                        |";
				cout << endl;
			}
			counter++;
		}
	}
	cout << "---------------------------------------------------------------------------------------------------------------------\n";
	fun.global_color_off();
	counter = 0;
	counter_1 = 0;
	bool common = false;
	while (common == false) {
		if (counter >= 4) {
			fun.color_text(12, "Error input, return to Customer_view Menu\n");
			fun.menu_cus();
			select2();
			break;
		}
		cout << "Please enter a Gift ID that you want to redeem: ";
		cin >> input_ID;
		found_name = false;
		counter_3 = 0;
		for (const auto& row : gifts) {
			if (!row.empty()) {
				visit(visitor, row[0]);
				if (found_name) {
					break;
				}
				counter_3++;
			}
		}
		cout << "Please enter the amount of CC points used for redemption: ";
		if (!(cin >> CC_point)) {
			fun.color_text(12, "Please do not enter text in CC point that you are willing to pay\n");
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			counter++;
		}
		else if (!found_name) {
			fun.color_text(12, "Wrong Gift ID, Please enter again\n");
			counter++;
		}
		else if (input_ID.find(category) != 0) {
			fun.color_text(12, "Wrong Gift category, Please enter again\n");
			counter++;
		}
		else if (CC_point < 0 || CC_point > get<int>(customers[counter_2][2])) {
			fun.color_text(12, "Out-of-range CC points for the redemption\n");
			counter++;
		}
		else if (CC_point > get<int>(gifts[counter_3][3])) {
			fun.color_text(12, "The CC points for the redemption is higher than the gift required\n");
			counter++;
		}
		else if (counter != counter_1) {
			counter_1 = counter;
			continue;
		}
		else								//after checking, then common turn to true, to avoid logic error
			common = true;
	}
	float extra_m;
	int required_pt = 0;
	if (common) {
		required_pt = get<int>(gifts[counter_3][3]) * percentage_off;
		if (CC_point != required_pt) {
			extra_m = (static_cast<float>(required_pt) - static_cast<float>(CC_point)) * 0.2;
			cout << "The extra money needed : $" << extra_m << endl;
			redeem_gifts_transaction.push_back({ get<string>(customers[counter_2][0]) , input_ID , to_string(CC_point) , to_string(extra_m) });
		}
		else {
			cout << "There is no extra money needed." << endl;
			redeem_gifts_transaction.push_back({ get<string>(customers[counter_2][0]) , input_ID , to_string(CC_point) , "0" });
		}
		point_change_record.push_back({ get<string>(customers[counter_2][0])  , to_string(get<int>(customers[counter_2][2])) + " ->" + to_string(get<int>(customers[counter_2][2]) - CC_point) });
		string input_1;
		fun.color_text(14, "Confirm (Yes / No) ?");
		cin >> input_1;
		if (input_1 == "Yes") {
			fun.global_color_on(14);
			cout << get<int>(customers[counter_2][2]);
			cout << " -> ";
			customers[counter_2][2] = get<int>(customers[counter_2][2]) - CC_point;
			cout << get<int>(customers[counter_2][2]);
			fun.global_color_off();
			cout << endl;
			fun.color_text(14, "Finished");
			cout << endl;
		}
		else if (input_1 == "No") {
			fun.color_text(14, "Cancelled");
			cout << endl;
		}
		else {
			fun.color_text(12, "Error input! returning to customer menu ...");
			cout << endl;
		}
	}
	record = true;
	fun.menu_cus();
	select2();
}

void cus_view3() {
	functions_list fun;
	find_target visitor;
	VariantPrinter visitor_1;
	int CC_pt = -1;
	cout << "Current CC point:";
	cout << get<int>(customers[counter_2][2]);
	before_change = get<int>(customers[counter_2][2]);
	while (CC_pt < 0) {
		cout << endl << "Enter a new CC point:";
		cin >> CC_pt;
		cout << "New CC point:";
		fun.color_text(14, to_string(CC_pt));
		cout << endl;
		cout << "The customer ";
		fun.global_color_on(14);
		cout << get<string>(customers[counter_2][0]);
		fun.global_color_off();
		cout << " CC point has been changed from ";
		fun.global_color_on(14);
		cout << before_change;
		fun.global_color_off();
		cout << " to " << endl;
		fun.global_color_on(14);
		fun.color_text(14, to_string(CC_pt));
		get<int>(customers[counter_2][2]) = CC_pt;
	}
	if (CC_pt > before_change) {
		modify_transaction.push_back({ input_ID_cus , "Increase" ,to_string(before_change) + " -> " + to_string(CC_pt) });
	}
	else {
		modify_transaction.push_back({ input_ID_cus , "Decrease",to_string(before_change), to_string(CC_pt) });
	}
	point_change_record.push_back({ get<string>(customers[counter_2][0])  , to_string(before_change) + " ->" + to_string(get<int>(customers[counter_2][2])) });
	fun.global_color_off();
	cout << endl;
	record = true;
	fun.menu_cus();
	select2();
}

void cus_view4() {
	functions_list fun;
	fun.global_color_on(240);
	fun.dynamic_output(return_1, 25, false);
	fun.global_color_off();
	cout << " ";
	fun.global_color_on(14);
	fun.dynamic_output(return_2, 300, true);
	fun.global_color_off();
	fun.menu_1();
	select();
}

void Show_Transaction_History() {
	functions_list fun;
	find_target visitor;
	int length_for_output = 0;
	check_data_loaded();
	counter = 0;
	if (record) {
		cout << "Which customer's transaction history you want to find : ";
		cin >> input_ID;
		found_name = false;
		counter_4 = 0;
		for (const auto& row : customers) {
			if (!row.empty()) {
				visit(visitor, row[0]);
				if (found_name) {
					break;
				}
				counter_4++;
			}
		}
		cout << "Add CC points transaction report (order : ID -> Change in Balamce -> Money Spent) :";
		cout << endl;
		found_name = false;
		for (const auto& row : add_cc_points_transaction) {
			if (!row.empty()) {
				found_name = (row[0] == input_ID) ? true : false;
				if (found_name) {
					for (const auto& elem : row) {
						cout << elem;
						length_for_output = 20 - elem.length();
						cout << setw(length_for_output);
						cout << " | ";
					}
					cout << endl;
				}
				found_name = false;
			}
		}
		float amount = 0;
		cout << "Redeem gifts transaction report (order : ID -> Gift ID -> Change in Balance -> Extra Money Spent):";
		cout << endl;
		found_name = false;
		for (const auto& row : redeem_gifts_transaction) {
			if (!row.empty()) {
				found_name = (row[0] == input_ID) ? true : false;
				if (found_name) {
					for (const auto& elem : row) {
						cout << elem;
						length_for_output = 20 - elem.length();
						cout << setw(length_for_output);
						cout << " | ";
						if (elem == row[3]) {
							amount = amount + stof(elem);
						}
					}
					cout << endl;
				}
				found_name = false;
			}
		}
		cout << "Modify transaction report (order : ID -> type -> Change in Balance) :";
		cout << endl;
		found_name = false;
		for (const auto& row : modify_transaction) {
			if (!row.empty()) {
				found_name = (row[0] == input_ID) ? true : false;
				if (found_name) {
					for (const auto& elem : row) {
						cout << elem;
						length_for_output = 20 - elem.length();
						cout << setw(length_for_output);
						cout << " | ";
					}
					cout << endl;
				}
				found_name = false;
			}
		}
		cout << "Summary of transaction report (order : ID -> Original Balance -> Final Balance -> Total Amount of Extra money -> Change record) :";
		cout << endl;
		found_name = false;
		for (const auto& row : summary_report) {
			if (!row.empty()) {
				found_name = (row[0] == input_ID) ? true : false;
				if (found_name) {
					for (const auto& elem : row) {
						cout << elem;
						length_for_output = 20 - elem.length();
						cout << setw(length_for_output);
						cout << " | ";
					}
					break;
				}
			}
		}
		cout << get<int>(customers[counter_4][2]);
		length_for_output = 20 - to_string(get<int>(customers[counter_2][2])).length();
		cout << setw(length_for_output);
		cout << " | ";
		cout << amount;
		length_for_output = 20 - to_string(amount).length();
		cout << setw(length_for_output);
		cout << " | ";
		found_name = false;
		counter = 0;
		for (const auto& row : point_change_record) {
			if (!row.empty()) {
				found_name = (row[0] == input_ID) ? true : false;
				if (found_name) {
					for (const auto& elem : row) {
						if (counter == 0 && elem != row[0]) {
							cout << elem;
							length_for_output = 20 - elem.length();
							cout << setw(length_for_output);
							cout << " | ";
							counter++;
						}
						else if (elem != row[0]) {
							length_for_output = 73;
							cout << setw(length_for_output);
							cout << " | ";
							cout << elem;
							length_for_output = 20 - elem.length();
							cout << setw(length_for_output);
							cout << " | ";
							counter++;
						}
					}
					cout << endl;
					found_name = false;
				}
			}
		}
		found_name = false;
		string choose = "|";
		while (choose != "Y") {
			fun.color_text(14, "Exit ? (Y/N)");
			cin >> choose;
			if (choose == "Y") {
				break;
			}
			else if (choose == "N") {
				cout << "The systime will be asked again in 10 sec ...";
				sleep_for(milliseconds(10000));
			}
		}
	}
	else {
		fun.color_text(14, "None transaction has done");
		cout << endl;
	}
	fun.menu_1();
	select();
}

void Credits_And_Exit() {
	functions_list fun;
	string confirmation;
	string second_con;
	check_data_loaded();
	bool common2 = false;
	bool goBackToMainMenu = false;
	while (true) {
		cout << "User Confirmation to see the Credits and Exit (Y/y or N/n): ";				//first confirm
		cin >> confirmation;

		if (confirmation == "n" || confirmation == "N") {
			common2 = false;
			cout << "Are you sure you want to go back to the main menu (N/n or Y/y): ";			//second confirm
			cin >> second_con;

			while (true) {
				if (second_con == "Y" || second_con == "y") {
					common2 = true;
					goBackToMainMenu = true;
					break;
				}
				else if (second_con == "N" || second_con == "n") {
					Credits_And_Exit();
					break;
				}
				else {
					fun.color_text(12, "Please enter a valid input.\n");
					cout << "Are you sure you want to go back to the main menu (N/n or Y/y): ";
					cin >> second_con;
				}
			}
		}
		else if (confirmation == "Y" || confirmation == "y") {
			common2 = false;

			while (true) {
				cout << "Are you sure you want to see the Credits and Exit (N/n or Y/y): ";		//second confirm
				cin >> second_con;

				if (second_con == "Y" || second_con == "y") {
					fun.color_text(14, "Credits list:\n");
					fun.color_text(14, "Name were erased due to the privacy\n");
					return;			//use for exit
					break;
				}
				else if (second_con == "N" || second_con == "n") {
					Credits_And_Exit();
					break;
				}
				else {
					fun.color_text(12, "Please enter a valid input.\n");
				}
			}

			break;
		}
		else {
			fun.color_text(12, "Please enter a valid input.\n");
		}
		if (common2) {
			break;
		}
	}

	if (goBackToMainMenu) {
		fun.menu_1();
		select();
	}
}


void select() {						//let user to choose the function in main menu
	functions_list fun;
	if (record) {
		summary_report.push_back({ get<string>(customers[counter_2][0]) , to_string(ori_pt) });
	}
	option_of_menu = '|';
	while (option_of_menu != '6') {
		fun.global_color_on(13);
		cin >> option_of_menu;
		fun.global_color_off();
		switch (option_of_menu) {
		case '1':
			Load_Starting_Data();
			break;
		case '2':
			Show_Records();
			break;
		case '3':
			Edit_Customers();
			break;
		case '4':
			Enter_Customer_View();
			break;
		case '5':
			Show_Transaction_History();
			break;
		case '6':
			Credits_And_Exit();
			break;
		default:
			cout << "No such option" << endl;
			cout << "Please enter again" << endl;
			fun.menu_1();
			break;
		}
	}
}

void select2() {					//let user to choose the function in customer menu
	functions_list fun;
	option_of_customer_menu = '|';
	while (option_of_customer_menu != '4') {
		fun.global_color_on(13);
		cin >> option_of_customer_menu;
		fun.global_color_off();
		switch (option_of_customer_menu) {
		case '1':
			cus_view1();
			break;
		case '2':
			cus_view2();
			break;
		case '3':
			cus_view3();
			break;
		case '4':
			cus_view4();
			break;
		default:
			cout << "No such option" << endl;
			cout << "Please enter again" << endl;
			fun.menu_cus();
			break;
		}
	}
}

int main() {
	functions_list fun;
	fun.menu_1();
	select();
	return 0;
}

