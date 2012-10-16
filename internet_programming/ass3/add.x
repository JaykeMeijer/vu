struct param {
    string      name<>;
    string      title<>;
    struct param *next;
};

typedef int add_out;

program PAPER_SERVER
{
    version PAPER_VERSION
    {
        add_out ADD(param) = 78;
    } = 1;
} = 0x20002345;
