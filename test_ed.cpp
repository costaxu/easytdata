/*
 * =====================================================================================
 *
 *       Filename:  test_ed.cpp
 *
 *    Description:  i
 *
 *        Version:  1.0
 *        Created:  01/29/2013 06:22:55 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Costaxu (), costaxu@tencent.com
 *        Company:  Tencent, China
 *
 * =====================================================================================
 */

#include "easytdata.h"
using namespace easytdata;
int test_print()
{
    EasyTData t;
    t = 100;

    EasyTData t1(enum_ed_type_vector);
    t1.Append(t);
    t1.print();
    printf("\n");

    t1 = 1000;
    t1.print();
    printf("\n");

    EasyTData t2(enum_ed_type_map);
    t2["bar"] = 10000;
    t2["foo"] = "costaxu";
    t2.print();
    printf("\n");
    return 0;
}


int test_remove()
{
    EasyTData t;
    t.Append(100);
    t.Append(100);
    t.Append(100);
    t.Append(100);
    t.Append(100);

    t.print();
    printf("\n");

    t.Remove(100);
    t.print();
    printf("\n");

    t.Remove(100);
    t.print();
    printf("\n");

    t.Remove(100);
    t.print();
    printf("\n");

    t.Remove(100);
    t.print();
    printf("\n");

    t.Remove(100);
    t.print();
    printf("\n");

    t.Remove(100);
    t.print();
    printf("\n");
}

int main()
{
    test_remove();
    return 0;
}
