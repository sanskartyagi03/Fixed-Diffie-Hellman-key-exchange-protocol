#include <iostream>
#include <fstream>
#include <cryptopp/integer.h>
#include <cryptopp/osrng.h>

using namespace CryptoPP;
using namespace std;

// Function to read the global parameters (g, p, q) from the binary file
void read_from_binary_file(const string& filename, Integer& g, Integer& p, Integer& q) {
    ifstream file(filename, ios::binary);
    if (file.is_open()) {
        size_t g_size = 128;
        size_t p_size = 128;
        size_t q_size = 20;

        byte* g_bytes = new byte[g_size];
        byte* p_bytes = new byte[p_size];
        byte* q_bytes = new byte[q_size];

        file.read(reinterpret_cast<char*>(g_bytes), g_size);
        file.read(reinterpret_cast<char*>(p_bytes), p_size);
        file.read(reinterpret_cast<char*>(q_bytes), q_size);

        g.Decode(g_bytes, g_size);
        p.Decode(p_bytes, p_size);
        q.Decode(q_bytes, q_size);

        delete[] g_bytes;
        delete[] p_bytes;
        delete[] q_bytes;

        file.close();
    } else {
        cerr << "Unable to open file " << filename << " for reading." << endl;
    }
}

// Function to generate a private key in the range [1, q-1]
Integer generate_private_key(const Integer& q) {
    AutoSeededRandomPool rng;
    Integer private_key;

    do {
        private_key.Randomize(rng, q.BitCount());
    } while (private_key < 1 || private_key >= q);

    return private_key;
}

// Function to save the private key to a binary file
void save_private_key_to_file(const string& filename, const Integer& private_key) {
    ofstream file(filename, ios::binary);
    if (file.is_open()) {
        size_t key_size = private_key.MinEncodedSize();
        byte* key_bytes = new byte[key_size];

        private_key.Encode(key_bytes, key_size);

        file.write(reinterpret_cast<const char*>(key_bytes), key_size);

        delete[] key_bytes;
        file.close();
    } else {
        cerr << "Unable to open file " << filename << " for writing." << endl;
    }
}

int main() {
    Integer g, p, q;

    // Step 1: Read the global parameters (g, p, q) from the binary file
    read_from_binary_file("params.bin", g, p, q);

    // Step 2: Generate private keys α (for Alice) and β (for Bob)
    Integer alpha = generate_private_key(q);
    Integer beta = generate_private_key(q);

    cout<<alpha.MinEncodedSize()<<endl;
    cout<<beta.MinEncodedSize()<<endl;

    // Step 3: Save the private keys in separate binary files
    save_private_key_to_file("privatekeyA.bin", alpha);
    save_private_key_to_file("privatekeyB.bin", beta);

    cout << "Private keys generated and saved to 'privatekeyA.bin' and 'privatekeyB.bin'" << endl;

    return 0;
}
