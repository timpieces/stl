# array

https://en.cppreference.com/w/cpp/container/array.html

- It was good to remember a little how ctad works and how to write deduction guides, as it's not something I often do in practice. I don't think that my deduction guide for std::array{1,2,3} is actually correct. It looks a bit janky
- The 'NoDefault' test is interesting. It constructs and array of 0 elements with a struct that has a deleted constructor. For now I've solved it by putting mElems inside of an anonymous union but I'm not 100% convinced that this will work. For example I think that this breaks the copy assignable test.
- The copy-assignability is quite confusing. I don't understand why this isn't automatically deleted? Need to come back to this
- I found out about to_array. Meaning wasn't clear initially but it made sense when I saw that the main use case it to specify the type while deducing the length. The impl is quite confusing and I wonder if I did something wrong though. I've created an index sequence and then initialized by moving elements individually
- Unqualified-std-cast-call. Haven't seen this before. What's the deal?
- Surprised thta T* data() is constexpr. what gives?
- There's a constexpr test which creates an array of 0 size, and then calls 'data()' on it... what gives? what's the intended semantics here?
- fill semantics should work for array of zero size too...
- interesting that array has empty and max_size even though it doesn't make sense for these containers. want to find out why these api methods are required (ranges or something?). size only returns whether the array dim is zero

Did not know about this?
```
include/stl/array/array.h|97 col 23| note: implicit use of 'this' pointer is only allowed within the evaluation of a call to a 'constexpr' member function
||    97 |         if constexpr (empty()) {
```

This kind of makes sense, because the function needs to be called in non-constexpr. I think my mental model of constexpr functions was a little off here. I thought that when you do e.g. 'if constexpr', it essentially tries to evaluate the function at that point, and will succeed so long as it's constexpr and then it will 'inline' the branch. Still need to fix this. It's like 'somewhat' correct. chatgpt seems to just say 'it is a language rule'
https://en.cppreference.com/w/cpp/language/constant_expression.html

- We need a const rvalue overload of std::get otherwise example on get_const_rv.pass.cpp:39 doesn't work. Kind of weird that we need this overload. It's a pretty contrived place.

- We even try to access '.at' of a zero element array and except it to throw wtf. And annoyingly it even needs to return a value? To fix added 'noreturn' to throw_out_of_range which I haven't used in a long long time if  ever

- operator spaceship + default is pretty baller

- curious to understand the reason that the stl (libc++) has so many underscores everywhere. what's wrong with having actual identifiers? I suppose it's to try and prevent issues with macro replacement?

- I was failing on overload resolution for operator== forever, before I found out that I need operator<=> *and* operator==. But why? I don't understand this at all. need to read through the docs very carefully

- We need the free operators (e.g. operator==) to also work on zero-length arrays T.T

- Once done, find out how libc++ handles zero-sized arrays. Generally compare at end

- is it possible to abstract over const for iterators?

- how to implement iterator access for 0 element array wtf? would be nice to just abort

- initially I went for a common base class to ensure that it worked fine with 0 elements. after implementing all methods I realise that totally different classes would probably be more appropriate. actually perhaps I'm wrong there, I think most of the methods should just succeed and it's UB for the 0 element case

- interesting test: if (c.size() > 0) { C::reference r = c[0] }
this tests that c[0] works for a zero-length array, but it will never actually be called in practice. at least in this case we can std::abort

- for rbegin, interesting that you actually can't use 'data() - 1' as the end, because it will fail in constexpr

- was not aware of std::reverse_iterator before this - would have saved me quite a lot of time. I initially wrote both my own forward *and* reverse iterators before realizing that they aren't needed :facepalm:
