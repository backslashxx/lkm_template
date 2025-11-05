#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdint.h>

// compile with -Wno-format

int main(int argc, char *argv[]) {
	if (argc < 5) {
		// count is the number of arguments.
		// we pass this as delimiter
		printf("Usage: %s <option> <option> <count> <string> ...\n", argv[0]);
		return 1;
	}

	unsigned long magic1 = strtoul(argv[1], NULL, 0);
	unsigned long magic2 = strtoul(argv[2], NULL, 0);
	unsigned int cmd = strtoul(argv[3], NULL, 0);
	const char *arg = argv[4];

	if (cmd < 1) {
		printf("count is weird\n");
		return 1;
	}

	// pass argv[4] as reference, this is where our argument starts
	printf("SYS_reboot(%lu, %lu, %d, %p)\n", magic1, magic2, cmd, (void *)arg);
	syscall(SYS_reboot, magic1, magic2, cmd, (void *)arg);

	// if our arg contains our pointer then its good
	printf("reply: 0x%lx verdict: %s\n", *(uintptr_t *)arg, *(uintptr_t *)arg == (uintptr_t)arg ? "ok" : "fail" );
	
	return 0;

}
