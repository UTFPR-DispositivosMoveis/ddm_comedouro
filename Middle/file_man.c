#include <stdio.h>
#include <stdlib.h>


/* Sistema para guardar dados dos parâmetros em arquivo */
int main(void) {
	FILE *ptr;
	ptr = fopen("configs.save", "r+");
	fprintf(ptr, "teste teste teste");
	fclose(ptr);
}