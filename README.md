# set
A library for set\<T\> in C++

# Constructors
set();

set(set&);

# Methods
size_t size();

bool empty();

void swap(set&);

iterator find(T&);

iterator lower_bound(T&);

std::pair\<iterator,bool\> insert(T&);

size_t erase(T&);

iterator erase(iterator);

void clear();

iterator begin();

iterator end();

set& operator=(set&);
