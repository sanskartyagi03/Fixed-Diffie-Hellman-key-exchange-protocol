#include <string>
#include <iostream>
#include <fstream>
#include <cryptopp/integer.h>

using namespace CryptoPP;
using namespace std;

Integer compute_session_key(const Integer& publicKeyOther, const Integer& privateKey, const Integer& p) {
    return a_exp_b_mod_c(publicKeyOther, privateKey, p);
}

// Function to read the global parameters (g, p, q) from the binary file
void read_global_params(const string& filename, Integer& g, Integer& p, Integer& q) {
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

Integer read_public_key_from_file(const string& filename) {
    ifstream file(filename, ios::binary);
    Integer public_key;

    if (file.is_open()) {
        size_t key_size = 128;
        byte* key_bytes = new byte[key_size];

        file.read(reinterpret_cast<char*>(key_bytes), key_size);
        public_key.Decode(key_bytes, key_size);

        delete[] key_bytes;
        file.close();
    } else {
        cerr << "Unable to open file " << filename << " for reading." << endl;
    }

    return public_key;
}

// Function to save the session key to a binary file
void save_session_key_to_file(const string& filename, const Integer& sessionKey) {
    ofstream file(filename, ios::binary);
    if (file.is_open()) {
        size_t key_size = sessionKey.MinEncodedSize();
        byte* key_bytes = new byte[key_size];
        sessionKey.Encode(key_bytes, key_size);
        file.write(reinterpret_cast<const char*>(key_bytes), key_size);
        delete[] key_bytes;
        file.close();
    } else {
        cerr << "Unable to open file " << filename << " for writing." << endl;
    }
}

int main(){

    //global parameters
    Integer g, p, q;
    read_global_params("params.bin", g, p, q);

    //Read the private keys alpha (for Alice) and beta (for Bob)
    Integer alpha = read_private_key_from_file("privatekeyA.bin");
    Integer beta = read_private_key_from_file("privatekeyB.bin");

    //Read the public keys alpha (for Alice) and beta (for Bob)
    Integer alicePubKey = read_public_key_from_file("publicKeyA.bin");
    Integer bobPubKey = read_public_key_from_file("publicKeyB.bin");

    //compute 
    Integer sessionKeyA = compute_session_key(bobPubKey, alpha, p);
    Integer sessionKeyB = compute_session_key(alicePubKey, beta, p);

    // Save session keys to files
    save_session_key_to_file("SSNKA.bin", sessionKeyA);
    save_session_key_to_file("SSNKB.bin", sessionKeyB);

    cout << "Session keys saved to 'SSNKA.bin' and 'SSNKB.bin'" << endl;

    return 0;
}