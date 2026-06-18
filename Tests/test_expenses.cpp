#include "../include/expenses.h"
#include <cassert>
#include <iostream>
#include <tuple>

// Esta función prueba que la estrategia de ordenamiento funcione correctamente
// organizando los gastos según su monto en orden descendente (de mayor a menor).
void test_strategy_sort_by_amount_descending() {
    ExpenseList expenses{
        {"2026-06-01", "food", "lunch", 18.50},
        {"2026-06-02", "transport", "bus", 3.20},
        {"2026-06-03", "books", "cpp", 45.00}
    };

    auto by_amount = [](const Expense& a, const Expense& b) {
        return a.amount > b.amount;
    };

    sort_with(expenses, by_amount);

    assert(expenses.front().amount == 45.00);
    assert(expenses.back().amount == 3.20);
}

// Esta función verifica que los gastos se puedan ordenar usando un criterio compuesto:
// primero agrupa alfabéticamente por categoría y, en caso de empate, ordena por fecha.
void test_strategy_sort_by_category_then_date() {
    ExpenseList expenses{
        {"2026-06-03", "transport", "bus", 3.20},
        {"2026-06-01", "books", "cpp", 45.00},
        {"2026-06-02", "books", "algorithms", 38.00}
    };

    auto by_category_then_date = [](const Expense& a, const Expense& b) {
        return std::tie(a.category, a.date) < std::tie(b.category, b.date);
    };

    sort_with(expenses, by_category_then_date);

    assert(expenses[0].category == "books");
    assert(expenses[0].date == "2026-06-01");
    assert(expenses[2].category == "transport");
}

// Esta función comprueba que el patrón Factory logre instanciar un exportador CSV
// y que la cadena generada contenga datos en el formato correcto separado por comas.
void test_factory_create_csv_exporter() {
    ExpenseList expenses{
        {"2026-06-01", "food", "lunch", 18.50},
        {"2026-06-02", "transport", "bus", 3.20}
    };

    auto exporter = make_exporter<CsvExporter>();
    auto output = exporter(expenses);

    assert(output.find("food") != std::string::npos);
    assert(output.find("transport") != std::string::npos);
}

// Esta función evalúa la creación de un exportador JSON mediante la fábrica de funciones,
// asegurando que las claves y la estructura representativa de un JSON estén presentes.
void test_factory_create_json_exporter() {
    ExpenseList expenses{
        {"2026-06-01", "food", "lunch", 18.50}
    };

    auto exporter = make_exporter<JsonExporter>();
    auto output = exporter(expenses);

    assert(output.find("\"category\"") != std::string::npos);
    assert(output.find("\"food\"") != std::string::npos);
    assert(output.find("\"amount\"") != std::string::npos);
}

// Esta prueba asegura que el decorador de auditoría cumpla su labor de agregar
// el rastro de auditoría sin modificar ni ocultar la exportación del CSV subyacente.
void test_decorator_audit_preserves_inner_export() {
    ExpenseList expenses{
        {"2026-06-01", "food", "lunch", 18.50},
        {"2026-06-02", "transport", "bus", 3.20}
    };

    AuditedExporter exporter{CsvExporter{}};
    auto output = exporter.export_expenses(expenses);

    assert(output.find("transport") != std::string::npos);
    assert(output.find("audit") != std::string::npos);
}

// Esta función verifica que el decorador de resumen calcule eficientemente el
// sumatorio de todos los montos de la lista y lo concatene al final de la exportación.
void test_decorator_summary_adds_total() {
    ExpenseList expenses{
        {"2026-06-01", "food", "lunch", 18.50},
        {"2026-06-02", "transport", "bus", 3.20}
    };

    SummaryExporter exporter{TextExporter{}};
    auto output = exporter.export_expenses(expenses);

    assert(output.find("food") != std::string::npos);
    assert(output.find("total") != std::string::npos);
    assert(output.find("21.7") != std::string::npos || output.find("21.70") != std::string::npos);
}

// Esta prueba de integración evalúa el funcionamiento global del sistema:
// ordena el listado, y luego procesa los datos pasando por los decoradores hacia el CSV final.
void test_integration_export_sorted_expenses() {
    ExpenseList expenses{
        {"2026-06-01", "food", "lunch", 18.50},
        {"2026-06-02", "transport", "bus", 3.20},
        {"2026-06-03", "books", "cpp", 45.00}
    };

    auto by_amount = [](const Expense& a, const Expense& b) {
        return a.amount > b.amount;
    };

    sort_with(expenses, by_amount);

    auto exporter = AuditedExporter{
        SummaryExporter{
            CsvExporter{}
        }
    };

    auto output = exporter.export_expenses(expenses);

    assert(expenses.front().amount == 45.00);
    assert(output.find("total") != std::string::npos);
    assert(output.find("audit") != std::string::npos);
}


int main() {
    test_strategy_sort_by_amount_descending();
    test_strategy_sort_by_category_then_date();
    test_factory_create_csv_exporter();
    test_factory_create_json_exporter();
    test_decorator_audit_preserves_inner_export();
    test_decorator_summary_adds_total();
    test_integration_export_sorted_expenses();

    std::cout << "Todas las pruebas pasaron correctamente." << std::endl;
    return 0;
}