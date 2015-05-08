#ifndef _PORTAL_H_
#define _PORTAL_H_

#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include "variable.h"

class Portal
{
public:
        Portal(Node * bean)
        {
                this->Bean = bean;
        }
        ~Portal()
        {
                /*map<VariableDef*, Variable*>::iterator i;
                for(i = this->SharedVar.begin(); i!= this->SharedVar.end(); i++)
                {
                        delete i->second;
                }*/
        }
        void UpdateSharedVariable(VariableDef * def, Variable * var)
        {
                map<VariableDef*, Variable*>::iterator pos = this->SharedVar.find(def);
                if(pos!=this->SharedVar.end())
                {
                        this->SharedVar.erase(def);
                }
                this->SharedVar.insert(pair<VariableDef*, Variable*>(def, var));
        }

        Variable * GetSharedVariable(VariableDef * def)
        {
                map<VariableDef*, Variable*>::iterator pos = this->SharedVar.find(def);
                if(pos!=this->SharedVar.end())
                {
                        return pos->second;
                }
                return NULL;
        }
private:
        Node * Bean;
        map<VariableDef*, Variable*> SharedVar;
};

#endif
