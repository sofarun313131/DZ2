#include <bits/stdc++.h>
using namespace std;

struct Node {
    string val;
    Node* next;
    Node* prev;
    Node(const string& v): val(v), next(nullptr), prev(nullptr) {}
};
class LinkedList {
    Node* head;
    Node* tail;
    size_t cnt;
    bool doubly;
    bool circular;
public:
    LinkedList(bool isDoubly=false, bool isCircular=false)
            : head(nullptr), tail(nullptr), cnt(0), doubly(isDoubly), circular(isCircular) {}
    ~LinkedList() { clear(); }
    bool empty() const { return cnt == 0; }
    size_t size() const { return cnt; }
    string typeSymbol() const {
        if (!doubly && !circular) return ">";
        if (doubly && !circular) return "<>";
        if (!doubly && circular) return ">0";
        return "<>0";
    }
    void append(const string& value) {
        Node* n = new Node(value);
        if (empty()) {
            head = tail = n;
            if (circular) {
                head->next = head;
                if (doubly) head->prev = head;
            }
        } else {
            if (circular) {
                n->next = head;
                if (doubly) {
                    n->prev = tail;
                    tail->next = n;
                    head->prev = n;
                } else {
                    tail->next = n;
                }
                tail = n;
            } else {
                tail->next = n;
                if (doubly) n->prev = tail;
                tail = n;
            }
        }
        ++cnt;
    }
    void prepend(const string& value) {
        Node* n = new Node(value);
        if (empty()) {
            head = tail = n;
            if (circular) {
                head->next = head;
                if (doubly) head->prev = head;
            }
        } else {
            if (circular) {
                n->next = head;
                if (doubly) {
                    n->prev = tail;
                    head->prev = n;
                    tail->next = n;
                } else {
                    tail->next = n;
                }
                head = n;
            } else {
                n->next = head;
                if (doubly) head->prev = n;
                head = n;
            }
        }
        ++cnt;
    }
    void print(ostream& os = cout) const {
        if (empty()) { os << "(пустой список)\n"; return; }
        os << "Тип: " << typeSymbol() << ", количество: " << cnt << "\n";
        os << "Список: ";
        Node* cur = head;
        size_t seen = 0;
        while (cur && seen < cnt) {
            os << cur->val;
            ++seen;
            if (seen < cnt) os << " -> ";
            cur = cur->next;
        }
        os << "\n";
    }
    vector<size_t> findAll(const string& target) const {
        vector<size_t> res;
        if (empty()) return res;
        Node* cur = head;
        size_t idx = 1;
        size_t seen = 0;
        while (cur && seen < cnt) {
            if (cur->val == target) res.push_back(idx);
            ++idx;
            ++seen;
            cur = cur->next;
        }
        return res;
    }
    bool removeFirst(const string& target) {
        if (empty()) return false;
        Node* cur = head;
        Node* prev = nullptr;
        size_t seen = 0;
        while (cur && seen < cnt) {
            if (cur->val == target) {
                if (cnt == 1) {
                    delete cur;
                    head = tail = nullptr;
                    cnt = 0;
                    return true;
                }
                if (!circular) {
                    if (cur == head) {
                        head = head->next;
                        if (doubly && head) head->prev = nullptr;
                        delete cur;
                    } else {
                        if (doubly) {
                            Node* p = cur->prev;
                            Node* n = cur->next;
                            p->next = n;
                            if (n) n->prev = p;
                            if (cur == tail) tail = p;
                            delete cur;
                        } else {
                            prev->next = cur->next;
                            if (cur == tail) tail = prev;
                            delete cur;
                        }
                    }
                } else {
                    if (cur == head) {
                        Node* newHead = head->next;
                        tail->next = newHead;
                        if (doubly) newHead->prev = tail;
                        delete head;
                        head = newHead;
                    } else {
                        if (doubly) {
                            Node* p = cur->prev;
                            Node* n = cur->next;
                            p->next = n;
                            n->prev = p;
                            if (cur == tail) tail = p;
                            delete cur;
                        } else {
                            prev->next = cur->next;
                            if (cur == tail) tail = prev;
                            delete cur;
                        }
                    }
                }
                --cnt;
                return true;
            }
            if (!doubly) prev = cur;
            cur = cur->next;
            ++seen;
        }
        return false;
    }
    size_t removeAll(const string& target) {
        if (empty()) return 0;
        size_t removed = 0;
        if (!circular) {
            while (head && head->val == target) {
                Node* tmp = head;
                head = head->next;
                if (head && doubly) head->prev = nullptr;
                delete tmp;
                --cnt;
                ++removed;
                if (cnt == 0) { head = tail = nullptr; return removed; }
            }
            Node* cur = head;
            while (cur && cur->next) {
                if (cur->next->val == target) {
                    Node* toDel = cur->next;
                    cur->next = toDel->next;
                    if (doubly && cur->next) cur->next->prev = cur;
                    if (toDel == tail) tail = cur;
                    delete toDel;
                    --cnt;
                    ++removed;
                } else cur = cur->next;
            }
            return removed;
        } else {
            tail->next = nullptr;
            if (doubly) head->prev = nullptr;
            while (head && head->val == target) {
                Node* tmp = head;
                head = head->next;
                if (head && doubly) head->prev = nullptr;
                delete tmp;
                --cnt; ++removed;
                if (cnt == 0) { head = tail = nullptr; return removed; }
            }
            Node* cur = head;
            while (cur && cur->next) {
                if (cur->next->val == target) {
                    Node* toDel = cur->next;
                    cur->next = toDel->next;
                    if (doubly && cur->next) cur->next->prev = cur;
                    if (toDel == tail) tail = cur;
                    delete toDel;
                    --cnt; ++removed;
                } else cur = cur->next;
            }
            if (cnt > 0) {
                tail->next = head;
                if (doubly) head->prev = tail;
            } else {
                head = tail = nullptr;
            }
            return removed;
        }
    }
    void clear() {
        if (empty()) return;
        if (!circular) {
            Node* cur = head;
            while (cur) {
                Node* nx = cur->next;
                delete cur;
                cur = nx;
            }
            head = tail = nullptr;
            cnt = 0;
        } else {
            if (cnt == 1) {
                delete head;
                head = tail = nullptr;
                cnt = 0;
                return;
            }
            tail->next = nullptr;
            if (doubly) head->prev = nullptr;
            Node* cur = head;
            while (cur) {
                Node* nx = cur->next;
                delete cur;
                cur = nx;
            }
            head = tail = nullptr;
            cnt = 0;
        }
    }
    bool saveToFile(const string& filename) const {
        ofstream ofs(filename);
        if (!ofs) return false;
        ofs << typeSymbol() << "\n";
        Node* cur = head;
        size_t seen = 0;
        while (cur && seen < cnt) {
            ofs << cur->val;
            ++seen;
            if (seen < cnt) ofs << ",";
            cur = cur->next;
        }
        ofs << "\n";
        return true;
    }
    vector<string> toVector() const {
        vector<string> v;
        Node* cur = head;
        size_t seen = 0;
        while (cur && seen < cnt) {
            v.push_back(cur->val);
            cur = cur->next;
            ++seen;
        }
        return v;
    }
    bool isDoubly() const { return doubly; }
    bool isCircular() const { return circular; }
    void stealFrom(LinkedList* other) {
        if (!other) return;
        clear();
        head = other->head;
        tail = other->tail;
        cnt = other->cnt;
        doubly = other->doubly;
        circular = other->circular;
        // detach other without deleting nodes
        other->head = other->tail = nullptr;
        other->cnt = 0;
        other->doubly = false;
        other->circular = false;
    }
    static LinkedList* fromTypeAndElems(int t, const vector<string>& elems) {
        bool d=false, c=false;
        if (t==1){d=false;c=false;}
        else if(t==2){d=true;c=false;}
        else if(t==3){d=false;c=true;}
        else if(t==4){d=true;c=true;}
        else return nullptr;
        LinkedList* lst = new LinkedList(d,c);
        for (const auto& e: elems) lst->append(e);
        return lst;
    }
    static LinkedList* loadFromFile(const string& filename, string& error) {
        ifstream ifs(filename);
        if (!ifs) { error = "Не удалось открыть файл."; return nullptr; }
        string typeLine;
        if (!getline(ifs, typeLine)) { error = "Файл пуст или некорректен."; return nullptr; }
        auto trim = [](string s)->string {
            size_t a = s.find_first_not_of(" \t\r\n");
            if (a==string::npos) return "";
            size_t b = s.find_last_not_of(" \t\r\n");
            return s.substr(a,b-a+1);
        };
        typeLine = trim(typeLine);
        int t = -1;
        if (typeLine == ">") t = 1;
        else if (typeLine == "<>") t = 2;
        else if (typeLine == ">0") t = 3;
        else if (typeLine == "<>0") t = 4;
        else { error = "Неизвестный тип в файле: " + typeLine; return nullptr; }
        string elemsLine;
        if (!getline(ifs, elemsLine)) elemsLine = "";
        elemsLine = trim(elemsLine);
        vector<string> elems;
        if (!elemsLine.empty()) {
            string cur;
            for (char ch: elemsLine) {
                if (ch == ',') {
                    elems.push_back(trim(cur));
                    cur.clear();
                } else cur.push_back(ch);
            }
            if (!cur.empty()) elems.push_back(trim(cur));
        }
        LinkedList* res = fromTypeAndElems(t, elems);
        if (!res) error = "Ошибка создания списка из файла.";
        return res;
    }
};
static vector<string> splitByCommaPreserve(const string& s) {
    vector<string> out;
    string cur;
    for (size_t i=0;i<s.size();++i) {
        char ch = s[i];
        if (ch == ',') {
            size_t a = cur.find_first_not_of(" \t\r\n");
            if (a == string::npos) out.push_back("");
            else {
                size_t b = cur.find_last_not_of(" \t\r\n");
                out.push_back(cur.substr(a, b-a+1));
            }
            cur.clear();
        } else cur.push_back(ch);
    }
    size_t a = cur.find_first_not_of(" \t\r\n");
    if (a == string::npos) {
        if (!cur.empty() || s.empty()) out.push_back("");
    } else {
        size_t b = cur.find_last_not_of(" \t\r\n");
        out.push_back(cur.substr(a,b-a+1));
    }
    return out;
}
static string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b-a+1);
}
bool parseArgs(int argc, char* argv[], int& t_val, int& c_val, vector<string>& elems, string& err) {
    map<string, string> mp;
    for (int i=1;i<argc;++i) {
        string arg = argv[i];
        if (arg == "-t" || arg == "-c" || arg == "-e") {
            if (i+1 >= argc) { err = "Параметр " + arg + " ожидает значение."; return false; }
            mp[arg] = argv[i+1];
            ++i;
        } else { err = "Неизвестный параметр: " + arg; return false; }
    }
    if (mp.find("-t")==mp.end() || mp.find("-c")==mp.end() || mp.find("-e")==mp.end()) {
        err = "Требуются параметры -t, -c и -e."; return false;
    }
    try { t_val = stoi(mp["-t"]); } catch(...) { err = "Неверное значение для -t."; return false; }
    if (t_val < 1 || t_val > 4) { err = "Параметр -t должен быть 1..4."; return false; }
    try { c_val = stoi(mp["-c"]); } catch(...) { err = "Неверное значение для -c."; return false; }
    if (c_val < 0) { err = "Параметр -c не может быть отрицательным."; return false; }
    string raw = mp["-e"];
    elems = splitByCommaPreserve(raw);
    if (raw.empty()) elems.clear();
    if ((int)elems.size() != c_val) {
        err = "Количество элементов в -e (" + to_string(elems.size()) + ") не совпадает с -c (" + to_string(c_val) + ").";
        return false;
    }
    return true;
}
void interactiveMenu(LinkedList* list) {
    if (!list) return;
    while (true) {
        cout << "\n=== Меню ===\n";
        cout << "1) Добавить элемент в конец\n";
        cout << "2) Вывести связный список\n";
        cout << "3) Найти элемент (все позиции)\n";
        cout << "4) Удалить элемент (первое вхождение)\n";
        cout << "5) Удалить элемент (все вхождения)\n";
        cout << "6) Удалить связный список (очистить)\n";
        cout << "7) Записать связный список в файл\n";
        cout << "8) Считать связный список из файла\n";
        cout << "0) Выйти\n";
        cout << "Выберите действие: ";
        string choice;
        if (!getline(cin, choice)) break;
        if (choice.empty()) continue;
        if (choice == "0") { cout << "Выход.\n"; break; }
        if (choice == "1") {
            cout << "Введите элемент: ";
            string v; getline(cin, v); v = trim(v);
            list->append(v);
            cout << "Добавлено.\n";
            continue;
        }
        if (choice == "2") { list->print(); continue; }
        if (choice == "3") {
            cout << "Введите искомый элемент: ";
            string q; getline(cin, q); q = trim(q);
            vector<size_t> pos = list->findAll(q);
            if (pos.empty()) cout << "Не найдено.\n";
            else {
                cout << "Позиции: ";
                for (size_t i=0;i<pos.size();++i) {
                    if (i) cout << ", ";
                    cout << pos[i];
                }
                cout << "\n";
            }
            continue;
        }
        if (choice == "4") {
            cout << "Введите значение для удаления (первое): ";
            string v; getline(cin, v); v = trim(v);
            bool ok = list->removeFirst(v);
            cout << (ok ? "Удалено.\n" : "Не найдено.\n");
            continue;
        }
        if (choice == "5") {
            cout << "Введите значение для удаления (все): ";
            string v; getline(cin, v); v = trim(v);
            size_t removed = list->removeAll(v);
            cout << "Удалено: " << removed << "\n";
            continue;
        }
        if (choice == "6") {
            cout << "Подтвердите удаление всего списка (y/n): ";
            string a; getline(cin, a);
            if (!a.empty() && (a[0]=='y' || a[0]=='Y')) {
                list->clear();
                cout << "Список очищен.\n";
            } else cout << "Отмена.\n";
            continue;
        }
        if (choice == "7") {
            cout << "Введите имя файла: ";
            string fn; getline(cin, fn); fn = trim(fn);
            if (fn.empty()) { cout << "Пустое имя — отмена.\n"; continue; }
            if (list->saveToFile(fn)) cout << "Сохранено в " << fn << "\n";
            else cout << "Ошибка записи.\n";
            continue;
        }
        if (choice == "8") {
            cout << "Введите имя файла для загрузки: ";
            string fn; getline(cin, fn); fn = trim(fn);
            if (fn.empty()) { cout << "Пустое имя — отмена.\n"; continue; }
            if (!list->empty()) { cout << "Ошибка: удалите текущий список перед загрузкой.\n"; continue; }
            string err;
            LinkedList* loaded = LinkedList::loadFromFile(fn, err);
            if (!loaded) cout << "Ошибка загрузки: " << err << "\n";
            else {
                list->stealFrom(loaded);
                delete loaded;
                cout << "Список загружен из файла.\n";
            }
            continue;
        }
        cout << "Неизвестная команда.\n";
    }
}
int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t_val = 0;
    int c_val = 0;
    vector<string> elements;
    string err;
    if (!parseArgs(argc, argv, t_val, c_val, elements, err)) {
        cerr << "Ошибка парсинга: " << err << "\n";
        cerr << "Использование: " << argv[0] << " -t <1..4> -c <count> -e <elem1,elem2,...>\n";
        cerr << "Типы: 1=однонаправленный, 2=двунаправленный, 3=однонаправленный кольцевой, 4=двунаправленный кольцевой\n";
        return 1;
    }
    LinkedList* list = LinkedList::fromTypeAndElems(t_val, elements);
    if (!list) {
        cerr << "Не удалось создать список.\n";
        return 1;
    }
    cout << "Список создан. Тип: " << list->typeSymbol() << ", элементов: " << list->size() << "\n";
    interactiveMenu(list);
    delete list;
    return 0;
}
