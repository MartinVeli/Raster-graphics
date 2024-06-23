#pragma once
#include "Vector.hpp"
#include "MyString.h"
#include <fstream>
class File
{
protected:
	size_t with = 0;
	size_t height = 0;
	MyString fileName;
	int type = 0;
public:
	File() = default;

	virtual void grayscale();
	virtual void monochrome();
	virtual void rotate(char direction) = 0; 
	virtual void negative() = 0;
	virtual void undo() = 0;
	virtual void save() = 0;
	virtual void saveAs(const char* file) = 0;
	virtual const MyString& collage(const char* file1, const char* file2, char direction, const char* newFile)const = 0 ;

	virtual File* clone() const = 0;
	virtual ~File() = default;

	const MyString& getFileName()const;
	int getType()const;
	size_t getWith()const;
	size_t getHeight()const;

};

