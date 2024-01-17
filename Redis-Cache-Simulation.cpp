#include <iostream>
#include <unordered_map>
using namespace std;

class Node1
{
public:
    int key;
    int data;
    Node1 *next;
    Node1 *prev;

    Node1(int key, int data)
    {
        this->key = key;
        this->data = data;
        this->next = NULL;
        this->prev = NULL;
    }
};

class Node2
{
public:
    int key;
    int data;
    Node2 *right;
    Node2 *left;

    Node2(int key, int data)
    {
        this->key = key;
        this->data = data;
        right = NULL;
        left = NULL;
    }
};

class Node3
{
public:
    int key;
    int val;
    Node3 *next;

    Node3(int key, int val)
    {
        this->key = key;
        this->val = val;
        next = NULL;
    }

    ~Node3()
    {
        if (next != NULL)
        {
            delete next;
        }
    }
};

class LRU_LL
{
public:
    Node1 *head;
    Node1 *tail;
    int capacity;
    unordered_map<int, Node1 *> map;

    LRU_LL(int capacity)
    {
        this->capacity = capacity;
        head = NULL;
        tail = NULL;
    }

    // Move the given node to the head of the linked list
    void MovetoHead(Node1 *temp)
    {
        // Remove the node from its current position
        if (temp->prev)
        {
            temp->prev->next = temp->next;
        }
        else
        {
            // Node is the head, update the head
            head = temp->next;
        }

        if (temp->next)
        {
            temp->next->prev = temp->prev;
        }
        else
        {
            // Node is the tail, update the tail
            tail = temp->prev;
        }

        // Move the node to the head
        temp->prev = NULL;
        temp->next = head;
        if (head)
        {
            head->prev = temp;
        }
        head = temp;

        // If the node was the tail, update the tail
        if (!tail)
        {
            tail = temp;
        }
    }

    // Get the value associated with the given key from the cache
    int Get(int key)
    {
        if (map.find(key) != map.end())
        {
            // Key found, move the corresponding node to the head
            Node1 *node = map[key];
            MovetoHead(node);
            return node->data;
        }
        return -1; // Key not found
    }

    // Put a key-value pair into the cache
    void Put(int key, int value)
    {
        if (map.find(key) != map.end())
        {
            // Key already exists, update the value and move the node to the head
            Node1 *temp = map[key];
            temp->data = value;
            MovetoHead(temp);
        }
        else
        {
            if (map.size() >= capacity)
            {
                // Cache is full, remove the tail node
                int tailKey = tail->key;
                map.erase(tailKey);

                Node1 *newTail = tail->prev;
                if (newTail)
                {
                    newTail->next = NULL;
                }
                delete tail;
                tail = newTail;
            }

            // Create a new node with the given key and value
            Node1 *temp = new Node1(key, value);
            map[key] = temp;

            // Insert the new node at the head of the linked list
            temp->next = head;
            if (head)
            {
                head->prev = temp;
            }
            head = temp;

            if (!tail)
            {
                tail = head;
            }
        }
    }

    // Remove the node with the given key from the cache
    void Remove(int key)
    {
        if (map.find(key) != map.end())
        {
            Node1 *temp = map[key];

            // Remove the node from the linked list
            if (temp->prev)
            {
                temp->prev->next = temp->next;
            }
            else
            {
                // Node is the head, update the head
                head = temp->next;
            }

            if (temp->next)
            {
                temp->next->prev = temp->prev;
            }
            else
            {
                // Node is the tail, update the tail
                tail = temp->prev;
            }

            map.erase(key);
            delete temp;
        }
    }

    // Display the key-value pairs in the cache
    void Display() const
    {
        cout << "LRU Cache Map:" << endl;
        for (const auto &entry : map)
        {
            cout << entry.first << ": " << entry.second->data << endl;
        }
        cout << "-----------------" << endl;
    }
};

class LRU_BST
{
public:
    int capacity;
    Node2 *root;

    LRU_BST(int capacity)
    {
        this->capacity = capacity;
        root = NULL;
    }

    // Insert a new node with key and data into the BST

    Node2 *Insert(Node2 *root, int k, int d)
    {
        if (root == NULL)
        {
            return new Node2(k, d);
        }

        if (k < root->key)
        {
            root->left = Insert(root->left, k, d);
        }

        else if (k > root->key)
        {
            root->right = Insert(root->right, k, d);
        }

        return root;
    }

    // Find the node with the minimum key in the BST

    Node2 *findMin(Node2 *root)
    {
        if (root == NULL)
        {
            return NULL;
        }

        while (root->left != NULL)
        {
            root = root->left;
        }

        return root;
    }

    // Delete a node with the given key from the BST

    Node2 *Delete(Node2 *root, int k)
    {
        if (root == NULL)
        {
            return root;
        }

        if (k < root->key)
        {
            root->left = Delete(root->left, k);
        }
        else if (k > root->key)
        {
            root->right = Delete(root->right, k);
        }
        else
        {
            if (root->left == NULL)
            {
                Node2 *temp = root->right;
                delete root;
                return temp;
            }
            else if (root->right == NULL)
            {
                Node2 *temp = root->left;
                delete root;
                return temp;
            }

            Node2 *temp = findMin(root->right);
            root->key = temp->key;
            root->data = temp->data;
            root->right = Delete(root->right, temp->key);
        }
        return root;
    }

    // Search for a node with the given key in the BST

    Node2 *Search(Node2 *root, int key)
    {
        while (root != NULL)
        {
            if (key == root->key)
            {
                return root;
            }

            else if (key < root->key)
            {
                root = root->left;
            }
            else
            {
                root = root->right;
            }
        }

        return NULL;
    }

    // Return the number of nodes in the BST

    int cachemap_Size(Node2 *root)
    {
        if (root == NULL)
        {
            return 0;
        }

        return cachemap_Size(root->left) + cachemap_Size(root->right) + 1;
    }

    // Check if the BST contains a node with the given key

    bool Contains_Key(Node2 *root, int key)
    {
        return Search(root, key) != NULL;
    }

    Node2 *RotateRight(Node2 *y)
    {
        Node2 *x = y->left;
        y->left = x->right;
        x->right = y;
        return x;
    }

    Node2 *RotateLeft(Node2 *x)
    {
        Node2 *y = x->right;
        x->right = y->left;
        y->left = x;
        return y;
    }

    // Move the node with the specified key to the root of the BST

    Node2 *MovetoRoot(Node2 *root, int key)
    {
        if (root == NULL || root->data == key)
        {
            return root;
        }

        if (key < root->data)
        {
            root->left = MovetoRoot(root->left, key);
            return RotateRight(root);
        }

        else
        {
            root->right = MovetoRoot(root->right, key);
            return RotateLeft(root);
        }
    }

    // Insert a key-value pair into the BST or update the value if the key already exists
    void Put(int key, int val)
    {
        if (Contains_Key(root, key))
        {
            Search(root, key)->data = val;
            MovetoRoot(root, key);
        }

        else
        {
            if (cachemap_Size(root) >= capacity)
            {
                int Min_Key = findMin(root)->key;
                root = Delete(root, Min_Key);
            }

            root = Insert(root, key, val);
        }
    }

    void Evict(int key)
    {
        if (Contains_Key(root, key))
        {
            root = Delete(root, key);
        }
    }

    void Display(Node2 *root)
    {
        if (root == NULL)
        {
            return;
        }

        Display(root->left);

        cout << "( Key : " << root->key << " , Data : " << root->data << " )"
             << " ";

        Display(root->right);
    }

    void Print()
    {
        Display(root);
        cout << endl;
    }
};

// Hash_Map class for implementing a simple hash map using chaining
class Hash_Map
{
public:
    static const int mapSize = 10;

    Node3 *map[mapSize];

    Hash_Map()
    {
        // Initialize the hash map with NULL pointers
        for (int i = 0; i < mapSize; i++)
        {
            map[i] = NULL;
        }
    }

    // Hash function to determine the index for a given key
    int Hash_Function(int key)
    {
        return key % mapSize;
    }

    // Insert a key-value pair into the hash map
    void Insert(int key, int data)
    {
        int index = Hash_Function(key);

        // Create a new node and insert it at the beginning of the list at the calculated index
        Node3 *n1 = new Node3(key, data);
        n1->next = map[index];
        map[index] = n1;
    }

    // Search for a key-value pair in the hash map
    bool Search(int key, int data)
    {
        int index = Hash_Function(key);

        // Traverse the linked list at the calculated index to find the key-value pair
        Node3 *current = map[index];

        while (current != NULL)
        {
            if (current->key == key && current->val == data)
            {
                return true; // Key-value pair found
            }
            current = current->next;
        }

        return false; // Key-value pair not found
    }

    // Find and return the node with the specified key in the hash map
    Node3 *find(int key)
    {
        int index = Hash_Function(key);

        // Traverse the linked list at the calculated index to find the node with the specified key
        Node3 *curr = map[index];

        while (curr != NULL)
        {
            if (curr->key == key)
            {
                return curr; // Node with the specified key found
            }
            curr = curr->next;
        }

        return NULL; // Node with the specified key not found
    }

    // Remove all nodes with the specified key from the hash map
    void remove(int key)
    {
        int index = Hash_Function(key);

        // Traverse the linked list at the calculated index to remove all nodes with the specified key
        Node3 *current = map[index];
        Node3 *nextNode;

        while (current != NULL)
        {
            nextNode = current->next;

            current->next = NULL; // Disconnect the node from the list to prevent deleting the whole list

            delete current;       // Delete the current node

            current = nextNode; // Move to the next node in the list
        }

        map[index] = NULL; // Ensure the list head at the map index is set to NULL
    }

    // Display the linked list at the specified index in the hash map
    void Display(int key)
    {
        int index = Hash_Function(key);

        // Traverse and display the linked list at the calculated index
        Node3 *curr = map[index];

        while (curr != NULL)
        {
            cout << "Key : " << curr->key << ", data : " << curr->val << " -> ";
            curr = curr->next;
        }

        cout << endl;
    }

    // Display the entire hash map with index-wise linked list contents
    void Display_HashMap()
    {
        for (int i = 0; i < mapSize; ++i)
        {
            cout << "Index " << i << " : ";

            Node3 *curr = map[i];

            while (curr != NULL)
            {
                cout << "Key : " << curr->key << ", data : " << curr->val << " -> ";
                curr = curr->next;
            }
            cout << "NULL" << endl;
        }
    }
};

// LRU_HM class for implementing an LRU cache using a hash map
class LRU_HM
{
public:
    int capacity;
    int recent;
    Hash_Map cachemap;

    LRU_HM(int capacity)
    {
        this->capacity = capacity;
        recent = 0;
    }

    // Evict the least recently used entry from the cache
    void Evict()
    {
        if (capacity > 0)
        {
            if (cachemap.find(recent) != NULL)
            {
                cachemap.remove(recent);
                --capacity;
            }
        }
    }

    // Get the value associated with the given key from the cache
    int Get(int key)
    {
        Node3 *found = cachemap.find(key);

        if (found != NULL)
        {
            recent = key;
            return found->val;
        }
        else
        {
            return -1; // Key not found in the cache
        }
    }

    // Put a key-value pair into the cache or update the value if the key already exists
    void Put(int key, int val)
    {
        if (capacity == 0)
        {
            return; // Cache is disabled (capacity is 0)
        }

        Node3 *exist = cachemap.find(key);

        if (exist == NULL)
        {
            // Insert a new key-value pair into the cache and update the recent key
            cachemap.Insert(key, val);
            recent = key;

            // Evict the least recently used entry if the cache is full
            if (cachemap.find(recent) != NULL && capacity == 1)
            {
                Evict();
            }
        }
        else
        {
            // Update the value of an existing key and update the recent key
            exist->val = val;
            recent = key;
        }
    }

    // Insert a linked list into the cache starting with the given key
    void Insert_List(int key, Node3 *List)
    {
        if (capacity == 0)
        {
            return; // Cache is disabled (capacity is 0)
        }

        Node3 *curr = cachemap.find(key);

        if (curr == NULL)
        {
            // Insert the entire linked list into the cache starting with the given key
            cachemap.Insert(key, List->val);
            recent = key;

            // Evict the least recently used entry if the cache is full
            if (cachemap.find(recent) != NULL && capacity == 1)
            {
                Evict();
            }

            Node3 *temp = List->next;

            // Insert the remaining nodes of the linked list into the cache
            while (temp != NULL)
            {
                cachemap.Insert(key, temp->val);
                temp = temp->next;
            }
        }
    }

    // Remove a key-value pair from the cache with the specified key
    void Remove(int key)
    {
        cachemap.remove(key);
    }

    // Display the linked list at the specified key in the cache
    void display(int key)
    {
        cachemap.Display(key);
    }

    // Display the entire cache with index-wise linked list contents
    void Display()
    {
        cachemap.Display_HashMap();
    }
};

int main()
{
    int x;

    cout << " *********    **********    *********      *********    ***********" << endl;
    cout << " ***    **    **********    **********     *********    ***********" << endl;
    cout << " ***    **    ***           ***      **        *        ***      **" << endl;
    cout << " *********    **********    ***       **       *           ***     " << endl;
    cout << " ********     **********    ***       **       *              ***  " << endl;
    cout << " ***   ***    ***           ***      **        *        **       **" << endl;
    cout << " ***    ***   **********    **********     *********    ***********" << endl;
    cout << " ***     ***  **********    *********      *********    ***********" << endl;

    cout << "\n\n REMOTE                     DICTIONARY                   SERVER\n\n" << endl;

    do
    {
        cout << "-----------------------------------\n";
        cout << "|             Main Menu           |\n";
        cout << "-----------------------------------\n";
        cout << "| 1. Linked List Data             |\n";
        cout << "| 2. Binary Search Tree Data      |\n";
        cout << "| 3. Hash Map Data                |\n";
        cout << "| 0. Exit                         |\n";
        cout << "-----------------------------------\n";
        cout << "Enter your choice : " << endl;
        cin >> x;

        if (x == 1)
        {
            int y;

            cout << "Enter the size of the cache : " << endl;
            cin >> y;

            LRU_LL cache1(y);

            cout << "Input Data " << endl;

            int a, b,c;

            cout << "Enter the size of data : " << endl;
            cin >> a;

            for (int i = 0; i < a; i++)
            {
                cout << "Enter the Key : " << endl;
                cin >> b;
                
                cout << "Enter the value : " << endl;
                cin >> c;

                cache1.Put(b,c);
            }

            int choice;

            do
            {
                cout << "---------------------------\n";
                cout << "| 1. Get Data             |\n";
                cout << "| 2. Input More Data      |\n";
                cout << "| 3. Delete Data          |\n";
                cout << "| 4. Display Data         |\n";
                cout << "| 5. Exit                 |\n";
                cout << "---------------------------\n";
                cout << "Enter your choice: ";
                cin >> choice;

                if (choice == 1)
                {
                    int key;

                    cout << "Enter the key: ";
                    cin >> key;

                    int data = cache1.Get(key);

                    if (data != -1)
                    {
                        cout << "Data : " << data << endl;
                    }
                    else
                    {
                        cout << "Key not found!" << endl;
                    }
                }

                else if (choice == 2)
                {
                    int key, data;

                    cout << "Enter the key: " << endl;
                    cin >> key;
                    
                    cout << "Enter the data : ";
                    cin >> data;

                    cache1.Put(key, data);
                }

                else if(choice == 3)
                {
                    int key;

                    cout << "Enter the key: " << endl;
                    cin >> key;

                    cache1.Remove(key);
                }

                else if (choice == 4)
                {
                    cache1.Display();
                }

                else if (choice == 5)
                {
                    cout << "Exiting...\n" << endl;
                }

                else
                {
                    cout << "Invalid choice. Try again.\n" << endl;
                }
            } while (choice != 5);
        }

        else if (x == 2)
        {
            int y;

            cout << "Enter the size of the cache : " << endl;
            cin >> y;

            LRU_BST cache2(y);

            cout << "Input Data " << endl;

            int a, b, c;

            cout << "Enter the size of data : " << endl;
            cin >> c;

            for (int i = 0; i < c; i++)
            {
                cout << "Enter the key : " << endl;
                cin >> a;

                cout << "Enter the data : " << endl;
                cin >> b;

                cache2.Put(a, b);
            }

            int x;

            do
            {
                cout << "---------------------------\n";
                cout << "| 1. Display Data         |\n";
                cout << "| 2. Input More Data      |\n";
                cout << "| 3. Delete Data          |\n";
                cout << "| 4. Exit                 |\n";
                cout << "---------------------------\n";
                cout << "Enter your choice : " << endl;
                cin >> x;

                if (x == 1)
                {
                    cache2.Print();
                }

                else if (x == 2)
                {
                    int key;
                    int val;

                    cout << "Enter the key : " << endl;
                    cin >> key;

                    cout << "Enter the data : " << endl;
                    cin >> val;

                    cache2.Put(key, val);
                }

                else if (x == 3)
                {
                    int key;

                    cout << "Enter the key : " << endl;
                    cin >> key;

                    cache2.Evict(key);
                }

                else if (x == 4)
                {
                    cout << "Exiting...\n"
                         << endl;
                }

                else
                {
                    cout << "Invalid choice. Try again.\n"
                         << endl;
                }

            } while (x != 4);
        }

        else if (x == 3)
        {
            int y;

            cout << "Enter the size of the cache : " << endl;
            cin >> y;

            LRU_HM cache3(y);

            int x;

            do
            {
                cout << "---------------------------\n";
                cout << "| 1. Input List           |\n";
                cout << "| 2. Input Data           |\n";
                cout << "| 3. Delete Data          |\n";
                cout << "| 4. Display List         |\n";
                cout << "| 5. Display Map          |\n";
                cout << "| 6. Exit                 |\n";
                cout << "---------------------------\n";
                cout << "Enter your choice : " << endl;
                cin >> x;

                if (x == 1)
                {
                    int a;
                    int key, val;

                    cout << "Enter the size of the list : " << endl;
                    cin >> a;

                    Node3 *List = NULL;
                    Node3 *tail = NULL;

                    cout << "Enter the key : " << endl;
                    cin >> key;

                    for (int i = 0; i < a; i++)
                    {
                        cout << "Enter data for node " << i + 1 << ": ";
                        cin >> val;

                        Node3 *newNode = new Node3(key, val);

                        if (List == NULL)
                        {
                            List = tail = newNode;
                        }
                        else
                        {
                            tail->next = newNode;
                            tail = newNode;
                        }
                    }

                    cache3.Insert_List(key, List);
                }

                else if (x == 2)
                {
                    int key, val;

                    cout << "Enter the key : " << endl;
                    cin >> key;

                    cout << "Enter data : " << endl;
                    cin >> val;

                    cache3.Put(key, val);
                }

                else if (x == 3)
                {
                    int key;

                    cout << "Enter the key : " << endl;
                    cin >> key;

                    cache3.Remove(key);
                }

                else if (x == 4)
                {
                    int key;

                    cout << "Enter the key : " << endl;
                    cin >> key;

                    cache3.display(key);
                }

                else if (x == 5)
                {
                    cache3.Display();
                }

                else if (x == 6)
                {
                    cout << "Exiting...\n"
                         << endl;
                }

                else
                {
                    cout << "Invalid choice. Try again.\n"
                         << endl;
                }

            } while (x != 6);
        }
    } while (x != 0);

    return 0;
}
