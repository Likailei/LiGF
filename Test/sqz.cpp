#include <iostream>

int main(){
	int a = 515;
	int b = 512;
	std::cin >> a;
	printf("%d\n", (a+b-1)&~(b-1));
	printf("%d\n", 0/512*512);
	return 0;
}