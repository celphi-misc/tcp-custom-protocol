// This file includes the custom protocol used in the experiment
// Note that the protocol transmits in big endian
// Below is the definition of our protocol: 

// Protocol format
// HEX  || BE EF |             |       |
// byte || -- -- | -- -- -- -- | -- -- | --...
// desc || Magic | Body Length | Type  | Body

typedef enum {
    REQ_TIME        = 0x0001,
    REQ_HOSTNAME    = 0x0002,
    REQ_SOCK_DESC   = 0x0003,
    REQ_SOCK_ALL    = 0x0004,
    REQ_CLIENT_IP   = 0x0005,
    REQ_CLIENT_PORT = 0x0006,
    REQ_SEND_MSG    = 0x0007,
    RPL_TIME        = 0x0101,
    RPL_HOSTNAME    = 0x0102,
    RPL_SOCK_DESC   = 0x0103,
    RPL_SOCK_ALL    = 0x0104,
    RPL_CLIENT_IP   = 0x0105,
    RPL_CLIENT_PORT = 0x0106,
    RPL_SEND_MSG    = 0x0107
} MessageType;

#define PROTOCOL_HEADER_LEN 8

// To test if the input is of our protocol
int is_custom_protocol(const char *buf);
// Get length of the body
int get_body_length(const char *buf);
// Get message type
MessageType get_msg_type(const char *buf);

// Write time request message to dest
int request_time_msg(char *dest);
// Write hostname request message to dest
int request_hostname(char *dest);

// Write time reply message to dest
int reply_time_msg(char *dest);
// Write hostname reply message to dest
int reply_hostname(char *dest, const char *src);