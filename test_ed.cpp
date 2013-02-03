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

int main()
{
    _EasyTData * ed = ed_factory_int(9);
    ed_print(ed);
    printf("\n");

    ed = ed_factory_string("123");
    ed_print(ed);
    printf("\n");

    ed = ed_factory_vector();
    _EasyTData * ed1 =ed_factory_int(19);
    _EasyTData * ed2 =ed_factory_int(29);

    ed_vector_add(ed, ed1);
    ed_vector_add(ed, ed2);
        
    ed_print(ed);
    printf("\n");

    ed = ed_factory_map();
    ed1 =ed_factory_int(19);
    ed2 =ed_factory_int(29);
    _EasyTData * ed3= ed_factory_string("123");
    _EasyTData * ed4= ed_factory_string("acccc");
    ed_map_add(ed,ed3,ed4);

    ed_map_add(ed, ed1 ,ed2);
    ed_print(ed);
    printf("\n");

}

