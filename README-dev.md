# Krypton Document for developers - A Encrypt Program

## Module Structure

The module structure of krypton is just like Monadic Programming, like this:

```haskell
Just 3 >>= \x -> return $ x+3
```

The code snippet above is a naive example to show the usage of Monad. The Monadic programming in Haskell is like an execution chain: receive the return value before and inherit the scope(such as let bindings) before. The Monadic Programming will specify the order of the functions will be execute, in order to deal with side effect and execution order in Functional Programming Language.(Since those arguments in some functions without side effect can be calculated in any order, like the functions `fun1,fun2,...` in  `fun(fun1, fun2,...)`, this feature is useful while parallel calculation). For Lisp programmer who cannot use Monad to deal with those problem, they will use a style called CPS(Continuing Passing Style), which use function nesting to specify order and side effects.

Now, let's focus on the module structure of Krypton. We provide some basic encryptors (such as AES, RSA, etc.) and use template nesting to create a encryptor for complex encryption.

In summary, a Module is a functor which receives a functor for inputting and returns a new functor(`BigInt a => Func a`) for outputting. Just like this:

```cpp
template<typename TInput>
class Module1{
private:
    TInput input_mod;
public:
    explicit Module1(TInput && ti):input_mod(ti){}
    BigInt operator()() {   // Output
        BigInt cur;
        while((cur=input_mod())!=-1) {    // Read until EOF
            // Calculation
            ;
        }
        return ans;
    }
};

int a = 123,b;
function<void(void)> f = [&a,&b](){ b = a ;};
Deco<decltype(f)> d(f); 
d()

```

However, for different types of mod, the interface will be different. Here're 2 types of mod: Output Mod: Type:`Packet p -> String`; Applicator Mod: `BigInt n -> Packet p`.

A Module Chain will just like this: `Frontend(Input) -> Applicator -> ... -> Applicator -> Output -> Frontend(Output)`

The monadic structure will divide a program into some small pieces, it's good for debugging, testing, etc..
