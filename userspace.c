#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdint.h>

// always use u64 for pointers regardless, this way we wont have any
// issues like that nasty 32-on-64 pointer mismatch.
struct basic_payload {
	uint64_t reply_ptr;
	char text[256];
};

int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("Usage: %s <option> <option> <string>\n", argv[0]);
		return 1;
	}

	unsigned long magic1 = strtoul(argv[1], NULL, 0);
	unsigned long magic2 = strtoul(argv[2], NULL, 0);
	const char *arg = argv[3];
	
	unsigned long reply_value = 0; // we get reply on this

	struct basic_payload payload;
	payload.reply_ptr = (uint64_t)&reply_value;
	strncpy(payload.text, arg, sizeof(payload.text) - 1);
	payload.text[sizeof(payload.text) - 1] = '\0';

	printf("SYS_reboot(%lu, %lu, 0, %p)\n", magic1, magic2, (void *)&payload);
	syscall(SYS_reboot, magic1, magic2, 0, (void *)&payload);

	// pointer reuse here
	// force cast to uint to satisfy -Wpedantic
	printf("reply: 0x%x\n", *(unsigned int *)&payload);
	
	return 0;

}
