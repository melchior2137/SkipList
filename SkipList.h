#include <iostream> // deklaracje strumieni cout, cin, cerr
#include <cassert>  // assert()
#include <random>   // rand()
#include <cstring>  // memset()

template <typename T>
struct SkipNode
{
    T value;
    SkipNode<T> **forward;
    SkipNode(T value, int level)
    {
        this->value = value;
        // tablica przechowujaca wskazniki poszczegolnych poziomow
        forward = new SkipNode<T> *[level + 1];

        // wypelnienie 0 (nullptr)
        memset(forward, 0, sizeof(SkipNode<T> *) * (level + 1));
    }
    ~SkipNode()
    {
        delete[] forward;
    }
};

template <typename T>
class SkipList
{
    SkipNode<T> *head;

    int MAXLVL;

    float P;

    int level;

public:
    SkipList() : head(new SkipNode<T>(-1, 2)), MAXLVL(2), P(0.5){};
    SkipList(int MAXLVL, float P)
    {
        this->MAXLVL = MAXLVL;
        this->P = P;
        level = 0;

        head = new SkipNode<T>(-1, MAXLVL);
    }

    ~SkipList()
    {
        // czyszczenie wezlow
        SkipNode<T> *current = head;

        while (current != nullptr)
        {
            SkipNode<T> *next = current->forward[0];
            delete current;
            current = next;
        }
    }

    int random_level();
    void insert(const T &new_value);
    bool contains(const T &new_value);
    int remove(const T &delete_value);
    void display();
};

// losowanie poziomu dodawanego elementu
template <typename T>
int SkipList<T>::random_level()
{
    int level = 0;

    while (level < MAXLVL && (float)rand() / RAND_MAX < P)
    {
        level++;
    }
    return level;
}

template <typename T>
bool SkipList<T>::contains(const T &search_value)
{
    SkipNode<T> *current = head;

    for (int i = level; i >= 0; i--)
    {
        while (current->forward[i] != nullptr && current->forward[i]->value < search_value) 
        // jesli wartosc jest wieksza badz równa to schodzimy "nizej"
        {
            current = current->forward[i];
        }
    }
    current = current->forward[0];

    if (current != nullptr && current->value == search_value)
    {
        return true;
    }
    else
    {
        return false;
    }
}

template <typename T>
void SkipList<T>::insert(const T &new_value)
{
    SkipNode<T> *current = head;

    // tablica update w ktorej bedziemy przechowywac wskazniki do elementow których next trzeba zmienic po dodadniu elementu
    SkipNode<T> *update[MAXLVL + 1]; // MAXLVL + 1 bo indeksujemy od 0
    // wypelnienie 0 (nullptr)
    memset(update, 0, sizeof(SkipNode<T> *) * (MAXLVL + 1));

    for (int i = level; i >= 0; i--)
    {
        while (current->forward[i] != nullptr && current->forward[i]->value < new_value)
        {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];

    /* jesli w tym miejscu nie ma jeszcze tego elementu
    to go dodajemy */
    if (current == nullptr || current->value != new_value)
    {
        // losowy poziom
        int rlevel = random_level();

        // jesl wylosowany poziom jest wiekszy
        // niz aktualny poziom listy
        if (rlevel > level)
        {
            // inicjalizujemy update wskaznikami do head (bo dla nowych poziomow ustawiamy potem head.next na nasz nowo dodany element)
            for (int i = level + 1; i < rlevel + 1; i++)
                update[i] = head;

            // aktualizujemy aktualny poziom
            level = rlevel;
        }

        // tworzymy nowy element
        SkipNode<T> *n = new SkipNode<T>(new_value, rlevel);

        // wkładamy element, przepinajac wskazniki
        for (int i = 0; i <= rlevel; i++)
        {
            n->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = n;
        }
    }
}

template <typename T>
int SkipList<T>::remove(const T &delete_value)
{
    SkipNode<T> *current = head;
    SkipNode<T> *update[MAXLVL + 1];
    memset(update, 0, sizeof(SkipNode<T> *) * (MAXLVL + 1));

    int level_count = 0; // na ilu poziomach jest element ktory chcemy usunac
    for (int i = level; i >= 0; i--)
    {
        while (current->forward[i] != nullptr && current->forward[i]->value < delete_value)
        {
            current = current->forward[i];
        }
        if (current->forward[i] != nullptr && current->forward[i]->value == delete_value)
        {
            update[i] = current;
            level_count++;
        }
    }

    current = current->forward[0];

    /* jesli nie znaleziono elementu zwracamy -1 */
    if (current == nullptr || current->value != delete_value)
    {
        return -1;
    }
    // jesli znaleziono usuwamy element i przepinamy wskazniki
    // zwracamy 1 przy sukcesie
    else
    {
        for (int j = 0; j < level_count; j++)
        {
            update[j]->forward[j] = current->forward[j];
        }
        delete current;
        return 1;
    }

    // usuwanie pustych poziomów
    while (level > 0 && head->forward[level] == nullptr)
        level--;
}

template <typename T>
void SkipList<T>::display()
{
    for (int i = 0; i <= level; i++)
    {
        SkipNode<T> *x = head->forward[i];
        while (x != nullptr)
        {
            std::cout << x->value << " ";
            x = x->forward[i];
        }
        std::cout << "\n";
    }
};

