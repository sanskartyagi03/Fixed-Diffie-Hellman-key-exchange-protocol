#include <iostream>
#include <fstream>
#include <cryptopp/integer.h>
#include <cryptopp/osrng.h>

using namespace CryptoPP;

// Miller-Rabin Primality Test
bool miller_rabin_test(const Integer& n, const Integer& a) {
    Integer n_minus_1 = n - 1;
    Integer d = n_minus_1;
    int r = 0;

    while (d.IsEven()) {
        d >>= 1;
        r++;
    }

    Integer x = a_exp_b_mod_c(a, d, n);
    if (x == 1 || x == n_minus_1) {
        return true;
    }

    for (int i = 0; i < r - 1; i++) {
        x = a_exp_b_mod_c(x, Integer::Two(), n);
        if (x == n_minus_1) {
            return true;
        }
    }

    return false;
}

// Function to check if a number is prime using the Miller-Rabin test
bool IsPrime(const Integer& n, int iterations = 50) {
    if (n <= 1 || n.IsEven()) return false;
    if (n == 2 || n == 3) return true;

    AutoSeededRandomPool rng;

    for (int i = 0; i < iterations; i++) {
        Integer a;
        a.Randomize(rng, 2, n - 2); // a in [2, n-2]

        if (!miller_rabin_test(n, a)) {
            return false;
        }
    }

    return true;
}

// Function to generate a large prime number with specified bit length
Integer generate_large_prime(int bits) {
    AutoSeededRandomPool rng;
    Integer prime;

    do {
        prime.Randomize(rng, bits);
        prime |= Integer::Power2(bits-1);
    } while (!prime.IsOdd() || !IsPrime(prime));  // Ensure prime is odd and passes primality test

    return prime;
}

// Function to find a generator for the subgroup of order q in Zp*
Integer find_generator(const Integer& p, const Integer& q) {
    AutoSeededRandomPool rng;
    Integer h, g;

    Integer p_minus_1 = p - 1;
    do {
        h.Randomize(rng, 2, p_minus_1 - 1); // h in range [2, p-2]
        g = a_exp_b_mod_c(h, (p_minus_1 / q), p);
    } while (g == 1);

    return g;
}

// Function to save the parameters to a binary file
void save_to_binary_file(const std::string& filename, const Integer& g, const Integer& p, const Integer& q) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        size_t g_size = g.MinEncodedSize();
        size_t p_size = p.MinEncodedSize();
        size_t q_size = q.MinEncodedSize();

        std::cout<<g_size<<" "<<p_size<<" "<<q_size<<std::endl;

        byte* g_bytes = new byte[g_size];
        byte* p_bytes = new byte[p_size];
        byte* q_bytes = new byte[q_size];

        g.Encode(g_bytes, g_size);
        p.Encode(p_bytes, p_size);
        q.Encode(q_bytes, q_size);

        file.write(reinterpret_cast<const char*>(g_bytes), g_size);
        file.write(reinterpret_cast<const char*>(p_bytes), p_size);
        file.write(reinterpret_cast<const char*>(q_bytes), q_size);

        delete[] g_bytes;
        delete[] p_bytes;
        delete[] q_bytes;

        file.close();
    } else {
        std::cerr << "Unable to open file " << filename << " for writing." << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <p_bits> <q_bits>" << std::endl;
        return 1;
    }

    int p_bits = std::stoi(argv[1]);
    int q_bits = std::stoi(argv[2]);

    Integer q = generate_large_prime(q_bits);
    Integer p = generate_large_prime(p_bits);
    Integer g = find_generator(p, q);

    save_to_binary_file("params.bin", g, p, q);

    std::cout << "Setup completed. Parameters saved to 'params.bin'" << std::endl;

    return 0;
}
