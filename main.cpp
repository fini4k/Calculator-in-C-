#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <cmath>
#include <stdexcept>
#include <map>

using namespace std;

map<string, double> variables; //Хранение элементов переменных

//Вычисление приоритета математических операций
int getPriority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3; 
    return 0;
}

// Выполнение операций
double applyOperation(double a, double b, char op) {
    switch (op) {
    case '+': return a + b;                                     // Сумма
    case '-': return a - b;                                    // Разность
    case '*': return a * b;                                   // Умножение
    case '/':                                                // Деление
        if (b == 0) throw runtime_error("Деление на ноль"); // Запрет деления на ноль
        return a / b;
    case '^': return pow(a, b);
    default: throw runtime_error("Неверный оператор"); // Ошибка неверных операторов
    }
}

// Вычисление математических функций
double applyFunction(const string& func, double value) {
    if (func == "sin") return sin(value);                     // Синус
    if (func == "cos") return cos(value);                    // Косинус
    if (func == "tan") return tan(value);                   // Тангенс
    if (func == "log") return log(value);                  // Логорифм
    if (func == "sqrt") return sqrt(value);               // Уравнения
    throw runtime_error("Неизвестная функция: " + func); // Ошибка неизвесной функции
}

// Вычисление выражений
double evaluateExpression(const string& expression) {
    stack<double> values;  // Стек чисел
    stack<char> ops;      // Стек операторов

    for (int i = 0; i < expression.length(); ++i) {
        char current = expression[i];

       
        if (isspace(current)) continue; // Игнорирование пробелов

        // Обработка чисел
        if (isdigit(current) || current == '.') {
            double value = 0;
            int decimalPoint = -1;
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                if (expression[i] == '.') {
                    decimalPoint = 0;
                }
                else {
                    if (decimalPoint == -1) {
                        value = value * 10 + (expression[i] - '0');
                    }
                    else {
                        value = value + (expression[i] - '0') * pow(10, decimalPoint);
                        decimalPoint++;
                    }
                }
                i++;
            }
            i--;
            values.push(value);
        }
        // Обработка скобок
        else if (current == '(') {
            ops.push(current);
        }
        else if (current == ')') {
            while (!ops.empty() && ops.top() != '(') {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOperation(a, b, op));
            }
            ops.pop(); 
        }
        
        else if (current == '+' || current == '-' || current == '*' || current == '/' || current == '^') {
            while (!ops.empty() && getPriority(ops.top()) >= getPriority(current)) {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOperation(a, b, op));
            }
            ops.push(current);
        }
        // Обработка математических функций
        else if (current == 's' || current == 'c' || current == 't' || current == 'l') {
            string func;
            if (current == 's') func = "sin";
            else if (current == 'c') func = "cos";
            else if (current == 't') func = "tan";
            else if (current == 'l') func = "log";
            i += func.size() - 1;
            double value = evaluateExpression(expression.substr(i + 1));
            values.push(applyFunction(func, value));
        }
    }

    // Обработка оставшихся операций
    while (!ops.empty()) {
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOperation(a, b, op));
    }

    return values.top();
}
// Функция для решения уравнений
void solveEquation(const string& equation) {
    // Линейные уравнения
    double a = 0, b = 0;
    size_t xPos = equation.find("x");

    if (xPos == string::npos) {
        cout << "Неверный формат уравнения!" << endl;
        return;
    }

    
    size_t equalPos = equation.find('=');
    if (equalPos == string::npos) {
        cout << "Неверный формат уравнения!" << endl;
        return;
    }

    string left = equation.substr(0, equalPos);
    string right = equation.substr(equalPos + 1);

    
    a = stod(left.substr(0, xPos)); 
    b = stod(right); 
    
    if (a == 0) {
        if (b == 0) {
            cout << "Бесконечно много решений!" << endl;
        }
        else {
            cout << "Нет решения!" << endl;
        }
    }
    else {
        double x = -b / a;
        cout << "Решение: x = " << x << endl;
    }
}
// /help
void displayHelp() {
    cout << "          \n";
    cout << "|          Команды калькулятора:\n";
    cout << "|          /help - Показать это сообщение\n";
    cout << "|          Основные операции: +, -, *, /, ^ (для возведения в степень)\n";
    cout << "|          Математические функции: sin, cos, tan, log, sqrt\n";
    cout << "|          Вы можете задавать переменные: x = 10\n";
    cout << "|          Используйте скобки для определения порядка операций.\n";
    cout << "|          Для решения уравнений типа '2x+3=0' используйте команду 'solve'.\n";
    cout << "|          Пример: solve 2x+3=0\n";
    cout << "          \n";
}

// Заключение в цикл
int main() {
    string expression;

    cout << "Простой калькулятор" << endl;

    while (true) {
        cout << "Введите выражение или введите '/help' для справки: ";
        getline(cin, expression);

        if (expression == "/help") {
            displayHelp();
        }
        else if (expression.find("solve") != string::npos) {
            solveEquation(expression.substr(5));
        }
        else if (expression == "exit") {
            cout << "Программа завершена." << endl;
            break; 
        }
        else {
            try {
                double result = evaluateExpression(expression);
                cout << "          \n";
                cout << "Результат: " << result << endl;
                cout << "          \n";

            }
            catch (const exception& e) {
                cout << "Ошибка: " << e.what() << endl;
            }
        }
    }

    return 0;
}








/*
███████╗██╗███╗░░██╗██╗░░██╗██╗██╗░░██╗
██╔════╝██║████╗░██║██║░██╔╝██║██║░██╔╝
█████╗░░██║██╔██╗██║██║██╔╝░██║█████═╝░
██╔══╝░░██║██║╚████║██║███████║██╔═██╗░
██║░░░░░██║██║░╚███║██║╚════██║██║░╚██╗
╚═╝░░░░░╚═╝╚═╝░░╚══╝╚═╝░░░░░╚═╝╚═╝░░╚═╝*/
