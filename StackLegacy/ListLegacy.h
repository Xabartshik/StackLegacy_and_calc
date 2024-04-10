#pragma once
//Сделал: Ошлаков Данил, ИВТ-22
#include <sstream>
#include <iostream>
#include <cassert>


using namespace std;
// Класс Node представляет собой узел двусвязного списка.
template<typename T>
class Node {
public:
    // Данные, хранящиеся в узле.
    T n_data;

    // Указатель на предыдущий узел в списке.
    Node<T>* n_prev;

    // Указатель на следующий узел в списке.
    Node<T>* n_next;

    // Конструктор по умолчанию.
    Node() : n_data(T()), n_prev(nullptr), n_next(nullptr) {}

    // Конструктор, принимающий данные.
    Node(const T& data) : n_data(data), n_prev(nullptr), n_next(nullptr) {}

    // Конструктор, принимающий данные и указатели на предыдущий и следующий узлы.
    Node(const T& data, Node<T>* prev, Node<T>* next) : n_data(data), n_prev(prev), n_next(next) {}
    //Деструктор:
    ~Node() {
        // Удаляем указатели на предыдущий и следующий узлы
        if (n_prev != nullptr)
        n_prev = nullptr;
        if (n_next != nullptr)
        n_next = nullptr;
    }
    //Конструктор копирования :

    Node(const Node& other) : n_data(other.n_data), n_prev(nullptr), n_next(nullptr) {
        // Копируем указатели на предыдущий и следующий узлы
        if (other.n_prev != nullptr) {
            n_prev = new Node(other.n_prev->n_data);
        }
        if (other.n_next != nullptr) {
            n_next = new Node(other.n_next->n_data);
        }
    }


    //Конструктор перемещения :

    Node(Node&& other) : n_data(other.n_data), n_prev(other.n_prev), n_next(other.n_next) {
        // Перемещаем указатели на предыдущий и следующий узлы
        other.n_prev = nullptr;
        other.n_next = nullptr;
    }


    //Оператор копирования :

    Node & operator=(const Node& other) {
        // Проверяем самоприсваивание
        if (this == &other) {
            return *this;
        }

        // Удаляем текущие указатели на предыдущий и следующий узлы
        delete n_prev;
        delete n_next;

        // Копируем данные и указатели из другого узла
        n_data = other.n_data;
        if (other.n_prev != nullptr) {
            n_prev = new Node(other.n_prev->n_data);
        }
        if (other.n_next != nullptr) {
            n_next = new Node(other.n_next->n_data);
        }

        return *this;
    }


    //Оператор перемещения :

    Node & operator=(Node&& other) {
        // Проверяем самоприсваивание
        if (this == &other) {
            return *this;
        }

        // Перемещаем данные и указатели из другого узла
        n_data = other.n_data;
        n_prev = other.n_prev;
        n_next = other.n_next;

        // Очищаем указатели в перемещаемом узле
        other.n_prev = nullptr;
        other.n_next = nullptr;

        return *this;
    }
};
/*
| Операция                       | Средняя сложность |
|--------------------------------|------|
| push_front                     | O(1) |
| push_back                      | O(1) |
| insert(node)                   | O(1) |
| insert (list, index)           | O(n) |
| remove                         | O(1) |
| delete_(size_t index)          | O(n) |
| delete_(size_t index, size_t n)| O(n) |
| find                           | O(n) |
| size                           | O(1) |
| is_empty                       | O(1) |
| clear                          | O(n) |

где n - размер списка.

*/
// Класс ListLegacy представляет собой реализацию двусвязного списка.
template<typename T>
class ListLegacy {
private:
    // Указатель на головной узел списка.
    Node<T>* l_head;

    // Указатель на хвостовой узел списка.
    Node<T>* l_tail;

    // Размер списка.
    size_t l_size;


    //Список отдает свои данные другому списку. Используется для того, чтобы отобрать указатели на начало и конец, без удаления данных
    void forget() {
        l_head = nullptr;
        l_tail = nullptr;
        l_size = 0;
    }
public:
    //--------------------------------------------------------------------КОНСТРУКТОРЫ И ОПЕРАТОРЫ ИЗ ПРАВИЛА ПЯТИ------------------------------------------------------
    // Конструктор по умолчанию.
    ListLegacy()
    {
        l_head = nullptr;
        l_tail = nullptr;
        l_size = 0;
    }
    // Конструктор, заполняющий список n элементами с значением value.
    ListLegacy(size_t n, const T& value) {
        for (size_t i = 0; i < n; i++) {
            push_back(value);
        }
    }


    // Конструктор копирования.
    ListLegacy(const ListLegacy& other){
        l_head = nullptr;
        l_tail = nullptr;
        l_size = 0;
        // Копируем элементы из другого списка.
        copy(other);
    }

    // Конструктор перемещения.
    ListLegacy(ListLegacy&& other){
        // Перемещаем элементы из другого списка.
        l_head = other.l_head;
        l_tail = other.l_tail;
        l_size = other.l_size;
        other.forget();
    }

    // Оператор присваивания копированием.
    ListLegacy& operator=(const ListLegacy& other) {
        if (this == &other) {
            return *this;
        }

        // Очищаем текущий список.
        clear();

        // Копируем элементы из другого списка.
        copy(other);

        return *this;
    }

    // Оператор присваивания перемещением.
    ListLegacy& operator=(ListLegacy&& other) {
        if (this == &other) {
            return *this;
        }

        // Очищаем текущий список.
        clear();

        // Перемещаем элементы из другого списка.
        l_head = other.l_head;
        l_tail = other.l_tail;
        l_size = other.l_size;

        other.forget();

        return *this;
    }

    // Конструктор, который создает список из инициализатора. (ListLegacy<size_t> numbers{ 1, 2, 3, 4, 5 };)
    ListLegacy(initializer_list<T> list) : l_head(nullptr), l_tail(nullptr), l_size(0) {
        for (const T& value : list) {
            push_back(value);
        }
    }

    // Деструктор.
    ~ListLegacy() {
        // Очищаем список.
        clear();
    }


    //Копирование списка (глубокое)
    void copy(const ListLegacy<T>& other) {
        clear();

        Node<T>* current = other.l_head;
        while (current != nullptr) {
            push_back(current->n_data);
            current = current->n_next;
        }
    }












    //----------------------------------------------------------------ВСТАВКИ И УДАЛЕНИЯ-----------------------------------------------------------------------------------------------------
    // Добавляет элемент в начало списка.
    void push_front(const T& data) {
        // Создаем новый узел.
        Node<T>* new_node = new Node<T>(data);

        // Если список пуст, то новый узел становится головным и хвостовым.
        if (l_head == nullptr) {
            l_head = new_node;
            l_tail = new_node;
        }
        else {
            // Иначе добавляем новый узел в начало списка.
            new_node->n_next = l_head;
            l_head->n_prev = new_node;
            l_head = new_node;
        }

        // Увеличиваем размер списка.
        l_size++;
    }

    // Добавляет элемент в конец списка.
    void push_back(const T& data) {
        // Создаем новый узел.
        Node<T>* new_node = new Node<T>(data);

        // Если список пуст, то новый узел становится головным и хвостовым.
        if (l_head == nullptr) {
            l_head = new_node;
            l_tail = new_node;
        }
        else {
            // Иначе добавляем новый узел в конец списка.
            l_tail->n_next = new_node;
            new_node->n_prev = l_tail;
            l_tail = new_node;
        }

        // Увеличиваем размер списка.
        l_size++;
    }

    // Удаляет последний элемент из списка. Возвращает значение по умолчание
    T pop_back() {
        if (l_tail == nullptr) {
            return T();
        }

        T value = l_tail->n_data;

        if (l_head == l_tail) {
            delete l_head;
            l_head = nullptr;
            l_tail = nullptr;
        }
        else {
            Node<T>* new_tail = l_tail->n_prev;
            delete l_tail;
            l_tail = new_tail;
            l_tail->n_next = nullptr;
        }

        l_size--;

        return value;
    }

    // Удаляет первый элемент из списка.
    T pop_front() {
        if (l_head == nullptr) {
            return T();
        }

        T value = l_head->n_data;

        if (l_head == l_tail) {
            delete l_head;
            l_head = nullptr;
            l_tail = nullptr;
        }
        else {
            Node<T>* new_head = l_head->n_next;
            delete l_head;
            l_head = new_head;
            l_head->n_prev = nullptr;
        }

        l_size--;

        return value;
    }

    // Вставляет новый узел, содержащий data, после указанного узла node.
    void insert_node(Node<T>* node, const T& data) {
        // Если указанный узел равен nullptr, то добавляем элемент в конец списка.
        if (node == nullptr) {
            push_back(data);
            return;
        }

        // Создаем новый узел.
        Node<T>* new_node = new Node<T>(data);

        // Вставляем новый узел после указанного узла.
        new_node->n_next = node->n_next;
        if (node->n_next != nullptr) {
            node->n_next->n_prev = new_node;
        }
        else {
            l_tail = new_node;
        }
        node->n_next = new_node;
        new_node->n_prev = node;

        // Увеличиваем размер списка.
        l_size++;

    }
    //Вставить элемент после index, сдвигая остальные элементы
    void insert(size_t index, const T& data) {
        if (index > l_size) {
            throw out_of_range("Invalid index");
            return;
        }

        if (index == 0) {
            push_front(data);
            return;
        }

        if (index == l_size) {
            push_back(data);
            return;
        }

        Node<T>* new_node = new Node<T>(data);

        Node<T>* current = l_head;
        for (int i = 0; i < index; i++) {
            current = current->n_next;
        }

        new_node->n_next = current->n_next;
        current->n_next->n_prev = new_node;
        current->n_next = new_node;
        new_node->n_prev = current;

        l_size++;
    }
    //Вставить список после узла index
    void insert(size_t index, ListLegacy<T>& other, bool cut = true) {
        if (index >= this->l_size) {
            throw out_of_range("Invalid index");
            return;
        }

        if (other.is_empty()) {
            return;
        }
        //Переход к индексу
        Node<T>* current = this->l_head;
        for (size_t i = 0; i < index; i++) {
            current = current->n_next;
        }
        //Передача значений
        if (cut)
        {
            other.l_tail->n_next = current->n_next;
            if (current->n_next != nullptr) {
                current->n_next->n_prev = other.l_tail;
            }
            else {
                this->l_tail = other.l_tail;
            }
            current->n_next = other.l_head;
            other.l_head->n_prev = current;

            this->l_size += other.l_size;

            other.forget();
        }
        else
        {
            ListLegacy<T> temp = other;
            temp.l_tail->n_next = current->n_next;
            if (current->n_next != nullptr) {
                current->n_next->n_prev = temp.l_tail;
            }
            else {
                this->l_tail = temp.l_tail;
            }
            current->n_next = temp.l_head;
            temp.l_head->n_prev = current;

            this->l_size += temp.l_size;

            temp.forget();
        }
    }
    //Склеивание двух списков
    void merge(ListLegacy<T>& other, bool cut = true) {
        if (other.is_empty()) {
            return;
        }

        if (this->is_empty()) {
            *this = other;
            return;
        }

        if (cut) {
        this->l_tail->n_next = other.l_head;
        other.l_head->n_prev = this->l_tail;
        this->l_tail = other.l_tail;
        this->l_size += other.l_size;

        // Если параметр cut равен true, то очищаем другой список.
        other.forget();
        }
        else
        {
            ListLegacy<T> temp = other;
            this->l_tail->n_next = temp.l_head;
            temp.l_head->n_prev = this->l_tail;
            this->l_tail = temp.l_tail;
            this->l_size += temp.l_size;


            temp.forget();
        }
    }
    //Оператор, склеивающий два списка
    ListLegacy<T> operator+(const ListLegacy<T>& other) {
        ListLegacy<T> result;
        result.merge(this, false);
        result.merge(other, false);
        return result;
    }

    // Удаляет указанный узел из списка.
    void remove(Node<T>* node) {
        // Если указанный узел равен nullptr, то ничего не делаем.
        if (node == nullptr) {
            return;
        }
        // Если указанный узел является головным узлом, то делаем следующим узлом головным.
        if (node == l_head) {
            l_head = l_head->n_next;
            if (l_head != nullptr) {
                l_head->n_prev = nullptr;
                node->n_next = nullptr;
            }
            else {
                l_tail = nullptr;
            }
        }
        else if (node == l_tail) {
            // Если указанный узел является хвостовым узлом, то делаем предыдущим узлом хвостовым.
            l_tail = l_tail->n_prev;
            if (l_tail != nullptr) {
                l_tail->n_next = nullptr;
            }
            else {
                l_head = nullptr;
            }
        }
        else {
            // Иначе удаляем указанный узел из середины списка.
            node->n_prev->n_next = node->n_next;
            node->n_next->n_prev = node->n_prev;
        }

        // Уменьшаем размер списка.
        l_size--;

        // Удаляем указанный узел.
        delete node;
    }


    // Удаляет элемент по индексу.
    void delete_(size_t index) {
        if (index < 0 || index >= l_size) {
            return;
        }

        Node<T>* current = l_head;
        for (size_t i = 0; i < index; i++) {
            current = current->n_next;
        }

        remove(current);
    }

    // Удаляет n элементов после индекса.
    void delete_(size_t index, size_t n) {
        if (index >= l_size) {
            throw out_of_range("Invalid index");
            return;
        }
        //Переход к следующему узлу (узлу по индексу)
        Node<T>* current = l_head;
        for (size_t i = 0; i < index; i++) {
            current = current->n_next;
        }

        for (size_t i = 0; i < n; i++) {
            Node<T>* next_node = current->n_next;
            remove(current);
            current = next_node;
        }
    }


    
    //Вставляет значения, заменяя их в списке
    void assign(const initializer_list<T>& list) {
        clear();

        for (const T& value : list) {
            push_back(value);
        }
    }
    //Вставляет значения, заменяя их в списке
    void assign(size_t n, const T& value) {
        clear();

        for (size_t i = 0; i < n; i++) {
            push_back(value);
        }
    }

//--------------------------------------------------------------------------------------------------Прочее
    // Находит узел со значением, равным data.
    Node<T>* find(const T& data) {
        // Ищем узел со значением, равным data.
        Node<T>* current = l_head;
        while (current != nullptr) {
            if (current->n_data == data) {
                return current;
            }
            current = current->n_next;
        }

        // Узел не найден.
        return nullptr;
    }

    // Возвращает размер списка.
    size_t size() const {
        return l_size;
    }

    // Возвращает true, если список пуст, иначе false.
    bool is_empty() const {
        return l_size == 0;
    }

    // Очищает список.
    void clear() {
        // Удаляем все узлы из списка.
        while (l_head != nullptr) {
            Node<T>* next_node = l_head->n_next;
            delete l_head;
            l_head = next_node;
        }

        forget();
    }

    //Инвертирует список
    void reverse() {
        Node<T>* current = l_head;
        Node<T>* next_node = nullptr;
        Node<T>* prev_node = nullptr;

        while (current != nullptr) {
            next_node = current->n_next;
            current->n_next = prev_node;
            current->n_prev = next_node;
            prev_node = current;
            current = next_node;
        }

        l_head = prev_node;
        l_tail = current;
    }

    // Возвращает строковое представление списка.
    string to_string() const {
        stringstream ss;
        ss << "{";

        Node<T>* current = l_head;
        while (current != nullptr) {
            ss << current->n_data;
            if (current->n_next != nullptr) {
                ss << ", ";
            }
            current = current->n_next;
        }

        ss << "}";
        return ss.str();
    }

    // Выводит список в стандартный вывод.
    void print() const {
        cout << to_string() << endl;
    }
    //Сортировка массива
    void sort() {
        if (l_size <= 1) {
            return;
        }

        Node<T>* current = l_head;
        Node<T>* next_node = nullptr;

        while (current != nullptr) {
            next_node = current->n_next;

            while (next_node != nullptr) {
                if (current->n_data > next_node->n_data) {
                    swap(current->n_data, next_node->n_data);
                }

                next_node = next_node->n_next;
            }

            current = current->n_next;
        }
    }
    //Удаление дубликатов 
    void unique() {
        Node<T>* current = l_head;
        Node<T>* next_node = nullptr;

        while (current != nullptr) {
            next_node = current->n_next;

            while (next_node != nullptr) {
                if (current->n_data == next_node->n_data) {
                    Node<T>* next_next_node = next_node->n_next;
                    remove(next_node);
                    next_node = next_next_node;
                }
                else {
                    next_node = next_node->n_next;
                }
            }

            current = current->n_next;
        }
    }
    //------------------------------------------------------------------------------------------------Геттеры и Сеттеры
    //Доступы к элементу по индексу
    T at(size_t index) const {
        if (index < 0 || index >= l_size) {
            throw std::out_of_range("Index is out of range");
        }

        Node<T>* current = l_head;
        for (size_t i = 0; i < index; i++) {
            current = current->n_next;
        }

        return current->n_data;
    }

    T& at(size_t index) {
        if (index < 0 || index >= l_size) {
            throw std::out_of_range("Index is out of range");
        }

        Node<T>* current = l_head;
        for (size_t i = 0; i < index; i++) {
            current = current->n_next;
        }

        return current->n_data;
    }

    T& operator[](size_t index) {
        return at(index);
    }
    const T operator[](size_t index) const {
        return at(index);
    }
    //Возвращает узел списка в index
    Node<T>* get_node(size_t index) const {
        if (index < 0 || index >= l_size) {
            throw std::out_of_range("Index is out of range");
        }

        Node<T>* current = l_head;
        for (size_t i = 0; i < index; i++) {
            current = current->n_next;
        }

        return current;
    }
    //Устанавливает новый узел в index, удаляя старый
    void set_node(size_t index, Node<T>* new_node) {
        if (index < 0 || index >= l_size) {
            throw std::out_of_range("Index is out of range");
        }

        Node<T>* current = l_head;
        for (size_t i = 0; i < index; i++) {
            current = current->n_next;
        }
        Node<T>* temp_prev = current->n_prev;
        // Удаляем старый узел
        remove(current);

        // Вставляем новый узел
        insert_node(temp_prev, new_node->n_data);
    }

    //Указатель на начало списка
    T& begin() {
        if (l_head == nullptr) {
            return T();
        }

        return l_head->n_data;
    }

    //Указатель на конец списка
    T& end() {
        if (l_tail == nullptr) {
            return T();
        }
        return l_tail->n_data;
    }

    // Возвращает первый элемент списка
    T first() const{
        if (l_head == nullptr) {
            return T();
        }
        return l_head->n_data;
    }

    // Возвращает последний элемент списка. Бросает исключение, если пуст
    T last() const {
        if (l_tail == nullptr) {
            return T();
        }
        return l_tail->n_data;
    }
    //--------------------------------------------------------------------------------------------------------------------------------------
    //Проверка на равенство списков
    bool operator==(const ListLegacy<T>& other) const {
        if (l_size != other.l_size) {
            return false;
        }

        Node<T>* current1 = l_head;
        Node<T>* current2 = other.l_head;

        while (current1 != nullptr) {
            if (current1->n_data != current2->n_data) {
                return false;
            }

            current1 = current1->n_next;
            current2 = current2->n_next;
        }

        return true;
    }
    //Проверка на неравенство списков
    bool operator!=(const ListLegacy<T>& other) const {
        if (l_size != other.l_size) {
            return false;
        }

        Node<T>* current1 = l_head;
        Node<T>* current2 = other.l_head;

        while (current1 != nullptr) {
            if (current1->n_data != current2->n_data) {
                return true;
            }

            current1 = current1->n_next;
            current2 = current2->n_next;
        }

        return false;
    }
};
//Тестирование класса
void test_list()
{
    // Конструктор по умолчанию
    ListLegacy<int> list1;
    assert(list1.is_empty());
    assert(list1.size() == 0);

    // Конструктор, заполняющий список n элементами с значением value
    ListLegacy<int> list2(5, 10);
    assert(!list2.is_empty());
    assert(list2.size() == 5);
    for (int i = 0; i < 5; i++) {
        assert(list2[i] == 10);
    }

    // Конструктор копирования
    ListLegacy<int> list3(list2);
    assert(!list3.is_empty());
    assert(list3.size() == 5);
    for (int i = 0; i < 5; i++) {
        assert(list3[i] == 10);
    }

    // Конструктор перемещения
    ListLegacy<int> list4(std::move(list2));
    assert(list2.is_empty());
    assert(list4.size() == 5);
    for (int i = 0; i < 5; i++) {
        assert(list4[i] == 10);
    }

    // Оператор присваивания копированием
    list1 = list3;
    assert(!list1.is_empty());
    assert(list1.size() == 5);
    for (int i = 0; i < 5; i++) {
        assert(list1[i] == 10);
    }

    // Оператор присваивания перемещением
    list1 = std::move(list3);
    assert(list3.is_empty());
    assert(list1.size() == 5);
    for (int i = 0; i < 5; i++) {
        assert(list1[i] == 10);
    }

    // Конструктор, который создает список из инициализатора
    ListLegacy<int> list5{ 1, 2, 3, 4, 5 };
    assert(!list5.is_empty());
    assert(list5.size() == 5);
    for (int i = 0; i < 5; i++) {
        assert(list5[i] == i + 1);
    }

    // Деструктор
    {
        ListLegacy<int> list6;
        list6.push_back(10);
        list6.push_back(20);
        list6.push_back(30);
    } // list6 уничтожается здесь

    // Добавление элемента в начало списка
    list1.push_front(0);
    assert(list1.size() == 6);
    assert(list1[0] == 0);

    // Добавление элемента в конец списка
    list1.push_back(6);
    assert(list1.size() == 7);
    assert(list1[6] == 6);

    // Удаление последнего элемента из списка
    assert(list1.pop_back() == 6);
    assert(list1.size() == 6);
    assert(list1[5] == 10);

    // Удаление первого элемента из списка
    assert(list1.pop_front() == 0);
    assert(list1.size() == 5);
    assert(list1[0] == 10);

    // Вставка нового узла, содержащего data, после указанного узла node
    list1.push_front(2);
    list1.insert_node(list1.find(2), 8);
    assert(list1.size() == 7);
    assert(list1[0] == 2);
    assert(list1[1] == 8);

    // Вставка элемента после index, сдвигая остальные элементы
    list1.insert(3, 4);
    assert(list1.size() == 8);
    assert(list1[4] == 4);

    // Вставка списка после узла index
    ListLegacy<int> list6{ 6, 7, 8, 9, 10 };
    list1.insert(5, list6);
    assert(list1.size() == 13);

    for (int i = 6; i < 10; i++) {
        assert(list1[i] == i);
    }

    // Склеивание двух списков
    ListLegacy<int> list7{ 13, 14, 15, 16, 17 };
    list1.merge(list7);
    assert(list1.size() == 18);
    for (int i = 13; i < 18; i++) {
        assert(list1[i] == i);
    }

    // Удаление указанного узла из списка
    list1.remove(list1.find(2));
    assert(list1.size() == 17);
    assert(list1[0] == 8);
    assert(list1[3] == 4);

    // Удаление элемента по индексу
    list1.delete_(3);
    assert(list1.size() == 16);
    assert(list1[3] == 10);
    list1.print();

    // Удаление n элементов после индекса
    list1.delete_(5, 3);
    assert(list1.size() == 13);
    list1.print();
    assert(list1[5] == 10);

    // Нахождение узла со значением, равным data
    assert(list1.find(10) == list1.get_node(1));
    cout << "All tests passed!" << endl;
}