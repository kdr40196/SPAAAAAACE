#pragma once
#include<iostream>
#include<vector>
#include<memory>
#include"text.hpp"
#include"button.hpp"
using namespace std;


class Menu {
	vector<TextButton> items;
	Text title;
	int noOfItems, currentIndex;
	vector<Text> info;
	public:
		Menu();
		void addMenuTitle(string text);
		void addMenuItem(string text);
		void addMenuInfo(string text);
		void refresh();
		void exit();
		string handleInput(SDL_Event& e);
		void display();
		string getTitle();
};
