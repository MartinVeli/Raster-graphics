#pragma once
#include "Session.h"
#include "Vector.hpp"

constexpr size_t MaxFileNameSize = 64;

class ConsoleEditor
{
	Vector<Session> sessions;
	size_t current = 0;
	size_t id = 0;

	void load();
	void close();
	void save();
	void saveAs();
	void help();
	void switchSession();
	void collage();
	void undo();
	void sessionInfo();
	void rotate();
	void negative();
	void monochrome();
	void grayscale();
	void add();
	void exit();

	void doYouWantToSave();
public:
	void userCommands();
};

