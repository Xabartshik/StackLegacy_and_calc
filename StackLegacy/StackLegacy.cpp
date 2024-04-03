// StackLegacy.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "StackLegacy.h"
#include "Calculator.h"
#include "VectorLegacy.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <functional>
#include <map>
//Сделал Ошлаков Данил, ИВТ-22
using namespace std;



int main()
{
    test_calc();


    //Строка постфиксная
    string str = "-30 2 / 6 - 5 -3 * +";
    //Строка инфиксная
    string str2 = "((-(-3)))";
    VectorLegacy<string> test = extractEverything(str2);
    VectorLegacy<string> characters = extractEverything(str);
    //cout << "Must be: " << endl;
    //cout << "30 2 / 6 - 5 3 * +" << endl;
    cout << "Унарные минусы: " << endl;
    cout << refactor(str2) << endl;
    cout << "Распил строки инфиксной: " << endl;
    test.print();
    cout << "Инфикс в постфикс: " << endl;
    string postfix = infixToPostfix(test);
    cout << postfix << endl;
    cout << "Распил строки постфиксной: " << endl;
    characters.print();
    cout << "Распил строки постфиксной преобразованной из инфикса: " << endl;
    test = extractEverything(postfix);
    test.print();
    cout << "Строка постфиксная введенная: " << endl;
    cout << str << endl;
    cout << "Результат постфикса введенного: " << endl;
    cout << postfixCalculator(str) << endl;
    cout << "Результат функция infixCalculator(): " << endl;
    cout << infixCalculator(str2) << endl;
    cout << "Результат постфикса преобразованного: " << endl;
    cout << postfixCalculator(postfix);
    return 0;
    StackLegacy<double> stack;
    stack.push(2);
    stack.push(3);
    stack.push(4);
    cout << stack.peek() << endl;
    cout << stack.size() << endl;
    test_stack();
}


