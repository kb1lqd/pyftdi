/*
    Modified version of https://github.com/BuLogics/libftdi/blob/master/examples/bitbang_cbus.c
    BITMASK
    CBUS Bits
    3210 3210
    xxxx xxxx
    |    |------ Output Control 0->LO, 1->HI
    |----------- Input/Output   0->Input, 1->Output
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ftdi.h>

unsigned char VendorId = 0x0403;
unsigned char ProductId = 0x6015;

#define CBUS0_DIR 0x10
#define CBUS1_DIR 0x20
#define CBUS2_DIR 0x40
#define CBUS3_DIR 0x80
#define CBUS0_STATE 0x1
#define CBUS1_STATE 0x2
#define CBUS2_STATE 0x4
#define CBUS3_STATE 0x8

int main(void)
{
    struct ftdi_context *ftdi;
    int f;
    unsigned char buf[1];
    unsigned char bitmask;
    char input[10];

    if ((ftdi = ftdi_new()) == 0)
    {
        fprintf(stderr, "ftdi_new failed\n");
        return EXIT_FAILURE;
    }

    f = ftdi_usb_open(ftdi, 0x0403, 0x6015);
    if (f < 0 && f != -5)
    {
        fprintf(stderr, "unable to open ftdi device: %d (%s)\n", f, ftdi_get_error_string(ftdi));
        ftdi_free(ftdi);
        exit(-1);
    }
    printf("ftdi open succeeded: %d\n",f);

    while (1)
    {
        // Set bitmask from input
    	//printf("Set bitmask from input: ");
        //fgets(input, sizeof(input) - 1, stdin);
        //if (input[0] == '\n') break;
        //bitmask = strtol(input, NULL, 0);
    	bitmask = CBUS0_DIR | CBUS0_STATE;
        printf("Using bitmask 0x%02x\n", bitmask);
        f = ftdi_set_bitmode(ftdi, bitmask, BITMODE_CBUS);
        sleep(5);
        bitmask = CBUS0_DIR;
		printf("Using bitmask 0x%02x\n", bitmask);
		f = ftdi_set_bitmode(ftdi, bitmask, BITMODE_CBUS);
		sleep(5);
//        if (f < 0)
//        {
//            fprintf(stderr, "set_bitmode failed for 0x%x, error %d (%s)\n", bitmask, f, ftdi_get_error_string(ftdi));
//            ftdi_usb_close(ftdi);
//            ftdi_free(ftdi);
//            exit(-1);
//        }
    }
    printf("disabling bitbang mode\n");
    ftdi_disable_bitbang(ftdi);

    ftdi_usb_close(ftdi);
    ftdi_free(ftdi);

    return 0;
}
