#include <stdio.h>

void converter() {
	float num;
	do {
		printf("Enter a value: ");
		scanf("%f", &num);
		printf("\t%.2f in = %.2f cm\n", num, num * 2.54);
		printf("\t%.2f cm = %.2f in\n\n", num, num * 0.393701);
	} while (1);
}

int main()
{
	converter();
	return 0;
}
