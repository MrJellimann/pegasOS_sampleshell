#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_DEBUG 0
#define HASH_DEBUG 0
#define MINIHASH_DEBUG 0

typedef struct hash
{
	unsigned char sum[32];
} hash;

typedef struct minihash
{
	unsigned char sum[4];
} minihash;

minihash grabIntValues(int x)
{
	minihash result;
	unsigned char tArr[4];
	int temp = x;

	// for (int i = 0; i < sizeof(int); i++)
	// {
	// 	tArr[i] = (unsigned char)((temp >> ((sizeof(int) * 8) - (i * 8))) & 0xFF);
	// }

	tArr[0] = (x >> 24) & 0xFF;
	tArr[1] = (x >> 16) & 0xFF;
	tArr[2] = (x >> 8) & 0xFF;
	tArr[3] = (x >> 0) & 0xFF;

	// printf("%02x %02x %02x %02x\n", tArr[0], tArr[1], tArr[2], tArr[3]);

	result.sum[0] = tArr[0];
	result.sum[1] = tArr[1];
	result.sum[2] = tArr[2];
	result.sum[3] = tArr[3];

	return result;
}

void printMinihash(minihash h)
{
	printf("%02x %02x %02x %02x\n", h.sum[0], h.sum[1], h.sum[2], h.sum[3]);
}

void helpme()
{
	int num = 0x12345678;

	for (int i = 0; i < sizeof(int); i++)
	{
		printf("%x\n", num);
		num = num >> (sizeof(char) * 8);
	}

	printf("Final: %x\n", num);
}

void printHexArray(minihash arr)
{
	for (int i = 0; i < sizeof(int); i++)
	{
		printf("%x ", arr.sum[i]);
	}

	printf("\n");
}

int main(int argc, char *argv[])
{
	int r[8];

	if (argc > 1)
	{
		for (int arg = 1; arg < argc; arg++)
		{
			// Initialize our base numbers
			for (int i = 0; i < 4; i++)
				r[i] = 0;

			// Get our first number
			for (int i = 0; i < strlen(argv[arg]); i++)
			{
				r[0] += (int)((argv[arg][i])*(argv[arg][i]));
			}

			// Get our second number
			for (int i = 0; i < strlen(argv[arg]); i++)
			{
				if (i % 2 == 0)
					r[1] += (int)(argv[arg][i]);
				else
					r[1] *= (int)(argv[arg][i]);
			}

			// Get our third number
			for (int i = 0; i < strlen(argv[arg]); i++)
			{
				if (i == 0)
					r[2] = (int)(argv[arg][i]);
				else
					r[2] *= (int)(argv[arg][i]);
			}

			// Get our fourth number
			for (int i = 0; i < strlen(argv[arg]); i++)
			{
				if (i % 3 == 0)
					r[3] += (int)(argv[arg][i]);
				else if (i % 3 == 1)
					r[3] *= (int)(argv[arg][i]);
				else
					r[3] -= (int)(argv[arg][i]);
			}

			// Get our fifth number
			for (int i = 0; i < strlen(argv[arg]); i++)
			{
				if (i % 3 == 0)
					r[4] -= (int)(argv[arg][i]);
				else if (i % 3 == 1)
					r[4] *= (int)(argv[arg][i]);
				else if (i % 3 == 2)
					r[4] += (int)((argv[arg][i]) * (argv[arg][i]));
			}

			// Get our sixth number
			for (int i = 0; i < strlen(argv[arg]); i++)
			{
				if (i == 0)
					r[5] = (int)(argv[arg][i]);
				else if (i == 1)
					r[5] += (int)(argv[arg][i]);
				else
				{
					r[5] *= (int)(argv[arg][i]);
					r[5] += (int)(argv[arg][i-1]);
					r[5] *= (int)(argv[arg][i-2]);
				}
			}

			// Get our seventh number
			for (int i = 0; i < strlen(argv[arg]); i++)
			{
				if (i == 0)
					r[6] = (int)(argv[arg][i] * argv[arg][i] * argv[arg][i]);
				else
				{
					r[6] *= r[6];
					r[6] /= (int)(argv[arg][i]);
				}
			}

			// Get our eigth number
			for (int i = 0; i < strlen(argv[arg]); i++)
			{
				if (i % 4 == 0)
					r[7] += (int)(0xFF - argv[arg][i]);
				else if (i % 4 == 1)
				{
					r[7] *= (int)(argv[arg][i]);
					r[7] += r[7] * (int)(argv[arg][i-1]);
				}
				else if (i % 4 == 2)
					r[7] -= (int)(argv[arg][i]);
				else
					r[7] *= (r[7] + ((int)argv[arg][i]));
			}

			if (NUM_DEBUG)
			{
				// Print out all of the numbers we have
				for (int i = 0; i < 8; i++)
				{
					printf("%i", r[i]);

					if (i < 3)
						printf("\n");
					else
						printf("\n\n");
				}
			}

			if (HASH_DEBUG)
			{
				for (int i = 0; i < 8; i++)
				{
					printMinihash(grabIntValues(r[i]));
				}
				printf("\n");
			}

			hash myHash;
			int count = 0;

			for (int i = 0; i < 8; i++)
			{
				minihash t = grabIntValues(r[i]);
				myHash.sum[count++] = t.sum[0];
				myHash.sum[count++] = t.sum[1];
				myHash.sum[count++] = t.sum[2];
				myHash.sum[count++] = t.sum[3];

				if (MINIHASH_DEBUG)
				{
					printf("%02x%02x%02x%02x", t.sum[0], t.sum[1], t.sum[2], t.sum[3]);
				}
			}
			if (MINIHASH_DEBUG)
				printf("\n");

			// Print out our hash value
			for (int i = 0; i < 32; i++)
			{
				printf("%02x", myHash.sum[i]);
			}
			printf("\n");
		}
	}

	return 0;
}