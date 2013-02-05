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
#include <exception>

using namespace std;


namespace easytdata
{

typedef enum   EnumEasyTDataType
{
    enum_ed_type_int    = 1,
    enum_ed_type_string = 2,
    enum_ed_type_map    = 3,
    enum_ed_type_vector = 4, 
    enum_ed_type_bool   = 5,
    enum_ed_type_none   = 6,
    enum_ed_type_pair   = 7,
}enum_ed_type, EnumEasyTDataType, EnumEasyTdataType;

class EasyTData;
class EasyTDataException;
class BasicException;
class EasyTDataException;

typedef EasyTData EasyTdata;
typedef EasyTDataException EasyTdataException,ETException;

typedef vector<EasyTdata*> ETVector;
typedef ETVector::iterator ETVectorIterator ;

typedef map<string , EasyTdata*> ETMap;
typedef ETMap::iterator          ETMapIterator,etmiterator;

typedef pair<string, EasyTdata*> ETPair;



class BasicException: public exception
{
    public:
    string m_message;
    EasyTDataException(const char* pc = "")
        :m_message(pc)
    {

    }
    virtual const char* what()
    {
        return m_message.c_str();
    }
};

class EasyTDataException: public BasicException
{};

class EasyTData 
{
    public:
        enum_ed_type type;
        union{
            int         idata;
            bool        bdata;
            string  *   sdata;
            ETVector*   vdata;
            ETMap   *   mdata;
            ETPair  *   pdata;
        }data; 

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
            throws ETException
        {
            if(this->type == enum_ed_type_vector)
            {
                EasyTdata * p_et_new = new EasyTData(et);
                this->data.vdata.push_back(p_et_new);
                return;
            }
            throw ETException("EasyTData type error. not a vector");
        }

        void 

        EasyTData& operator[](const int i)
        {
            if(this->type == enum_ed_type_vector &&
                    i< this->data.vdata->size())
            {
                return *(this->data.vdata[i]);     
            }
            throw EasyTDataException("operate [int] error");
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
        free()
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
                        EasyTData* et = *etvi;
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

#define ed_map_add_pair(ed, pair) ed_map_add(ed, pair->data.pdata->first, pair->data.pdata->second)
void ed_print(_EasyTData *ed);
}
#endif
