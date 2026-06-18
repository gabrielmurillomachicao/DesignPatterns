//
// Created by Gabriel on 18/06/2026.
//

#ifndef EC_PATRONES_DE_DISENO_EXPENSE_H
#define EC_PATRONES_DE_DISENO_EXPENSE_H

#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <sstream>
#include <concepts>
//Esto es una prueba
struct Expense {
    std::string date;
    std::string category;
    std::string detail;
    double amount{};
};

using ExpenseList = std::vector<Expense>;

template<class T>
concept ExpenseExporter =
    requires(T exporter, const ExpenseList& expenses)
{
    { exporter.export_expenses(expenses) } -> std::same_as<std::string>;
};

using ExportFunction = std::function<std::string(const ExpenseList&)>;

struct CsvExporter {
    std::string export_expenses(const ExpenseList& expenses) const {
        std::ostringstream oss;
        for (const auto& e : expenses) {
            oss << e.date << "," << e.category << "," << e.detail << "," << e.amount << "\n";
        }
        return oss.str();
    }
};

struct JsonExporter {
    std::string export_expenses(const ExpenseList& expenses) const {
        std::ostringstream oss;
        oss << "[\n";
        for (size_t i = 0; i < expenses.size(); ++i) {
            oss << "  {\n"
                << "    \"date\": \"" << expenses[i].date << "\",\n"
                << "    \"category\": \"" << expenses[i].category << "\",\n"
                << "    \"description\": \"" << expenses[i].detail << "\",\n"
                << "    \"amount\": " << expenses[i].amount << "\n"
                << "  }" << (i == expenses.size() - 1 ? "" : ",") << "\n";
        }
        oss << "]";
        return oss.str();
    }
};

struct TextExporter {
    std::string export_expenses(const ExpenseList& expenses) const {
        std::ostringstream oss;
        for (const auto& e : expenses) {
            oss << e.date << " | " << e.category << " | " << e.amount << "\n";
        }
        return oss.str();
    }
};

template<ExpenseExporter E, class... Args>
ExportFunction make_exporter(Args&&... args) {
    return [args...](const ExpenseList& expenses) -> std::string {
        E exporter{args...};
        return exporter.export_expenses(expenses);
    };
}

template<class R, class Cmp>
concept SortStrategy =
    std::ranges::random_access_range<R> &&
    std::sortable<std::ranges::iterator_t<R>, Cmp>;

void sort_with(auto& values, auto cmp)
requires SortStrategy<decltype(values), decltype(cmp)> {
    std::ranges::sort(values, cmp);
}

template<ExpenseExporter Inner>
struct AuditedExporter {
    Inner inner;
    std::string export_expenses(const ExpenseList& expenses) const {
        return inner.export_expenses(expenses) + "\n[audit] exported at ...";
    }
};

template<ExpenseExporter Inner>
struct SummaryExporter {
    Inner inner;
    std::string export_expenses(const ExpenseList& expenses) const {
        double total = 0;
        for (const auto& e : expenses) {
            total += e.amount;
        }
        return inner.export_expenses(expenses) + "\ntotal: " + std::to_string(total);
    }
};

#endif //EC_PATRONES_DE_DISENO_EXPENSE_H