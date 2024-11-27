#include <cryptopp/md5.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace CryptoPP;
using namespace std;

// Function to compute MD5 checksum of a file
std::string compute_md5_checksum(const std::string& filename) {
    MD5 hash;
    std::string digest;
    FileSource fs(filename.c_str(), true /*pumpAll*/, new HashFilter(hash, new HexEncoder(new StringSink(digest))));
    
    return digest;
}

int main() {
    // Compute MD5 checksums for session key files
    std::string md5ChecksumA = compute_md5_checksum("SSNKA.bin");
    std::string md5ChecksumB = compute_md5_checksum("SSNKB.bin");

    cout << "MD5 Checksum for SSNKA.bin: " << md5ChecksumA << endl;
    cout << "MD5 Checksum for SSNKB.bin: " << md5ChecksumB << endl;

    return 0;
}
