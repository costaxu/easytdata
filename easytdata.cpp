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
 *         Author:  Costaxu (), costaxu@tencent.com
 *        Company:  Tencent, China
 *
 * =====================================================================================
 */
#include "easytdata.h"

_EasyTdata* ed_factory_int(int idata)
{
    _EasyTdata * ed = new _EasyTdata;
    ed->type = enum_ed_type_int;
    ed->data.idata = idata;
    return ed;
}


_EasyTdata* ed_factory_string(const char* pc)
{
    _EasyTdata * ed = new _EasyTdata;
    ed->type = enum_ed_type_string;
    ed->data.sdata = new string(pc);
    return ed;
}

_EasyTdata* ed_factory_map()
{
    _EasyTdata * ed = new _EasyTdata;
    ed->type = enum_ed_type_map;
    ed->data.mdata = new map<_EasyTdata*, _EasyTData *>;
    return ed;
}

_EasyTdata* ed_factory_vector()
{
    _EasyTdata * ed = new _EasyTdata;
    ed->type = enum_ed_type_vector;
    ed->data.vdata = new vector<_EasyTData*>;
    return ed;
}

_EasyTdata * ed_factory_bool(bool b)
{
    _EasyTdata * ed = new _EasyTdata;
    ed->type = enum_ed_type_bool;
    ed->data.bdata= b;
    return ed;
}

_EasyTdata * ed_factory_none()
{
    _EasyTdata *ed = new _EasyTdata;
    ed->type = enum_ed_type_none;
    return ed;
}

_EasyTdata * ed_factory_pair( _EasyTdata* p1, _EasyTdata * p2)
{
    _EasyTdata *ed = new _EasyTdata;
    ed->type = enum_ed_type_pair;
    ed->data.pdata = new pair<_EasyTdata* , _EasyTdata*>(p1,p2);
    return ed;

}

void ed_vector_add(_EasyTData * ed, _EasyTData * item)
{
    if(ed->type != enum_ed_type_vector)return;
    ed->data.vdata->push_back(item);
}

void ed_map_add(_EasyTData * ed, _EasyTData * item, _EasyTData* value)
{
    if(ed->type != enum_ed_type_map)return;
    (*(ed->data.mdata))[item] = value;
}


void ed_print(_EasyTData *ed)
{
    if(!ed) return;
    switch(ed->type)
    {
        case enum_ed_type_int:
            printf("%d", ed->data.idata);
            break;
        case enum_ed_type_string:
            printf("\"%s\"",ed->data.sdata->c_str());
            break;
        case enum_ed_type_vector:
            {
                ETVectorIterator iter = ed->data.vdata->begin();
                printf("[");
                for(;iter != ed->data.vdata->end(); iter++)
                {
                    ed_print(*iter); 
                    printf(",");
                }
                printf("]");
                break;
            }    
        case enum_ed_type_map:
            {
                ETMapIterator iter = ed->data.mdata->begin();
                printf("{");
                for(;iter != ed->data.mdata->end(); iter++)
                {
                    ed_print(iter->first); 
                    printf(":");
                    ed_print(iter->second); 
                    printf(",");
                }
                printf("}");
                break;
            }    
        case enum_ed_type_bool:
            if (ed->data.bdata)
                printf("TRUE");
            else
                printf("FALSE");
            break;
        case enum_ed_type_none:
            printf("NIL");
            break;
    }
}

