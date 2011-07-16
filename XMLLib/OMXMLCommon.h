/****************************************************************************************
 * Copyright (c) 2010, Takaya Kakizaki(kacky)
 * All rights reserved.

  ソースコード形式かバイナリ形式か、変更するかしないかを問わず、以下の条件を満たす場合に限り、再頒布および使用が許可されます。 

  ・ソースコードを再頒布する場合、上記の著作権表示、本条件一覧、および下記免責条項を含めること。 

  ・バイナリ形式で再頒布する場合、頒布物に付属のドキュメント等の資料に、上記の著作権表示、本条件一覧、および下記免責条項を含めること。 

  ・書面による特別の許可なしに、本ソフトウェアから派生した製品の宣伝または販売促進に、オープン麻雀の名前またはコントリビューターの名前を使用してはならない。


  本ソフトウェアは、著作権者およびコントリビューターによって「現状のまま」提供されており、明示黙示を問わず、
  商業的な使用可能性、および特定の目的に対する適合性に関する暗黙の保証も含め、またそれに限定されない、いかなる保証もありません。
  著作権者もコントリビューターも、事由のいかんを問わず、 損害発生の原因いかんを問わず、かつ責任の根拠が契約であるか厳格責任であるか
  （過失その他の）不法行為であるかを問わず、仮にそのような損害が発生する可能性を知らされていたとしても、本ソフトウェアの使用によって発生した
  （代替品または代用サービスの調達、使用の喪失、データの喪失、利益の喪失、業務の中断も含め、またそれに限定されない）
  直接損害、間接損害、偶発的な損害、特別損害、懲罰的損害、または結果損害について、一切責任を負わないものとします。 

****************************************************************************************/
#pragma once
#include "OpenMahjongDef.h"

/* Qt XML Parserを MSXML(Visual C++)で扱うためのラッパー */

#ifdef _MSC_VER
/* XSXMLの場合ポインタによるアクセスのため、間接参照演算子を使う */
#define QDomNode  IXMLDOMNodePtr
#define QDomElement IXMLDOMElementPtr
#define QDomDocument IXMLDOMDocumentPtr
#define QDomNodeList IXMLDOMNodeListPtr
#define QString CString
#define OM_DEFARRAY(type,inst) CArray<type,type&> inst
#define clear() RemoveAll()
#define append(item) Add(item)
#define size() GetSize()
#define remove(i) RemoveAt(i)
#define insert(i,inst) InsertAt(i,inst)
#define OM_EVAL(inst,func)  (inst)->func
#define OM_GETELEMENT(inst,name) OM_EVAL(inst,selectSingleNode(name))
#define OM_GETELEMENTLIST(inst,name) OM_EVAL(inst,selectNodes(name))
#define OM_LISTLENGTH(inst) (inst)->Getlength()
#define OM_LISTITEM(inst,index) (inst)->Getitem(index)
#define OM_GETTEXT(inst,str) (inst)->get_text(&str)
#define OM_CREATETEXT(inst,str) OM_EVAL(inst,createTextNode((LPCTSTR)str))
#define OM_STRTOL(str) (_tcstol((const TCHAR*)(str),NULL,0))
#define OM_NUMTOSTR(str,val) ((str).Format(_T("%d"),val))
#define OM_ISNULL(inst) ((inst) == NULL)
#define OM_ISEMPTY(inst) ((inst) == NULL)
#define OM_COPYARRAY(dst,src) ((src).Copy(dst))


#else
#include <QtXml>
#define OM_DEFARRAY(type,inst) QVector<type> inst
#define OM_EVAL(inst,func)  (inst).func
#define OM_LISTLENGTH(inst) (inst).size()
#define OM_LISTITEM(inst,index) (inst).item(index)
#define OM_GETELEMENT(inst,name) OM_EVAL(inst,firstChildElement(name))
#define OM_GETELEMENTLIST(inst,name) OM_EVAL(inst.toElement(),elementsByTagName(name))
#define OM_GETTEXT(inst,str) (str = (inst).firstChild().nodeValue().data())
#define OM_CREATETEXT(inst,str) OM_EVAL(inst,createTextNode(str))
#define OM_STRTOL(str) ((str).toLong())
#define OM_NUMTOSTR(str,val) ((str) = (str).sprintf(_T("%d"),val))
#define OM_ISNULL(inst) ((inst).isNull())
#define OM_ISEMPTY(inst) ((inst).isEmpty())
#define OM_COPYARRAY(dst,src) ((dst) = (src))
#define _T(str) (str)
#define Format sprintf
typedef QChar *BSTR;
typedef char TCHAR;
typedef bool BOOL;
typedef uint UINT;
typedef ushort USHORT;
typedef void* LPVOID;
#define WINAPI __stdcall
#define TRUE true
#define FALSE false
#define AfxDebugBreak()

#endif

#define OM_TOLONG(inst,val) \
    if(!OM_ISNULL(inst)){\
        BSTR pStr;\
        QString text;\
        OM_GETTEXT(inst,pStr);\
        text = QString(pStr);\
        val = OM_STRTOL(text);\
    }

#define OM_TOBOOL(inst,val) \
    if(!OM_ISNULL(inst)){\
        BSTR pStr;\
        QString text;\
        OM_GETTEXT(inst,pStr);\
        text = QString(pStr);\
        val = (text == _T("true")) ? TRUE : FALSE;\
    }
