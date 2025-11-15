#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <iomanip>

struct Circle {
    double x, y, r;
};

bool inside_all(const double x, const double y, const std::vector<Circle>& cs) {
    for (const auto& c : cs) {
        const double dx = x - c.x;
        const double dy = y - c.y;
        if (dx * dx + dy * dy > c.r * c.r) {
            return false;
        }
    }
    return true;
}

double monte_carlo_area(const std::vector<Circle>& cs,
                        double x_min, double x_max,
                        double y_min, double y_max,
                        int n,
                        std::mt19937_64& gen) {
    std::uniform_real_distribution<double> dist_x(x_min, x_max);
    std::uniform_real_distribution<double> dist_y(y_min, y_max);

    int M = 0;
    for (int i = 0; i < n; ++i) {
        double x = dist_x(gen);
        double y = dist_y(gen);
        if (inside_all(x, y, cs)) {
            ++M;
        }
    }

    const double s_rec = (x_max - x_min) * (y_max - y_min);
    return s_rec * static_cast<double>(M) / static_cast<double>(n);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    const std::vector<Circle> cs = {
        {1.0, 1.0, 1.0},
        {1.5, 2.0, std::sqrt(5.0) / 2.0},
        {2.0, 1.5, std::sqrt(5.0) / 2.0}
    };

    const double PI = std::acos(-1.0);
    const double S_exact =
        PI / 4.0 + 1.25 * std::asin(0.8) - 1.0;

    double x_min = 1e100, x_max = -1e100;
    double y_min = 1e100, y_max = -1e100;
    for (const auto& c : cs) {
        x_min = std::min(x_min, c.x - c.r);
        x_max = std::max(x_max, c.x + c.r);
        y_min = std::min(y_min, c.y - c.r);
        y_max = std::max(y_max, c.y + c.r);
    }
    const double wx_min = x_min, wx_max = x_max;
    const double wy_min = y_min, wy_max = y_max;

    std::mt19937_64 gen(32048717);

    std::cout.setf(std::ios::fixed);
    std::cout << std::setprecision(10);

    std::cout << "#N S_wide rel_err_wide S_narrow rel_err_narrow\n";

    for (int N = 100; N <= 100000; N += 500) {
        constexpr double ny_max = 2.0, nx_max = 2.0, ny_min = 0.8, nx_min = 0.8;;
        const double s_wide   = monte_carlo_area(cs, wx_min, wx_max, wy_min, wy_max, N, gen);
        const double s_narrow = monte_carlo_area(cs, nx_min, nx_max, ny_min, ny_max, N, gen);

        const double rel_err_wide   = std::fabs(s_wide   - S_exact) / S_exact;
        const double rel_err_narrow = std::fabs(s_narrow - S_exact) / S_exact;

        std::cout << N << " " << s_wide   << " " << rel_err_wide   << " " << s_narrow << " " << rel_err_narrow << "\n";
    }

    return 0;
}
