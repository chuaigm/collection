#include <stdio.h>

int main()
{
	const char fn[] = "testfile.txt";
	FILE* fp=fopen(fn, "rb");
	for(int i=1; i<10; i++) {
		fwrite(&i, sizeof(int),1,fp);
	}
	fseek(fp, 0, SEEK_SET);

	int tmp=0;
	fread(&tmp, sizeof(int), 1, fp);
	printf("%d\n", tmp);
}
