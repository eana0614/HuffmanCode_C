#define _CRT_SECURE_NO_DEPRECATE

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<memory.h>

struct node {
	char c;
	int freq;
	struct node *left;
	struct node *right;
	char path[14];

};

typedef struct node Node;
int treeLength = 27;

int initArray(Node* arr, int lenght);
void countTreeElement(char* filename, Node* t);
void sortTree(Node *t);
Node* extractMin(Node* q);
void insertTree(Node *q, Node z);
Node* huffman(Node* t);
void createTable(Node* t, Node* table);
void writeTable(char* filename, Node* table);
void encodingHuffman(char* read, char* write, Node *ta);
void decodingHuffman(char* read, char* write, Node *ta);

int main(void) {

	char* original = "original.txt";
	char* encodingTxt = "encoding.txt";
	char* decodingTxt = "decoding.txt";
	char* tableTxt = "table.txt";

	Node *tree = (Node*)malloc(sizeof(Node)*27);
	Node *table = (Node*)malloc(sizeof(Node) * 27);

	printf("[ Encoding > Decoding > Exit. ]\n");

	printf("!] Add Tree Element \n");
	initArray(tree, 27);
	countTreeElement(original, tree);
	sortTree(tree);

	printf("!] Run Huffman \n");
	tree = huffman(tree);
	tree = tree->left;

	printf("!] Create Table. \n");
	createTable(tree, table);
	writeTable(tableTxt, table);

	printf("!] Encoding. \n");
	encodingHuffman(original, encodingTxt, table);

	printf("!] Decoding. \n");
	decodingHuffman(encodingTxt, decodingTxt, table);

	printf("!] Done. \n");
}

void decodingHuffman(char* read, char* write, Node *ta) {
	int length;
	char temp;
	char str[30];
	char* re = (char*)malloc(sizeof(char));

	FILE *r = fopen(read, "r");
	FILE *w = fopen(write, "w");

	if (r == NULL || w == NULL) {
		exit(1);
	}

	strcpy(str, "");
	while (!feof(r)) {
		fscanf(r, "%c", &temp);
		re[0] = temp;
		strncat(str, re, 1);

		if (strlen(str) >= 3) {
			for (int i = 0; i < 27; i++) {
				length = strlen(ta[i].path);
				if (strcmp(str, ta[i].path, length) == 0) {
					fprintf(w, "%c", ta[i].c);
					strcpy(str, "");
					break;
				}
			}
		}
	}

	fclose(w);
	fclose(r);
}

void encodingHuffman(char* read, char* write, Node *ta) {
	char temp;

	FILE *r = fopen(read, "r");
	FILE *w = fopen(write, "w");

	if (r == NULL || w == NULL) {
		exit(1);
	}

	while (EOF != (temp = fgetc(r))) {
		if (temp == ' ') {
			fprintf(w, "%s", ta[0].path);
		}else {
			fprintf(w, "%s", ta[temp-'a'+1].path);
		}
	}

	fclose(w);
	fclose(r);
}

void writeTable(char* filename, Node* table) {
	FILE *file = fopen(filename, "w");

	if (file == NULL) {
		exit(1);
	}

	for(int i=0; i<27; i++){
		fprintf(file, "%c,%s", table[i].c, table[i].path);
	}

	fclose(file);
}

void createTable(Node* t, Node* table) {
	char str[30];
	char temp = t ->c;

	if (temp >= 'a' && temp <= 'z') {
		table[temp - 'a' + 1].c = temp;
		strcpy(table[temp - 'a' + 1].path, t ->path);
	}
	else if (temp == ' ') {
		table[0].c = ' ';
		strcpy(table[0].path, t->path);
	}

	if (t->left) {
		strcpy(str, t ->path);
		strcat(str, "0");
		strcpy((t->left)-> path, str);
		createTable(t->left, table);
	}

	if (t->right) {
		strcpy(str, t->path);
		strcat(str, "1");
		strcpy((t->right)->path, str);
		createTable(t->right, table);
	}
}

Node* huffman(Node* t) {
	Node z, *x, *y;
	Node *q = t;
	int n = treeLength;

	for (int i = 0; i < n; i++) {
		x = extractMin(q);
		z.left = x;
		y = extractMin(q);
		z.right = y;

		z.freq = x->freq + y->freq;
		strcpy(z.path, "");

		insertTree(q, z);
	}

	return extractMin(q);
}

void insertTree(Node *q, Node z) {
	int freq = z.freq;
	int i;

	for (i = 0; i < treeLength; i++) {
		if (q[i].freq > z.freq) {
			break;
		}
	}

	for (int j = treeLength + 1; j > i; j--) {
		q[j] = q[j - 1];
	}

	q[i] = z;
	treeLength++;
}

Node* extractMin(Node* q) {
	Node *temp = (Node*)malloc(sizeof(Node));
	temp[0] = q[0];

	for (int i = 0; i < treeLength; i++) {
		q[i] = q[i + 1];
	}

	treeLength--;

	return temp;
}

void sortTree(Node *t) {
	Node temp;
	int j;

	for (int i = 0; i < 27; i++) {
		temp = t[i];
		j = i - 1;

		while (j >= 0 && t[j].freq > temp.freq) {
			t[j + 1] = t[j];
			j = j - 1;
		}

		t[j + 1] = temp;
	}
}

void countTreeElement(char* filename, Node* t) {
	char temp;

	FILE *file = fopen(filename, "r");
	
	if (file == NULL) {
		exit(1);
	}

	while (EOF != (temp = fgetc(file))) {
		if (temp == ' ') {
			t[0].freq++;
		}
		else {
			t[temp - 'a' + 1].freq++;
		}
	}

	fclose(file);
}

int initArray(Node* arr, int lenght) {
	
	for (int i = 0; i < lenght; i++) {
		if (i == 0) {
			arr[0].c = ' ';
			arr[0].freq = 0;
			arr[0].left = NULL;
			arr[0].right = NULL;
			strcpy(arr[0].path, "");
		}
		else {
			arr[i].c = 'a' + i - 1;
			arr[i].freq = 0;
			arr[i].left = NULL;
			arr[i].right = NULL;
			strcpy(arr[i].path, "");
		}
	}
}