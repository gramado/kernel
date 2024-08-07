// kioctl.h
// see: kioctls.c
// Created by Fred Nora.

#ifndef ____IOCTL_H
#define ____IOCTL_H    1

/*
 * Window/terminal size structure.  
 * This information is stored by the kernel
 * in order to provide a consistent interface.
 */

// Implementation of 'winsize' structure.
struct winsize_d
{
    unsigned short ws_row;  // Rows, in characters. 
    unsigned short ws_col;  // Columns, in characters. 
    unsigned short ws_xpixel;  // Horizontal size, pixels. 
    unsigned short ws_ypixel;  // Certical size, pixels.
};
#define winsize  winsize_d


/*
 * Pun for SUN.
 */
/* 
struct ttysize {
	unsigned short	ts_lines;
	unsigned short	ts_cols;
	unsigned short	ts_xxx;
	unsigned short	ts_yyy;
};
*/
//#define	TIOCGSIZE	TIOCGWINSZ
//#define	TIOCSSIZE	TIOCSWINSZ


/*
 * ??
 * Ioctl's have the command encoded in the lower word, and the size of
 * any in or out parameters in the upper word.  The high 3 bits of the
 * upper word are used to encode the in/out status of the parameter.
*/

#define IOCPARM_MASK    0x1fff    /* parameter length, at most 13 bits */
#define IOCPARM_LEN(x)  (((x) >> 16) & IOCPARM_MASK)
#define IOCBASECMD(x)   ((x) & ~IOCPARM_MASK)
#define IOCGROUP(x)     (((x) >> 8) & 0xff)

//#define IOCPARM_MAX  NBPG      /* max size of ioctl, mult. of NBPG */
#define IOC_VOID     0x20000000  /* no parameters */
#define IOC_OUT      0x40000000  /* copy out parameters */
#define IOC_IN       0x80000000  /* copy in parameters */
#define IOC_INOUT    (IOC_IN|IOC_OUT)
#define IOC_DIRMASK  0xe0000000  /* mask for IN/OUT/VOID */

#define TIOCM_LE    0001    /* line enable */
#define TIOCM_DTR   0002    /* data terminal ready */
#define TIOCM_RTS   0004    /* request to send */
#define TIOCM_ST    0010    /* secondary transmit */
#define TIOCM_SR    0020    /* secondary receive */
#define TIOCM_CTS   0040    /* clear to send */
#define TIOCM_CAR   0100    /* carrier detect */
#define TIOCM_CD    TIOCM_CAR
#define TIOCM_RNG   0200        /* ring */
#define TIOCM_RI    TIOCM_RNG
#define TIOCM_DSR    0400    /* data set ready */
                             /* 8-10 compat */

//#define TIOCPKT_DATA        0x00  /* data packet */
//#define TIOCPKT_FLUSHREAD   0x01  /* flush packet */
//#define TIOCPKT_FLUSHWRITE  0x02  /* flush packet */
//#define TIOCPKT_STOP        0x04  /* stop output */
//#define TIOCPKT_START       0x08  /* start output */
//#define TIOCPKT_NOSTOP      0x10  /* no more ^S, ^Q */
//#define TIOCPKT_DOSTOP      0x20  /* now do ^S ^Q */
//#define TIOCPKT_IOCTL       0x40  /* state change of pty driver */

#define TTYDISC   0  /* termios tty line discipline */
#define TABLDISC  3  /* tablet discipline */
#define SLIPDISC  4  /* serial IP discipline */

#endif  //____IOCTL_H

//
// End
//
