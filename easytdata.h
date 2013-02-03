/*
 * =====================================================================================
 *
 *       Filename:  easytdata.h
 *
 *    Description:  i
 *
 *        Version:  1.0
 *        Created:  01/29/2013 07:45:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Costaxu (), costaxu@tencent.com
 *        Company:  Tencent, China
 *
 * =====================================================================================
 */
#ifndef EASYTDATA_H
#define EASYTDATA_H 1
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <stdio.h>
using namespace std;


namespace easytdata
{

typedef enum   EnumEasyTDataType
{
    enum_ed_type_int =1,
    enum_ed_type_string = 2,
    enum_ed_type_map = 3,
    enum_ed_type_vector = 4, 
    enum_ed_type_bool = 5,
    enum_ed_type_none = 6,
    enum_ed_type_pair = 7,
}enum_ed_type, EnumEasyTDataType, EnumEasyTdataType;

struct _EasyTData;
typedef struct _EasyTData  _EasyTData, _EasyTdata;

typedef vector<_EasyTdata*> ETVector;
typedef vector<_EasyTData*>::iterator ETVectorIterator ;

typedef map<string , _EasyTdata*> ETMap;
typedef ETMap::iterator ETMapIterator,etmiterator;

typedef pair<string, _EasyTdata*> ETPair;

typedef struct _EasyTData
{
    enum_ed_type type;
    union{
        int         idata;
        bool        bdata;
        string  *   sdata;
        ETVector*   vdata;
        ETMap   *   mdata;
        ETPair  *   pdata;
    }data;    
} _EasyTData, _EasyTdata;



class EasyTData;
typedef EasyTData EasyTdata;

class EasyTData: public _EasyTData
{
    public:
        EasyTData(enum_ed_type t= enum_ed_type_none)
        {
            type = t; 
            switch(t)
            {
                case enum_ed_type_vector:
                    this->data.vdata = new ETVector; 
                    break;
                case enum_ed_type_map:
                    this->data.mdata = new ETMap;
                    break;
                case enum_ed_type_pair:
                    this->data.pdata = new ETPair;
                    break;
            }
        }

        EasyTData & operator = (int idata)
        {
            this->type = enum_ed_type_int; 
            this->data.idata = idata;
            return *this;
        }

        EasyTData & operator =(const char* pc)
        {
            this->type = enum_ed_type_string;
            this->data.sdata = new string(pc);
            return *this;
        }

        EasyTData& operator=(const string& str)
        {
            this->type = enum_ed_type_string;
            this->data.sdata = new string(str);
            return *this;
        }

        void Append(const EasyTData& et)
        {
            if(this->type == enum_ed_type_vector)
            {
                EasyTdata * p_et_new = new EasyTData(et);
                this->data.vdata.push_back(p_et_new);
            }
        }

        EasyTData& operator[](const int i)
        {
            if(this->type == enum_ed_type_vector &&
                    i< this->data.vdata->size())
            {
                return *(this->data.vdata[i]);     
            }
            throw EasyTDataException("operate [] error");
        }

        EasyTData& operator[](const char* pc)
        {
            if(this->type == enum_ed_type_map)
            {
                if (this->data.mdata->contains(pc))
                {
                    return *(this->data.mdata[pc]); 
                }     
            }
            else
            {
                
                this->type = enum_ed_type_map;
                this->data.vdata = new map<_EasyTData*>;
                _EasyTData* et = new _EasyTData;
                et->type = enum_ed_type_none;
                
                this->data.vdata[pc] = et; 
                return et;
            }
   
        }

private :
        free_data()
        {
            switch(this->type)
            {
                case enum_ed_type_string:
                    delete this->data.sdata;
                    break;
                case enum_ed_type_vector:
                {
                    ETVector * etv = this->data.vdata;
                    ETVectorIterator etvi = etv->begin();
                    for(;etvi!=etv->end();etvi++)
                    {
                        _EasyTData* et = *etvi;
                        delete et;
                    }
                    delete etv;
                    break;        
                }
                case enum_ed_type_map:
                {
                    ETMap * etmap = this->dat.mdata;
                    ETMapIterator * etIter = etmap->begin();
                    for(;etIter!=etmap->end();etIter++)
                    {
                        _EasyTData * et = etIter->second;
                        delete et;

                    }
                    delete etmap;
                    break;
                }
            }
        }
};

_EasyTdata* ed_factory_int(int idata);
_EasyTdata* ed_factory_string(const char* pc);
_EasyTdata* ed_factory_map();
_EasyTdata* ed_factory_vector();
_EasyTdata * ed_factory_bool(bool b);
_EasyTdata * ed_factory_none();
_EasyTdata * ed_factory_pair( _EasyTdata* p1, _EasyTdata * p2);
void ed_vector_add(_EasyTData * ed, _EasyTData * item);
void ed_map_add(_EasyTData * ed, _EasyTData * item, _EasyTData* value);

#define ed_map_add_pair(ed, pair) ed_map_add(ed, pair->data.pdata->first, pair->data.pdata->second)
void ed_print(_EasyTData *ed);
}
#endif
