# set
A library for set\<T\> in C++

# Constructors
set();

set(set&);

# Methods
size_t size();

bool empty();

size_t count(T&);

iterator find(T&);

iterator lower_bound(T&);

iterator insert(T&);

iterator insert(iterator, T&);

size_t erase(T&);

iterator erase(iterator);

void clear();

iterator begin();

iterator end();

set& operator=(set&);
