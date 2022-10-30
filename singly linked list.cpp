//
// Created by A on 10/24/2022.
//

#include <bits/stdc++.h>
using namespace std;

/*
 * definitions:
 *  - singly: b. it has a "single" arrow to the next Node only
 * advantages:
 *  - appending elements dynamically in O(1) time
 */

#define forNode for(Node *cur = head; cur; cur = cur->next)

struct Node {
    int data;
    Node* next { }; // ptr from the same type

    Node(int _data) : data(_data) {} // constructor
    ~Node() { // destructor
        cout << "Destroy the value: " << data << " at the address: " << this << endl;
    }
};

/// function on Nodes ///

// display Nodes:
void print_iterate(Node *head) { // notice that: passing by ptr is not like passing by ptr, e.g. u can pass the pointers by ref !
    for(; head != nullptr; head = head->next)
        cout << head->data << ' ';
    cout << endl;
}
void print_recurse(Node* head) {
    if(head == nullptr) {
        cout << endl;
        return;
    }

    cout << head->data << ' ';
    print_recurse(head->next);
}
void print_recurse_reverse(Node* head) {
    if(head == nullptr) {
        return;
    }

    print_recurse_reverse(head->next);
    cout << head->data << ' ';
}

// search:
Node* find(Node* head, int val) {
    while(head != nullptr) {
        if(val == head->data) return head;
        else head = head->next;
    }
    return nullptr;
}

/////////////////////////////////////

class LinkedList {
    Node* head {};
    Node* tail {};
    int length = 0; // don't forget to update it

    vector<Node*> debug_data; // to keep the elements of the linked list safe in case of any corruptness
    void debug_add_node(Node *node) {
        debug_data.push_back(node);
    }
    void debug_remove_node(Node *node) {
        auto it = find(debug_data.begin(), debug_data.end(), node);
        if(it == debug_data.end()) cout << "Node is not exist\n";
        else debug_data.erase(it);
    }

public: // the operations on data
    LinkedList() = default;
    LinkedList(const LinkedList&) = delete;
    LinkedList &operator=(const LinkedList &another) = delete;

    ~LinkedList() { // O(n) time - O(1) memory
        while(head) {
            Node* cur = head->next;
            delete head;
            head = cur;
        }
    }

    // simple functions:
    void add_node(Node* node) {
        debug_add_node(node);
        length++;
    }
    void delete_node(Node* node) {
        debug_remove_node(node);
        length--;
        delete node;
    }

    void print() {
        Node* cur = head; // to avoid changing 'head' value
        while(cur != nullptr) {
            cout << cur->data << ' ';
            cur = cur->next;
        }
        cout << endl;
    }

    void insert_end(int value) {
        Node* nod = new Node(value); // we need to delete it

        if(head == nullptr) {
            head = tail = nod;
        } else {
            tail->next = nod;
            tail = nod;
        }
        debug_add_node(nod);

        length++;
    }
    void insert_front(int value) {
        Node* front = new Node(value);
        if (head == nullptr) {
            head = tail = front;
        }
        else {
            front->next = head;
            head = front;
        }

        length++;
        debug_add_node(front);

        verify_data();
    } // O(1)

    void pop_front() {
        assert(length);

        Node* tmp = head->next;
        delete_node(head);
        head = tmp;

        if(length < 1) tail = head;

        verify_data();
    }

    Node* get_item(int idx_1) {
        // notice that the index is one based

        if(idx_1 > length or idx_1 < 1) return nullptr;
        else {
            Node* cur = head;
            for (int i = 1; i < idx_1; ++i) {
                cur = cur->next;
            }
            return cur;
        }
    } // O(idx)
    Node* get_item_back(int idx_1) {
        // notice that the index is one based

        if(idx_1 > length or idx_1 < 1) return nullptr;
        else return get_item(length - idx_1 + 1);
    } // O(idx)

    int search(int val) {
        // returning the index 0-based, or -1

        Node* cur = head;
        int idx = 0;
        for( ; cur; cur = cur->next, idx++)
            if(cur->data == val) return idx;

        return -1; //not found
    }
    int improved_search(int val) {
        // search with shifting to the left

        int idx = 0;
        for(Node *cur = head, *prev = nullptr; cur; prev = cur, cur = cur->next, idx++) { // notice the order of increment
            if(cur->data == val) {
                if(!prev) return idx;

                swap(prev->data, cur->data); // notice
                return (idx - 1);
            }
        }

        return -1;
    }

    bool is_same(const LinkedList& other) {
        return(debug_to_string() == other.debug_to_string());
    }

    /// debugging and testing ///
    void verify_data() {
        // my data: head, tail, length

        if(length == 0) assert(head == nullptr and tail == nullptr);
        else {
            assert(head != nullptr and tail != nullptr);
            if(length == 1) assert(head == tail);
            else assert(head != tail);

            assert(!tail->next);
        }

        int len = 0;
        forNode {
            len++;
            assert(len < 1e4);
        };

        assert(len == length);
        assert(length == (int)debug_data.size());
    }
    string debug_to_string() const {
        string ret;
        forNode {
            ret += (to_string(cur->data) + ' ');
        };

        return ret;
    }

    // debug using the vector:
    void debug_print_node(Node *node, bool is_seperated = false) {
        if(is_seperated) cout << "Sep: ";
        if(node == nullptr) {
            cout << "null\n";
            return;
        }

        cout << node->data << ' ';
        if(node->next == nullptr) cout << "X ";
        else cout << node->next->data << ' ';

        // head and tail:
        if(node == head) cout << "Head";
        else if(node == tail) cout << "Tail";

        cout << '\n';
    }
    void debug_print_list() {
        int siz = (int)debug_data.size();
        for (int i = 0; i < siz; ++i) {
            debug_print_node(debug_data[i]);
        }
        cout << "***************\n" << flush;
    }
};

///////////////////////////////////////////////////////

class ReducedLinkedList {
    Node *head {};
public:
    void print() {
        for(Node *cur = head; cur != nullptr; cur = cur->next)
            cout << cur->data << ' ';
        cout << endl;
    } // O(n)

    void add_element(int value) {
        Node *node = new Node(value);
        node->next = head;
        head = node;
    } // O(1)

    Node* get_tail() {
        if(head == nullptr) return nullptr;

        Node *cur;
        for(cur = head; cur->next; cur = cur->next)
            ;
        return cur;
    } // O(n)
};

/////////////////////////////////////////////////////////

int main()
{
    /// start with the Nodes:
    /*Node* Node1 = new Node(10); // head
    Node* Node2 = new Node(20);
    Node* Node3 = new Node(30);
    // link them:
    Node1->next = Node2;
    Node2->next = Node3;
    Node3->next = nullptr;

    // displaying Nodes:
    Node* head = Node1;

    print_iterate(head);
    /// why head didn't change, even if it's a pass by ptr?!
    print_recurse(head);
    print_recurse_reverse(head);
    cout << endl;

    // search:
    auto res = find(head, 1);
    cout << (res == nullptr? "not found\n" : "found!\n");*/

    /// linked list:
    LinkedList list, list2;

    // insert front:
    for (int i = 1; i <= 40; i += 8) {
        list.insert_front(i);
    }
    list2.insert_end(30);
    list.print();
    list2.print();

    // is same
    //if(list.is_same(list2)) cout << "same !!\n";

    // pop front:
    list.pop_front();
    list2.pop_front();
    list.print();
    list2.print();

    //if(!list2.is_same(list)) cout << "not same !!\n";


    // insert end:
    /*for (int i = 1; i <= 10; ++i) {
        list.insert_end(i*i);
    }
    list.print();

    // get nth element: (front/back)
    if(list.get_item(10)) cout << "get item: " << list.get_item(10)->data << endl;
    if(list.get_item_back(14)) cout << "get item back: " << list.get_item_back(14)->data << endl;

    // searching:
    cout << list.search(100) << endl;
    cout << list.improved_search(100) << endl;
    cout << list.search(100) << endl;*/


    /// Reduced Linked List:
    /*ReducedLinkedList rlist;
    for (int i = 1; i <= 7; ++i) {
        rlist.add_element(i);
    }
    rlist.print();
    auto tail = rlist.get_tail();
    if(tail) cout << tail->data << endl;*/

    cout << "\nNo RTE ;)\n"; // just for asserting
    return 0;
}

/*
 * TO DO:
 * clear()
 */