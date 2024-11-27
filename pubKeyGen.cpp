#include <iostream>
#include <fstream>
#include <cryptopp/integer.h>

using namespace CryptoPP;
using namespace std;

// Function to read the global parameters (g, p, q) from the binary file
void read_from_binary_file(const string& filename, Integer& g, Integer& p, Integer& q) {
    ifstream file(filename, ios::binary);
    if (file.is_open()) {
        size_t g_size = 128;
        size_t p_size = 128;
        size_t q_size = 20;

        //cout<<g_size<<" "<<p_size<<" "<<q_size<<endl;

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

// Function to read the private key from a binary file
Integer read_private_key_from_file(const string& filename) {
    ifstream file(filename, ios::binary);
    Integer private_key;

    if (file.is_open()) {
        size_t key_size = 20;
        byte* key_bytes = new byte[key_size];

        file.read(reinterpret_cast<char*>(key_bytes), key_size);
        private_key.Decode(key_bytes, key_size);

        delete[] key_bytes;
        file.close();
    } else {
        cerr << "Unable to open file " << filename << " for reading." << endl;
    }

    return private_key;
}

// Function to save the public key to a binary file
void save_public_key_to_file(const string& filename, const Integer& public_key) {
    ofstream file(filename, ios::binary);
    if (file.is_open()) {
        size_t key_size = public_key.MinEncodedSize();
        byte* key_bytes = new byte[key_size];

        public_key.Encode(key_bytes, key_size);

        file.write(reinterpret_cast<const char*>(key_bytes), key_size);

        delete[] key_bytes;
        file.close();
    } else {
        cerr << "Unable to open file " << filename << " for writing." << endl;
    }
}

// Function to compute the public key: g^private_key mod p
Integer compute_public_key(const Integer& g, const Integer& private_key, const Integer& p) {
    return a_exp_b_mod_c(g, private_key, p);
}

int main() {
    Integer g, p, q;

    // Step 1: Read the global parameters (g, p, q) from the binary file
    read_from_binary_file("params.bin", g, p, q);

    // Step 2: Read the private keys α (for Alice) and β (for Bob)
    Integer alpha = read_private_key_from_file("privatekeyA.bin");
    Integer beta = read_private_key_from_file("privatekeyB.bin");

    // Step 3: Generate the public keys KA (for Alice) and KB (for Bob)
    Integer publicKeyA = compute_public_key(g, alpha, p);
    Integer publicKeyB = compute_public_key(g, beta, p);

    // Step 4: Save the public keys in separate binary files
    save_public_key_to_file("publicKeyA.bin", publicKeyA);
    save_public_key_to_file("publicKeyB.bin", publicKeyB);

    cout << "Public keys generated and saved to 'publicKeyA.bin' and 'publicKeyB.bin'" << endl;

    return 0;
}
