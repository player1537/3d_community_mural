#include <stdio.h>
#include <ospray/ospray.h>

int
main(int argc, const char **argv) {
	OSPError error;
	FILE *infolog, *errorlog;
	
	infolog = stdout;
	errorlog = stderr;
	
	fprintf(infolog, "Initializing OSPRay\n");
	error = ospInit(&argc, argv);
	if (error != OSP_NO_ERROR) {
		fprintf(errorlog, "Error: ospInit %d\n", error);
		return 1;
	}
	
	fprintf(infolog, "Shutting down OSPRay\n");
	ospShutdown();
	
	return 0;
}
