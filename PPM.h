#pragma once
#include"File.h"
class PPM : public File
{
	struct Pixel
	{
		Vector<unsigned char> rgb;
	};

protected:
	Vector<Pixel> pixels;
	Vector<Pixel>* snapshot = nullptr;
	unsigned char maxValue = 0;

public:
	PPM() = default;
	PPM(const char* fileName);
	PPM(const PPM& other, const char* file);
	File* clone() const override;

	void grayscale() override;
	void monochrome() override;
	void rotate(char direction) override;
	void negative() override;
	void undo()override;
	void save()override;
	void saveAs(const char* file)override;
	const MyString& collage(const char* first, const char* second, char direction, const char* newFile)const override;
};

