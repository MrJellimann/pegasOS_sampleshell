#include <stdio.h>

#define BUFFER_SIZE 1024

int main(int argc, char * argv[])
{
	FILE *f;
	char buffer[BUFFER_SIZE];

	for (int i = 0; i < argc; i++)
		printf("%s, ", argv[i]);

	if (argc > 1)
	{
		for (int i = 0; i < argc-1; i++)
		{
			f = fopen(argv[i], "r");

			do
			{
				fgets(buffer, BUFFER_SIZE, f);
				printf("%s", buffer);
			}
			while (!feof(f));
		}
	}

	return 0;
}