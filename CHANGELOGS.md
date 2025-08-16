# Changelogs

## v0.2.10 Mana Update
released on 07-15 2025 build#1344

### language updates

+ Rename `namu` to `byeol`.

- If you write a blank space with a comment, the blank space would somehow be printed 
  to the screen.

### framework updates

+ I've finally been able to remove `nullable reference`, which has been a long time coming.
  my code is now more compliant with the C++ standard.
  It's true that nullable references have had a lot of benefits in terms of code readability, 
  and while we haven't had issues in major compilers for 4-5 years, there's no guarantee that 
  we will in the future.

  I'm removing nullable references from all of my code due to the maintenance and 
  risk involved going forward.
  It's not just a simple string substitution, but a lot of work, including rewriting 
  logic where necessary.

  What initially prompted me to do this was a suspicion that compiling in release mode 
  on mac-arm64 would crash due to nullable references, but that was a separate issue, 
  as we'll discuss later (...)

+ Support wrappers for different types of return values.
  In a way, it's a natural addition with the demise of nullable references.

  * tmay<T> is used when returning as a value, nested with the semantics of an error.
    This is more like a delegation of std::optional.

  * tres<T, R> is used to go one step further than tmay<T>, and to embed the desired 
    type of information in the error.

  * tmedium<T> is for use in OR macros only, and is not recommended for use outside of macros.

+ introduces the concept of `SIDE_FUNC`.
  This also came about as a result of the deprecation of nullable references.
  Now that references and pointers were clearly distinguished, pointers had to be 
  used more aggressively.
  I have tried to replace pointers with references as much as possible through OR macros to
  reduce the number of places where pointers are used, but pointers are still used a lot.
  SIDE_FUNC is a convenience function for this, which is a single-line macro that 
  takes T& as an argument and delegates to if the original function takes T* and is not null.
  The code convention for writing SIDE_FUNC is detailed in this document.

+ The part that deals with dynamic shared objects is platform-dependent, so I named it 
  dlib and moved it to the indep module.

+ I've improved the `TO` and `OR` macros:
  I've cleaned up their usage and made them work better together when used in combination.
  I've also added support for various corner cases, such as returning T* if there is a 
  referent in the middle of `TO`.

+ I've written a code convention rule for the return type of functions.
  I'm also drafting a section in `/doc` to help guide code modifications.
  but didn't have time to write a retrospective or organize the topics I wanted to 
  talk about, so wrote it as it was in my head, the tone is a bit harsh and emotional.
  I plan to rewrite and translate it later, organized by topic, when it's ready for 
  public consumption.

+ fixed everything to actively use `WHEN` throughout the code.
  It was too much work to end with this one line.

+ As the language is renamed, I take the following naming policy.
  Always write `byeol` verbatim and never abbreviate it in parts that are visible to 
  the user (e.g., source code file extensions)
  However, internally, the framework can use the abbreviation `BY` for usability.
  this changes some things like macro prefixes, namespaces, etc. from `nm` to `by`.

- Removed three code duplicates: `verify` `parse` `expand` in `interpreter`.

- Fixed a mysterious crash on mac-arm64 that hadn't been fixed for a long time.
  The reason was an extra formatting character (e.g. printf("%s %s\n", "only single string"); )
  Strangely enough, it worked on all other OSes, except mac-x64, so this was a good thing.
  This inspired me to buy a newer MacBook, as I've been using an Intel Mac and couldn't
  reproduce it, no matter how hard I tried.

- Fixed a couple of memlick issues during parsing.

- Fixed cmake 4.0 compatibility issue

- Fixed a compilation error in nseq --> tucontainable, caused by a tricky 
  template instantiation in MSVC.
  Keep it up, MSVC.

### development environment updates
+ Fixed clang-format behaving differently on different OSes and minor versions.
  Use docker to always use the same version of clang-format wherever you are.
  If you want to run it locally yourself, use the command `./builder.py format`.

+ The github workflow will generate the full binary with the bundle pack.

+ The `mod` folder was very easily misunderstood as `MOD`, which is a gaming term,
  so I changed it to `module`.

+ Enforce the 80 soft 120 hard column rule.
  In simple terms, this means that if you have more than 80 columns, you should start 
  thinking about adding new line, but never more than 120 columns.

+ Add the `builder.py prerequisites` command.
  It will carefully check if you have installed any external libraries or tools 
  required for your development environment.

+ Support for `Claude AI`.

- Significantly reduced the number of warnings generated by windows builds.

- Fixed MSVC to be more compliant with C++ standards using the `/permissive-` option.
  I don't know why I didn't know about this before. 95% of my errors were caught in 
  one fell swoop.
  The dozens of hours I've spent struggling with it are going through my head.
  I'd like to take this opportunity to thank claude-ai for his profound insight and 
  enlightenment, 
 `/permissive-`, as my head has been smashed dozens of times in the early hours of 
  the morning by the powerful combination of MSVC and templates.


## v0.2.9 Mana Update
released on 02-26 2024
build#1167

### language updates

\+ shorten-property-definition grammar changes.
Previously, `@` was used, but now it is changed to `'`. The function itself is the same.
```byeol
def Person
age := 12

main() void
    Person'
    person.age = 39
```
It is more intuitive because it resembles the symbols used in mathematics.

\+ String template has been added.
Use `$identifier` or `${expression}`.
```byeol
// Previous:
print("your name is " + name + " and double of your age is " + (age * 2) as str)
// Changed:
print("your name is $name and double of your age is ${age * 2}")
```

\+ String iterators now work based on codepoints. Namu language used UTF-8 as the base, but
since the existing strings always advanced by 1 byte in the case of iterators, it could not
properly output multibyte Unicode.
From now on, this has been improved so that the iterators advance/retreat only based on codepoint.
```byeol
src := "abcd🏁efg" # There is a multibyte Unicode flag in the middle.
print(src[2]) # c
print(src[5]) # e
print(src[4]) # 🏁

# In the case of the flag above, `0xf0 0x9f 0x8f 0x81` is a total of 4 bytes,
# but `len()` considers it as 1.
print(src.len()) # 8
```

\+ `abstract function` can now be defined. An abstract function is a function definition statement
that does not have a function implementation (body).
This definition can be used as a function type or as an interface that must be filled in a derived
class.
```byeol
foo(input int) int # abstract function
# no body here.
```

\+ A nested function is added.
A nested function means a function defined within a function.
A nested function has the characteristic of capturing the scope when defined.
```byeol
main() int
    name := "kniz"
    hello(who str) str
        msg := "$who like Nintendo."
        ret "$name: $msg" # Capture the local variable `msg`.
                          # A variable that has been captured once has no effect even if its
                          # value changes later.
    name = "charles"
    print(hello("I")) # kniz: I like Nintendo.
```

If you return a nested function, it becomes a `closure`, which is commonly referred to in other languages.

```byeol
foo(input int) int: 0
makeClosure(n int) foo # The return type is a function.
    ret if n >= 0
        # This nested function captures the previously defined `n`.
        multiply(input int) int: input * n
    else
        # This nested function captures the previously defined `n`.
        subtract(input int) int: input - n

main() int
    sub := makeClosure(-5) # This is a function that does `input + 5`.
    multi := makeClosure(5) # This is a function that does `input * 5`.
    multi(3) + sub(3) # 15 + 8 --> 23
```

\+ Lambda syntax is added.
Lambda is an anonymous function, and can only be defined within a function call expression.
Since it is an anonymous function, the form that only lacks the function name in the existing
function definition is the most consistent form with lambda syntax.
Therefore, it is defined as follows.
```byeol
(value value-type) return-type
    stmt1
    stmt2
    ...
```
For example, it becomes like this.
```byeol
foo(n int) int
add(foo', input int) int
    ret foo(input)

main() int
    ret add((n int) int # <-- lambda definition in progress.
        n + 3
    , 10) # <-- Be careful about the indentation of `,`.
    # If you abbreviate the above code, it becomes: add((n int) int: n + 3).
```
Lambda's `parameter deduction` and `return-type deduction` will be added in the future.

\+ You can specify an explicit type when defining an assignment.

```byeol
foo() student: .... # Assuming that student is a derived object of person.
you person := foo() # you points to a student object, but it is of type person.
```

\+ The structure of `leaf` has been applied to the parser/scanner structure of the latest `core`.

\- Fixed an error that allowed containers with different parameterized types to be converted.

\- Removed an error that allowed `_` to be used at the beginning of an identifier.
If _ is used at the beginning, it should be recognized as a `protected` access modifier, not a name.

\- Removed an issue that prevented octal and hexadecimal numbers from being parsed properly.

\- Removed the issue where `call-complete` could be used on incomplete objects.
```byeol
def Person("myName") # <-- You should not use call-complete on incomplete objects.
    ctor(newName str)
    ...
def person("myName") # OK
    ...
```

\- Removed Character type.
Now, even a single character is expressed as a string like python.

```byeol
// Previous:
b := 104
b as char == `h`

// Changed:
b := 104
b as str == "h"
```

\- Fixed the error where it was impossible to call directly by receiving a closure like
`getClosure(3)(object)`.

### framework updates

\+ Visitor can now decide whether to revisit or not.

When inheriting Visitor, you can decide with a boolean value.

```cpp
class yourVisitor : public visitor {
    yourVisitor(): visitor(false/*not reentrant*/) {}
};

yourVisotor yours;
yours.setReturnable(true); // Changed to reentrant
```

\+ The submini language was named `seedling`, but it has been changed to `leaf`.
I like it because it is shorter and more intuitive.

\+ Type conversion has been optimized a bit more. It is expected to have a slight performance
advantage.

\+ The return type of `isSub()/isSuper()` has been expanded. Previously, both `isSub()/isSuper()`
were provided as `boolean` types, and were considered true and returned even if they were the same type.
From now on, each function will return one of the three `int`s.
```cpp
constexpr nint NO_RELATION = 0;
constexpr nint SUPER = 1;
constexpr nint SUB = 1;
constexpr nint SAME = 2;
```

\+ Type Convergence has been slightly optimized for speed.

\+ `WHEN` macro has been added.

\+ Sequential unordered multimap is added.
A container that `remembers the insertion order` is added to the STL's unordered multimap.
This will be mainly used to iterate the scope in that order by remembering the insertion order.

\+ All iterators are now bidirectional, so they can also go backwards.

\+ `chain` can now `link()` based on `iter`.

This is a very important feature, playing a key role in `scope` and bringing great extensibility.

For example, previously, you could `link()` based on `chain`. If a new element was added to the
container after `link()`, it would also be included in `chain`.
This problem creates an issue where if a local variable is captured by `closure`, a variable with
the same name would be accessible from `closure` later.
To solve this problem, `link()` should be based on `iter`.

Also, since `iter` can proceed backward, `chain` can be configured to proceed backward only in
some sections.

```cpp
typedef tnchain<std::string, int> chain;
chain m;
m.add("meat", 1);
m.add("banana", 2);
chain m2;
m2.add("apple", 3);
m2.add("banana", 4);
m2.add("pineapple", 5);
chain m3;
m3.add("mango", 6);
m3.add("melon", 7);

// Create a reverse iterator for b2, and link it from there after moving 1 step backwards.
m.link(m2.rbegin() + 1);

// Elements of the container that m has: "meat" -> "banana"
// Elements of m2: "apple" -> "banana" -> "pineapple"
// Elements of m as a chain: "meat" -> "banana" -> "banana" -> "apple"
//                           The reason why "banana" appears twice in a row is because m2 was
//                           linked backwards.
// m2.link(m3.begin() + 1); // Element of m3: "mango" -> "melon"
// Element of m as a chain: "meat" -> "banana" -> "banana" -> "apple" -> "melon"

m2.add("watermelon", 8); // This element is added at the end.
                         // Since m linked from +1 in the reverse direction, this element
                         // is not visible from m's perspective.
m3.add("strawberry", 9); // Since m2 inserted m3 in the forward direction, this element
                         // is also visible from m's perspective.
m.add("pumpkin", 10); // It is added at the end of the container that m has.

// At this point, the elements of m as a chain:
// "meat" -> "banana" -> "pumpkin" -> "banana" -> "apple" -> "melon" -> "strawberry"
```

\+ Rewrote the Type Convergence module by restructuring it further.

\- Fixed an issue where `expander` did not remove the `@expand` function after the generic was evaluated.

\- Optimized by creating a `mockNode` instead of creating and evaluating the actual object in the
`verification` step.

\- Removed memory leaks that occurred during parsing.

\- `getExtra()` is removed from `type`.
Since it is void* in the end, it was not used because it was unstable and annoying to have to do an
unstable type conversion on the user side.

## v0.2.8 Mana Update
released on 08-28 2024
build#1064

### language updates

\+ We now use '#' to represent comments. This makes comments simpler to express.
```namu
def person
    age int # You can write a comment like this.
```

\+ Supports multiline comments.
   Use `##` to indicate the start and end.
   However, be careful with whitespace when ending a multiline comment in the middle of code.
```namu
def Person
    age int ## This is where the multiline comment starts.
The comment is still valid: ##    grade flt # Make sure to put a four spaces before it so
                                            # that they are in the same block.
```

\+ We can now define a custom constructor.
   Specify it with the `ctor` identifier. and omit the return type.
```namu
def Person
    age int
    ctor(): ; # empty block statement.
    ctor(newAge int): age = newAge

main() int
    p1 := Person # ctor() will be called.
    p2 := Person(33) # ctor(int) will be called.
```

\+ An `err` object is added to `builtin`.
   We can now represent an err.

\+ `is` syntax is added.
   returns a bool value indicating whether the given lhs object is compatible with the rhs object.

\+ The `complete-object` syntax is added.
   It is now possible to represent a complete object when defining an object with `def`.
   In the namu language, all properties are represented by convention rules.
   Similarly, if the first letter of an identifier is lowercase, it is treated as a complete object.
```namu
def pERSON # This is a complete object.
   age := 23
main int: pERSON.age # A complete object can be used just like a regular object.
```
   Incomplete objects have their first letter capitalized, and all identifiers must be non-capitalized.

```namu
def Person # This is an incomplete object.
    age := 23
main() int
    ret Person().age # Incomplete objects must be created via constructor.
```

\+ `const` expressions are now added.
   Again, following the philosophy of the NAMU language, we represent this as a convention rule.
   Identifiers of two or more characters and all capitalized letters means `const`.
   However, you cannot use `const` when defining an object.
```namu
def PERSON # Error. There is no point in using const when defining an object.
    ...

def Person
    age := 23
    ctor(newAge int): age = newAge

PERSON := Person(20) # OK, this PERSON is a const.
main() int
    PERSON = Person(40) # ERROR. You can't assign another object to PERSON.
```

\+ `callComplete` syntax is added.
   You can give a specific value when creating a complete object. To use this, when defining the object,
   put any argument list besides of it.
```namu
def person(Teacher(“kniz”)) # using callComplete syntax.
    t teacher
    ctor(newTeacher teacher): t = newTeacher

def Teacher
    ctor(newName str): name = newName
    name str

main int
    ret person.t.name.len() # this is just like, 'ret "kniz".len()'
```

\+ all packs are complete objects by default.

\+ the unary `+` operator is removed.
   it gets in the way of expressing visibility, which comes later, and it's not really needed.

\+ visibility is now added.
   again, following the philosophy of the namu language, this is represented as a convention rule.
   - By default, we don't mark anything otherwise, it means `public`.
   - If you prefix it with `_` at any an identifier, it means `protected`.
   Note that the `_` is not part of the identifier.
```namu
def person
    _age := 38
    foo() void
        # We're accessing age, not _age.
        print("I'm " + age as str + " yo.")
```

You can explicitly represent a func that it is overrided with `+`.
   This is currently meaningless because we have no extension yet,
   but it allows you to explicitly state that a function/property is overrided.
   The order with underscore, which represents the visibility, is irrelevant.
```namu
def student ... # Suppose we expanded from person here.
    _+foo() void # We are stating that we have overridden the "foo() void" function in person.
                # If person didn't have a foo() function, this would be an error.
        print(“ok”)
```

\+ The `end` keyword is added.
   `end` defines an additional block statement after it.
   This causes the block statement to be executed just before the function ends.
   If multiple `end`s are defined in a function, the most recently defined `end` will executed first.
```namu
foo() int
    a := file()
    end
        a.close()      # 1)
    if !a.isOpen()
        b := file()
        end: b.close() # 2)

        # If the function was terminated at this point, the end block statements would be executed
        # in the order 2 -> 1.
```

\+ Adds a function type.
   You can use the name of the function as the function type.
   More detailed usage will be covered in the below `closure` topic.
```namu
def Person
    foo(n int) int: ....
    getClosure() foo # Use the function name as the return type.
        ..??..       # you don't know how to return it yet.
```

\+ closure is added.
   A closure is an assignable function-call that captures the obj scope and arguments.
   This allows for completely first-class-citizen like behavior.
```namu
def Person
    someClosure(n int) int: 0
    foo(n int) int
        ret n + 3
    getClosure() someClosure
        ret foo # OK. foo and someClosure have the same parameters and the same return type.
```

   Implicit casting between function types is not supported.
   This means that you can't pass a function to another function type just because some of the parameters are slightly different.

```namu
def Person
    someClosure(n flt) int: 0 # note that n is now of flt type.
    foo(n int) int: 3
    getClosure() someClosure: foo # Error!
        # Implicit casting is usually triggered when putting an int argument into an flt parameter.
        # and it has worked fine so far.
        #
        # However, when dealing with function types, implicit casting is not allowed for any
        # parameters and return type.
        # So someClosure and foo are completely different types.
```

   Function types also capture obj scope.
   the namu language can expect that you are about to want a closure,
   when you put a function as an argument or return a function to current function scope.
   in that case, namu language automatically makes a closure from a function call instead of you.
   with this feature, we can write code like the following.
```namu
def a
    foo(n int) int: n + 5
def b
    foo(n int) int: n - 2 # as you may know, a.foo and b.foo are same function type.

foo(f a.foo, n int) int
    f(n) # this runs closure. it doesn't care which object a given function belongs to.

main() int
    foo(a.foo, 1) + foo(b.foo, 1) # 6 + -1 ==> 5
```

\+ Supports type convergence.
   Type convergence is the feature to convert and converge a given object to its type when executing code.
   This means that not only objects defined by `def` function as types, but also function parameters,
   properties, and even local variables.
```namu
def Person
    ctor(newAge age) # newAge will eventually be converted to an int.
        age = newAge # even if you don't know what actual type of newAge,
                     # but you can be sure that it's compatible to age.
    age := 25

    foo() age
        val age
        val = age + 5
        val2 val # local variables can also be used as types.
        ret val2
```

   This contributes greatly to the philosophy of the NAMU language, which is to blur the distinction between objects and classes.
   There are only objects and types, not class any longer. and every object has a type.

   Type convergence is handled by the our `expander`, so it doesn't depend on the order of the sequence.
   Look at the code example above again.
   The `ctor` of `Person` defines `age` as a parameter type,
   but `age` is actually defined two lines down in the line,
   so if the interpreter was simply evaluating the code from top to bottom, it would have no choice but to resolve the above code as an error.
   NAMU languages solves this problem easily thanks to expander.

\- Fixed an issue with misreading dedication when expressing some complex lambdas.

\- Fixed an issue that prevented syntax like `value a.b.c` from being used.

\- Fixed an issue where it was possible to define a property even though there was no constructor with no arguments.
```namu
def Person
    ctor(n int) # the constructor has an int parameter.
        ... # doSomething

main() int
    p Person # Error. Because this code should be able to call Person() ctor.
```

### c++ framework updates

\+ Print the four digits of the address value together to aid in debugging.
   This little value, which I've named addrId, is represented as something like `@12d4`.
   It acts like a sort of abbreviated hash value, making it easier to tell which object is at which address.

\+ The tbridger class is now able to express inheritance. Use `extend()` to do so.

\+ `err` can be exported as an exception.
   blockExpr checks whether an exception has been added after eval every line.
   To express err in native environments, inherit from or use `nerr`.

\+ If an error occurs while fetching a pack, leave a detailed log.

\+ Manage parsing, verification, and expanding as states.
   This allows us to clearly express what stage each AST object is in and optimize it accordingly.
   For example, on the first call to subs() for an object in the LINKED state, if it is a complete object,
   `callComplete` will be executed.

\+ Added common construcutor.
   The initialization of an object's properties must be executed every time, no matter what constructor it goes through.
   To manage this efficiently, the common constructor contains common logic that all constructors must execute.
```namu
def Person
    age := 23
    age2 := 24
    age3 := 25
    age4 .....

    ctor(n int): .... # 1)
    ctor(n flt): .... # 2)
    ctor(n char): ... # 3)
        ## If there is no common constructor, then for each constructor 1, 2, and 3, we'll use
            age = 23
            age2 = 24
            age3 = 25
        for each constructor, you would have to have all the above codes. ##
```

\+ Completely improves the design of constructors.
   The traditional prespective was “call constructor == create object”, but this is problematic in the case of inheritance.
   This is because inheritance requires calling multiple constructors.
   The new perspective separates constructor calls and object creation.
   This works well if you only want to call the constructor for an already created object (callComplete, etc.).
   However, objects defined in native, such as bridge objects, are created immediately when the constructor is called.

\+ `tbaseObjOrigin` is added to represent the origin of the native object.
   It's all because of visibility, which doesn't need to be held for each cloned local object.
   So the appropriate place to hold it would be the origin, but we've returned the origin of the native object as a constant so far.
   This means that, using nInt as an example, once nInt's visibility is made public, all nInt objects's visibility will always be public.
   To solve this, there is only one origin object for an nInt by default, but you have the flexibility to point to a cloned origin if needed.
   This job is handled by tbaseObjOrigin.

\+ Changed `has()` function in container to `in()`.
   This is to pair with the `in` keyword.

\+ Parameter information for functions is now managed in ntype.

\+ Added lazyMgdType class to lazily evaluate parameter, ret, to avoid getting stuck in infinite recursion.

\+ `OR_RET` macro is added.
   If you define a variable with an initial value and it ends up in a null state,
   you may `return` the function or `continue` a loop is very common and leads to a lot of redundant code.
   The `OR_RET` macro will return the function with the appropriate value if the preceding initial value is determined to be null.
```cpp
// 1 - before:
A* a = getPainter().getBrush().getA();
if(!a)    return false;
// 1 - after:
A* a = getPainter().getBrush().getA() OR_RET false;


// 2 - before:
while(true) {
    A* a = getA();
    if(!a) continue;
}
// 2 - after:
while(true) {
    A* a = getA() OR_CONTINUE;
} // note that you need to use curly brackets. when you `OR_CONTINUE`, it expands to 2 lines.
````

\+ a big improvement to the safe navigation macro.
   Previously, we created and used `safeGet` macro, but it had a few problems.

   - `OR_RET` required similar behavior to `safeGet`, but `safeGet` is not reusable structure.
     I couldn't combine them into common logic.
   - It was not possible to use `OR_RET` directly on the result of `safeGet(a, b, c)`.
   - I had to put round brackets around the expr, `a.b.c`, which was kind of ugly.
        e.g. safeGet(a, b, c)
   - If you did a `safeGet(a, b(), c)` and then did another `safeGet` inside `b()`,
     the value would sometimes be twisted.
     This was because `safeGet` holds the value in a global variable instead of storing it in a temporary one.

   The new and improved macro is the `THEN` macro, which solves all of the above problems.
   Because it is designed to be generic so it was easy to create an `OR_RET` macro reusing it.
   It is also possible to apply `OR_RET` to the result of using `THEN`.
   Since we used `->*` overloading, which is a suffix operator, we don't need the outer curly brackets.
   Also, since we're returning from function to function,
   we don't need to store the value in a global variable somewhere. usage for it will follow.

```namu
// before:
A* a = safeGet(c, getB(), getA());
if(!a) return false;

// after:
A* a = c THEN(getB()) THEN(getA()) OR_RET false;
```

   Note that it has all the functionality of safeGet.
   - If null is encountered in the middle of code, the final value will be null.
     during running above codes, the program never crash.
   - If you get a T* in the middle or if you get a T&& or if you get a T or if you get a T&,
     it's all taken care of. Even if it's a pointer. I mean, dereferencing is not necessary.
```namu
// assume that when you call getB(), it returns nullptr of B*.
const int& a = getC()->getB().getAge(); // this will crash your app.

const int& a = getC() THEN(getB()) THEN(getAge()); // this won't crash your app.
nul(a); // true
```

\- Fixed an issue where demangle would not work for templates.

\- Fixed an issue where colors were not changing on windows.

\- No default constructor is created if the user defines a constructor.

\- Fixed an issue where overloading matching would misbehave.

\- Fixed an issue that sometimes caused the error log to be printed twice.

\- Fixed an issue where having an empty block statement would cause a crash.

\- Fixed an issue where `is` could not be used for primitive types.

\- baseFunc is no longer expr.
   The characteristic of expr is that run is executed when you do as().
   The as of a function should be the return value, not the execution of the function.

### etc
\- wasm binary couldn't leave some logs

\+ The 'make install' command is now available in Linux environments. Hooray!

\+ Support for code coverage.
   If you use the `cov` command in builder.py, it will calculate coverage using `llvm-cov` and `lcov`.
   Compute coverage via github action and you can check the result as a badge in the our README.

## v0.2.7 Mana Update
released on 06-15 2024
build#957

* apply code integration tools.
    1. clang-tidy, clang-format.
    add compiler options like '-Wextra', 'Wpedantic' to warn defect codes.

    2. generate language server file for LSP.

    values from rich obj. for instance,
        NM_E("type name of obj is %s, value 'name' of std::string is %s", objptr->getType().getName().c_str(), name.c_str())

    Now, richLog converts automatically depends on its dynamic type.
    if you pass an object class, richLog will convert and get const char* from its type name.
    if you pass an std::string class, it'll get const char* by running 'it.c_str()' instead of you.
    and it can handle nevertheless you put a pointer/reference type of T!
    for instance,
        NM_E("type name of obj is %s, value 'name' of std::string is %s", objptr, name); // very intuitive and code was shorten!

    5. rename genericObj to genericOrigin

    6. make safeGet macro.
    this is like safe-navigation feature of other modern languages.
    this let me implement far consice code.
    e.g.
        // as-is:
        if(nul(context)) return NM_E("context is null"), false;

        canvas& c = context.getCanvas();
        if(nul(c)) return NM_E("canvas is null"), false;

        paint* p = canvas.getPaint();
        if(nul(p)) return NM_E("paint is null"), false;

        brush& b = p->getBrush();
        if(nul(b)) return NM_E("brush is null"), false;
        b.doSomething();

        // to be:
        brush& b = safeGet(context, getCanvas(), getPaint(), getBrush());
        if(nul(b)) return NM_E("brush is null"), false;
        b.doSomething();

* made new bridge API.
    tbridger defines a bridge to a func.
    tbridge is bridgeObj can take bridge instances made with tbridger.
    but if you cares, you can define a baseObj which can bridge funcs with bridger.
    with for previous bridge version, you can't do this. because bridges to func always got attached to
    bridgeObj, not baseObj.

    so code duplication removed by defining a func based on baseFunc with tbridger.

    and tbridger also now can provide API to make a constructor which takes various parameters.
    and tbridger provide making a bridger func to your lambda.

    however, when you define a bridge func to this new bridger API, bridge funcs are stored into static variable.
    please take care of it.

    here is sample codes.

        ```cpp
            struct yourClass2 {};
            struct yourClass {
                yourClass(int n, yourClass2* p) {}

                int say(int n) {
                    return n + 1;
                }
                void fly() {}
                std::string fly(const char* msg) {
                    return std::string(msg);
                }
            };

            tbridger<yourClass>::ctor<int, yourClass2*>()
                .ctor<yourClass>()
                .func("say", &yourClass::say)
                .func<std::string, const char*>("fly", &yourClass::fly)
                .closure<yourClass*, yourClass, int, yourClass2*>("runMyLambda", [&](int n, yourClass2* ptr) -> yourClass* {
                    return new yourClass(n, ptr);
                });
            // now 'tbridger<yourClass>' contains 5 bridge funcs in static memory.

            scope& managedFuncs = tbridger<yourClass>::subs(); // usage-way 1)

            str derivedBaseObj = tbridger<yourClass>::make(new yourClass(0, nullptr)); // usage-way 2)
            derivedBaseObj->isSub<baseObj>() // true.
            str res = derivedBaseObj->run("say", narr{nInt(23)});
            res->cast<nint>(); // 24

            str arg2 = tbridger<yourClass2>::ctor().make(new yourClass2());
            str res = derivedBaseObj->run("runMyLambda", narr{nInt(0), *args2})
            // res is new tbridge<yourClass> instance.
        ```

* origin object has been added.
    origin shouldn't be visible to most places. so when you pass, handle origin object,
    you treat it just as a baseObj. there is no specific API which is only available
    to origin class, not baseObj.

* fix minor memory leak issues.

* add reldbg build mode.
    it's code has been optimized out just like rel mode, but still includes debug info.

* logging readability improved.
    1. mostly NM_TAG value was redundant. so removed.
    add some padding to align values.

    2. when AST structure display, prints pointer address by representing 4 hex digit.

    3. on verification, prints what it verifies, step by step.

* code refactoring to get better class structure.
    * make defVarExpr to reduce code duplication between defPropExpr and defAssignExpr.

    * remove code duplication between whileExpr, forExpr.

    * delete redundant classes.

    * add 'worker' as common class of 'verifier, interpreter, visitor, parser, preEvaluator, starter'.

* add new frame API

* signal catch.
    * when system interrupt raised, it can be catched to our signaler class.
    * before crash app, prints all current running AST structure.

* exception handling.
    * when unexpected 'err' instance found on running one of statements in blockExpr, it'll be judged to an exception.
    * when exception found, leave to errReport, codes keep unwinding. and finally app will stop its doing if there is proper
      handling logic somewhere.
    * native call stack can be shown.

* enhanced graphVisitor
    * show more text in color.
    * show more info to debug and know in detail.
    * show each node's address in 4 hex digit. very useful when you debug.

* performance optimization
    * add 'mockNode'
    this is literally doesn't contains copied object info but have only origin object info.
    but when this mockNode placed on verification, verifier regards that this is something cloned by origin object.
    so it think this as complete object. and that's what I want.
    in consequences, I don't copy each nodes and member variables for verification.

* new syntax, 'break'.

* new syntax, 'overloading'.
    function overloading is now functional.
    when you call a function and it could be one of possible functions in your context,
    verification will calculate affinity of each possible calls and score it.
    I named it as 'match'.
    each match represents function-call against all available functions in current frame.
    there are 4 kind of matches now.

        EXACT_MATCH:        this means perfect score match. this is probably what user wants.
        NUMERIC_MATCH:      to call this function, some of user's parameter should numeric
                            implicit cast. numeric implicit cast has slightly high priority to
                            usual cast.
        IMPLICIT_MATCH:     to call this func, some of user's paremeter should usual cast.
                            mostly, it would be upcasting. (e.g. DerivedClass --> SuperClass)
        NO_MATCH:           with implicit casting, there is no way to be enough to call this func.
                            explicit casting may required.

    and each match also has calling score(== lv). the lower the higher priority.
    each score can easily be calculated. it's how far distance does it to top of the current frame.

* new syntax, ': ;'.
    it's inline empty block.

* remove old syntax, 'elif'
    use 'else if' instead.


## v0.2.6 Mana Update
released on 05-07 2023
build#684

### major changes
* f07cce485 - core: fix: prector on generic object wasn't evaluated
* 2110f0163 - core: feat: filescope
* e7bf4db35 - core: refactor: preEvaluation
* 0024b87ac - core: feat: supports escape sequence
* a2f4e3838 - core: fix: type deduction doesn't work on add operation
* enhanced graphVisitor
    * show more text in color.
    * show more info to debug and know in detail.
    * show each node's address in 4 hex digit. very useful when you debug.
* d0fa146d3 - core: feat: return integer when program exit
* d84a15aa0 - core: feat: setElem conversion for addAssign
* ea5807303 - fix: memlite: optimization of vaults has defect scenario
* 8a1184436 - core: fix: do implicit casting, not explicit when call func
* e578f6d38 - core: feat: forExpr compatiable for string type
* 48413d985 - core: feat: add 'iterate(int)' on str
* 4b54dc21f - core: feat: add 'get(seq)' substring func on str
* c931b76ed - core: feat: add char type core: feat: char concat to string
* 50933ae00 - core: feat: add byte type
* c385e8259 - core: feat: hex, octal literal expression
* c4a14b644 - core: fix: nameless arr object doesn't work
* fddac12b4 - core: feat: main() only allowed for int or void rettype
* 562a1b881 - core: fix: to define variable to void isn't allowed
* 08cfe2cec - core: fix: type deducing crashes app on wasm binary
* 17f11a852 - core: fix: 'p1 person' accesses origin incomplete object
* 71c702e09 - core: fix: keep prompt for inputting a value in emscripten
* 6bb18159f - core: refactor: a little bit become easier to understand error log
* 6c19062e7 - core: feat: let exprs contain src info
* abea245e6 - core: refactor: no warnings for running a script without pack
* 3ae8e9a8f - core: fix: carrage return in comment block shows empty line
* b79efbd0d - refactor: change file extension to 'nm'
* fbc68d5dc - core: feat: add 'input' builtin funcs
* 9a7031027 - core: feat: add builtin 'print' func
* b78d7e7da - core: fix: copyctor of array means 'deepcopy'
* f20ccb72f - doc: move documents into gh-pages branch
* ad618b52a - doc: use jekyll on _guide
* 099b13ae3 - chore: make document using jekyll, not m.css
* 9b2172b6a - wasm-frontend: feat: use tab for indentation
* 5260d32ee - namu: feat: add simple wasm ide
* 4b84f8f65 - namu: feat: add namu-wasm-fronted
* 022f7e313 - namu: fix: show errors after run script
* 8d87bf410 - fix: namu: Uncaught Runtime error
* 6aa5433ba - namu: feat: add 'bufferSrcFlag'
* b9822faa0 - fix: core: app crash on release because of null-check optimization

## v0.2.5 Mana Update
released on 02-28 2023
build#654

### major changes
* ef80970c core: fix: pointed and refer wrong address on release binary
* 0cc5e5b5 feat: supports wasm build
* f2809e22 memlite: refactor: improve speed performance for vaults using vector
* 6d98fcd5 test: feat: add 'verbose' mode
* 3025b1a9 core: doc: add example 'map.namu'
* 41471603 core: feat: add '~', '<<', '>>' more bitwise operators
* 5d11c240 core: feat: add bitwise operators, '|', '&', '^'
* 45e229a7 core: feat: add '-=', '*=', '/=', '%=' operators
* 620c4d81 core: feat: add '+=' operator
* db7da4c1 core: feat: add postfix '++', '--'
* fe79b1af core: feat: add '||' and '&&' logical binary operators
* 370aceb8 core: refactor: replace 'return' keyword to 'ret'
* d71c2aa6 core: fix: when passes argument to func, it won't be implicit casting
* 49005661 core: fix: when use 'break' without value, it causes app crash
* 9cd405fe core: fix: to use 'return' inside of for doesn't work
* c0e43409 core: fix: using 'break' inside of ifexpr doesn't work
* 304bfdb4 namu: fix: if src code declared to a pack concatnated with dot it won't start.
* 5e39b732 core: feat: add 'get' func on str
* 517eb0b0 core: feat: add 'len()' on str
* 3bdeac6c core: fix: clang isn't necessary on windows
* d39fc497 core: feat: change concept for boolean implicit casting
* be19e42c core: feat: add '!' prefix unary operator
* 36325069 core: feat: add unary prefix operators
* 67012ffa core: fix: can't parse negative number
* 519ed930 core: feat: add 'elif' keyword
* 8f0c327a core: feat: supports '<' and '>' binary operator
* 2c3807a0 core: fix: whileExpr don't loop properly
* 1dc9227e doc: add 'RSPGame' example
* 2343b9db core: feat: add logical boolean binary operators
* af4067f7 core: feat: add 'while' keyword
* 861667f8 core: fix: return is not expression
* c89fc26f core: feat: add 'if & else' keyword
* aa7f1af6 core: feat: add 'next' keyword
* 5a65aba7 core: feat: add 'break' keyword
* 9304922a core: fix: window compatibility for 'add' of arr
* e755c28f chore: build error not judged to an error

## v0.2.4 Mana Update
released on 11-03 2022
build#623

### major changes
* 0a38bb00 - core: feat: add 'ret' expr
* 5cc5b5bc - core: test: add benchmark test for native and managed
* a246d1db - core: feat: add 'mgdType'
* 62b45e00 - core: fix: for windows(MSVC) compatible
* 955ef3e7 - core: feat: verify deduction of array
* 49caeb95 - core: feat: mock a constructor
* 75ae332c - core: refactor: rename 'reduce()' to 'deduce()'
* 0988d703 - core: feat: add 'defArrayExpr'
* 270738f5 - core: feat: supports seq as a container of 'for' expr
* d112f02a - core: feat: put getElemTypeFunc and let it be more generic
* 66b5d3ae - core: feat: add 'forExpr'
* 02a690df - core: feat: 'iterate' func has been bridged
* da725997 - core: doc: defAssign is not a expression, but statement.
* b0829187 - core: fix: return type implicit casting doesn't work
* 175970dc - core: feat: add syntax for 'seq'
* a9fb1c9c - core: feat: add seq
* edee0b95 - core: fix: remove redundant func at mgd containers
* 9c916c06 - core: feat: improve exception handling for nseq
* f0104598 - core: feat: add nseq
* fdeb1350 - core: fix: element of array didn't applied to implicit casting
* 61ccc8e2 - core: feat: implicitly define an array
* 3e134b5d - core: feat: add syntax for accessing element of an array
* f04944ac - core: fix: return type of generic func doesn't change
* 32c3de8e - core: feat: add syntax for 'definition of array'
* 1418fe88 - core: feat: implement general marshaling
* 406488aa - core: feat: add more primitive marshalings
* 437c4e3d - core: feat: add 'len' to subs of array
* 4af3ed6c - core: feat: bridge can take const-member function
* e7273038 - core: feat: add tarr, arr
* a34f80e9 - bundle: feat: add 'bundle' module

## v0.2.3 Mana Update
released on 09-16 2022
build#584

### major changes
* 44e048c4 - core: feat: implement deepClone
* 1d7ee101 - core: feat: add "generalizer"
* a34a8a0e - core: feat: add new verifier using visitor
* 1b811a2e - core: feat: add graphVisitor
* d5081be2 - core: feat: add visitor
* f3e7199c - test: feat: negative() func added
* 37cb7f8c - clog: feat: add NAMU_CALLSTACK()
* 57057621 - core: feat: add generics feature
* 72c59be5 - test: refactor: rename namu to core
* 9bff0b36 - refactor: decide language name, "namu"



## v0.2.2 Mana Update
released on 07-17 2022
build#520

### major changes
* cae5b20b (HEAD -> master, origin/master, origin/HEAD) builder: feat: release outputs for Windows on stable branch
* 64b27401 wrd: fix: compatiblity issue for windows
* 6533a5a4 wrd: feat: keyword 'aka' changed to new syntax
* 12420876 wrd: refactor: replace ntype to node of getEvalType()
* 22df5700 wrd: feat: add 'aka'
* b9966971 wrd: feat: add muna
* b949093c wrd: fix: missing implicit casting
* 5e9ef105 wrd: fix: core crashed when iterpret paranthesis '(', ')'



## v0.2.1 Mana Update
released on 06-18 2022
build#495

### major changes
* 39355eb3 bundle: feat: add input() func
* b96c3c56 wrd: fix: when parse 'aka' it makes a blockexpr
* 1cc87161 wrd: refactor: add 'frame::_setObj' to store obj reference
* 2b1ffc88 wrd: feat: add frameInteract
* 5c82285a wrd: feat: add a tray between verifications
* 5d336854 wrd: fix: parser can recognize escape sequence
* f521fe13 wrd: refactor: use std::string for asStr
* 87fdf6c9 wrd: fix: 3 identifier concatenating with dot doesn't work
* ff37fd75 wrd: fix: default ctors has been added into owns area
* fc3d3c08 wrd: refactor: refine slot loading logic
* 889cdfc5 wrd: feat: mashaling for string& reference types
* 46cf36ac wrd: feat: add 4 more fundermental arithmetic operation
* 1de7e847 wrd: doc: remove prototypings
* a713c5ea wrd: refactor: preconstructor
* 8faa6738 wrd: fix: apply new refactoring of slot to TC files
* 2118cf34 wrd: refactor: pack -> slot and apply autoslot, preconstructor
* d8af510f wrd: feat: add(iter, iter, iter) new func on ucontainer
* 8493d0ec wrd: refactor: rename obj -> baseObj
* 33d1daf9 wrd: feat: add 'addExpr'
* 6b3c8eff wrd: feat: add 'asExpr'
* 88b48011 wrd: fix: type casting & as()
* f8f73429 wrd: feat: add defAssignExpr
* e854d62b wrd: refactor: change syntax of aka-deduced
* 8fd64ebf wrd: feat: add defAssign expr
* 4ab7b541 wrd: refactor: remove redundant expr, literalExpr
* c5eb689c wrd: fix: apply dum containers into primitives
* 5d33e74a wrd: feat: add assignExpr
* 623c390e wrd: feat: add 'comment' syntax



## v0.2.0 Mana Update
released on 05-14 2022
build#444

### major changes

* 9598b6ce chore: supports rpath of macos
* 6bb3b13a wrd: feat: add location info of a node when it's parsed
* eacc8855 chore: builder can make a ubuntu deb package
* c8aaae9f wrd: refactor: move prototype def into implementation files
* 849fd435 chore: replace tab to 4-width spaces
* e6f488f8 (origin/stables) chore: add github action for stable branch
* 820b0297 clog: feat: colorize logs
* 31585660 chore: fix not to build wrd twice
* 0c8ef376 wrd: fix: fix for window compatibilities
* 3021b43c wrd: refactor: rename INTERFACE macro -> ADT
* e3605229 wrd: fix: rename __super to _super_
* 9f95bd69 indep: fix: fsystem doesn't work when slash at the end
* db1740a9 wrd: feat: handling all of stream at one function call
* 0da3c982 cli: feat: starts with silent mode



## v0.1.7 Essence Update Final
released on 04-21 2022
build# 365

### major changes
* c50f7fb0 cli: feat: implement cli
* qd264caa4 wrd: feat: implement starter
* q782ba8fd wrd: feat: implement interpreter
* qbfdee5ae wrd: feat: add runExpr
* qc6305dc5 wrd: feat: err code is managed by a enum
* qe129020d wrd: feat: change concept of pack parsing
* qb9f5e95a wrd: fix: scanner couldn't recognize 'tab' token
* qf42eabf3 wrd: fix: evalType of func is the func, not return type
* q3dcf9d5d wrd: feat: verify duplicated variable defined
* qa010f8fe wrd: refactor: boost performance to verify subnodes
* q3e1b1bbf wrd: fix: member variable couldn't be verified
* qfd40cdbd wrd: feat: prints number of err
* q8b367ed7 wrd: refactor: remove doesNeedScope()
* q82d222ba wrd: feat: param can take default value and type
* q05cfee2b wrd: refactor: massive refactoring to apply 'map' to subs()
* q6752dc9f memlite: tactic instance won't be copied when binder assigned
* q59e7604d wrd: feat: apply tnmap as subs
* q403aaaf8 wrd: refactor: change how to implement immutable
* q17562542 wrd: feat: add tnmap class
* q5a1dc13e wrd: feat: add has() func on containers
* qd11c3412 wrd: fix: container shouldn't takes str as param
* q384a735f wrd: feat: add bicontainer
* q5b154aaf wrd: refactor: redesign container classes for map
* q54bff10c wrd: refactor: rename stackFrame to frames
* q0d66f3b0 wrd: refactor: pack is a obj
* q56509a60 wrd: refactor: obj class interact to frame if it needs
* q4bd8db53 wrd: feat: class can be exposed as private to verfication
* q6f7861da wrd: feat: add returnExpr
* q2ce96c59 wrd: feat: add literalExpr
* q792caf51 wrd: feat: add 'defVar' expression
* qaea9d8f9 wrd: feat: show error code on report
* qe8cba3b9 wrd: feat: parse 'block' expression
* q2da39552 wrd: feat: parsing 'pack' keyword
* q465c432b wrd: feat: make syntaxTest be resuable
* q4a66304d wrd: refactor: pack lazy importing
* qdea7b81d wrd: fix: remove '.*' syntax of aka
* q8b0f78e4 wrd: feat: can parse 'aka' keyword
* qd3ab6d6d wrd: feat: '' is for a character, "" is for a string
* qce80c805 wrd: fix: can't build release mode
* q1b50bc4a wrd: feat: parse can recognize 'func' definition
* q414a491e wrd: refactor: restructure of lowparser and parser



## v0.1.6 Essence Update 0.6
released on 09-22 2021
build# 244

### major changes
* f1c9f95c - wrd: refactor: all expr now can holds row and col info
* 7283c2ef - wrd: feat: err object can reports area of source code
* 6694792b - wrd: feat: add verifier
* c317d671 - chore: clean more folders on build directory
* e16edbe1 - wrd: feat: add dummyErrReport
* a6cfdf0e - wrd: feat: errReport has only public api to be needed
* 7106393f - wrd: refactor: rename class name 'fail' & 'failReport'
* 954f245d - wrd: refactor: improve implementation of immutable
* b12d7cfd - wrd: refactor: loose limitation of explicit ctor of binders
* 28fb0204 - wrd: fix: expr couldn't check validness when it comes to recursive
* 47862906 - wrd: feat: fill missing implementation for blockExpr
* 4d26e01d - wrd: feat: add blockExpr
* ac7dabd1 - wrd: refactor: merge and reuse 'getEvalType' for returnType of func
* 14855735 - wrd: feat: add exprMaker
* d9b25dff - wrd: refactor: change structure of 'as' casting
* 30c26ef4 - wrd: fix: remove legacy world codes
* a17b0a58 - wrd: refactor: rename interp module to wrd
* c324fe20 - interp: feat: you can specify src info for bridge API
* c42d67ee - interp: feat: add generic visitor class
* 90330081 - interp: feat: universal WRD macro.
* a8f4a749 - interp: refactor: rename srcArea to area
* f9fb8d16 - chore: publish doc has error
* d5e3e2e4 - chore: github action for publish docs


## v0.1.5 Essence Update 0.5
released on 07-31 2021
build# 233

### major changes
* interp: feat: basic structure of parser & scanner
* interp: feat: introduce new packLoader
* interp: chore: generates visualized bison reports
* doc: remove dependency to m.css


## v0.1.4 Essence Update 0.4
released on 06-26 2021
build# 226.

### major changes
* all: doc: license has been changed from LGPL to MIT
* all: chore: unittests now runs at 'bin' dir.
* interp: feat: add c++ bridge API
* interp: feat: pack importing
* interp: feat: tnchain supports deep(= nested) chaining
* interp: refactor: container components
* interp: feat: abstracts extracting origin


## v0.1.3 Essence Update 0.3
released on 05-03 2021.
build # was 209 and major changes are as follows:

### features
* implmentation of Essence Update Iteration#3.
* add basic components of ast. (obj, pack, node, ...)


## v0.01a Dust Update build# 9444
it was 02-25 2016.
Dust is the name of an update used by the 'Node Engine' project that was origin of the 'world language'.
'NodeEngine' is a result of consideration how to visualize programming elements and let users express and orgranize their thoughts in visual. Unfortunately, as a result of deliberation, I had to accept not to release to public because it's not enough useful to what I intended to be.

it has been developed by myself for quite a while, so even if it's not released officially I want to keep it below as records.

## v0.01a Dust Update build# 8392
updated on 04-24 2015. not released in public.
## v0.01a Dust Update build# 7001
updated on 03-29 2015, not released in public.
## v0.01a Dust Update build# 4429
updated on 02-02 2014, not released in public.
## v0.01a Dust Update build# 4317
updated on 10-30 2013, not released in public.
    + graphics(dx9) module was added.

## v0.01a Dust Update build# 4162
updated on 01-29 2013, not released in public.

## v0.01a Dust Update build# 3727
updated on 10-19 2012, not released in public.

## v0.01a Dust Update build# 3600
updated on 05-19 2012, not released in public.

## v0.01a Dust Update build# 3559
updated on 05-05 2012, not released in public.

## v0.01a Dust Update build# 3531
updated on 04-15 2012, not released in public.

## v0.01a Dust Update build# 3445
updated on 03-15 2012, not released in public.

## v0.00a Dust Update build# 3377
updated on 02-20 2012, not released in public.

## v0.00a Dust Update Build# 3086
updated on 01-15 2012, not released in public.

## v0.00a Dust Update Build# 2976
updated on 10-03 2011, not released in public.

## start developing NodeEngine on 2010.
