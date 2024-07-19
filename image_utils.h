#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <exception>

using namespace std;

// Pack the BMP struct to save memory
#pragma pack(push, 1)
struct BMPHeader {
    uint16_t signature;         // "BM" (0x42 0x4D)
    uint32_t fileSize;          // File size in bytes
    uint16_t reserved1;         // Reserved (0)
    uint16_t reserved2;         // Reserved (0)
    uint32_t dataOffset;        // Offset of pixel data (headers' size)
    uint32_t headerSize;        // Header size (40 bytes)
    int32_t width;              // Image width in pixels
    int32_t height;             // Image height in pixels
    uint16_t planes;            // Number of color planes (1)
    uint16_t bitsPerPixel;      // Bits per pixel (24 for RGB)
    uint32_t compression;       // Compression method (0 for BI_RGB)
    uint32_t imageSize;         // Image size in bytes (can be 0 for BI_RGB)
    int32_t xPixelsPerMeter;    // Horizontal resolution (pixels per meter, typically 0)
    int32_t yPixelsPerMeter;    // Vertical resolution (pixels per meter, typically 0)
    uint32_t colorsUsed;        // Number of colors in the palette (0 for BI_RGB)
    uint32_t importantColors;   // Number of important colors (0 for BI_RGB)
};
#pragma pack(pop)

void writeBMP(const char* input, int width, const char* output) {
    // Get file location and interpret into readable data.
    ifstream dataInit(input, ios::binary);
    if (!dataInit) {
        throw("FILE COULD NOT LOAD");
    }
    vector<char> fileData((istreambuf_iterator<char>(dataInit)), istreambuf_iterator<char>());
    dataInit.close();

    // Calculate the height based on the total size of fileData and the width
    size_t dataSize = fileData.size();
    size_t height = dataSize / (width * 3); // Each pixel has 3 bytes (BGR)

    std::ofstream outFile(output, std::ios::binary);
    if (!outFile) {
        std::cerr << "Failed to open output file " << output << std::endl;
        return;
    }

    // BMP header setup
    BMPHeader header;
    header.signature = 0x4D42; // "BM"
    header.fileSize = sizeof(BMPHeader) + dataSize;
    header.reserved1 = 0;
    header.reserved2 = 0;
    header.dataOffset = sizeof(BMPHeader);
    header.headerSize = 40;
    header.width = width;
    header.height = height;
    header.planes = 1;
    header.bitsPerPixel = 24; // 24 bits per pixel (RGB)
    header.compression = 0;   // BI_RGB (no compression)
    header.imageSize = 0;     // Can be 0 for BI_RGB
    header.xPixelsPerMeter = 0;
    header.yPixelsPerMeter = 0;
    header.colorsUsed = 0;
    header.importantColors = 0;

    outFile.write(reinterpret_cast<const char*>(&header), sizeof(BMPHeader));

    // Calculate padding (BMP rows are padded to 4-byte boundaries)
    int paddingSize = (4 - (width * 3) % 4) % 4;

    // Write pixel data row by row (BMP is stored bottom-to-top)
    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            int index = ((height - 1 - y) * width + x) * 3; // Each pixel has 3 bytes (BGR)

            // Ensure index is within bounds of fileData
            if (index + 2 < dataSize) {
                // Write pixel data (BMP is stored in BGR order)
                outFile.put(fileData[index]);     // Blue
                outFile.put(fileData[index + 1]); // Green
                outFile.put(fileData[index + 2]); // Red
            }
            else {
                // If out of bounds, write default black pixel
                outFile.put(0);
                outFile.put(0);
                outFile.put(0);
            }
        }

        // Write padding bytes (if any)
        for (int p = 0; p < paddingSize; ++p) {
            outFile.put(0);
        }
    }

    outFile.close();
    std::cout << "BMP file " << output << " written successfully." << std::endl;
}
