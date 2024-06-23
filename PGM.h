#pragma once
#include"File.h"

class PGM : public File
{
protected:
	Vector<unsigned char> pixels;
	Vector<unsigned char>* snapshot = nullptr;

	unsigned char maxValue;

public:
	PGM() = default;
	PGM(const char* fileName);
	PGM(const PGM& other, const char* name);
	File* clone() const override;

	void monochrome() override;
	void rotate(char direction) override;
	void negative() override;
	void undo()override;
	void save()override;
	void saveAs(const char* file)override;
	const MyString& collage(const char* file1, const char* file2, char direction, const char* newFile)const override;
};

