# Move Semantics

## Useful links

- [universal-references-in-c11-scott-meyers](https://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers)

## Summary

- const disables move semantics
  - if I define a const member and I do want to move that object, what should I do?
  - [move-constructor-and-const-member-variables](https://stackoverflow.com/questions/6317429/move-constructor-and-const-member-variables)

- Don't return by const value

- const&& is possible, but a semantic contradiction

- To always adopt values you can take by value and move

- T&& and auto&& are universal references

- universal reference do not always forward

- Use auto&& in generic range-based for loop

- Use std::move() for member functions