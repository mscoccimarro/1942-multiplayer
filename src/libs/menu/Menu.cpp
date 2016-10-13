#ifndef MENU_H
#include "Menu.h"
#endif
#include <iostream>

using namespace std;

Menu::Menu(string title) {
  this->title = title;
  this->optionNumber = 0;
}

void Menu::addOption(string optionName, function<void()> action) {
  this->optionNumber++;
  this->options.insert(pair<int, string>(this->optionNumber, optionName)); 
  this->actions.insert(pair<int, function<void()>>(this->optionNumber, action));
}

void Menu::addOption(string optionName, function<void(string)> action, string argument) {
  this->optionNumber++;
  this->options.insert(pair<int, string>(this->optionNumber, optionName)); 
  map<string, function<void(string)>> f;
  f.insert(make_pair(argument, action));
  this->iactions.insert(pair<int, map<string, function<void(string)>> >(this->optionNumber, f));
}

void Menu::removeOption(string optionName) {}

bool Menu::validInput(string input) {
  for(int i = 0; i < input.length(); i++) {
    if(!isdigit(input[i])) return false;
  }

  return true;
}

int Menu::requireOption() {
  string input = "";
  map<int, string>::iterator it = this->options.end(); 
  string::size_type sz;

  while(it == this->options.end()) {
    cout << endl << "Ingrese opcion: ";
    cin >> input;

    if(!this->validInput(input)) {
      cout << endl << "Debe ingresar el numero de opcion" << endl;
      continue;
    }

    it = this->options.find(stoi(input, &sz));
    
    if(it == this->options.end()) {
      cout << endl << "El numero de opcion ingresado es invalido" << endl;
    }
  }

  return stoi(input, &sz);
}

void Menu::triggerOption(int optionNumber) {
  // get corresponding option number action
  map<int, function<void()>>::iterator it = this->actions.find(optionNumber);
 
  // if option not found on actions, search iactions
  if(it == this->actions.end()) {
    map<int, map<string, function<void(string)>> >::iterator it = this->iactions.find(optionNumber);
    map<string, function<void(string)>>::iterator it2 = (it->second).begin();
    string arg = it2->first;
    it2->second(arg);
  } else {
    // trigger action 
    it->second();
  }
  
  this->display();
}

void Menu::display() {
  cout << endl << "----\t" << this->title << "\t----" << endl << endl;
  for(map<int, string>::iterator it = this->options.begin(); it!=this->options.end(); it++) {
    cout << it->first << ". " << it->second << endl;
  }

  int input = this->requireOption();

  this->triggerOption(input);
}
