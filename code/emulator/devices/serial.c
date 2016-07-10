#include "serial.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static bool serial_select(int sock, int mode)
{
	fd_set set;
	struct timeval timeout;
	FD_ZERO(&set); /* clear the set */
	FD_SET(sock, &set); /* add our file descriptor to the set */

	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	int rv = select(
		sock + 1, 
		((mode == 0) ? &set : NULL), 
		((mode == 1) ? &set : NULL), 
		((mode == 2) ? &set : NULL), 
		&timeout);
	return rv > 0;
}

static void serial_update(devserial_t *serial)
{
	if(serial_select(serial->sock, 0))
	{
    int client = accept (serial->sock, NULL, NULL);
		
		if(serial->remote >= 0) {
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
		if(serial->remote >= 0 && serial_select(serial->remote, 0)) {
			if(read(serial->remote, &bits, 1) <= 0) {
				serial->remote = -1;
				return -1;
			}
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
			if(serial_select(serial->remote, 1)) {
				if(send(serial->remote, &bits, 1, MSG_NOSIGNAL) <= 0) {
					serial->remote = -1;
				}
			}
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
	
#if defined(USE_UNIX_SOCKET)
	unlink(serial->fileName);
#endif
}

extern char basePath[];

device_t *devserial_create(int portNum)
{

	devserial_t *serial = malloc(sizeof(devserial_t));
	
	strcpy(serial->device.name, "Serial Port v1.0");
	serial->device.shutdown = (void(*)(device_t*))serial_shutdown;
	serial->device.update = (void(*)(device_t*))serial_update;
	serial->device.read = (uint32_t(*)(device_t*,uint16_t))serial_read;
	serial->device.write = (void(*)(device_t*, uint16_t, uint32_t))serial_write;

	serial->remote = -1;
	
#if defined(USE_UNIX_SOCKET)

	snprintf(serial->fileName, PATH_MAX, "./COM%d", portNum);

	unlink(serial->fileName);

	serial->sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if(serial->sock == -1) {
		fprintf(stderr, "failed to create socket.\n");
	}
	
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(struct sockaddr_un));

	/* Bind socket to socket name. */

	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, serial->fileName, sizeof(addr.sun_path) - 1);  			      /* set the server port number */ 
	
	if (bind(serial->sock, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
		fprintf(stderr, "failed to bind socket.\n");
	};
	
	if (listen(serial->sock, 10) == -1){
		fprintf(stderr, "failed to listen.\n");
	}
	
	fprintf(stdout, "Serial port listening on %s\n", serial->fileName);
#else
	serial->sock = socket(AF_INET, SOCK_STREAM, 0);
	if(serial->sock == -1) {
		fprintf(stderr, "failed to create socket.\n");
	}
	
	struct sockaddr_in my_addr; /* socket info about our server */
	socklen_t socksize = sizeof(struct sockaddr_in);
	memset(&my_addr, 0, sizeof(my_addr));           /* zero the struct before filling the fields */
	my_addr.sin_family = AF_INET;                /* set the type of connection to TCP/IP */
	my_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //htonl(INADDR_ANY); /* set our address to any interface */
	my_addr.sin_port = htons(0);      			      /* set the server port number */ 
	
	if (bind(serial->sock, (struct sockaddr *) &my_addr, sizeof(my_addr)) == -1) {
		fprintf(stderr, "failed to bind socket.\n");
	};
	
	if (listen(serial->sock, 10) == -1){
		fprintf(stderr, "failed to listen.\n");
	}
	
	int len = sizeof(my_addr);
	if(getsockname(serial->sock, (struct sockaddr *)&my_addr, &len) == -1) {
		fprintf(stderr, "failed to get sock name.\n");
	}
	if(len != sizeof(my_addr)) {
		fprintf(stderr, "Invalid address length??.\n");
	}
	
	fprintf(stdout, "Serial port listening on %s:%d\n", 
		inet_ntoa(my_addr.sin_addr),
		ntohs(my_addr.sin_port));
#endif

	return &serial->device;
}