#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <numeric>
#include <string>

// Hydrophobicity or custom nucleotide encoding
std::map<char, double> encoding = {
    {'A', 1.0}, {'C', 2.0}, {'G', 3.0}, {'T', 4.0} // Placeholder values
};

// Simulated generations of a dynamic sequence
std::vector<std::string> evolving_seq = {
    "ACGTACGT", // Gen 1
    "ACGTACGC", // Gen 2
    "ACGTCCGC", // Gen 3
    "ACTTCCGC"  // Gen 4
};

// Static reference sequence to compare
std::string static_ref = "ACGTTCGA";

// Convert a sequence into numerical vector
std::vector<double> encode(const std::string& seq) {
    std::vector<double> result;
    for (char base : seq)
        result.push_back(encoding[base]);
    return result;
}

// Compute moving average (low-frequency trend)
std::vector<double> moving_average(const std::vector<double>& series, int window = 2) {
    std::vector<double> result(series.size(), 0.0);
    for (size_t i = 0; i < series.size(); ++i) {
        int count = 0;
        double sum = 0.0;
        for (int j = -window; j <= window; ++j) {
            if (i + j >= 0 && i + j < series.size()) {
                sum += series[i + j];
                count++;
            }
        }
        result[i] = sum / count;
    }
    return result;
}

// Subtract trend to get high-frequency IMF-like signal
std::vector<double> detail_component(const std::vector<double>& original, const std::vector<double>& trend) {
    std::vector<double> result(original.size());
    for (size_t i = 0; i < original.size(); ++i)
        result[i] = original[i] - trend[i];
    return result;
}

// Compare two vectors using dot product similarity
double similarity_score(const std::vector<double>& a, const std::vector<double>& b) {
    double dot = 0, norm_a = 0, norm_b = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        dot += a[i] * b[i];
        norm_a += a[i] * a[i];
        norm_b += b[i] * b[i];
    }
    return dot / (std::sqrt(norm_a) * std::sqrt(norm_b) + 1e-8);
}

int main() {
    int L = evolving_seq[0].size();
    int G = evolving_seq.size();

    // Create time series 
    std::vector<std::vector<double>> position_series(L);
    for (int pos = 0; pos < L; ++pos) {
        for (int gen = 0; gen < G; ++gen) {
            position_series[pos].push_back(encoding[evolving_seq[gen][pos]]);
        }
    }


    std::vector<std::vector<double>> trends(L), details(L);
    for (int i = 0; i < L; ++i) {
        // (evolving signal)
        trends[i] = moving_average(position_series[i]);
        details[i] = detail_component(position_series[i], trends[i]);
    }

    // Encode static reference
    std::vector<double> static_vec = encode(static_ref);
    std::vector<double> static_trend = moving_average(static_vec);
    std::vector<double> static_detail = detail_component(static_vec, static_trend);

    // Compare each detail IMF (evolving signal) to static sequence slice
    std::cout << "Similarity of dynamic evolution to static reference by position:\n";
    for (int pos = 0; pos < L; ++pos) {

        // Compare position's detail signal to corresponding static base (repeated across generations)
        std::vector<double> static_series(G, static_vec[pos]);
        double sim = similarity_score(details[pos], static_series);
        std::cout << "Position " << pos << ": Similarity = " << sim << "\n";
    }

    return 0;
}
