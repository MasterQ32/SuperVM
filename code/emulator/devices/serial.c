#include "serial.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/select.h>

struct remote {
	int sock;
};

static void serial_update(devserial_t *serial)
{
	fd_set set;
	struct timeval timeout;
	FD_ZERO(&set); /* clear the set */
	FD_SET(serial->sock, &set); /* add our file descriptor to the set */

	timeout.tv_sec = 0;
	timeout.tv_usec = 1;

	int rv = select(serial->sock + 1, &set, NULL, NULL, &timeout);
	if(rv > 0)
	{
    int client = accept (serial->sock, NULL, NULL);
		
		if(serial->remote != 0) {
			const char *msg = "Another client is already connected.\nGood Bye!\n";
			write(client, msg , strlen(msg));
			close(client);
		} else {
			serial->remote = client;
		}
	}
}

static uint32_t serial_read(devserial_t *serial, uint16_t reg)
{
	uint8_t bits;
	switch (reg)
	{
	case SERIAL_REG_DATA: 
		if(serial->remote >= 0) {
			read(serial->remote, &bits, 1);
			return bits;
		} else {
			return -1;
		}
	default:
		return -1;
	}
}

static void serial_write(devserial_t *serial, uint16_t reg, uint32_t value)
{
	uint8_t bits = value;
	switch (reg)
	{
	case SERIAL_REG_DATA:
		if(serial->remote >= 0) {
			write(serial->remote, &bits, 1);
		}
		break;
	}
}

static void serial_shutdown(devserial_t *serial)
{
	if(serial->remote >= 0) {
		close(serial->remote);
	}
	close(serial->sock);
	if(unlink(serial->sockName) == -1) {
		fprintf(stderr, "Failed to delete %s\n", serial->sockName);
	}
}

extern char basePath[];

device_t *devserial_create()
{
	devserial_t *serial = malloc(sizeof(devserial_t));
	
	strcpy(serial->device.name, "Serial Port v1.0");
	serial->device.shutdown = (void(*)(device_t*))serial_shutdown;
	serial->device.update = (void(*)(device_t*))serial_update;
	serial->device.read = (uint32_t(*)(device_t*,uint16_t))serial_read;
	serial->device.write = (void(*)(device_t*, uint16_t, uint32_t))serial_write;

	serial->sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if(serial->sock == -1) {
		fprintf(stderr, "failed to create socket.\n");
	}
	
	sprintf(serial->sockName, "%sCOM", basePath);
	
	/* Clear structure */
	struct sockaddr_un my_addr;
	memset(&my_addr, 0, sizeof(struct sockaddr_un));
	my_addr.sun_family = AF_UNIX;
	strncpy(
		my_addr.sun_path, 
		serial->sockName,
		sizeof(my_addr.sun_path) - 1);

	if (bind(serial->sock, (struct sockaddr *) &my_addr, sizeof(struct sockaddr_un)) == -1) {
		fprintf(stderr, "failed to bind socket.\n");
	}
	
	if (listen(serial->sock, 10) == -1){
		fprintf(stderr, "failed to listen.\n");
	}
	
	fprintf(stdout, "Serial port listening on %s\n", serial->sockName);
	
	return &serial->device;
}