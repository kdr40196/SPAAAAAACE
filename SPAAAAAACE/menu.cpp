#include "menu.hpp"
#include "g.hpp"

Menu::Menu() {
	noOfItems = 0;
	currentIndex = -1;
	gMenuItemPointer.setText(">");
}

void Menu::addMenuTitle(string text) {
	title.setText(text);
	title.setFont(gLargeFont);
	title.setPosition((gScreenWidth - title.getTextWidth()) / 2, gScreenHeight / 4);
}

void Menu::addMenuItem(string text) {
	TextButton item(0, 0, make_shared<Text>(Text(text, gSmallFont)));
	if (noOfItems == 0) {
		item.setPosition(gScreenWidth / 4, gScreenHeight / 2);
		currentIndex = 0;
	}
	else {
		item.setPosition(gScreenWidth / 4, items[noOfItems - 1].getY() + items[noOfItems - 1].getHeight());
	}
	items.push_back(item);
	noOfItems++;
	refresh();
}

void Menu::addMenuInfo(string text) {
	info.push_back(Text(text, gSmallFont));
	int maxInfoWidth = 0;
	for (int i = 0; i < info.size(); i++) {
		if (info[i].getTextWidth() > maxInfoWidth)
			maxInfoWidth = info[i].getTextWidth();
	}
	maxInfoWidth += 0.05 * gScreenWidth;
	int yOffset = info.size() * info[0].getTextHeight() + 0.05 * gScreenHeight;
	for (int i = 0; i < info.size(); i++) {
		info[i].setPosition(gScreenWidth - maxInfoWidth, gScreenHeight - yOffset);
		yOffset -= info[i].getTextHeight();
	}
}

void Menu::refresh() {
	for (int i = 0; i < noOfItems; i++) {
		items[i].setPosition(items[i].getX(), items[i].getY() - items[i].getHeight() / 2);
	}
}

void Menu::exit() {
	currentIndex = 0;
}

string Menu::handleInput(SDL_Event& e) {
	string action = "";
	if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym) {
		case SDLK_w:
		case SDLK_UP:
			if (currentIndex <= 0)
				currentIndex = noOfItems - 1;
			else currentIndex--;
			gMenuItemPointer.setPosition(items[currentIndex].getX() - gMenuItemPointer.getTextWidth(), items[currentIndex].getY());
			break;

		case SDLK_s:
		case SDLK_DOWN:
			if (currentIndex >= noOfItems - 1)
				currentIndex = 0;
			else currentIndex++;
			gMenuItemPointer.setPosition(items[currentIndex].getX() - gMenuItemPointer.getTextWidth(), items[currentIndex].getY());
			break;

		case SDLK_RETURN:
			action = items[currentIndex].getButtonText();
			currentIndex = 0;

		}
	}
	else if (e.type == SDL_MOUSEMOTION) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		
		x /= (float(gWindow.getWidth()) / gScreenWidth);
		y /= (float(gWindow.getHeight()) / gScreenHeight);
		
		for (int i = 0; i < noOfItems; i++) {
			if (x >= items[i].getX() && x <= items[i].getX() + items[i].getWidth()
				&& y >= items[i].getY()	&& y <= items[i].getY() + items[i].getHeight()) {
				currentIndex = i;
				break;
			}
		}
		
	}
	else if (e.type == SDL_MOUSEBUTTONDOWN) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		x /= (float(gWindow.getWidth()) / gScreenWidth);
		y /= (float(gWindow.getHeight()) / gScreenHeight);
		for (int i = 0; i < noOfItems; i++) {
			if (x >= items[i].getX() && x <= items[i].getX() + items[i].getWidth()
				&& y >= items[i].getY() && y <= items[i].getY() + items[i].getHeight()) {
				currentIndex = i;
				action = items[currentIndex].getButtonText();
				currentIndex = 0;
				break;
			}
		}
	}
	return action;
}

void Menu::display() {
	title.render();
	for (int i = 0; i < noOfItems; i++) {
		items[i].render();
	}
	for (int i = 0; i < info.size(); i++) {
		info[i].render();
	}
	gMenuItemPointer.setPosition(items[currentIndex].getX() - gMenuItemPointer.getTextWidth(), items[currentIndex].getY());
	gMenuItemPointer.render();
}

string Menu::getTitle() {
	return title.getText();
}
