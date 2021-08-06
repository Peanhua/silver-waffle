BEGIN{
    skip = 0;
}
/^#include "[-A-Za-z0-9_/.]+"$/{
    skip = 1;
    includefn = substr($2, 2, length($2) - 2);
    printf("/****************************************************************************/\n");
    printf("/* BEGIN %s */\n", $0);
    rv = system(sprintf("awk -vROOT=\"%s\" -f %s/includes.awk <%s/%s", ROOT, ROOT, ROOT, includefn));
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
