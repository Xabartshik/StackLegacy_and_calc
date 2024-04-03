#pragma once
//Сделал Ошлаков Данил, ИВТ-22
#include <iostream>
#include "StackLegacy.h"
#include "VectorLegacy.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <functional>
#include <map>
#include <cassert>
using namespace std;
//Вставляет символ в строку, сдвигая остальные элементы в право используя мемсру для большей эффективности
string insert_symbol(string str, int& i, char c) {
    string newStr(str);  // Создать новую строку на основе исходной

    // Выделить память для нового символа
    newStr.resize(str.size() + 1);

    // Сдвинуть часть строки после позиции i вправо на один символ
    memmove(&newStr[i + 1], &newStr[i], str.size() - i);

    // Вставить символ c в позицию i
    newStr[i] = c;
    //Увеличение итератора, чтобы не зациклиться навечно (необходимо для стабильной работы кода)
    i++;

    return newStr;
}
//Нормализация строки. Заменяет унарные минусы на тильду, добавляет где нужно * (между числом и "(")
string refactor(const string& str) {
    string str_copy = str;
    for (int i = 0; i < str_copy.length(); i++) {
        if (str_copy[i] == '-' && (i == 0 || (!isdigit(str_copy[i - 1]) && (str_copy[i - 1] != ')') && ((str_copy[i - 1] != ' ') || (str_copy[i + 1] != ' '))))) {
            str_copy[i] = '~';
        }
        if (i != 0 && isdigit(str_copy[i - 1]) && str_copy[i] == '(' && str_copy[i - 1] != '*') {
            str_copy = insert_symbol(str_copy, i, '*');
        }
    }

    return str_copy;
}
//Возвращает массив чисел в виде строк. Заменяет запятую на точку для правильности работы программы
VectorLegacy<string> extractNumbers(const string& str) {
    VectorLegacy<string> result;
    string str_copy = refactor(str);
    //Паттерн числа. Может отрицательного, может дробного, может с запятой
    regex pattern("(~?\\d+(\\.\\d+)?(,\\d+)?)");
    //Константа итератора, который проходит от начала строки и до ее конца
    sregex_iterator it(str_copy.begin(), str_copy.end(), pattern);
    //Константа конца
    sregex_iterator end;

    //while (it != end) {
    //    numbers.push_back(it->str());
    //    it++;
    //}
    while (it != end) {
        string match = it->str();
        //npos == Обозначение позиции после конца строки в заголовочном файле string
        if (match.find(',') != string::npos) {
            match.replace(match.find(','), 1, ".");
        }
        if (match.find('~') != string::npos) {
            match.replace(match.find('~'), 1, "-");
        }
        result.push_back(match);
        it++;
    }
    return result;
}
//Извлекает только операторы из строки, сохраняя их как массив строк
VectorLegacy<string> extractOperators(const string& str) {
    VectorLegacy<string> result;
    //Паттерн числа. Может отрицательного, может дробного, может с запятой
    regex pattern("[~\+-/*\(\)]");
    //Константа итератора, который проходит от начала строки и до ее конца
    sregex_iterator it(str.begin(), str.end(), pattern);
    //Константа конца
    sregex_iterator end;

    //while (it != end) {
    //    numbers.push_back(it->str());
    //    it++;
    //}
    while (it != end) {
        string match = it->str();
        //npos == Обозначение позиции после конца строки в заголовочном файле string
        if (match.find('~') != string::npos) {
            match.replace(match.find('~'), 1, "-");
        }
        result.push_back(match);
        it++;
    }

    return result;
}


//Извлекает все из массива, будь то операторы, числа и прочее
VectorLegacy<string> extractEverything(const string& str) {
    VectorLegacy<string> result;

    string str_copy = refactor(str);
    //Паттерн числа. Может отрицательного, может дробного, может с запятой
    regex pattern("(~?\\d+(\\.\\d+)?(,\\d+)?)|[~+-/*\(\)\^]");
    //Константа итератора, который проходит от начала строки и до ее конца
    sregex_iterator it(str_copy.begin(), str_copy.end(), pattern);
    //Константа конца
    sregex_iterator end;

    //while (it != end) {
    //    numbers.push_back(it->str());
    //    it++;
    //}
    while (it != end) {
        string match = it->str();
        //npos == Обозначение позиции после конца строки в заголовочном файле string
        if (match.find(',') != string::npos) {
            match.replace(match.find(','), 1, ".");
        }
        if (match.find('~') != string::npos) {
            match.replace(match.find('~'), 1, "-");
        }
        result.push_back(match);
        it++;
        //cout << match << " ";
    }
    //result.print();
    return result;
}
//Проверка на число
bool isNumber(string str) {
    // Регулярное выражение для проверки числа
    regex pattern("^-?\\d+(\[\.,]\\d+)?$");

    return regex_match(str, pattern);
}

// Массив делегатов
vector<function<string(double, double)>> delegates = {
    [](double first, double second) { return to_string(first + second); },
    [](double first, double second) { return to_string(first - second); },
    [](double first, double second) { return to_string(first * second); },
    [](double first, double second) { return to_string(first / second); },
    [](double first, double second) { return to_string(pow(first, second)); }
};
// Массив строк
vector<string> keys = { "+", "-", "*", "/", "^" };
//Калькулятор, считающий в постфиксе. Принимает на вход значения в виде строки, запсианной постфиксом
double postfixCalculator(string equation) {
    //Память под стэк
    StackLegacy<string> stack;
    VectorLegacy<string> buffer = extractEverything(equation);
    double result = 0;
    double operand1 = 0, operand2 = 0;

    unsigned index = 0;

    while (index != buffer.size()) {
        string token = buffer[index];

        if (isNumber(token)) {
            stack.push(token);
        }
        else {
            operand2 = stod(stack.pop());
            if (!stack.is_empty())
                operand1 = stod(stack.pop());
            else
                operand1 = 0;
            int index_del = -1;
            for (int i = 0; i < keys.size(); i++) {
                if (keys[i] == token) {
                    index_del = i;
                    break;
                }
            }

            stack.push(delegates[index_del](operand1, operand2));
        }
        index++;
    }
    result = stod(stack.pop());
    return result;
}

// Функция для определения приоритета оператора
unsigned precedence(string op) {
    if (op == "+" || op == "-") {
        return 1;
    }
    else if (op == "*" || op == "/") {
        return 2;
    }
    else if (op == "^") {
        return 3;
    }
    else {
        return 0;
    }
}

// Функция для преобразования инфиксного выражения в постфиксное
string infixToPostfix(VectorLegacy<string> infix) {
    // Стек для хранения операторов
    StackLegacy<string> operators;
    // Строка для хранения постфиксного выражения
    string postfix;

    // Перебираем инфиксное выражение
    for (string token : infix) {
        // Если токен - операнд (число), добавляем его в постфиксное выражение
        if (isNumber(token)) {
            postfix += token + " ";
        }
        // Если токен - открывающаяся скобка, помещаем её в стек
        else if (token == "(") {
            operators.push(token);
        }
        // Если токен - закрывающаяся скобка, извлекаем операторы из стека и добавляем их в постфиксное выражение, пока не встретим открывающуюся скобку
        else if (token == ")") {
            while (!operators.is_empty() && (operators.peek() != "(")) {
                postfix += operators.pop() + " ";
            }
            operators.pop();
        }
        // Если токен - оператор, извлекаем операторы из стека с более высоким или равным приоритетом и добавляем их в постфиксное выражение, затем помещаем текущий оператор в стек
        else {
            while (!operators.is_empty() && precedence(token) <= precedence(operators.peek())) {
                postfix += operators.pop() + " ";
            }
            operators.push(token);
        }
    }

    // Извлекаем оставшиеся операторы из стека и добавляем их в постфиксное выражение
    while (!operators.is_empty()) {
        postfix += operators.pop() + " ";

    }

    // Возвращаем постфиксное выражение
    //cout << postfix << endl;
    return postfix;
}

//Калькулятор, считающий в инфиксе. Принимает на вход значения в виде строки, запсианной инфиксом. 
//Я написал переводчик в инфикс, поэтому оставить его без внимания было бы грехом.
//Да и в конце концов, это не так уж и плохо, на решении двух задач сделать третью, разве нет?
//Программист должен быть ленивым.
double infixCalculator(string equation) {
    //Строка для хранения постфикса и выполнения вычислений
    string temp = equation;
    //Разделяем ее на значения
    VectorLegacy<string> array_elements = extractEverything(temp);
    //array_elements.print();
    //Тасуем значения для получения постфикса
    temp = infixToPostfix(array_elements);
    //Вычисляем в постфиксе
    double result = postfixCalculator(temp);
    return result;
}

void test_calc() {
    // Тестирование функции extractNumbers
    initializer_list<string> test{ "1.2", "3.4", "5.6" };
    assert(extractNumbers("1,2 3.4 5,6") == VectorLegacy<string>(test));
    initializer_list<string> test2{ "-1.2", "3.4", "5.6" };
    assert(extractNumbers("-1,2 3,4 5,6") == VectorLegacy<string>(test2));
    initializer_list<string> test3{ "1", "-2", "3.4", "5.6" };
    assert(extractNumbers("1~2 3.4 5,6") == VectorLegacy<string>(test3));

    //// Тестирование функции extractOperators
    initializer_list<string> test4 = { "+", "*", "-", "/" };
    assert(extractOperators("1+2*3-4/5") == VectorLegacy<string>(test4));
    initializer_list<string> test5 = { "-", "*", "-", "/" };
    assert(extractOperators("1~2*3-4/5") == VectorLegacy<string>(VectorLegacy<string>(test5)));
    initializer_list<string> test6 = { "+", "*", "(", "-", ")", "/" };
    assert(extractOperators("1+2*(3-4)/5") == VectorLegacy<string>(test6));

    //// Тестирование функции insert_symbol
    int i = 1;
    assert(insert_symbol("123", i, '*') == "1*23");
    i=0;
    assert(insert_symbol("123", i, '*') == "*123");
    i = 3;
    assert(insert_symbol("123", i, '*') == "123*");

    //// Тестирование функции refactor
    assert(refactor("1-2*3-4/5") == "1-2*3-4/5");
    assert(refactor("1~2*3-4/5") == "1~2*3-4/5");
    assert(refactor("1+2(3-4)/5") == "1+2*(3-4)/5");

    //// Тестирование функции extractEvertything
    initializer_list<string> test7 = { "1", "+", "2", "*", "3", "-", "4", "/", "5" };
    assert(extractEverything("1+2*3-4/5") == VectorLegacy<string>(test7));
    initializer_list<string> test8 = { "1", "-2", "*", "3", "-", "4", "/", "5" };
    assert(extractEverything("1~2*3-4/5") == VectorLegacy<string>(test8));
    initializer_list<string> test9 = { "1", "+", "2", "*", "(", "3", "-", "4", ")", "/", "5" };
    assert(extractEverything("1+2*(3-4)/5") == VectorLegacy<string>(test9));

    //// Тестирование функции isNumber
    assert(isNumber("123") == true);
    assert(isNumber("12.3") == true);
    assert(isNumber("-123") == true);
    assert(isNumber("-12,3") == true);
    assert(isNumber("abc") == false);
    assert(isNumber("12~3") == false);

    //// Тестирование функции postfixCalculator
    assert(postfixCalculator("1 2 + 3 4 * +") == 15);
    assert(postfixCalculator("1 2 3 * + 4 5 * +") == 27);
    assert(postfixCalculator("1 2 3 4 5 + * + +") == 30);
    initializer_list<string> test10 = { "1", "+", "2", "*", "3", "-", "4", "/", "5" };
    //// Тестирование функции infixToPostfix
    assert(infixToPostfix(VectorLegacy<string>(test10)) == "1 2 3 * + 4 5 / - ");
    initializer_list<string> test11 = { "1", "~", "2", "*", "3", "-", "4", "/", "5" };
    assert(infixToPostfix(VectorLegacy<string>(test11)) == "1 2 3 * 4 5 / - ~ ");
    initializer_list<string> test12 = { "1", "+", "2", "*", "(", "3", "-", "4", "/", "5", ")" };
    assert(infixToPostfix(VectorLegacy<string>(test12)) == "1 2 3 4 5 / - * + ");

    //// Тестирование функции infixCalculator
    assert(infixCalculator("1+2*3-10/5") == 5);
    assert(infixCalculator("1-(2*3)-15/5") == -8);
    assert(infixCalculator("1+2*(3-4)/5") == 0.6);
    assert(infixCalculator("10/-15*10.3+3-3-(-3)") == -3.866670);
    assert(infixCalculator("23 * 50(20-10) / 5") == 2300);
    cout << "All tests passed!" << endl;
}