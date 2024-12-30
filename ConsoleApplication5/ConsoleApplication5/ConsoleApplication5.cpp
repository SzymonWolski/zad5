#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <chrono>
#include <cmath>

// Funkcja obliczająca fragment całki
void calculate_partial_integral(double start, double end, double step, double& result) {
    double partial_sum = 0.0;
    for (double x = start; x < end; x += step) {
        partial_sum += 4.0 / (1.0 + x * x) * step;
    }
    result = partial_sum;
}

int main() {
    // Ustawienia
    const double lower_bound = 0.0;
    const double upper_bound = 1.0;
    double total_result = 0.0;

    // Wczytanie ilości podziałów oraz wątków
    size_t num_intervals=1000000;
    size_t num_threads;

    
    std::cout << "Podaj liczbe watkow: ";
    std::cin >> num_threads;

    // Wyznaczanie kroku całkowania
    double step = (upper_bound - lower_bound) / num_intervals;

    // Podział przedziałów na fragmenty dla wątków
    std::vector<std::thread> threads;
    std::vector<double> results(num_threads, 0.0);

    auto start_time = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < num_threads; ++i) {
        double start = lower_bound + i * (upper_bound - lower_bound) / num_threads;
        double end = lower_bound + (i + 1) * (upper_bound - lower_bound) / num_threads;

        threads.emplace_back(calculate_partial_integral, start, end, step, std::ref(results[i]));
    }

    // Czekanie na zakończenie wątków
    for (auto& thread : threads) {
        thread.join();
    }

    // Sumowanie wyników cząstkowych
    for (const auto& partial_result : results) {
        total_result += partial_result;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // Wyświetlanie wyników
    std::cout << "Przybliżona wartosc liczby PI: " << total_result << std::endl;
    std::cout << "Czas obliczen: " << duration.count() << " ms" << std::endl;

    return 0;
}