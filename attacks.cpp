#include "attacs.h"


std::vector<int> attack::superposition(const std::vector<int>& f, const std::vector<int>& g) {
    std::vector<int> result;
    result.reserve(f.size());
    for (int i = 0; i < f.size(); ++i) {
        result.push_back(f[g[i]]);
    }
    return result;
}
Umap attack::get_basis(std::vector<std::vector<int>> S) {
    std::vector<int> id(256);
    std::iota(id.begin(), id.end(), 0);
    Umap R;
    Umap current;
    byte psi(0);
    byte e(1);
    R.insert(std::make_pair(id, byte(0)));
    while (R.size() < 256) {
        std::vector<int> f;
        if (!S.empty()) {
            f = S[0];
            S.erase(S.begin());
        }
        auto it = R.find(f);
        if (it == R.end()) {
            e.xtime();
            byte psi_f(e);
            for (auto item : R) {
                psi = e + item.second;
                std::cout << psi << " ";
                current.insert(std::make_pair(superposition(f, item.first), psi));
            }
            R.insert(current.begin(), current.end());
            current.clear();
        }
    }
    return R;

}
std::vector<std::vector<int>> attack::get_permutations() {
    const std::string path  = ".\\data\\256Permutations.txt";
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        exit(0);
    }

    std::vector<std::vector<int>> vectors;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::vector<int> intVector;

        std::copy(std::istream_iterator<int>(lineStream),
            std::istream_iterator<int>(), std::back_inserter(intVector));

        vectors.push_back(intVector);
    }
    return vectors;
}
void attack::run() {
    std::vector<std::vector<int>> permutations = get_permutations();
    Umap basis = get_basis(permutations);

    std::vector<std::pair<std::vector<int>, byte>> sorted_vector(256);

    for (auto item : basis) {
        int index = static_cast<int>(item.second);
        sorted_vector[index] = item;
    }

    /*std::vector<int> Q_tilda;
    Q_tilda.reserve(256);
    for (int i = 0; i < sorted_vector.size(); ++i) {
        Q_tilda.push_back(sorted_vector[i].first[0]);     
    }*/


    std::string filePath = "result.txt";


    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        std::cerr << "Error opening the file for writing." << std::endl;
        exit(0); // Return an error code
    }

    for (int i = 0; i < sorted_vector.size(); ++i) {
        //outFile << Q_tilda[i] << ' ';
        outFile << sorted_vector[i].first[0] << ' ';
    }

    outFile.close();

   /* for (auto b : sorted_vector) {
        std::cout << b.second << std::endl;
    }
    std::vector<int> id(256, 0);
    bool isperm = true;
    
    std::cout << "is_permutation " << (isperm ? "true" : "false") << std::endl;*/

    
}