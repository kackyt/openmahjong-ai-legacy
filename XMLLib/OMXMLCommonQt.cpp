#include "OMXMLCommon.h"


long OMStrtol(OMString val)
{
    return val.toLong();
}

void OMNumToStr(OMString &str,int val)
{
    str = OMString::number(val);
}

void OMCreateDocument(OMDomDocument &inst)
{

}

void OMLoadXML(OMDomDocument &inst,OMString &str)
{
    inst.setContent(str);
}

OMDomElement OMGetElement(OMDomNode &inst,OMString name)
{
    QStringList list = name.split("/");
    OMDomElement node;
    bool isFirst = true;
    while(!list.isEmpty()){
        QString nm = list.takeFirst();
        if(isFirst){
            node = inst.firstChildElement(nm);
            isFirst = false;
        }else{
            node = node.firstChildElement(nm);
        }
        if(node.isNull()) break;
    }
    return node;
}

static void OMGetElementListSub(OMDomNode &node,OMDomNodeList &nodelist,QStringList &list,int idx)
{
    OMDomElement element;

    element = node.firstChildElement(list.at(idx));

    while(!element.isNull()){
        if(idx == list.count() - 1){
            nodelist.append(element);
        }else{
            OMGetElementListSub(element,nodelist,list,idx+1);
        }

        element = element.nextSiblingElement(list.at(idx));
    }

}


OMDomNodeList OMGetElementList(OMDomNode &inst,OMString name)
{
    QStringList list = name.split("/");
    OMDomNodeList nodelist;
#if 1
    OMGetElementListSub(inst,nodelist,list,0);

    return nodelist;

#else
    OMDomElement node;
    QString nm;
    bool isFirst = true;
    while(list.size() > 1){
        nm = list.takeFirst();
        if(isFirst){
            node = inst.firstChildElement(nm);
            isFirst = false;
        }else{
            node = node.firstChildElement(nm);
        }
        if(node.isNull()) break;
    }

    if(isFirst){
        node = inst.toElement();
    }

    if(!node.isNull()){
        nm = list.takeFirst();
        nodelist = node.elementsByTagName(nm);
    }

    return nodelist;
#endif
}

void OMToXML(OMDomDocument &inst,OMString &str)
{
    str = inst.toString(-1);
}

int OMListLength(OMDomNodeList &list)
{
    return list.size();
}

OMDomNode OMListItem(OMDomNodeList &list,int index)
{
    return list.item(index);
}

void OMGetAttribute(OMDomElement &inst,OMString name,OMString &str)
{
    str = inst.attribute(name);
}

bool OMIsNull(OMDomNode &inst)
{
    return inst.isNull();
}

OMDomNode OMCreateTextNode(OMDomDocument &doc,OMString str)
{
    return doc.createTextNode(str);
}

void OMGetText(OMDomNode &inst,OMString &str)
{
    if(!inst.isNull()){
        OMDomNode child = inst.firstChild();
        if(child.isText()){
            str = child.nodeValue();
        }
    }
}

void OMToNum(OMDomNode &inst,int &val)
{
    if(!inst.isNull()){
        OMString text;
        OMGetText(inst,text);
        val = OMStrtol(text);
    }
}

void OMToBool(OMDomNode &inst,BOOL &val)
{
    if(!inst.isNull()){
        OMString text;
        OMGetText(inst,text);
        val = (text == _T("true")) ? TRUE : FALSE;
    }else{
        val = FALSE;
    }

}

OMDomElement OMCreateElement(OMDomDocument &doc,OMString name)
{
    return doc.createElement(name);
}

void OMAppendChild(OMDomNode &parent,OMDomNode &child)
{
    parent.appendChild(child);
}

void OMSetAttribute(OMDomElement &inst,OMString name,OMString val)
{
    inst.setAttribute(name,val);
}

bool OMIsEmpty(OMDomNodeList &list)
{
    return list.isEmpty();
}

