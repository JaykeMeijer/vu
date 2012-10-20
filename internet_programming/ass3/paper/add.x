struct paper {
    long len;
    string val<>;
};

struct str_add {
    string  name<>;
    string  title<>;
    struct paper p;
};

struct str_list {
    int     number;
    string  name<>;
    string  title<>;
    struct str_list     *next;
};

struct str_info {
    int         number;
    string      name<>;
    string      title<>;
};

struct str_server {
    int           number;
    string        name<>;
    string        title<>;
    struct paper  p;
    struct str_server   *next;
};


typedef int int_out;

program PAPER_SERVER
{
    version PAPER_VERSION
    {
        int_out ADD(str_add) = 1;
        int_out REMOVE(int) = 2;
        str_list LIST(void) = 3;
        str_info INFO(int) = 4;
    } = 1;
} = 0x20002345;
