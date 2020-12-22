
#include<string>
#include<vector>
#define PROP(x)
class Y {
public: void doSomething();//first do something
PROP("serialize")
void doSomething(std::string str);
void doSomething(std::vector<std::string> strArr);
};

void z() { Y y; y.doSomething(); }

int doSomething(int i) {
    if (i == 0) return 0;
    return 1 + doSomething(i--);
}

int main() {
    return doSomething(2);
}
