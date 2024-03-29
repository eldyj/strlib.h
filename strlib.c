// requires libraries: stdio.h, stdlib.h, string.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strlib.h"

// String array functions
StringArray new_strarray() {
	StringArray sa;
	sa.array = NULL;
	sa.length = 0;
	return sa;
}

void sa_free(StringArray *sa) {
	if (sa->length == 0)
		return;
		
	for (unsigned long i = 0; i < sa->length; ++i) {
		free(sa->array[i]);
		sa->array[i] = NULL;
	}
	
	free(sa->array);
	sa->array = NULL;
	sa->length = 0;
}

void sa_add(StringArray *sa, char *str) {
	++sa->length;
	sa->array = realloc(sa->array, sa->length * sizeof(char*));
	sa->array[sa->length - 1] = malloc((strlen(str) + 1)*sizeof(char));
	strcpy(sa->array[sa->length - 1], str);
	strcat(sa->array[sa->length - 1], "\0");
}

void sa_add_sa(StringArray *sa, StringArray a) {
	for (unsigned long i = 0; i < a.length; ++i)
		sa_add(sa, sa_get(a, i));
}

void sa_set(StringArray *sa, unsigned int index, char *str) {
	if (index > sa->length)
		return;
	else if (index == sa->length)
		sa_add(sa, str);
	else {
		free(sa->array[index]);
		sa->array[index] = NULL;
		sa->array[index] = malloc((strlen(str) + 1) * sizeof(char));
		strcpy(sa->array[index], str);
		strcat(sa->array[index], "\0");
	}
}

void sa_delete(StringArray *sa, unsigned int index) {
	if (index >= sa->length)
		return;
		
	if (index + 1 == sa->length) {
		free(sa->array[index]);
		sa->array[index] = NULL;
	} else {
		for (unsigned int i = index; i<sa->length - 1; ++i) {
			free(sa->array[i]);
			sa->array[i] = NULL;
			sa->array[i] = malloc((strlen(sa->array[i + 1]) + 1) * sizeof(char));
			strcpy(sa->array[i], sa->array[i + 1]);
			strcat(sa->array[i], "\0");
		}
		free(sa->array[sa->length-1]);
	}
	sa->length--;
	sa->array = realloc(sa->array,sa->length * sizeof(char*));
}

char *sa_get(StringArray sa, unsigned int index) {
	if (index >= sa.length)
		return NULL;
	else
		return sa.array[index];
}

char *sa_join(StringArray array, char* sep) {
	string(result);
	for (unsigned long i = 0; i < array.length; ++i) {
		add_str(&result, sa_get(array, i));
		if (i != array.length - 1 && sep != NULL)
			add_str(&result, sep);
	}
	return result;
}

char *sa_repr(StringArray sa) {
	string(res);
	set_str(&res, "[");
	if (sa.length > 0) {
		for (unsigned long i = 0; i < sa.length; ++i) {
			add_str(&res, "'");
			add_str(&res, sa_get(sa, i));
			add_str(&res, "'");
			if (i < sa.length - 1)
				add_str(&res,", ");
		}
	}
	add_str(&res, "]");
	return res;
}

int sa_index_of(StringArray sa, char *str) {
	for (unsigned long i = 0; i < sa.length; ++i)
		if (equals_str(sa_get(sa, i), str))
			return i;
			
	return -1;
}

unsigned long sa_count(StringArray sa, char *str) {
	unsigned long res = 0;
	
	for (unsigned long i = 0; i < sa.length; ++i)
		if (equals_str(sa_get(sa, i), str))
			++res;
			
	return res;
}


bool sa_contains(StringArray sa, char *str) {
	return sa_index_of(sa, str) != -1;
}

void sa_remove(StringArray *sa, char *str) {
	while (sa_contains(*sa, str))
		sa_delete(sa, sa_index_of(*sa, str));
}

StringArray sa_slice_fromto(StringArray sa, unsigned int from, unsigned int to) {
	StringArray res = new_strarray();
	
	for (unsigned int i = from; i < to; ++i)
		sa_add(&res, sa_get(sa, i));
		
	return res;
}


StringArray sa_slice_from(StringArray sa, unsigned int from) {
	return sa_slice_fromto(sa, from, sa.length);
}


StringArray sa_slice_to(StringArray sa, unsigned int to) {
	return sa_slice_fromto(sa, 0, to);
}

void sa_insert(StringArray *sa, unsigned int to, char *a) {
	if (to >= sa->length)
		return sa_add(sa, a);
	
	StringArray before = sa_slice_to(*sa, to),
				after = sa_slice_from(*sa, to);
				
	sa_free(sa);
		
	if (before.length != 0)
		sa_add_sa(sa, before);
			
	sa_add(sa, a);
		
	if (after.length != 0)
		sa_add_sa(sa, after);
			
	sa_free(&before);
	sa_free(&after);
}

void sa_insert_sa(StringArray *sa, unsigned int to, StringArray a) {
	if (to < sa->length) {
		StringArray before = sa_slice_to(*sa, to),
					after = sa_slice_from(*sa, to);
					
		sa_free(sa);
		
		if (before.length)
			sa_add_sa(sa, before);
			
		if (a.length)
			sa_add_sa(sa, a);
			
		if (after.length)
			sa_add_sa(sa, after);
			
		sa_free(&before);
		sa_free(&after);
	} else if (to == sa->length && a.length)
			sa_add_sa(sa, a);
}

// String functions
// Sets the string value
void set_str(char **str, char *a) {
	if (*str != NULL) free(*str);
	*str = NULL;
	*str = malloc((strlen(a) + 1) * sizeof(char));
	strcpy(*str, a);
	strcat(*str, "\0");
}
// Appends string
void add_str(char **str, char *a) {
	if (*str != NULL) {
		*str = realloc(*str, (strlen(*str) + strlen(a) + 1) * sizeof(char));
		strcat(*str, a);
		strcat(*str, "\0");
	} else
		set_str(str, a);
}

// Concatenates two strings
char *concat(char *a, char *b) {
	string(res);
	set_str(&res, a);
	add_str(&res, b);
	return res;
}

// string reverse
char *reverse_str(char *a) {
	string(result);
	set_str(&result, a);
		
	for (long i = strlen(a) - 1; i > -1; --i)
		result[strlen(a) - i - 1] = a[i];
		
	add_str(&result, "\0");
	return result;
}

// compares two strings
bool equals_str(char *a, char *b) {
	return (a == NULL && b == NULL) || (a != NULL && b != NULL && strcmp(a, b) == 0);
}

bool starts_with(char *a, char *b) {
	char *s = bash_slice(a, 0, strlen(b));
	bool result = equals_str(b, s);
	free(s);
	return result;
}

bool ends_with(char *a, char *b) {
	return starts_with(reverse_str(a), reverse_str(b));
}

// slice from Bash: ${str:$a:$c}
char *bash_slice(char *str, unsigned int a, unsigned int c) {
	string(result);
	unsigned long length = 0, to = a + c;
	if (to >= strlen(str))
		to = strlen(str);
	for (unsigned long i = a; i < to; ++i) {
		++length;
		result = realloc(result, length * sizeof(char));
		result[length - 1] = str[i];
	}
	++length;
	result = realloc(result, length * sizeof(char));
	result[length - 1] = '\0';
	return result;
}

// Counts substrings in the string
unsigned int count_substr(char *a, char *b) {
	unsigned int result = 0;
	for (unsigned int i = 0; i < strlen(a); ++i) {
		char *s = bash_slice(a, i, strlen(b));
		if (equals_str(s, b))
			++result;
		free(s);
	}
	return result;
}

bool contains_substr(char *a, char *b) {
	return a != NULL && b != NULL && strstr(a, b) != 0;
}

bool consists_of(char *a, char *b) {
	return count_substr(a, b) * strlen(b) == strlen(a);
}

// returns a slice of the string from a to b
char *slice_fromto(char *str, int a, int b) {
	char *result = malloc((b - a + 1) * sizeof(char));
	int index = 0;
	for (int i = a; i < b; ++i) {
		result[index] = str[i];
		++index;
	}
	result[index] = '\0';
	return result;
}

char *slice_from(char *str, unsigned int a) {
	return slice_fromto(str, a, strlen(str));
}

char *slice_to(char *str, unsigned int a) {
	return slice_fromto(str, 0, a);
}

int index_of(char *a, char *b, unsigned int entry) {
	size_t entry_of = 0;
	for (size_t i = 0; i < strlen(a); ++i) {
		char *s = bash_slice(a, i, strlen(b));
		if (equals_str(s, b)) {
			++entry_of;
			free(s);
			
			if (entry_of == entry)
				return i;
		} else
			free(s);
	}
	return -1;
}

int index_of_char(char *a, char b, unsigned int entry) {
	unsigned long count = 0;
	for (unsigned long i = 0; i < strlen(a); ++i) {
		if (a[i] == b) {
			++count;
			if (count == entry)
				return i;
		}
	}
	return -1;
}

char *strip(char *str, char a) {
	size_t start = 0, end = strlen(str);
	
	while (str[start] == a && start < strlen(str))
		++start;
		
	while (str[end - 1] == a && end > 0)
		--end;
		
	if (start == end)
		return NULL;
	
	return slice_fromto(str, start, end);
}

// split the string `a` by the substring `b`
StringArray split(char*str, char*b) {
	StringArray res =  new_strarray();
	unsigned long count_b = count_substr(str, b),
					 blen = strlen(b),
				   lenstr = strlen(str),
				 *indexes = malloc(count_b * sizeof(unsigned long));
	
	if (count_b > 0) {
		for (unsigned long i = 0; i < count_b; ++i) {
			if (i == 0)
				indexes[i] = index_of(str, b, 1);
			else {
				char *s0 = slice_from(str, indexes[i - 1] + blen);
				indexes[i] = index_of(s0, b, 1) + indexes[i - 1] + blen;
				free(s0);
			}
		}
		
		if (indexes[0] != 0) {
			char *s1 = slice_to(str, indexes[0]);
			sa_add(&res, s1);
			free(s1);
		}
		
		for (unsigned long i = 0; i < count_b - 1; ++i) {
			if (indexes[i] + blen == indexes[i + 1])
				continue;

			char *s2 = slice_fromto(str, indexes[i] + blen, indexes[i + 1]);
			sa_add(&res, s2);
			free(s2);
		}
		
		if (indexes[count_b - 1] + blen < lenstr) {
			char *s3 = slice_from(str, indexes[count_b - 1] + blen);
			sa_add(&res, s3);
			free(s3);
		}
	} else
		sa_add(&res, str);

	free(indexes);
	return res;
}

char *replace(char *str, char *b, char *c) {
	unsigned long count_b = count_substr(str, b),
		blen = strlen(b),
		lenstr = strlen(str),
		*indexes = malloc(count_b * sizeof(unsigned long));
		
	string(result);
	
	if (!count_b) {
		result = malloc((lenstr + 1) * sizeof(char));
		strcpy(result, str);
		strcat(result, "\0");
	} else {
		for (unsigned long i = 0; i < count_b; ++i) {
			if (i == 0)
				indexes[i] = index_of(str, b, 1);
			else {
				char *s1 = slice_from(str, indexes[i - 1] + blen);
				indexes[i] = index_of(s1, b, 1) + indexes[i - 1] + blen;
				free(s1);
			}
		}
		
		if (indexes[0] != 0)
			add_str(&result, slice_to(str, indexes[0]));
		
		for (unsigned long i = 0; i < count_b - 1; ++i) {
			add_str(&result, c);
			
			if (indexes[i] + blen == indexes[i + 1])
				continue;
			
			char *s2 = slice_fromto(str, indexes[i] + blen, indexes[i + 1]);
			add_str(&result, s2);
			free(s2);
		}
		
		add_str(&result, c);
		
		if (indexes[count_b - 1] + blen < lenstr) {
			char *s3 = slice_from(str, indexes[count_b - 1] + blen);
			add_str(&result, s3);
			free(s3);
		}
		
		free(indexes);
	}
	return result;
}

// TODO: check for memory leaks
StringArray split_sa(char *a, StringArray sa) {
	StringArray res = new_strarray();
	
	if (sa.length == 1)
		sa_add_sa(&res, split(a, sa_get(sa, 0)));
	else if (!sa.length)
		sa_add(&res, a);
	else {
		string(res_str);
		set_str(&res_str, a);
		
		for (unsigned long i = 1; i < sa.length; ++i) {
			char *s0 = replace(res_str, sa_get(sa, i), sa_get(sa, 0));
			set_str(&res_str, s0);
			free(s0);
		}

		StringArray sa = split(res_str, sa_get(sa, 0));
		sa_add_sa(&res, sa);
		free(sa.array);
		sa.length = 0;
	}
	return res;
}

char *minus_str(char *a, char *b) {
	string(res);
	StringArray splitted = split(a, b);

	for (unsigned long i = 0; i<splitted.length; ++i)
		add_str(&res, sa_get(splitted, i));

	sa_free(&splitted);

	return res;
}

char *mult_str(char *str, unsigned int c) {
	string(result);

	for (unsigned int i = 0; i < c; ++i)
		add_str(&result, str);

	return result;
}

// Functions for working with files
char *read_to(FILE *file, char to) {
	string(res);
	int len = 0;
	char ch;
	
	if (file == NULL)
		return NULL;
		
	while (1) {
		ch = getc(file);
		if (ch == to) {
			++len;
			res = realloc(res, len * sizeof(char));
			res[len - 1] = '\0';
			break;
		}
		++len;
		res = realloc(res, len * sizeof(char));
		res[len - 1] = ch;
	}
	return res;
}

char *read_file(FILE *file) {
	return read_to(file, EOF);
}

char *read_line(FILE *file) {
	return read_to(file, '\n');
}
