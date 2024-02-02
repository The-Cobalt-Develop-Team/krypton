# Krypton - A Encrypt Program

## ModuleLoader

### Module Interface

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