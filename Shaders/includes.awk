BEGIN{
    skip = 0;
}
/^#include "[-A-Za-z0-9_/.]+"$/{
    skip = 1;
    includefn = substr($2, 2, length($2) - 2);
    printf("/****************************************************************************/\n");
    printf("/* BEGIN %s */\n", $0);
    rv = system(sprintf("awk -f includes.awk <%s", includefn));
    if(rv)
        exit rv;
    printf("/* END %s */\n", $0);
    printf("/****************************************************************************/\n");
}
{
    if(!skip)
        print;
    skip=0;
}
