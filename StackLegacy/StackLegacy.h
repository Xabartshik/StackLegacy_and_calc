#pragma once
//Сделал Ошлаков Данил, ИВТ-22
#include "ListLegacy.h"
#include "VectorLegacy.h"
template<typename T>
class StackLegacy :
    private ListLegacy<T>
{
    private:
        //Для хранения используется список
        ListLegacy<T> list;

    public:
        //Конструктор создания стека со списком
        StackLegacy(const initializer_list<T>& list) : list(list) {}
        //Конструктор создания пустого стека
        StackLegacy() : list() {}

        // Конструктор копирования
        StackLegacy(const StackLegacy<T>& other) : list(other.list) {}

        // Конструктор перемещения
        StackLegacy(StackLegacy<T>&& other) : list(move(other.list)) {}

        // Оператор присваивания
        StackLegacy<T>& operator=(const StackLegacy<T>& other) {
            if (this == &other) {
                return *this;
            }

            list = other.list;

            return *this;
        }

        // Оператор присваивания перемещения
        StackLegacy<T>& operator=(StackLegacy<T>&& other) {
            if (this == &other) {
                return *this;
            }

            list = move(other.list);

            return *this;
        }

        // Оператор сравнения
        bool operator==(const StackLegacy<T>& other) {
            return list == other.list;
        }

        //Оператор вставки значения
        void push(const T& data) {
            list.push_back(data);
        }
        //Оператор удаления значения
        T pop() {
            return list.pop_back();
        }
        //Возвращает последний добавленный элемент
        T peek() const {
            return list.last();
        }

        //Возвращает первый добавленный элемент
        T root() const {
            return list.first();
        }
        //Возвращает пустоту стека
        bool is_empty() const {
            return list.is_empty();
        }
        //Возвращает размер стека
        size_t size() const {
            return list.size();
        }




};
//Функция тестирования класса
void test_stack() {
    // Создаем стек и проверяем, что он пуст
    StackLegacy<int> stack;
    assert(stack.is_empty());

    // Добавляем несколько элементов в стек и проверяем их наличие
    stack.push(1);
    assert(!stack.is_empty());
    assert(stack.peek() == 1);

    stack.push(2);
    assert(!stack.is_empty());
    assert(stack.peek() == 2);

    stack.push(3);
    assert(!stack.is_empty());
    assert(stack.peek() == 3);

    // Удаляем элементы из стека и проверяем их правильность
    assert(stack.pop() == 3);
    assert(!stack.is_empty());
    assert(stack.peek() == 2);

    assert(stack.pop() == 2);
    assert(!stack.is_empty());
    assert(stack.peek() == 1);

    assert(stack.pop() == 1);
    assert(stack.is_empty());

    // Проверяем, что стек пуст после удаления всех элементов
    assert(stack.is_empty());

    // Создаем новый стек строк и проверяем его работу
    StackLegacy<string> stack2;
    assert(stack2.is_empty());

    stack2.push("Hello");
    assert(!stack2.is_empty());
    assert(stack2.peek() == "Hello");

    stack2.push("World");
    assert(!stack2.is_empty());
    assert(stack2.peek() == "World");

    assert(stack2.pop() == "World");
    assert(!stack2.is_empty());
    assert(stack2.peek() == "Hello");

    assert(stack2.pop() == "Hello");
    assert(stack2.is_empty());

    cout << "All tests passed!" << endl;
}
template<typename T>
//Поместьить значения в стэк
void push_to_stack(StackLegacy<T>& stack, VectorLegacy<T>& vector) {
    for (unsigned i = 0; i < vector.size(); i++) {
        stack.push(vector[i]);
    }
}
