#include <iostream>
#include <vector>

struct Blob {
public:
	std::vector<int> values{ 1,2,3,4 };
public:
	std::vector<int> GetValues() { return values; }
};
int main() {
	Blob b;
	printf("%p\n", b.GetValues().data());
	printf("%p\n", &b);
	printf("%p\n", b.values.data());
	return 0;
}
