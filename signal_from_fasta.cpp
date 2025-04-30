#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <cctype>

// Map for nucleotide encoding
std::unordered_map<char, double> nucleotide_map = {
    {'A', 1.0}, {'C', 2.0}, {'G', 3.0}, {'T', 4.0}, {'U', 4.0}
};

// Map for Kyte-Doolittle hydrophobicity
std::unordered_map<char, double> hydrophobicity = {
    {'A', 1.8}, {'R', -4.5}, {'N', -3.5}, {'D', -3.5}, {'C', 2.5},
    {'Q', -3.5}, {'E', -3.5}, {'G', -0.4}, {'H', -3.2}, {'I', 4.5},
    {'L', 3.8}, {'K', -3.9}, {'M', 1.9}, {'F', 2.8}, {'P', -1.6},
    {'S', -0.8}, {'T', -0.7}, {'W', -0.9}, {'Y', -1.3}, {'V', 4.2}
};

std::vector<double> encode_sequence(const std::string& sequence, const std::string& type) {
    std::vector<double> signal;
    for (char base : sequence) {
        char upper = std::toupper(base);
        if (type == "nucleotide") {
            signal.push_back(nucleotide_map.count(upper) ? nucleotide_map[upper] : 0.0);
        } else if (type == "protein") {
            signal.push_back(hydrophobicity.count(upper) ? hydrophobicity[upper] : 0.0);
        }
    }
    return signal;
}

std::string sanitize_id(const std::string& id) {
    std::string safe = id;
    for (char& c : safe) {
        if (!std::isalnum(c)) c = '_';
    }
    return safe;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./signal_from_fasta <file.fasta> <nucleotide|protein>\n";
        return 1;
    }

    std::string fasta_file = argv[1];
    std::string type = argv[2];

    if (type != "nucleotide" && type != "protein") {
        std::cerr << "Error: type must be 'nucleotide' or 'protein'\n";
        return 1;
    }

    std::ifstream infile(fasta_file);
    if (!infile) {
        std::cerr << "Error opening file: " << fasta_file << "\n";
        return 1;
    }

    std::string line, seq_id, sequence;
    while (std::getline(infile, line)) {
        if (line.empty()) continue;
        if (line[0] == '>') {
            if (!sequence.empty()) {
                std::vector<double> signal = encode_sequence(sequence, type);
                std::string safe_id = sanitize_id(seq_id);
                std::cout << "// " << seq_id << "\n";
                std::cout << "std::vector<double> signal_" << safe_id << " = { ";
                for (size_t i = 0; i < signal.size(); ++i) {
                    std::cout << std::fixed << std::setprecision(3) << signal[i];
                    if (i < signal.size() - 1) std::cout << ", ";
                }
                std::cout << " };\n\n";
                sequence.clear();
            }
            seq_id = line.substr(1);
        } else {
            sequence += line;
        }
    }

    // Process last sequence
    if (!sequence.empty()) {
        std::vector<double> signal = encode_sequence(sequence, type);
        std::string safe_id = sanitize_id(seq_id);
        std::cout << "// " << seq_id << "\n";
        std::cout << "std::vector<double> signal_" << safe_id << " = { ";
        for (size_t i = 0; i < signal.size(); ++i) {
            std::cout << std::fixed << std::setprecision(3) << signal[i];
            if (i < signal.size() - 1) std::cout << ", ";
        }
        std::cout << " };\n\n";
    }

    return 0;
}
