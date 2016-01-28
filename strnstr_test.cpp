#include <stdio.h>

char* strnstr(char* haystack, int haystack_len, char* needle, int needle_len) {
	int found_bytes;
	int i;
	char* found_start;
	found_bytes = 0;
	found_start = 0;
	for (i = 0; i < haystack_len; ++i) {
		if (haystack[i] == needle[found_bytes]) {
			if (found_bytes == 0) {
				found_start = haystack + i;
			}
			found_bytes++;
			if (found_bytes == needle_len) {
				return found_start;
			}
		}
		else {
			found_bytes = 0;
			found_start = 0;
		}
	}
	return 0;
}

void test(char* input) {
	if (input == 0) {
		printf("not found\n");
	}
	else {
		printf("%s\n", input);
	}
}

int main() {
	char* s;
	test(strnstr("foo", 3, "f", 1));
	test(strnstr("foo", 3, "foo", 3));
	test(strnstr("foo", 3, "oo", 2));
	test(strnstr("abefgabcd", 9, "abcd", 4));
	test(strnstr("foo", 3, "b", 1));

}