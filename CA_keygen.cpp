// #include <cryptopp/dsa.h>
// #include <cryptopp/osrng.h>
// #include <cryptopp/files.h>
// #include <cryptopp/hex.h>
// #include <iostream>

// using namespace CryptoPP;

// void savePrivateKey(const DSA::PrivateKey &key, const std::string &filename) {
//     ByteQueue queue;
//     key.DEREncodePrivateKey(queue); // Use DER encoding for compact storage

//     FileSink file(filename.c_str());
//     queue.CopyTo(file);
//     file.MessageEnd();
// }

// void savePublicKey(const DSA::PublicKey &key, const std::string &filename) {
//     ByteQueue queue;
//     key.DEREncodePublicKey(queue); // Use DER encoding for compact storage

//     FileSink file(filename.c_str());
//     queue.CopyTo(file);
//     file.MessageEnd();
// }

// void setup() {
//     AutoSeededRandomPool rng;

//     // Generate 2048-bit DSA key pair for CA
//     DSA::PrivateKey privateKey;
//     privateKey.GenerateRandomWithKeySize(rng, 2048);

//     DSA::PublicKey publicKey;
//     privateKey.MakePublicKey(publicKey);

//     // Save the private key in CA_Priv.bin
//     savePrivateKey(privateKey, "CA_Priv.bin");

//     // Save the public key in CA_Pub.bin
//     savePublicKey(publicKey, "CA_Pub.bin");

//     std::cout << "CA keys generated and saved successfully." << std::endl;
// }

// int main() {
//     setup();
//     return 0;
// }


#include <cryptopp/dsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <iostream>

using namespace CryptoPP;

void setup() {
    AutoSeededRandomPool rng;

    // Generate 2048-bit DSA key pair for CA
    DSA::PrivateKey privateKey;
    privateKey.GenerateRandomWithKeySize(rng, 2048);

    DSA::PublicKey publicKey;
    privateKey.MakePublicKey(publicKey);

    // Save the private key in CA_Priv.bin
    FileSink privFile("CA_Priv.bin");
    privateKey.Save(privFile);

    // Save the public key in CA_Pub.bin
    FileSink pubFile("CA_Pub.bin");
    publicKey.Save(pubFile);

    std::cout << "CA keys generated and saved successfully." << std::endl;
}

int main() {
    setup();
    return 0;
}
