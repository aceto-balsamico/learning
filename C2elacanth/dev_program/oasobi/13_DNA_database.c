#include "custom_common.h"

// 64種類のコドンをDNA塩基にマッピング
const char *codon_table[64] = {
	"AAA", "AAC", "AAG", "AAT", "ACA", "ACC", "ACG", "ACT",
	"AGA", "AGC", "AGG", "AGT", "ATA", "ATC", "ATG", "ATT",
	"CAA", "CAC", "CAG", "CAT", "CCA", "CCC", "CCG", "CCT",
	"CGA", "CGC", "CGG", "CGT", "CTA", "CTC", "CTG", "CTT",
	"GAA", "GAC", "GAG", "GAT", "GCA", "GCC", "GCG", "GCT",
	"GGA", "GGC", "GGG", "GGT", "GTA", "GTC", "GTG", "GTT",
	"TAA", "TAC", "TAG", "TAT", "TCA", "TCC", "TCG", "TCT",
	"TGA", "TGC", "TGG", "TGT", "TTA", "TTC", "TTG", "TTT"};

// コドンから元の値を取得するための逆変換
int decode_codon(const char *codon)
{
	for (int i = 0; i < 64; i++)
	{
		if (strncmp(codon, codon_table[i], 3) == 0)
		{
			return i;
		}
	}
	return -1;
}

// テキストをDNAエンコード
void encode_to_dna(const char *input, char *output)
{
	int len = strlen(input);
	int index = 0;
	for (int i = 0; i < len; i++)
	{
		unsigned char c = input[i]; // ASCIIを取得
		output[index++] = codon_table[(c >> 2) & 0x3F][0];
		output[index++] = codon_table[(c >> 2) & 0x3F][1];
		output[index++] = codon_table[(c >> 2) & 0x3F][2];
		output[index++] = codon_table[(c & 0x03) << 4][0];
		output[index++] = codon_table[(c & 0x03) << 4][1];
		output[index++] = codon_table[(c & 0x03) << 4][2];
	}
	output[index] = '\0';
}

// DNAをテキストにデコード
void decode_from_dna(const char *dna, char *output)
{
	int len = strlen(dna);
	int index = 0;
	for (int i = 0; i < len; i += 6)
	{
		int val1 = decode_codon(&dna[i]);
		int val2 = decode_codon(&dna[i + 3]);
		if (val1 == -1 || val2 == -1)
		{
			printf("Decoding error!\n");
			return;
		}
		output[index++] = ((val1 & 0x3F) << 2) | ((val2 & 0x30) >> 4);
	}
	output[index] = '\0';
}

//@@@function
void DNA_database()
{
	char text[] = "Hello, world!";
	char dna_output[256];
	char decoded_text[256];

	encode_to_dna(text, dna_output);
	printf("Original: %s\n", text);
	printf("DNA Encoded: %s\n", dna_output);

	decode_from_dna(dna_output, decoded_text);
	printf("Decoded: %s\n", decoded_text);

	return;
}
