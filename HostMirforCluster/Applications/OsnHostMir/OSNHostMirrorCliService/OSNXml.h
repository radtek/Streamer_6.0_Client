#ifndef _OSNXML_H
#define _OSNXML_H

#include <stdlib.h>

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
		this->m_pNode = NULL, m_pNodeSub = NULL;

		HRESULT hr = m_pDoc.CreateInstance(__uuidof(DOMDocument60)); 
		if (FAILED(hr))
		{
			printf("cannot create DOMDocument60! \n");
		}
	}

	//virtual ~COSNxml()
	//{
	//	this->m_pDoc                    = NULL; 
	//    this->m_pProInstruction         = NULL; 
	//	this->m_pComment                = NULL; 
	//	this->m_pRootElement            = NULL;
	//	this->m_pElement                = NULL; 
	//	this->m_pAttrNode               = NULL;
	//	this->m_pNode                   = NULL;
	//	this->m_pNodeSub                = NULL;

	//	//CoUninitialize();
	//}

public:
	DWORD CreateXMLFile(char *pRootName);

	DWORD GetXMLText(WCHAR *Msg);
	DWORD GetXMLNodeText(char *pRootName,char *pNodeName,char *pOutBuffer);

	DWORD UnicodeToUTF_8(wchar_t *pUnicode ,char *pUtf8);
	DWORD UTF_8ToUnicode(char *pUtf8 ,wchar_t *pUnicode);

	DWORD LoadFile(wchar_t *pXML);
	DWORD SaveFile(char *pNodeName);

	DWORD AddXMLAttribute(char *pNodeName,char *pAttributeName,char *pAttributeValue);
	DWORD AddXMLDisElement(char *pFatName,char *pSubName);
	DWORD AddXMLVolElement(char *pFatName,char *pSubName,char *pDiskGuid);
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