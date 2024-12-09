/*
 * XOR, short for "exclusive or," is a bitwise logical operation commonly used in computer programming and cryptography.
 * It takes two binary inputs and produces a binary output. The XOR operation returns 1 (TRUE) if the inputs are different
 * and 0 (FALSE) if the inputs are equal.
 *
 * In this program:
 * - XOR is used to encrypt data by combining each byte of the input file with a corresponding byte from a secret key.
 * - The same operation can be used to decrypt the data, as XOR is symmetric: (A XOR B) XOR B = A.
 */

#include <iostream>
#include <fstream>
#include <iomanip>

std::string KEY = "mysupersecretkey";

std::string xor_encrypt(const std::string &data, const std::string &key)
{
    std::string result;
    for (size_t i = 0; i < data.size(); ++i)
    {
        result += data[i] ^ key[i % key.size()];
    }

    return result;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    // Attempt to open the file provided as an argument in binary mode
    std::ifstream file(argv[1], std::ios::binary);
    if (!file)
    {
        std::cerr << "Error: Could not open file " << argv[1] << "\n";
        return 1;
    }

    // Read the entire content of the file into a string data
    // std::istreambuf_iterator reads the file as a stream of bytes
    std::string data((std::istreambuf_iterator<char>(file)), {});

    // Encrypt the data using the xor_encrypt function and the key KEY
    // The result is stored in the string encrypted
    std::string encrypted = xor_encrypt(data, KEY);

    std::cout << "{ ";
    for (size_t i = 0; i < encrypted.size(); ++i)
    {
        // Print each byte in hexadecimal
        std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0')
                  << (static_cast<unsigned char>(encrypted[i]) & 0xFF)
                  << (i < encrypted.size() - 1 ? ", " : " ");

        // After every 16 bytes, add a newline to improve readability
        if ((i + 1) % 16 == 0)
            std::cout << "\n  ";
    }

    std::cout << "};\n";

    return 0;
}