#pragma once
#include "File.h"

class PBM : public File
{
protected:
	Vector<bool> pixels;
	Vector<bool>* snapshot = nullptr;

public:
	PBM() = default;
	PBM(const char* fileName);
	PBM(const PBM& other, const char* file);
	File* clone() const override;

	void rotate(char direction) override;
	void negative() override;
	void undo()override;
	void save()override;
	void saveAs(const char* file)override;
	const MyString& collage(const char* first, const char* second, char direction, const char* newFile)const override;
};

