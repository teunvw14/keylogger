#include <iostream>
#include <fstream>
#include <string>
#include "src/Decryption.h"


int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Program needs two arguments, in and out!" << std::endl;
        return 2;
    }
        
    std::string in (argv[1]), out (argv[2]);
    std::ifstream inFile (in);
    if (!inFile)
    {
        std::cerr << "Could not find or read the input file " << in << "." << std::endl;
        return 3;
    }
    std::string inputData;
    inFile >> inputData;
    if (!inFile)
    {
        std::cerr << "The input file " << in << " might be corrupted, the program was unable to read it." << std::endl;
        return 4;
    }
    std::string outputData = Decryption::DecryptB64(inputData);
    
    std::ofstream outFile (out);
    if (!outFile)
    {
        std::cerr << "Can not write to output file" << out << "." << std::endl;
        return 5;
    }
    
    outFile << outputData;
    std::cout << "Decoding was succesful." << std::endl;

    return 0;
}