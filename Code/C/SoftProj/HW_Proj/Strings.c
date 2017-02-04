#include <stdio.h>
#include <stdlib.h>

void ln(int *len) {
	/* returns the length of the string */
	printf("%d", *len);
	printf("%c", '\n');

}

void rv(int *len, char *str){
	/* reverses the string */
	int i, mid;
	int size = *len;
	char start, finish;
	mid = (size / 2);
	for (i = 0; i < mid; ++i){/*run until half the word (out to middle) */
		start = str[i];/*repalce opposite char*/
		finish = str[size - i - 1]; /*repalce opposite char*/
		str[i] = finish;
		str[size - i - 1] = start;
	}

}

void ml(int *len, char* str, int k, char *newstr) {
	/* concatanates the current sting to itself k-1 times */
	int stringlen = (*len);
	int j;
	int l = 0;
	while (l < stringlen * k) {/*multiply the word k times and concate it*/
		for (j = 0; j < stringlen; j++) {
			newstr[l] = str[j];
			l++;
		}
	}
}

int fd(int len, char *str, int *subl, char *sub, int index){
	/* search for a substring in a string */
	int i,k;
	if (*subl <= len){/*if sub size is legal*/
		for (i=index; i < len; ++i){
			if (str[i] == sub[0]){/*find first char match*/
				k = 0;
				while (k < *subl){/*comapre rest of sub string*/
					if (str[i + k] == sub[k]){
						k++;
					}
					else/*break on first mismatch*/
						break;
				}
				if (k == *subl){/*if sub found return start insdex*/
					return i;
				}
			}
		}
	}
	return -1;/*if no match found*/

}

char* rp(int *len, char *str, int *first, int *last, int *substrlen, char *substr){
	/* replaces the subsequence between indexes i (first) and j (last) with x (substr) */
	int index, newstrsize, subIndex, tempIndex;
	char *newstr;
	if (*first > *len - 1) {/*check legal range*/
		printf("Index %d is out of range\n", *first);
		newstr=calloc(*len + 1, sizeof(char));
		if (newstr == NULL) {
			/*exit on error: Failed to allocate memory*/
			printf("Failed to allocate memory\n");
			exit(0);
		}
		for (index=0; index<*len; ++index){
			newstr[index]=str[index];
		}
	}
	else if (*last > *len - 1) {/*check legal range*/
		printf("Index %d is out of range\n", *last);
		newstr=calloc(*len + 1, sizeof(char));
		if (newstr == NULL) {
			/*exit on error: Failed to allocate memory*/
			printf("Failed to allocate memory\n");
			exit(0);
		}
		for (index=0; index<*len; ++index){
			newstr[index]=str[index];
		}
	}
	else {
		if (*first > *last) {/*switch between the indexes*/
			tempIndex = *last;
			last = &*first;
			first = &tempIndex;
		}
		newstrsize = *len - (*last - *first + 1) + *substrlen;
		newstr = calloc(newstrsize + 1, sizeof(char));
		if (newstr == NULL) {
			/*exit on error: Failed to allocate memory*/
			printf("Failed to allocate memory\n");
			exit(0);
		}
		for (index = 0; index < *first; ++index) { /*before first index*/
			newstr[index] = str[index];
		}
		for (subIndex = 0; subIndex < *substrlen; ++subIndex) { /*between first and last*/ 
			newstr[*first + subIndex] = substr[subIndex];
		}
		tempIndex =(*first + *substrlen);/*last latter index on the new string*/
		for (index = 0; index < *len - *last; ++index) { /*after last index*/
			newstr[tempIndex + index] = str[*last + 1 + index];
		}
		*len = newstrsize;
		printf("Current string is %s\n", newstr);
	}
	return newstr;
}

char* ra(int *len, char *str, int *sublen, char *substr, int *replen, char *repstr){
	/* replaces all instances of one substring with another */
	int i, j, ins,index;
	char *newstr;
	int dif = 0;
	ins = (int) fd(*len, str, &*sublen, substr,0);
	if (ins==-1){/*sub string doesn't exist*/
		newstr=calloc(*len + 1, sizeof(char));
		if (newstr == NULL) {
			/*exit on error: Failed to allocate memory*/
			printf("Failed to allocate memory\n");
			exit(0);
		}
		for (index=0; index<*len; ++index){
			newstr[index]=str[index];
		}
	}
	else{
	while (ins != -1){/*run until every sub string is replaced*/
		j = 0;
		if (*sublen < *replen){/*replace sub with longer string*/
			dif = (*replen - *sublen);
			newstr = calloc(*len + dif + 1, sizeof(char));
			if (newstr == NULL) {
				/*exit on error: Failed to allocate memory*/
				printf("Failed to allocate memory\n");
				exit(0);
			}
			while (j < ins){/*move before changing point*/
				newstr[j] = str[j];
				++j;
			}
			i = 0;
			while (i <= *replen){/*insert new sub string to str*/
				newstr[i + ins] = repstr[i];
				++i;
			}
			j = ins + *sublen;
			while (j <= *len){/*move chars after new sub string*/
				newstr[j + dif] = str[j];
				++j;
			}
			*len = *len + dif;
			str = newstr;
		}
		else{ /*replace sub with shorter or same size string*/
			dif = (*sublen - *replen);
			newstr = calloc(*len - dif + 1, sizeof(char));
			if (newstr == NULL) {
				/*exit on error: Failed to allocate memory*/
				printf("Failed to allocate memory\n");
				exit(0);
			}
			j = 0;
			while (j < ins){/*move chars before changing point*/
				newstr[j] = str[j];
				++j;
			}
			i = 0;
			while (i < *replen){/*add new sub string*/
				newstr[i + ins] = repstr[i];
				++i;
			}
			j = ins + *replen;
			while (j <= *len){/*move chars after new sub string*/
				{
					newstr[j] = str[j + dif];
					++j;
				}

			}
			*len = *len - dif;
			str = newstr;
		}
		ins = fd(*len, str, &*sublen, substr,ins+*replen);/*find next sub string*/
	}
	}
	return newstr;
}

char* tr(int *len, char *str, int *tr_i, int *tr_j) {
	/* trims the strings between two indexes */
	int temp, newstrsize, index;
	char *newstr;
	if (*tr_i > *len - 1) {/*check legal range*/
		printf("Index %d is out of range\n", *tr_i);
		newstr=calloc(*len + 1, sizeof(char));
		if (newstr == NULL) {
			/*exit on error: Failed to allocate memory*/
			printf("Failed to allocate memory\n");
			exit(0);
		}
		for (index=0; index<*len; ++index){
			newstr[index]=str[index];
		}
	}
	else if (*tr_j > *len - 1) {/*check legal range*/
			printf("Index %d is out of range\n", *tr_j);
			newstr=calloc(*len + 1, sizeof(char));
			if (newstr == NULL) {
				/*exit on error: Failed to allocate memory*/
				printf("Failed to allocate memory\n");
				exit(0);
			}
			for (index=0; index<*len; ++index){
				newstr[index]=str[index];
			}
		}
	else{
			if (*tr_i > *tr_j) {/*change indexes*/
				temp = *tr_j;
				tr_j = &*tr_i;
				tr_i = &temp;
			}
			newstrsize = (*tr_j - *tr_i + 1);
			newstr = calloc(newstrsize + 1, sizeof(char));
			if (newstr == NULL) {
				/*exit on error: Failed to allocate memory*/
				printf("Failed to allocate memory\n");
				exit(0);
			}
			if (newstr == NULL) {/*check memory error*/
				printf("Failed to allocate memory\n");
				return str;
			}
			for (index = 0; index < newstrsize; ++index) {/*create trim string*/
				newstr[index] = str[index + *tr_i];
			}
			*len = newstrsize;
			printf("Current string is %s\n", newstr);
		}
	return newstr;
}

void hs(int *len, char *str){
	/* returns the histogram of the string */
	int *app = calloc(28, sizeof(int));/*create an array of 28 cell for each eng alph char*/
	int i;
	for (i = 0; i < *len; ++i){/*update each cell by char found in the string*/
		app[str[i] - 97] = app[str[i] - 97] + 1;
	}
	for (i = 0; i < 28; ++i){/*print all occurrence of the str chars by alph order*/
		if (app[i]>0)
			printf("%d\t", app[i]);
	}
	printf("\n");
	for (i = 0; i < 28; ++i){/*print all the str chars by alph order*/
		if (app[i]>0)
			printf("%c\t", i + 97);
	}
	free(app);
	printf("\n");
}

int main(void){

	char *str, *substr, *repstr, *tempstr;
	int strsize, *len;
	int substrlen, repstrlen, newstrlen;
	int index, firstIndex, lastIndex, tr_i, tr_j;

	char com[3];

	len = &strsize;
	printf("Enter your string:\n");
	scanf("%d", &strsize);/*scan word length*/
	str = calloc(strsize + 1, sizeof(char));
	scanf("%s", &*str);/*scan the word*/

	if (str == NULL) {
		/*exit on error: Failed to allocate memory*/
		printf("Failed to allocate memory\n");
		exit(0);
	}


	while (com[0] != 'e' && com[1] != 'x'){/*run until 'ex' is excepted*/
		printf(">");
		scanf("%s", &*com);/*scan current command*/

		if (com[0] == 'l' && com[1] == 'n') {
			/* returns the length of the string */
			ln(len);
			continue;
		}
		if (com[0] == 'r' && com[1] == 'v'){
			/* reverses the string */
			rv(len, str);
			printf("Current string is %s\n", str);
			continue;
		}
		if (com[0] == 'm' && com[1] == 'l') {
			/* concatanates the current sting to itself k-1 times */
			scanf("%d", &newstrlen);
			tempstr = str;
			str = calloc((*len) * newstrlen + 1, sizeof(char));
			if (str == NULL) {/*exit on error: Failed to allocate memory*/
				printf("Failed to allocate memory\n");
				exit(0);
			}
			ml(len, tempstr, newstrlen, str);
			printf("Current string is %s\n", str);
			*len *= newstrlen;
			free(tempstr);
			continue;
		}
		if (com[0] == 'f' && com[1] == 'd'){
			/* search for a substring in a string */
			scanf("%d", &substrlen);
			substr = calloc(substrlen + 1, sizeof(char));
			if (substr == NULL) {/*exit on error: Failed to allocate memory*/
				printf("Failed to allocate memory\n");
				exit(0);
			}
			scanf("%s", &*substr);
			index = fd(*len, str, &substrlen, substr,0);
			printf("%d\n", index);
			free(substr);
			continue;
		}
		if (com[0] == 'r' && com[1] == 'p') {
			/* replaces the subsequence between indexes i and j with x */
			scanf("%d %d %d", &firstIndex, &lastIndex, &substrlen);
			substr = calloc(substrlen + 1, sizeof(char));
			scanf("%s", substr);
			if (substr == NULL) {/*exit on error: Failed to allocate memory*/
				printf("Failed to allocate memory\n");
				exit(0);
			}
			tempstr=str;
			str = rp(len, str, &firstIndex, &lastIndex, &substrlen, substr);
			free(tempstr);
			free(substr);
			continue;
		}
		if (com[0] == 'r' && com[1] == 'a'){
			/* replaces all instances of one substring with another */
			scanf("%d", &substrlen);
			substr = calloc(substrlen + 1, sizeof(char));
			if (substr == NULL) {/*exit on error: Failed to allocate memory*/
				printf("Failed to allocate memory\n");
				exit(0);
			}
			scanf("%s", &*substr);
			scanf("%d", &repstrlen);
			repstr = calloc(repstrlen + 1, sizeof(char));
			if (repstr == NULL) {/*exit on error: Failed to allocate memory*/
				printf("Failed to allocate memory\n");
				exit(0);
			}
			scanf("%s", &*repstr);
			tempstr=str;
			str = ra(len, str, &substrlen, substr, &repstrlen, repstr);
			printf("Current string is %s\n", str);
			free(tempstr);
			free(substr);
			free(repstr);
			continue;
		}
		if (com[0] == 't' && com[1] == 'r'){
			/* trims the strings between two indexes */
			scanf("%d %d", &tr_i, &tr_j);/*indexes for trimming*/
			tempstr=str;
			str = tr(len, str, &tr_i, &tr_j);
			free(tempstr);
			continue;
		}
		if (com[0] == 'h' && com[1] == 's'){
			/* returns the histogram of the string */
			hs(len, str);
			continue;
		}
		if (com[0] == 'n' && com[1] == 'w') {
			/* replaces the current string with a new one */
			scanf("%d", &newstrlen);
			tempstr=str;
			str = calloc(newstrlen + 1, sizeof(char));
			if (str == NULL) {/*exit on error: Failed to allocate memory*/
				printf("Failed to allocate memory\n");
				exit(0);
			}
			scanf("%s", str);
			*len = newstrlen;
			printf("Current string is %s\n", str);
			free(tempstr);
			continue;
		}
		else {
			/* unsupported operation/unknown command excepted */ 
			if (com[0] != 'e' && com[0] != 'x'){
				printf("Unsupported operation\n");
				continue;
			}
		}
	}
	free(str);
	return 0;
}

