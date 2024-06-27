#include "PPM.h"

PPM::PPM(const char* fileName)
{
    if (!fileName)
        throw std::invalid_argument("Name of the file is nullptr");
    size_t len = std::strlen(fileName);
    if (len > 4) {
        if (fileName[len] == 'm' && fileName[len - 1] == 'p' && fileName[len - 2] == 'p' && fileName[len - 3] == '.') {
            std::ifstream file(fileName, std::ios::binary);

        if (!file.is_open())
            throw std::runtime_error("Can`t open the file");
        file.read(reinterpret_cast<char*>(&with), sizeof(size_t));
        file.read(reinterpret_cast<char*>(&height), sizeof(size_t));
        file.read(reinterpret_cast<char*>(&maxValue), sizeof(unsigned char));
        
        size_t end = File::with * File::height;
        for (size_t i = 0; i < end; i++)
        {
            file.read(reinterpret_cast<char*>(&pixels[i].rgb[0]), sizeof(unsigned char));
            file.read(reinterpret_cast<char*>(&pixels[i].rgb[1]), sizeof(unsigned char));
            file.read(reinterpret_cast<char*>(&pixels[i].rgb[2]), sizeof(unsigned char));
        }
   
        File::fileName = fileName;
        File::type = 3;
        file.close();
        }
            else
                throw std::invalid_argument("The file is not 'ppm' type");
    }
        else
            throw std::invalid_argument("The file is not 'ppm' type");
}

PPM::PPM(const PPM& other, const char* file)
{
    if (!file)
        throw std::runtime_error("Name of the file is nullptr");
    size_t len = std::strlen(file);
    if (len > 4) {
        if (file[len] == 'm' && file[len - 1] == 'p' && file[len - 2] == 'p' && file[len - 3] == '.') {
            pixels = other.pixels;
            snapshot = nullptr;
            height = other.height;
            with = other.with;
            fileName = file;
            maxValue = other.maxValue;
            type = other.type;
        }
        else
            throw std::runtime_error("The file is not 'ppm' type");
    }
    else
        throw std::runtime_error("The file is not 'ppm' type");
}

size_t getFileSize(std::ifstream& inFile) {
    size_t currPos = inFile.tellg();
    inFile.seekg(0, std::ios::end);
    size_t res = inFile.tellg();
    inFile.seekg(currPos);

    return res;
}

File* PPM::clone() const
{
    File* newObj = new (std::nothrow)PPM(*this);
    return newObj;
}

void PPM::grayscale()
{
    if (snapshot) {
        delete snapshot;
    }
    snapshot = new Vector<Pixel>(pixels);

    for (size_t i = 0; i < pixels.getSize(); i++)
    {
        unsigned char gray = (pixels[i].rgb[0] + pixels[i].rgb[1] + pixels[i].rgb[2]) / 3;
        pixels[i].rgb[0] = pixels[i].rgb[1] = pixels[i].rgb[2] = gray;
    }

}

void PPM::monochrome()
{
    if (snapshot) {
        delete snapshot;
    }
    snapshot = new Vector<Pixel>(pixels);
    unsigned char half = maxValue / 2;
    for (size_t i = 0; i < pixels.getSize(); i++)
    {
        unsigned char gray = (pixels[i].rgb[0] + pixels[i].rgb[1] + pixels[i].rgb[2]) / 3;
        if (gray <= half) {
            pixels[i].rgb[0] = pixels[i].rgb[1] = pixels[i].rgb[2] = '#';
        }
        else {
            pixels[i].rgb[0] = pixels[i].rgb[1] = pixels[i].rgb[2] = '.';
        }
    }
}

void PPM::rotate(char direction)
{
    if (snapshot) {
        delete snapshot;
    }
    snapshot = new Vector<Pixel>(pixels);
    std::swap(File::with, File::height);

    if (direction == 'l') {
        for (size_t i = File::with - 1, z = 0; i >= 0; i--, z += File::height) {
            for (size_t j = 0, k = File::height - 1, g = pixels.getSize() - 1 - z; k >= 0; g--, j += with, k--) {
                pixels[i + j].rgb[0] = (*snapshot)[g].rgb[0];
                pixels[i + j].rgb[1] = (*snapshot)[g].rgb[1];
                pixels[i + j].rgb[2] = (*snapshot)[g].rgb[2];
            }
        }
    }
    else {
        for (size_t i = File::with - 1, z = 0; i >= 0; i--, z += File::height) {
            for (size_t j = 0, k = File::height - 1, g = 0 + z; k >= 0; g++, j += with, k--) {
                pixels[i + j].rgb[0] = (*snapshot)[g].rgb[0];
                pixels[i + j].rgb[1] = (*snapshot)[g].rgb[1];
                pixels[i + j].rgb[2] = (*snapshot)[g].rgb[2];
            }
        }
    }
}

void PPM::negative()
{
    if (snapshot) {
        delete snapshot;
    }
    snapshot = new Vector<Pixel>(pixels);

    for (size_t i = 0; i < pixels.getSize(); i++)
    {
        pixels[i].rgb[0] = maxValue - pixels[i].rgb[0];
        pixels[i].rgb[1] = maxValue - pixels[i].rgb[1];
        pixels[i].rgb[2] = maxValue - pixels[i].rgb[2];
    }
}

void PPM::undo()
{
    if (snapshot) {
        for (size_t i = 0; i < snapshot->getSize(); i++)
        {
            pixels[i] = (*snapshot)[i];
        }
    }
    delete snapshot;
    snapshot = nullptr;
}

void PPM::save()
{
   
        std::ofstream file(File::fileName.c_str(), std::ios::binary);
        if (!file.is_open())
            throw std::runtime_error("Can`t open the file");
        file.write(reinterpret_cast<const char*>(&with), sizeof(size_t));
        file.write(reinterpret_cast<const char*>(&height), sizeof(size_t));
        file.write(reinterpret_cast<const char*>(&maxValue), sizeof(int));

        for (size_t i = 0; i < pixels.getSize(); i++)
        {
            file.write(reinterpret_cast<const char*>(&pixels[i].rgb[0]), sizeof(unsigned char));
            file.write(reinterpret_cast<const char*>(&pixels[i].rgb[1]), sizeof(unsigned char));
            file.write(reinterpret_cast<const char*>(&pixels[i].rgb[2]), sizeof(unsigned char));
        }
        file.close();
    
}

void PPM::saveAs(const char* file1)
{
    std::ofstream file(file1, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Can`t open the file");
    file.write(reinterpret_cast<const char*>(&with), sizeof(size_t));
    file.write(reinterpret_cast<const char*>(&height), sizeof(size_t));
    file.write(reinterpret_cast<const char*>(&maxValue), sizeof(int));

    for (size_t i = 0; i < pixels.getSize(); i++)
    {
        file.write(reinterpret_cast<const char*>(&pixels[i].rgb[0]), sizeof(unsigned char));
        file.write(reinterpret_cast<const char*>(&pixels[i].rgb[1]), sizeof(unsigned char));
        file.write(reinterpret_cast<const char*>(&pixels[i].rgb[2]), sizeof(unsigned char));
    }
    file.close();
}

const MyString& PPM::collage(const char* first, const char* second, char direction, const char* newFile)const
{
    PPM firstFile(first);
    PPM secondFile(second);
 
        if (firstFile.getHeight() == secondFile.getHeight() &&
            firstFile.getWith() == secondFile.getWith())
        {
            PPM toReturn(first, newFile);
            if (direction == 'h') {
                for (size_t i = firstFile.pixels.getSize(), j = 0; j < secondFile.pixels.getSize(); i++, j++)
                {
                    toReturn.pixels[i] = secondFile.pixels[j];
                }
                toReturn.height *= 2;
            }
            else {
                for (size_t i = 0, j = firstFile.getWith(), k = 0; i < secondFile.pixels.getSize(); i++, j += firstFile.getWith(), k += firstFile.getWith()) {

                    for (size_t g = firstFile.getWith(); g > 0; g--, j++, k++)
                    {
                        toReturn.pixels[k].rgb[0] = firstFile.pixels[i].rgb[0];
                        toReturn.pixels[k].rgb[1] = firstFile.pixels[i].rgb[1];
                        toReturn.pixels[k].rgb[2] = firstFile.pixels[i].rgb[2];

                        toReturn.pixels[j].rgb[0] = secondFile.pixels[i].rgb[0];
                        toReturn.pixels[j].rgb[1] = secondFile.pixels[i].rgb[1];
                        toReturn.pixels[j].rgb[2] = secondFile.pixels[i].rgb[2];
                    }
                }
                toReturn.with *= 2;
            }
            if (firstFile.maxValue < secondFile.maxValue)
                toReturn.maxValue = secondFile.maxValue;
            toReturn.save();
            return toReturn.fileName;
        }
        else
            throw std::runtime_error("Difference in widths or heights of images");
    
}
