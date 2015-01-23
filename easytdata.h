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
 *         Author:  Costaxu ,xxb.sklse@gmail.com
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



class EasyTDataException: public exception
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

    ~EasyTDataException()
        throw()
    {}
};


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
                case enum_ed_type_string:
                    this->data.sdata = new string("");
                    break;
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

        virtual ~EasyTData()
        {
            free();
        }

        EasyTData & operator = (int idata)
        {
            free();
            this->type = enum_ed_type_int; 
            this->data.idata = idata;
            return *this;
        }

        EasyTData & operator =(const char* pc)
        {
            free();
            this->type = enum_ed_type_string;
            this->data.sdata = new string(pc);
            return *this;
        }

        EasyTData& operator=(const string& str)
        {
            free();
            this->type = enum_ed_type_string;
            this->data.sdata = new string(str);
            return *this;
        }

        bool operator==(const EasyTData& et)
        {
            if(et.type != type) return false;
            switch(type)
            {
                case enum_ed_type_none:
                    return true;
                case enum_ed_type_int:
                    return et.data.idata == data.idata;
                case enum_ed_type_string:
                    return et.data.sdata == data.sdata;
                case enum_ed_type_vector:
                case enum_ed_type_map:
                case enum_ed_type_pair:
                    return &et == this;
            }
            return false;
        }

        void Append(const EasyTData& et)
        {
            if(type == enum_ed_type_none)
            {
                type = enum_ed_type_vector;
                data.vdata = new ETVector;
            }
            if(type == enum_ed_type_vector)
            {
                EasyTdata * p_et_new = new EasyTData(et);
                data.vdata->push_back(p_et_new);
                return;
            }
            throw ETException("EasyTData type error. not a vector");
        }

        void Append(int idata)
        {
            EasyTData et ;et = idata;
            Append(et); 
        }

        void Append(const string& str)
        {
            
            EasyTData et ;et= str;
            Append(et); 
        }
        
        void Remove(const EasyTData & et)
        {
            if(this->type == enum_ed_type_vector)
            {
                ETVectorIterator iter = data.vdata->begin();
                for(;iter!=data.vdata->end();iter++)
                {
                    if(**iter == et)
                    {
                        data.vdata->erase(iter); 
                        return;
                    }
                }
            }
        }

        void Remove(int idata)
        {
            EasyTData et ;
            et = idata;
            Remove(et);
        }

        void Remove(const string& sdata)
        {
            EasyTData et ; et = sdata;

            Remove(et);
        }

        EasyTData& operator[](const int i)
        {
            if(this->type == enum_ed_type_vector &&
                    i< this->data.vdata->size())
            {
                return *(this->data.vdata->at(i));     
            }
            throw ETException("type error, operate [] not support");
        }

        EasyTData& operator[](const char* pc)
        {
            if(type == enum_ed_type_none)
            {
                type == enum_ed_type_map;
                data.mdata = new ETMap;
            }
            if(this->type == enum_ed_type_map)
            {
                if (this->data.mdata->find(pc) != this->data.mdata->end())
                {
                    return *(this->data.mdata->at(pc)); 
                }     
                else
                {
                    (*this->data.mdata)[pc] = new EasyTData(enum_ed_type_none); 
                    return *(*this->data.mdata)[pc];
                }
            }
            throw ETException("type error, operate [] not support");
        }

        void print();

private :
        void free()
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
                    ETMap * etmap = this->data.mdata;
                    ETMapIterator etIter = etmap->begin();
                    for(;etIter!=etmap->end();etIter++)
                    {
                        EasyTData * et = etIter->second;
                        delete et;

                    }
                    delete etmap;
                    break;
                }
            }
        }
};

}
#endif
