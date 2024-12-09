/*
 * Base64 Encoding and Decoding
 *
 * Base64 is a method of encoding binary data into an ASCII text format. It is commonly used in computer programming
 * to ensure that binary data can be safely transmitted or stored using text-based protocols such as HTTP, SMTP, or JSON.
 *
 * Key Features:
 * - Base64 takes binary data and represents it as a string of ASCII characters, making it human-readable and
 *   compatible with text-based systems.
 * - The encoding process splits the input data into groups of 3 bytes (24 bits), divides those into 4 groups
 *   of 6 bits each, and maps each group to a corresponding Base64 character.
 * - Padding (`=`) is added to the end of the encoded string if the input data is not a multiple of 3 bytes.
 *
 * In this program:
 * - Data is encoded into Base64 format for safe storage or transmission.
 * - Decoding reverses the process to retrieve the original binary data.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

const std::string BASE64_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Function to encode binary data to Base64
std::string encodeBase64(const std::vector<uint8_t> &data)
{
    std::string encoded;
    size_t i = 0;
    while (i < data.size())
    {
        uint32_t triple = 0;
        int padding = 0;

        // Read three bytes at a time
        for (int j = 0; j < 3; ++j)
        {
            triple <<= 8;
            if (i < data.size())
            {
                triple |= data[i++];
            }
            else
            {
                ++padding;
            }
        }

        // Convert 24 bits into 4 Base64 characters
        for (int j = 0; j < 4; ++j)
        {
            if (j < 4 - padding)
            {
                encoded += BASE64_CHARS[(triple >> (18 - 6 * j)) & 0x3F];
            }
            else
            {
                encoded += '=';
            }
        }
    }
    return encoded;
}

// Function to decode Base64 string to binary data
std::vector<uint8_t> decodeBase64(const std::string &base64Str)
{
    std::vector<uint8_t> decoded;
    uint32_t triple = 0;
    int count = 0;

    for (char c : base64Str)
    {
        if (c == '=')
            break;

        size_t index = BASE64_CHARS.find(c);
        if (index == std::string::npos)
        {
            throw std::runtime_error("Invalid Base64 character encountered.");
        }

        triple = (triple << 6) | index;
        ++count;

        if (count == 4)
        {
            decoded.push_back((triple >> 16) & 0xFF);
            decoded.push_back((triple >> 8) & 0xFF);
            decoded.push_back(triple & 0xFF);
            count = 0;
            triple = 0;
        }
    }

    // Handle remaining bytes
    if (count > 0)
    {
        triple <<= (4 - count) * 6;
        if (count >= 2)
            decoded.push_back((triple >> 16) & 0xFF);
        if (count == 3)
            decoded.push_back((triple >> 8) & 0xFF);
    }

    return decoded;
}

// Function to read a binary file into a vector
std::vector<uint8_t> readBinaryFile(const std::string &filepath)
{
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Error opening file for reading: " + filepath);
    }
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

// Function to write binary data to a file
void writeBinaryFile(const std::string &filepath, const std::vector<uint8_t> &data)
{
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Error opening file for writing: " + filepath);
    }
    file.write(reinterpret_cast<const char *>(data.data()), data.size());
}

// Function to read a Base64 string from a file
std::string readBase64String(const std::string &filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        throw std::runtime_error("Error opening file for reading Base64 string: " + filepath);
    }
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <encode|decode> <input_file|string>\n";
        return 1;
    }

    std::string operation = argv[1];
    std::string input = argv[2];

    try
    {
        if (operation == "encode")
        {
            // Read binary file and encode
            auto binaryData = readBinaryFile(input);
            std::string encoded = encodeBase64(binaryData);
            std::cout << "Encoded Base64 string:\n"
                      << encoded << "\n";
        }
        else if (operation == "decode")
        {
            std::string base64Str;

            // Check if input is a file or direct Base64 string
            if (std::ifstream(input).is_open())
            {
                base64Str = readBase64String(input);
            }
            else
            {
                base64Str = input;
            }

            // Decode Base64 and write to binary file
            auto decodedData = decodeBase64(base64Str);
            std::string outputFile = "output.bin";
            writeBinaryFile(outputFile, decodedData);
            std::cout << "Decoded binary file saved as: " << outputFile << "\n";
        }
        else
        {
            std::cerr << "Invalid operation. Use 'encode' or 'decode'.\n";
            return 1;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
