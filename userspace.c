#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("Usage: %s <option> <option> <string>\n", argv[0]);
		return 1;
	}

	unsigned long magic1 = strtoul(argv[1], NULL, 0);
	unsigned long magic2 = strtoul(argv[2], NULL, 0);
	const char *arg = argv[3];

	printf("SYS_reboot(%lu, %lu, 0, %p)\n", magic1, magic2, (void *)arg);
	syscall(SYS_reboot, magic1, magic2, 0, (void *)arg);

	// if our arg contains our pointer then its good
	printf("reply: 0x%lx verdict: %s\n", *(uint64_t *)arg, *(uint64_t *)arg == (uint64_t)arg ? "ok" : "fail" );
	
	return 0;

}
