#include <cryptopp/dsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/integer.h>
#include <cryptopp/filters.h>
#include <cryptopp/queue.h>
#include <string>
#include <iostream>
#include <sstream>

using namespace CryptoPP;
using namespace std;

// Function to read the public key from a binary file
DSA::PublicKey read_ca_public_key_from_file(const string& filename) {
    DSA::PublicKey caPublicKey;
    FileSource fs(filename.c_str(), true);
    caPublicKey.Load(fs);
    return caPublicKey;
}

// Function to read the certificate from a file
std::string read_certificate_from_file(const std::string& filename) {
    ifstream file(filename, ios::binary);
    stringstream buffer;

    if (file.is_open()) {
        buffer << file.rdbuf();
        file.close();
    } else {
        cerr << "Unable to open file " << filename << " for reading." << endl;
    }

    return buffer.str();
}

// Convert a hexadecimal string to a byte array
std::string hex_string_to_bytes(const std::string& hexString) {
    std::string bytes;
    StringSource ss(hexString, true, new HexDecoder(new StringSink(bytes)));
    return bytes;
}

// Verify the certificate
void verifyCertificate(const std::string &certFile, const std::string &caPubFile) {
    // Load CA's public key (DSA)
    DSA::PublicKey caPublicKey = read_ca_public_key_from_file(caPubFile);

    // Read the certificate data
    std::string certificateData = read_certificate_from_file(certFile);

    // Output the certificate contents
    std::cout << "Certificate Contents:\n" << certificateData << std::endl;

    // Extract public key, signature from the certificate
    size_t sigPos = certificateData.find("\nSignature: ");
    if (sigPos == std::string::npos) {
        cerr << "Invalid certificate format." << endl;
        return;
    }

    std::string certificateBody = certificateData.substr(0, sigPos);
    std::string signatureHex = certificateData.substr(sigPos + 11);

    std::string signature = hex_string_to_bytes(signatureHex);

    // Hash the certificate body using SHA-256
    SHA256 hash;
    std::string digest;
    StringSource(certificateBody, true, new HashFilter(hash, new StringSink(digest)));

    // Verify the signature using the CA's public key and the digest
    DSA::Verifier verifier(caPublicKey);
    bool result = verifier.VerifyMessage(reinterpret_cast<const byte*>(digest.data()), digest.size(), 
                                         reinterpret_cast<const byte*>(signature.data()), signature.size());

    if (result) {
        std::cout << "Certificate is valid." << std::endl;
    } else {
        std::cout << "Certificate verification failed." << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <Certificate File> <CA Public Key File>" << std::endl;
        return 1;
    }

    std::string certFile = argv[1];
    std::string caPubFile = argv[2];

    verifyCertificate(certFile, caPubFile);
    return 0;
}
