+++
title = "Typescript Braindump"
date = "2025-11-03"
+++

# Typescript Braindump

I've learned a decent bit about the language in my year and a half of using it and I've decided to synthesize a document with a lot of the more useful parts of the language. Lots of what I have learned are opinions and stylistic recommendations because at the end of the day, Typescript will do nothing to stop you if you really want to shoot yourself in the foot.

## Variables 

### Declaring Variables
Typescript allows you to declare variables using `const`, `let`, and `var`. Only one of these is correct :^)

```typescript
const suffering = 0;
```

Generally, you do not need mutable variables so const is the most common way of declaring a variable. Const is the same as with most other languages. Declaring something as const will prevent you from changing it's memory address but everything else is fair game. 

```typescript
const arrayThatIsConst = []
arrayThatIsConst.push('hi') // valid

const objectThatIsConst = {thing: 'hi'}
objectThatIsConst.thing = 'hello' // valid

const objectThatIsStillConst = {};
(objectThatIsStillConst as any).thing = 'hello' // valid unfortunately

const objectThatIsNotConst = {thing: 'hi'}
objectThatIsNotConst = {thing: 'otherthing'} // invalid
```

If you need to use a mutable value you *allegedly* should always use `let`. To my understanding the main difference between `let` and `var` is that `let` respects block-scopes (whenever you see this `{}`) and closures and `var` only respects closures. If you declare a variable with `let` in and out of a for loop, they are different variables. The same is not true for `var`.

Put more simply, this is the output when `var` is used:

```typescript
var a = 'a'

for (const i of Array(5).values()){
    var a = 'hmmm'
    console.log(i)
}

console.log(a) // hmmm
```

vs `let`

```typescript
let a = 'a'

for (const i of Array(5).values()){
    let a = 'hmmm'
    console.log(i)
}

console.log(a) // a
```

Perhaps there are situations in which `var` is applicable, but I have not run into any. It is generally agreed online that it should be shelved in favor of `let`. 


### Block Scopes

A block scope is what it sounds like.
Variables and functions declared within a block (everything that uses `{}` that is not an object declares a block-scope) are only accessible within that block.
In the below example, innerFn does not exist outside of the functionFactory and cannot be directly accessed unless it is returned at which point a copy of innerFn is assigned to a variable in the new scope.


```typescript
function functionFactory() {
    function innerFn(){
        const item = 'hello'
        console.log(item)
    }
    // console.log(item) // invalid
    return innerFn
}
// innerFn() // invalid
const newFn = functionFactory()
newFn() // hello
```
Inner scopes have access to the contents of their parent scopes, the opposite is not true.
Scopes are pretty simple overall, they work just like most other languages.
If you use `var` we enter the world of [function scopes and global scopes](https://www.freecodecamp.org/news/scope-in-javascript-global-vs-local-vs-block-scope), but the solution to that is to just not use `var`. 


### Closures
To quote the MDN docs: "*A closure is the combination of a function bundled together (enclosed) with references to its surrounding state (the **lexical environment**)*". Given what we have just learned about scopes, the lexical environment can be thought of as outer scopes.

In the following example, we see that the variable `outerVar` can still be accessed despite the outer function seemingly no longer existing. This is because the inner function is bundled with the relevant outer scope.

```typescript
const outerFunction = () => {
    const outerVar = 'hello';
    const innerFunction = () => {
        console.log(outerVar)
    }
    return innerFunction
}

const createdFunction = outerFunction()
createdFunction() // hello
```

In the below example, we can see a different closure being generated for each of the created functions. Every time we call `count` we bundle a newly created block-scope with newly created variables with the function.

```typescript
const count = () => {
    let increment = 0;
    const incrementFn = () => {
        increment += 1;
        console.log(increment)
    }
    return incrementFn
}
let count1 = count();
count1(); // 1
count1(); // 2

let count2 = count();
count2() // 1
```

### Enums

After writing this whole doc, I realized I never talked about enums anywhere and I think they're pretty cool and should be used everywhere. They generally improve code quality because they allow you to essentially a list of related constants and that improves readibility and type safety.

By default enums map a variable to a number starting from 0.

```typescript
enum BasicEnum {
    APPLE,
    BANANA,
    CARROT
}

console.log(BasicEnum.APPLE) // 0
console.log(BasicEnum.BANANA) // 1
console.log(BasicEnum.CARROT) // 2
```

I personally think it's best to use strings instead of the default number. You can imagine a request going from the frontend to the backend where the enums are for some reason out of order resulting in a wrong response. They also just make request bodies easier to read.

```typescript
enum BasicEnum {
    APPLE = 'APPLE',
    BANANA = 'BANANA',
    CARROT = 'CARROT'
}

console.log(BasicEnum.APPLE) // APPLE
console.log(BasicEnum.BANANA) // BANANA
console.log(BasicEnum.CARROT) // CARROT
```

I said earlier that enums increase readability and type safety, what does that mean? Well, for readability, it's cleaner than passing around some strings that can theoretically be anything. In the following example, if a string were used in place of an enum, theoretically the strings used to initialize the `Shop` class could be anything. Instead, we enforce uniformity. We also don't have to speculate about the type that fruit can be and instead just safely assume it's one of the three in the enum.

```typescript
enum BasicEnum {
    APPLE = 'APPLE',
    BANANA = 'BANANA',
    CARROT = 'CARROT'
}

console.log(BasicEnum.APPLE) // APPLE
console.log(BasicEnum.BANANA) // BANANA
console.log(BasicEnum.CARROT) // CARROT

class Shop {
    fruit: BasicEnum
    constructor(fruitType: BasicEnum){
        this.fruit = fruitType
    }
}

const newShop = new Shop(BasicEnum.APPLE)
switch(newShop.fruit){
    case BasicEnum.APPLE:
        console.log('Apples for sale')
}
```

With regards to type safety, enums allow us to infer the values and perform [exhaustive switch statements](https://www.meticulous.ai/blog/safer-exhaustive-switch-statements-in-typescript) to update functions whenever the underlying enum changes. I strongly recommend using exhaustive switches to make sure you don't forget to change anything important when updating your enums.

```typescript
// throws an error because Banana and Carrot are missing
// switch(newShop.fruit){
//     case BasicEnum.APPLE:
//         console.log('Apples for sale')
// }

switch(newShop.fruit){
    case BasicEnum.APPLE:
        console.log('Apples for sale')
    case BasicEnum.BANANA:
        console.log('Bannanas for sale')
    case BasicEnum.CARROT:
        console.log('Carrots for sale')
}

// if we add GRAPES to BasicEnum, the above would not compile until
// it is updated with Grapes.
```

## Functions

There are two types of functions in Typescript. There are your regular functions which are denoted via `function` and there are your arrow functions which are denoted via `=>`.

There are a couple of differences but generally they can be used interchangeably.

### Hoisting

Regular functionss are hoisted, but arrow functions are not. Hoisting essentially means they are pre-emptively declared so they can be used anywhere in the code. An example of hoisting can be seen in the following example.

```typescript
hi() // produces hello

function hi() {
    console.log('hello')
}

/* This would not work because hi was called before it was defined
hi()
const hi () => {
    console.log('hello)
}
*
```

### `This` for functions

See below for an explanation on what [this](#this) means

Arrow functions and regular functions handle the use of `this` differently. Arrow functions use the same `this` as where they are defined whereas regular functions create their own `this`. We can see this in action in the below example where the regular function now has access to myName. 

```typescript
const objWithArrowFn = {
    myName: 'Hello',
    greet: () => {
        console.log(this.myName); // type error, no this
    }
};

const objWithRegularFn =  {
    myName: 'Hello',
    greet: function () {
        console.log(this.myName);
    }
};
obj.greet();
```

The second example strikes me as an anti-pattern since you may as well just define a class with just a little bit more boiler plate.

```typescript
class classWithRegularFn  {
    hello: string
    constructor(){
        this.hello = 'Hello'
    }
    greet(): void { // greet can be an arrow function
        console.log(this.hello)
    }
}
```

However knowing this distinction is useful because now if you try and have a constructor be an arrow function you'll find that it fails to do so. The constructor does not create it's own this which is important for the [`new`](#new) keyword to function and as a result arrow functions seemingly are not recognized ways to initialize classes.

```typescript
class classWithArrowFn  {
    hello: string //Property 'hello' has no initializer and is not
    // definitely assigned in the constructor
    constructor = () => {
        this.hello = 'Hello'
        return this // The containing arrow function captures 
        // the global value of 'this'
    }
    greet(): void {
        console.log(this.hello)
    }
}
```

### Anonymity

Arrow functions are always anonymous. Regular functions can also be anonymous but you do have a choice and usually people do not create anonymous functions with the `function` keyword.

Anoymous functions may make it harder to debug. What do I mean by this? Consider a datadog flamegraph for an api that is particularly slow. 10 named functions are called in this api and 100 anonymous functions are called. The anonymous functions are most of the flamegraph. How do you figure out what is causing the slowness? You'd have to find the slowest named function and drill down from there, which isn't hard but is mildly more involved than if all the functions were named. In my experience it hasn't been an issue but if anonymous functions are everywhere then I can see it being one.

Being anonymous, it's a bit harder to perform recursive operations since you can not easily call yourself but it's still possible to do.

## This

The `this` or `self` keyword is used in a lot of different languages and it generally refers to the same idea. In Typescript it is a way of representing the current context. If there is no current context it refers to the global object and `this` = `globalThis`.

`this` is generally used within the context of objects like classes and is determined at runtime. It is used to access other methods or variables within the current context of the object. A trivial example below:

```typescript
class BasicClass {
    variable: string
    constructor(){
        this.variable = 'You can assign variables via the constructor'
    }
    method(){
        console.log(this.variable)
    }
}
```


### `New`
When dealing with `this` you will often need to use the `new` keyword. `new` does a few things, it creates a new object, binds that object to the `this` context, then it executes the constructor. 
```typescript
class objWithArrowFn {
    name = 'Hello'
    greet = () => {
        console.log(this.name);
    }
};

class objWithRegularFn {
    constructor(){
        console.log('getting constructed')
    }
    name ='Hello'
    greet () {
        console.log(this.name);
    }
};

const obj1 = new objWithArrowFn();
obj1.greet() // Hello
const obj2 = new objWithRegularFn(); // getting constructed
obj2.greet() // Hello
```

While writing this to check if I missed anything I learned that you can actually return a different object from the constructor causing the returned object to be bound instead. This is interesting but strikes me as an anti-pattern.

```typescript
class objWithArrowFn {
    name = 'obj1'
};

class objWithRegularFn {
    constructor(){
        return new objWithArrowFn()
    }
    name ='obj2'
};

const obj1 = new objWithArrowFn();
const obj2 = new objWithRegularFn();

console.log(obj1.name) // obj1
console.log(obj2.name) // obj1
```

### Passing `This` Between Objects (Skippable)
An interesting thing that I found on the MDN docs is that `this` always refers to the current context meaning if you take a function that references this out of 1 object and put it into another, it references the context of the second function. This makes sense becasuse as stated before it is determined at runtime. This is shown in the below example. If the use of `as any` wasn't warning enough, **DO NOT** do this. 

```typescript
// modified example from MDN
const obj4 = {
  name: "obj4",
  getThis() {
    return this;
  },
  log(){
    console.log(this.name)
  }
};

const obj5 = { name: "obj5" };

(obj5 as any).getThis = obj4.getThis;
(obj5 as any).log = obj4.log;
console.log((obj5 as any).getThis()); // { name: 'obj5', getThis: [Function: getThis] }
(obj5 as any).log(); // obj5
```

## Types and Interfaces

Types and interfaces are pretty cool and both share the main function of describing the shape of an object. I don't have much experience with some of the more abstract things that are possible, but I think I have a decent handle on the basics. 

### Types vs Interfaces
One of the bigger differences between types and interfaces is that interfaces can be declared multiple times and the definitions are merged, meanwhile types cannot be updated from their original declaration.

```typescript
interface ExampleInterface {
    something: string
}

interface ExampleInterface {
    someOtherThing: string
    // something: null // is not allowed because it contradicts the previous interface
}

const exampleObject: ExampleInterface = {
    something: 'hi',
    someOtherThing: 'hello'
}

type ExampleType = {
    something: string
}

// not allowed
// type ExampleType = {
//     someOtherThing: 'hello'
// }
```

There are some other differences which I found [here](https://stackoverflow.com/questions/37233735/interfaces-vs-types-in-typescript/52682220#52682220) but quoting from the stack overflow comment: "A class can implement an interface or type alias, both in the same exact way. Note however that a class and interface are considered static blueprints. Therefore, they can not implement / extend a type alias that names a union type." Somehow, I've never ran into that issue before but that's a neat distinction.


```typescript
// taken from the above
interface Point {
  x: number;
  y: number;
}

class SomePoint implements Point {
  x = 1;
  y = 2;
}

type Point2 = {
  x: number;
  y: number;
};

class SomePoint2 implements Point2 {
  x = 1;
  y = 2;
}

type PartialPoint = { x: number; } | { y: number; };

// FIXME: can not implement a union type
class SomePartialPoint implements PartialPoint {
  x = 1;
  y = 2;
}
```

In addition to classes not being able to implement union types, when we try and extend a union type with an interface we get the error: "An interface can only extend an object type or intersection of object types with statically known members."

```typescript
type AOrBType = { a: string } | { b: string }

interface NewExampleInterface extends AOrBType {
    thing: string
}
```

### Casting

Casting in typescript shouldn't happen very often. If you need to do it, what you can do is to use `as` to cast an object to appear something else. My personal preference for casting is to use `is` for ensuring type safety and transforming the type while doing so. Consider the following toy example:

```typescript
type BasicType = {
    optional?: string
}

function hasOptional(variable: BasicType): variable is BasicType & { optional: string } {
    return 'optional' in variable
}

const a: BasicType = {
    optional: 'hello'
}

if (hasOptional(a)){
    console.log(a)
}
```

### The Miracles of the Type System

The type system can do a lot and is apparently Turing complete so the world is really your oyster here. The best guide on type manipulation is probably from the [source](https://www.typescriptlang.org/docs/handbook/2/types-from-types.html). I'll just give some toy examples on the nicer concepts that I've used.

Recursion:
```typescript
type NestedType = (string | NestedType)[]
const a: NestedType = ['a']
const b: NestedType = [[[[[[[[a]]]]]]]] // also valid
```

Conditionals:
```typescript
type NestedType = (string | NestedType)[]

type IsArrayType<T> = T extends Array<infer Type> ? true : false

function isArray(item: NestedType): IsArrayType<NestedType>{
    return true
    // return false is not possible because NestedType is by definition an array
}

// Footgun
// type FlatResult<T> = T extends Array<infer NewType> ? FlatResult<NewType> : T
// Type instantiation is excessively deep and possibly infinite.(2589)
// function getFlatValue(item: NestedType): FlatResult<NestedType> | undefined{
//     if (Array.isArray(item[0])){
//         return undefined
//     }
//     return item[0]
// }
```

Using Keys of Other Types
```typescript
// similar to built in partial
type MyPartial<T> = {
    [Property in keyof T]: T[keyof T] | undefined
}

type RequiredStuff = {
    a: string
    b: string
}

type NoLongerRequiredStuff = MyPartial<RequiredStuff>

const example: NoLongerRequiredStuff = {
    a: undefined,
    b: undefined
}
```

## End

I think this was a useful exercise in figuring out how the language I use all the time functions. Sometimes I would go and try to describe something and realize there is a gap in my understanding so I would go to the [MDN docs](https://developer.mozilla.org/en-US/docs/Web/JavaScript) to really make sure things worked they way I thought they did. 

There's a lot more to this language, but I think one of the interesting parts of typescript is that it prevents a lot of the stranger things that you can do in javascript. I learned a lot of stuff writing this that I left out because there's honestly no reason to use some of it, in my opinion.