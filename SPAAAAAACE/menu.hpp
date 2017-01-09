#pragma once
#include<iostream>
#include<vector>
#include<memory>
#include"text.hpp"
using namespace std;


class Menu {
	vector<shared_ptr<Text>> items;
	Text title;
	int noOfItems, currentIndex;
	public:
		Menu();
		void addMenuTitle(string text);
		void addMenuItem(string text);
		void refresh();
		void exit();
		string handleInput(SDL_Event& e);
		void display();
		string getTitle();
};
