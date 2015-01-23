/*
 * =====================================================================================
 *
 *       Filename:  easytdata.cpp
 *
 *    Description:  :
 *
 *        Version:  1.0
 *        Created:  01/29/2013 09:28:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */
#include "easytdata.h"
namespace easytdata{
void EasyTData::print()
{
    switch(type)
    {
        case enum_ed_type_int:
            printf("%d", data.idata);
            break;
        case enum_ed_type_string:
            printf("\"%s\"",data.sdata->c_str());
            break;
        case enum_ed_type_vector:
            {
                ETVectorIterator iter = data.vdata->begin();
                printf("[");
                for(;iter != data.vdata->end(); iter++)
                {
                    (*iter)->print(); 
                    printf(",");
                }
                printf("]");
                break;
            }    
        case enum_ed_type_map:
            {
                ETMapIterator iter = data.mdata->begin();
                printf("{");
                for(;iter != data.mdata->end(); iter++)
                {
                    printf("%s:", iter->first.c_str());
                    (*(iter->second)).print(); 
                    printf(",");
                }
                printf("}");
                break;
            }    
        case enum_ed_type_bool:
            if (data.bdata)
                printf("TRUE");
            else
                printf("FALSE");
            break;
        case enum_ed_type_none:
            printf("NIL");
            break;
    }
}
}
