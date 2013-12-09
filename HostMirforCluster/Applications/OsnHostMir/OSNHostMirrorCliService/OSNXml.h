#ifndef _OSNXML_H
#define _OSNXML_H

#include <stdlib.h>
#include "Common.h"
#include "MirrorUIConfigure.h"

#import <msxml6.dll>
using namespace MSXML2;

class COSNxml
{
public:

	COSNxml()
	{
		this->m_pDoc                    = NULL; 
	    this->m_pProInstruction         = NULL; 
		this->m_pComment                = NULL; 
		this->m_pRootElement            = NULL;
		this->m_pElement                = NULL; 
		this->m_pAttrNode               = NULL;
		this->m_pAttrMap                = NULL;
		this->m_pAttrItem               = NULL;
		this->m_pNode                   = NULL;
		this->m_pNodeSub                = NULL;

		HRESULT hr = m_pDoc.CreateInstance(__uuidof(DOMDocument60)); 
		if (FAILED(hr))
		{
			LOG(INFO) << "cannot create DOMDocument60! \n";
		}
	}

	~COSNxml()
	{
		this->m_pDoc                    = NULL; 
	    this->m_pProInstruction         = NULL; 
		this->m_pComment                = NULL; 
		this->m_pRootElement            = NULL;
		this->m_pElement                = NULL; 
		this->m_pAttrNode               = NULL;
		this->m_pAttrMap                = NULL;
		this->m_pAttrItem               = NULL;
		this->m_pNode                   = NULL;
		this->m_pNodeSub                = NULL;
	}

public:
	DWORD CreateXMLFile(char *pRootName);

	DWORD GetXMLText(char *Msg);
	DWORD GetXMLNodeText(char *pRootName,char *pNodeName,char *pOutBuffer);
	DWORD GetXMLNodeAttribute(char *pAbsolutePath,char *pAttributeName,char *pOutBuffer,int Length);

	DWORD UnicodeToUTF_8(wchar_t *pUnicode ,char *pUtf8);
	DWORD UTF_8ToUnicode(char *pUtf8 ,wchar_t *pUnicode);

	DWORD LoadFile(wchar_t *pXML);
	DWORD SaveFile(char *pNodeName);

	DWORD AddXMLText(char *pNodeName,char *pTextValue);
	DWORD AddXMLAttribute(char *pNodeName,char *pAttributeName,char *pAttributeValue);
	DWORD AddXMLElement(char *pFatName,char *pSubName);
	DWORD AddXMLVolElement(char *pFatName,char *pSubName,char *pDiskGuid);
	DWORD AddXMLChannelElement(char *pFatName,char *pSubName);
	DWORD AddXMLElement(char *pNodeName);

    MSXML2::IXMLDOMDocumentPtr                  m_pDoc ;                         // xml文档
    MSXML2::IXMLDOMProcessingInstructionPtr     m_pProInstruction;               // xml声明
    MSXML2::IXMLDOMCommentPtr                   m_pComment;                       // 注释
    MSXML2::IXMLDOMElementPtr                   m_pRootElement, m_pElement;        // 根节点(元素)
    MSXML2::IXMLDOMNodePtr                      m_pNode, m_pNodeSub;         // 节点
    MSXML2::IXMLDOMAttributePtr                 m_pAttrNode;                     // 属性
	MSXML2::IXMLDOMNodeListPtr                  m_pNodeList;                   //节点链表
	MSXML2::IXMLDOMNamedNodeMapPtr              m_pAttrMap;  
    MSXML2::IXMLDOMNodePtr                      m_pAttrItem; 
};

#endif